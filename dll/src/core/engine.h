#pragma once

#include <cstdint>

// Initialize UE4 engine globals by pattern scanning
bool InitializeEngine();

// Accessors for resolved engine pointers
uintptr_t GetUWorld();
uintptr_t GetGObjects();
uintptr_t GetGNames();

// Walk the object chain to get game/player state
uintptr_t GetGameState();
uintptr_t GetPlayerState();
