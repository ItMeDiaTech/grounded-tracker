#pragma once

#include "game_state.h"
#include <cstdint>

// Reads discovered color themes from GameState's PartyComponent
void ReadScabSchemesFromMemory(uintptr_t gameState, ProgressSnapshot& snap);
