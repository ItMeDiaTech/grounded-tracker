#include "pipe_server.h"
#include "protocol.h"
#include "../util/logger.h"
#include <nlohmann/json.hpp>
#include <chrono>

using json = nlohmann::json;

// Serialize ProgressSnapshot to JSON matching SaveProgress schema
static std::string SerializeSnapshot(const ProgressSnapshot& snap) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time);
    char timeBuf[64];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", &tm);

    auto serializeItems = [](const auto& items, const char* statusKey) -> json {
        json arr = json::array();
        uint32_t collected = 0;
        for (auto& item : items) {
            json obj;
            obj["id"] = item.id;
            obj["name"] = item.name;

            if constexpr (requires { item.defeated; }) {
                obj[statusKey] = item.defeated;
                if (item.defeated) collected++;
            } else if constexpr (requires { item.discovered; }) {
                obj[statusKey] = item.discovered;
                if (item.discovered) collected++;
                if constexpr (requires { item.zone; }) {
                    obj["zone"] = item.zone;
                }
            } else if constexpr (requires { item.completed; }) {
                obj[statusKey] = item.completed;
                if (item.completed) collected++;
            } else if constexpr (requires { item.collected; }) {
                obj[statusKey] = item.collected;
                if (item.collected) collected++;
                if constexpr (requires { item.subcategory; }) {
                    obj["subcategory"] = item.subcategory;
                }
            }

            arr.push_back(obj);
        }
        return json{{"items", arr}, {"collectedCount", collected}, {"totalCount", items.size()}};
    };

    json j;
    j["saveName"] = "Live";
    j["savePath"] = "";
    j["lastModified"] = timeBuf;
    j["overallPercent"] = snap.overallPercent;
    j["bosses"] = serializeItems(snap.bosses, "defeated");
    j["creatureCards"] = serializeItems(snap.creatureCards, "collected");
    j["landmarks"] = serializeItems(snap.landmarks, "discovered");
    j["mixrChallenges"] = serializeItems(snap.mixrChallenges, "completed");
    j["scabSchemes"] = serializeItems(snap.scabSchemes, "collected");
    j["wendell"] = serializeItems(snap.wendell, "collected");
    j["ominent"] = serializeItems(snap.ominent, "collected");
    j["burglChips"] = serializeItems(snap.burglChips, "collected");
    j["stuff"] = serializeItems(snap.stuff, "collected");

    j["milkMolars"] = {
        {"regularCollected", snap.milkMolars.regularCollected},
        {"regularTotal", snap.milkMolars.regularTotal},
        {"megaCollected", snap.milkMolars.megaCollected},
        {"megaTotal", snap.milkMolars.megaTotal},
        {"totalSpent", snap.milkMolars.totalSpent},
    };

    return j.dump();
}

bool PipeServer::Start() {
    if (m_running.load()) return true;

    m_running.store(true);
    m_acceptThread = std::thread(&PipeServer::AcceptLoop, this);

    return true;
}

void PipeServer::Stop() {
    m_running.store(false);

    if (m_hPipe != INVALID_HANDLE_VALUE) {
        DisconnectNamedPipe(m_hPipe);
        CloseHandle(m_hPipe);
        m_hPipe = INVALID_HANDLE_VALUE;
    }

    if (m_acceptThread.joinable()) {
        m_acceptThread.join();
    }
}

void PipeServer::AcceptLoop() {
    LOG_INFO("Pipe server accept loop started");

    while (m_running.load()) {
        m_hPipe = CreateNamedPipeW(
            PIPE_NAME,
            PIPE_ACCESS_OUTBOUND,
            PIPE_TYPE_BYTE | PIPE_WAIT,
            1,              // max instances
            PIPE_BUFFER_SIZE,
            0,              // in buffer (not used for outbound)
            0,              // default timeout
            nullptr         // default security
        );

        if (m_hPipe == INVALID_HANDLE_VALUE) {
            LOG_ERROR("CreateNamedPipe failed: {}", GetLastError());
            Sleep(1000);
            continue;
        }

        LOG_INFO("Waiting for pipe client...");

        if (ConnectNamedPipe(m_hPipe, nullptr) || GetLastError() == ERROR_PIPE_CONNECTED) {
            LOG_INFO("Pipe client connected");
            // Stay connected — writes happen from PushUpdate
            while (m_running.load() && m_hPipe != INVALID_HANDLE_VALUE) {
                Sleep(100);
            }
        }

        // Client disconnected or shutdown
        std::lock_guard lock(m_writeMutex);
        if (m_hPipe != INVALID_HANDLE_VALUE) {
            DisconnectNamedPipe(m_hPipe);
            CloseHandle(m_hPipe);
            m_hPipe = INVALID_HANDLE_VALUE;
        }

        LOG_INFO("Pipe client disconnected, waiting for new client...");
    }

    LOG_INFO("Pipe server accept loop exiting");
}

void PipeServer::PushUpdate(const ProgressSnapshot& snap) {
    std::lock_guard lock(m_writeMutex);

    if (m_hPipe == INVALID_HANDLE_VALUE) return;

    std::string jsonStr = SerializeSnapshot(snap);
    if (!WriteMessage(jsonStr)) {
        LOG_WARN("Failed to write to pipe, disconnecting");
        DisconnectNamedPipe(m_hPipe);
        CloseHandle(m_hPipe);
        m_hPipe = INVALID_HANDLE_VALUE;
    }
}

bool PipeServer::WriteMessage(const std::string& json) {
    // Length-prefixed protocol: [u32 LE length][JSON bytes]
    uint32_t length = static_cast<uint32_t>(json.size());
    DWORD written = 0;

    if (!WriteFile(m_hPipe, &length, sizeof(length), &written, nullptr) || written != sizeof(length)) {
        return false;
    }

    if (!WriteFile(m_hPipe, json.data(), length, &written, nullptr) || written != length) {
        return false;
    }

    return true;
}
