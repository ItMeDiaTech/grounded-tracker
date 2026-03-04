#include "upgrades.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../util/logger.h"

void ReadUpgradesFromMemory(uintptr_t playerState, ProgressSnapshot& snap) {
    if (!playerState) return;

    // TODO: Read upgrade states from UE4 memory after SDK generation
    // PlayerState -> UpgradesComponent -> Upgrades (TArray<FUpgradeState>)
    // Currently upgrades don't appear as a separate frontend category,
    // but are used for milk molar spent calculations
    LOG_DEBUG("ReadUpgrades — stub, offsets not populated");
}
