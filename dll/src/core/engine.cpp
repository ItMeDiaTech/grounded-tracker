#include "engine.h"
#include "memory.h"
#include "offsets.h"
#include "../util/logger.h"
#include <Windows.h>

static uintptr_t g_GWorld = 0;
static uintptr_t g_GObjects = 0;
static uintptr_t g_GNames = 0;

bool InitializeEngine() {
    HMODULE gameModule = GetModuleHandleW(L"Maine-Win64-Shipping.exe");
    if (!gameModule) {
        LOG_ERROR("Game module not found");
        return false;
    }

    auto moduleBase = reinterpret_cast<uintptr_t>(gameModule);
    LOG_INFO("Game module base: 0x%llx", moduleBase);

    // Use offsets if they've been populated (from Dumper-7 SDK)
    if (Offsets::GWorld != 0) {
        g_GWorld = moduleBase + Offsets::GWorld;
        LOG_INFO("GWorld from offset: 0x%llx", g_GWorld);
    } else {
        // Pattern scan fallback — these patterns are UE4 version-specific
        // and will need updating after Dumper-7 generates the SDK
        auto addr = Memory::PatternScan(gameModule,
            "48 8B 05 ?? ?? ?? ?? 48 3B C8 74 06");
        if (addr) {
            g_GWorld = Memory::ResolveRIPRelative(addr + 3);
            LOG_INFO("GWorld from pattern: 0x%llx", g_GWorld);
        } else {
            LOG_ERROR("Failed to find GWorld");
            return false;
        }
    }

    if (Offsets::GObjects != 0) {
        g_GObjects = moduleBase + Offsets::GObjects;
        LOG_INFO("GObjects from offset: 0x%llx", g_GObjects);
    } else {
        auto addr = Memory::PatternScan(gameModule,
            "48 8B 05 ?? ?? ?? ?? 48 8B 0C C8 48 8D 04 D1");
        if (addr) {
            g_GObjects = Memory::ResolveRIPRelative(addr + 3);
            LOG_INFO("GObjects from pattern: 0x%llx", g_GObjects);
        } else {
            LOG_WARN("Failed to find GObjects (non-critical)");
        }
    }

    if (Offsets::GNames != 0) {
        g_GNames = moduleBase + Offsets::GNames;
        LOG_INFO("GNames from offset: 0x%llx", g_GNames);
    } else {
        auto addr = Memory::PatternScan(gameModule,
            "48 8D 05 ?? ?? ?? ?? EB 16 48 8D 0D");
        if (addr) {
            g_GNames = Memory::ResolveRIPRelative(addr + 3);
            LOG_INFO("GNames from pattern: 0x%llx", g_GNames);
        } else {
            LOG_WARN("Failed to find GNames (non-critical)");
        }
    }

    // Validate GWorld pointer resolves to a valid UWorld
    auto worldPtr = Memory::SafeReadPtr(g_GWorld);
    if (!worldPtr.has_value()) {
        LOG_ERROR("GWorld pointer does not resolve to valid UWorld");
        return false;
    }

    LOG_INFO("UWorld instance: 0x%llx", worldPtr.value());
    return true;
}

uintptr_t GetUWorld() {
    auto ptr = Memory::SafeReadPtr(g_GWorld);
    return ptr.value_or(0);
}

uintptr_t GetGObjects() {
    return g_GObjects;
}

uintptr_t GetGNames() {
    return g_GNames;
}

uintptr_t GetGameState() {
    auto world = GetUWorld();
    if (!world) return 0;

    // UWorld -> GameState offset (populated after SDK generation)
    if (Offsets::UWorld_GameState == 0) {
        LOG_WARN("UWorld::GameState offset not set");
        return 0;
    }

    auto gameState = Memory::SafeReadPtr(world, Offsets::UWorld_GameState);
    return gameState.value_or(0);
}

uintptr_t GetPlayerState() {
    auto world = GetUWorld();
    if (!world) return 0;

    // Walk: UWorld -> GameInstance -> LocalPlayers[0] -> PlayerController -> PlayerState
    if (Offsets::UWorld_GameInstance == 0) {
        LOG_WARN("UWorld::GameInstance offset not set");
        return 0;
    }

    auto gameInstance = Memory::SafeReadPtr(world, Offsets::UWorld_GameInstance);
    if (!gameInstance) return 0;

    auto localPlayers = Memory::SafeReadPtr(gameInstance.value(), Offsets::GameInstance_LocalPlayers);
    if (!localPlayers) return 0;

    // TArray<ULocalPlayer*> — read first element
    auto firstPlayer = Memory::SafeReadPtr(localPlayers.value());
    if (!firstPlayer) return 0;

    auto playerController = Memory::SafeReadPtr(firstPlayer.value(), Offsets::LocalPlayer_PlayerController);
    if (!playerController) return 0;

    auto playerState = Memory::SafeReadPtr(playerController.value(), Offsets::PlayerController_PlayerState);
    return playerState.value_or(0);
}
