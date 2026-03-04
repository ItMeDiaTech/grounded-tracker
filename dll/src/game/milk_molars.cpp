#include "milk_molars.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../util/logger.h"

static constexpr uint32_t REGULAR_TOTAL = 93;
static constexpr uint32_t MEGA_TOTAL = 14;

void ReadMilkMolarsFromMemory(uintptr_t gameState, uintptr_t playerState, ProgressSnapshot& snap) {
    snap.milkMolars = {};
    snap.milkMolars.regularTotal = REGULAR_TOTAL;
    snap.milkMolars.megaTotal = MEGA_TOTAL;

    if (!playerState) return;

    // TODO: Read molar counts from UE4 memory after SDK generation
    // PlayerState -> MolarsComponent -> RegularCount, MegaCount, SpentCount
    LOG_DEBUG("ReadMilkMolars — stub, offsets not populated");
}
