#include "upgrades.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"

void ReadUpgradesFromMemory(uintptr_t playerState, ProgressSnapshot& snap) {
    if (!playerState) return;

    auto upgradeComp = Memory::SafeReadPtr(playerState, Offsets::PlayerState_PlayerUpgradeComponent);
    if (!upgradeComp) {
        LOG_DEBUG("ReadUpgrades: PlayerUpgradeComponent null");
        return;
    }

    auto arr = ReadTArray(upgradeComp.value(), Offsets::UpgradeComp_PlayerUpgrades);
    if (!arr) {
        LOG_DEBUG("ReadUpgrades: PlayerUpgrades array not readable");
        return;
    }

    static bool loggedOnce = false;
    if (!loggedOnce) {
        LOG_INFO("=== Player Upgrades Dump (%d entries) ===", arr->count);
    }

    for (int32_t i = 0; i < arr->count; ++i) {
        uintptr_t elem = arr->data + i * Offsets::PlayerUpgrade_Size;

        // FName_NetCrc at +0x00 (12 bytes: FName(8) + CRC(4))
        std::string name = ResolveFNameAt(elem, Offsets::PlayerUpgrade_Name);
        auto tier = Memory::SafeRead<int32_t>(elem, Offsets::PlayerUpgrade_Tier);

        if (!loggedOnce && !name.empty()) {
            LOG_INFO("  Upgrade[%d]: '%s' tier=%d", i, name.c_str(),
                     tier.value_or(-1));
        }
    }

    if (!loggedOnce && arr->count > 0) {
        LOG_INFO("=== End Player Upgrades Dump ===");
        loggedOnce = true;
    }

    LOG_DEBUG("ReadUpgrades: %d upgrades found", arr->count);
}
