#include "pipe_server.h"
#include "protocol.h"
#include "../util/logger.h"
#include <nlohmann/json.hpp>
#include <chrono>

using json = nlohmann::json;

// Per-type serializers (no C++20 concepts needed)
static json SerializeBosses(const std::vector<ProgressSnapshot::BossInfo>& items) {
    json arr = json::array();
    uint32_t count = 0;
    for (auto& item : items) {
        arr.push_back({{"id", item.id}, {"name", item.name}, {"defeated", item.defeated}});
        if (item.defeated) count++;
    }
    return {{"items", arr}, {"collectedCount", count}, {"totalCount", items.size()}};
}

static json SerializeCards(const std::vector<ProgressSnapshot::CreatureCardInfo>& items) {
    json arr = json::array();
    uint32_t count = 0;
    for (auto& item : items) {
        arr.push_back({{"id", item.id}, {"name", item.name}, {"collected", item.collected}});
        if (item.collected) count++;
    }
    return {{"items", arr}, {"collectedCount", count}, {"totalCount", items.size()}};
}

static json SerializeLandmarks(const std::vector<ProgressSnapshot::LandmarkInfo>& items) {
    json arr = json::array();
    uint32_t count = 0;
    for (auto& item : items) {
        arr.push_back({{"id", item.id}, {"name", item.name}, {"zone", item.zone}, {"discovered", item.discovered}});
        if (item.discovered) count++;
    }
    return {{"items", arr}, {"collectedCount", count}, {"totalCount", items.size()}};
}

static json SerializeMixr(const std::vector<ProgressSnapshot::MixrInfo>& items) {
    json arr = json::array();
    uint32_t count = 0;
    for (auto& item : items) {
        arr.push_back({{"id", item.id}, {"name", item.name}, {"completed", item.completed}});
        if (item.completed) count++;
    }
    return {{"items", arr}, {"collectedCount", count}, {"totalCount", items.size()}};
}

static json SerializeSchemes(const std::vector<ProgressSnapshot::ScabSchemeInfo>& items) {
    json arr = json::array();
    uint32_t count = 0;
    for (auto& item : items) {
        arr.push_back({{"id", item.id}, {"name", item.name}, {"collected", item.collected}});
        if (item.collected) count++;
    }
    return {{"items", arr}, {"collectedCount", count}, {"totalCount", items.size()}};
}

static json SerializeCollectibles(const std::vector<ProgressSnapshot::CollectibleInfo>& items) {
    json arr = json::array();
    uint32_t count = 0;
    for (auto& item : items) {
        arr.push_back({{"id", item.id}, {"name", item.name}, {"subcategory", item.subcategory}, {"collected", item.collected}});
        if (item.collected) count++;
    }
    return {{"items", arr}, {"collectedCount", count}, {"totalCount", items.size()}};
}

static std::string SerializeSnapshot(const ProgressSnapshot& snap) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time);
    char timeBuf[64];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", &tm);

    json j;
    j["saveName"] = "Live";
    j["savePath"] = "";
    j["lastModified"] = timeBuf;
    j["overallPercent"] = snap.overallPercent;
    j["bosses"] = SerializeBosses(snap.bosses);
    j["creatureCards"] = SerializeCards(snap.creatureCards);
    j["landmarks"] = SerializeLandmarks(snap.landmarks);
    j["mixrChallenges"] = SerializeMixr(snap.mixrChallenges);
    j["scabSchemes"] = SerializeSchemes(snap.scabSchemes);
    j["wendell"] = SerializeCollectibles(snap.wendell);
    j["ominent"] = SerializeCollectibles(snap.ominent);
    j["burglChips"] = SerializeCollectibles(snap.burglChips);
    j["stuff"] = SerializeCollectibles(snap.stuff);

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
            LOG_ERROR("CreateNamedPipe failed: %lu", GetLastError());
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
