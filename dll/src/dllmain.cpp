#include <Windows.h>
#include <thread>
#include <atomic>

#include "core/engine.h"
#include "core/memory.h"
#include "game/game_state.h"
#include "ipc/pipe_server.h"
#include "overlay/dx11_hook.h"
#include "overlay/overlay_ui.h"
#include "util/logger.h"

static std::atomic<bool> g_shutdown{false};
static std::thread g_initThread;
static std::thread g_pollThread;

static GameStateReader g_reader;
static PipeServer g_pipeServer;

static constexpr DWORD g_pollIntervalMs = 2000;

void PollThreadFunc() {
    LOG_INFO("Poll thread started (interval: %lums)", g_pollIntervalMs);

    while (!g_shutdown.load()) {
        auto snap = g_reader.ReadAll();
        if (snap.valid) {
            g_pipeServer.PushUpdate(snap);
            OverlayUI::UpdateState(snap);
        }
        Sleep(g_pollIntervalMs);
    }

    LOG_INFO("Poll thread exiting");
}

void InitThreadSafe(HMODULE hModule) {
    // Initialize logger first (before anything else)
    wchar_t tempPath[MAX_PATH];
    GetTempPathW(MAX_PATH, tempPath);
    std::wstring logPath = std::wstring(tempPath) + L"GroundedTracker.log";
    Logger::Init(logPath);

    LOG_INFO("=== Grounded Tracker DLL Loaded ===");
    LOG_INFO("Module base: 0x%llx", reinterpret_cast<uintptr_t>(hModule));

    // Wait for the game module to be fully loaded
    HMODULE gameModule = nullptr;
    for (int i = 0; i < 100 && !g_shutdown.load(); ++i) {
        gameModule = GetModuleHandleW(L"Maine-Win64-Shipping.exe");
        if (gameModule) break;
        Sleep(100);
    }
    if (!gameModule || g_shutdown.load()) {
        LOG_ERROR("Game module not found after 10s");
        return;
    }

    LOG_INFO("Game module found at: 0x%llx", reinterpret_cast<uintptr_t>(gameModule));

    // Resolve UE4 engine globals
    if (!InitializeEngine()) {
        LOG_ERROR("Failed to initialize UE4 engine globals");
        return;
    }
    LOG_INFO("UE4 engine globals resolved");

    // Initialize game state reader
    if (!g_reader.Initialize()) {
        LOG_WARN("Game state reader init failed (will retry during polling)");
    } else {
        LOG_INFO("Game state reader initialized");
    }

    // Install DX11 hook for ImGui overlay (non-fatal)
    // Disabled for initial testing to avoid crashes
    // TODO: Re-enable after verifying pipe communication works
    LOG_INFO("DX11 hook SKIPPED (disabled for initial testing)");
    // if (!DX11Hook::Install()) {
    //     LOG_WARN("Failed to install DX11 hook (overlay disabled)");
    // } else {
    //     LOG_INFO("DX11 hook installed");
    // }

    // Start named pipe server
    if (!g_pipeServer.Start()) {
        LOG_ERROR("Failed to start pipe server");
        return;
    }
    LOG_INFO("Pipe server started");

    // Start polling thread
    g_pollThread = std::thread(PollThreadFunc);

    LOG_INFO("Initialization complete");
}

static DWORD g_lastException = 0;

// SEH filter function — captures exception code
static int CrashFilter(DWORD code) {
    g_lastException = code;
    return EXCEPTION_EXECUTE_HANDLER;
}

// SEH wrapper must not contain C++ objects with destructors
static void InitThreadSEH(HMODULE hModule) {
    __try {
        InitThreadSafe(hModule);
    }
    __except (CrashFilter(GetExceptionCode())) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[GroundedTracker] FATAL: Init thread crashed with exception 0x%08lX",
            g_lastException);
        OutputDebugStringA(msg);
    }
}

void InitThread(HMODULE hModule) {
    InitThreadSEH(hModule);
}

void Shutdown() {
    LOG_INFO("Shutdown signal received");

    g_shutdown.store(true);

    if (g_pollThread.joinable()) {
        g_pollThread.join();
    }

    g_pipeServer.Stop();
    DX11Hook::Remove();

    LOG_INFO("=== Grounded Tracker DLL Unloaded ===");
    Logger::Shutdown();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        g_initThread = std::thread(InitThread, hModule);
        g_initThread.detach();
        break;

    case DLL_PROCESS_DETACH:
        Shutdown();
        break;
    }
    return TRUE;
}
