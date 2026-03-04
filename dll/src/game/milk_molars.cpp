#include "milk_molars.h"
#include "achievements.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"

static constexpr uint32_t REGULAR_TOTAL = 78;
static constexpr uint32_t MEGA_TOTAL = 45;

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

        // Diagnostic: count all types and states
        static bool loggedOnce = false;
        bool shouldLog = !loggedOnce;
        uint32_t typeCounts[4] = {};      // count per type (0-3)
        uint32_t collectedByType[4] = {}; // collected (state==2) per type

        for (int32_t i = 0; i < arr->count; ++i) {
            uintptr_t elem = arr->data + i * Offsets::Collectible_Size;

            auto type = Memory::SafeRead<uint8_t>(elem, Offsets::Collectible_Type);
            auto state = Memory::SafeRead<uint8_t>(elem, Offsets::Collectible_State);

            if (!type || !state) continue;

            uint8_t t = type.value();
            uint8_t s = state.value();

            if (t < 4) {
                typeCounts[t]++;
                if (s == 2) collectedByType[t]++;
            }

            if (s == 2) {  // Collected
                if (t == 2) regularCollected++;       // Upgrade = regular molar
                else if (t == 3) megaCollected++;     // UpgradeParty = mega molar
            }
        }

        if (shouldLog && arr->count > 0) {
            LOG_INFO("=== TrackedCollectibles Dump (%d entries) ===", arr->count);
            LOG_INFO("  Type 0 (Invalid): %u total, %u collected", typeCounts[0], collectedByType[0]);
            LOG_INFO("  Type 1 (Color):   %u total, %u collected", typeCounts[1], collectedByType[1]);
            LOG_INFO("  Type 2 (Upgrade): %u total, %u collected", typeCounts[2], collectedByType[2]);
            LOG_INFO("  Type 3 (UpgradeParty): %u total, %u collected", typeCounts[3], collectedByType[3]);
            LOG_INFO("=== End TrackedCollectibles Dump ===");
            loggedOnce = true;
        }

        snap.milkMolars.regularCollected = regularCollected;
        snap.milkMolars.megaCollected = megaCollected;

        LOG_DEBUG("MilkMolars: %u/%u regular, %u/%u mega (from %d collectibles)",
                  regularCollected, REGULAR_TOTAL,
                  megaCollected, MEGA_TOTAL,
                  arr->count);
    }

    // Read PartyComponent upgrade points for secondary validation
    if (gameState) {
        auto partyComp = Memory::SafeReadPtr(gameState, Offsets::GameState_PartyComponent);
        if (partyComp) {
            auto partyPoints = Memory::SafeRead<int32_t>(
                partyComp.value(), Offsets::PartyComp_PartyUpgradePoints);
            auto personalPoints = Memory::SafeRead<int32_t>(
                partyComp.value(), Offsets::PartyComp_PersonalUpgradePoints);

            LOG_DEBUG("MilkMolars: PartyUpgradePoints=%d PersonalUpgradePoints=%d",
                      partyPoints.value_or(-1), personalPoints.value_or(-1));
        }
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
