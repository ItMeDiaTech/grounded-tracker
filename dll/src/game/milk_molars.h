#pragma once

#include "game_state.h"
#include <cstdint>

void ReadMilkMolarsFromMemory(uintptr_t gameState, uintptr_t playerState, ProgressSnapshot& snap);
