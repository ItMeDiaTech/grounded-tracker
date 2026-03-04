#include "mixr.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../util/logger.h"

// 8 MIX.R stations — matches src-tauri/src/categories/mixr.rs
static const struct { const char* id; const char* name; } MIXR_DATA[] = {
    {"mixr_0", "Central Grasslands MIX.R"},
    {"mixr_1", "Hedge MIX.R"},
    {"mixr_2", "Haze MIX.R"},
    {"mixr_3", "Sandbox MIX.R"},
    {"mixr_4", "Northern Grasslands MIX.R"},
    {"mixr_5", "Upper Grasslands Super MIX.R"},
    {"mixr_6", "Lawnmower Super MIX.R"},
    {"mixr_7", "Moldorc Super MIX.R"},
};
static constexpr size_t MIXR_COUNT = sizeof(MIXR_DATA) / sizeof(MIXR_DATA[0]);

void ReadMixrFromMemory(uintptr_t gameState, ProgressSnapshot& snap) {
    snap.mixrChallenges.clear();
    snap.mixrChallenges.reserve(MIXR_COUNT);

    for (size_t i = 0; i < MIXR_COUNT; ++i) {
        snap.mixrChallenges.push_back({
            MIXR_DATA[i].id,
            MIXR_DATA[i].name,
            false
        });
    }

    if (!gameState) return;

    // TODO: Read MIX.R station states from UE4 memory after SDK generation
    // GameState -> MixrStations (TArray<FMixrState>) -> check Completed flag
    LOG_DEBUG("ReadMixr — stub, offsets not populated");
}
