#pragma once

#include "game_state.h"
#include <cstdint>

// Reads table items and classifies into Wendell, Ominent, BURG.L chips, and Stuff
void ReadTableItemsFromMemory(uintptr_t gameState, ProgressSnapshot& snap);
