#include "milk_molars.h"
#include "achievements.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"

static constexpr uint32_t REGULAR_TOTAL = 93;
static constexpr uint32_t MEGA_TOTAL = 14;

void ReadMilkMolarsFromMemory(uintptr_t gameState, uintptr_t playerState, ProgressSnapshot& snap) {
    snap.milkMolars = {};
    snap.milkMolars.regularTotal = REGULAR_TOTAL;
    snap.milkMolars.megaTotal = MEGA_TOTAL;

    if (!gameState) return;

    // Read collected molars from TrackedCollectibles TArray<FCollectibleData>
    // ECollectibleType: Invalid=0, Color=1, Upgrade=2 (regular molar), UpgradeParty=3 (mega molar)
    // ECollectibleState: Unknown=0, Discovered=1, Collected=2
    auto arr = ReadTArray(gameState, Offsets::GameState_TrackedCollectibles);
    if (arr) {
        uint32_t regularCollected = 0;
        uint32_t megaCollected = 0;

        for (int32_t i = 0; i < arr->count; ++i) {
            uintptr_t elem = arr->data + i * Offsets::Collectible_Size;

            auto type = Memory::SafeRead<uint8_t>(elem, Offsets::Collectible_Type);
            auto state = Memory::SafeRead<uint8_t>(elem, Offsets::Collectible_State);

            if (!type || !state) continue;

            if (state.value() == 2) {  // Collected
                if (type.value() == 2) regularCollected++;       // Upgrade = regular molar
                else if (type.value() == 3) megaCollected++;     // UpgradeParty = mega molar
            }
        }

        snap.milkMolars.regularCollected = regularCollected;
        snap.milkMolars.megaCollected = megaCollected;

        LOG_DEBUG("MilkMolars: %u/%u regular, %u/%u mega (from %d collectibles)",
                  regularCollected, REGULAR_TOTAL, megaCollected, MEGA_TOTAL, arr->count);
    }

    // Read spent molars from achievement progress
    if (playerState) {
        auto spent = Achievements::GetAchievementProgress(playerState, "MilkMolarsSpent");
        if (spent) {
            snap.milkMolars.totalSpent = static_cast<uint32_t>(spent.value());
            LOG_DEBUG("MilkMolars: %u spent", snap.milkMolars.totalSpent);
        }
    }
}
