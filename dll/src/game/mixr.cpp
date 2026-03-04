#include "mixr.h"
#include "achievements.h"
#include "../core/engine.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"
#include <string>

// 8 MIX.R stations — matches src/data/mixr-challenges.ts
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

    auto world = GetUWorld();
    if (!world) return;

    // Scan all levels (PersistentLevel + streaming sub-levels) for MIXRTerminal actors
    // UWorld::Levels at 0x0190 is TArray<ULevel*> containing all loaded levels
    auto levels = ReadTArray(world, Offsets::UWorld_Levels);
    if (!levels || levels->count == 0) {
        LOG_DEBUG("ReadMixr: UWorld::Levels empty or not readable");
        return;
    }

    static bool loggedOnce = false;
    bool shouldLog = !loggedOnce;

    uint32_t mixrFound = 0;
    uint32_t mixrCompleted = 0;

    if (shouldLog) {
        LOG_INFO("=== MIX.R Scan: %d levels loaded ===", levels->count);
    }

    for (int32_t lvl = 0; lvl < levels->count; ++lvl) {
        auto levelPtr = Memory::SafeReadPtr(levels->data, lvl * 8);
        if (!levelPtr) continue;

        // ULevel::Actors at 0x0098
        auto actors = ReadTArray(levelPtr.value(), Offsets::ULevel_Actors);
        if (!actors || actors->count == 0) continue;

        for (int32_t a = 0; a < actors->count; ++a) {
            auto actorPtr = Memory::SafeReadPtr(actors->data, a * 8);
            if (!actorPtr) continue;

            // Read actor's UClass* at UObject+0x10
            auto classPtr = Memory::SafeReadPtr(actorPtr.value(), Offsets::UObject_ClassPrivate);
            if (!classPtr) continue;

            // Read class FName at UObject+0x18
            std::string className = ResolveFNameAt(classPtr.value(), Offsets::UObject_NamePrivate);
            if (className.empty()) continue;

            // Check if this is a MIXRTerminal (BP_MIXRTerminal_C or AMIXRTerminal)
            if (className.find("MIXRTerminal") == std::string::npos) continue;

            // Read MIXRCompletionCount at offset 0x0248
            auto completionCount = Memory::SafeRead<int32_t>(actorPtr.value() + Offsets::MIXRTerminal_CompletionCount);
            if (!completionCount) continue;

            // Read actor's own FName for identification
            std::string actorName = ResolveFNameAt(actorPtr.value(), Offsets::UObject_NamePrivate);

            if (shouldLog) {
                LOG_INFO("  MIXR[%u]: class='%s' actor='%s' completionCount=%d",
                         mixrFound, className.c_str(), actorName.c_str(), completionCount.value());
            }

            // MIXRCompletionCount is a global counter shared across all terminals.
            // It tracks how many unique MIX.R challenges have been completed total.
            // Use the max value found across any terminal.
            if (completionCount.value() > static_cast<int32_t>(mixrCompleted)) {
                mixrCompleted = completionCount.value();
            }
            mixrFound++;
        }
    }

    if (shouldLog && mixrFound > 0) {
        LOG_INFO("=== End MIX.R Scan: found %u terminals, %u completed ===",
                 mixrFound, mixrCompleted);
        loggedOnce = true;
    }

    // Mark first N stations as completed based on the global counter
    if (mixrCompleted > 0) {
        uint32_t toMark = mixrCompleted;
        if (toMark > MIXR_COUNT) toMark = static_cast<uint32_t>(MIXR_COUNT);
        for (uint32_t i = 0; i < toMark; ++i) {
            snap.mixrChallenges[i].completed = true;
        }
    }

    // Fallback: if no terminals found in levels, try achievement
    if (mixrFound == 0) {
        uintptr_t playerState = GetPlayerState();
        if (playerState) {
            auto completedCount = Achievements::GetAchievementProgress(playerState, "CompleteMIXRs");
            if (completedCount.has_value() && completedCount.value() > 0) {
                int32_t count = completedCount.value();
                if (count > static_cast<int32_t>(MIXR_COUNT)) count = static_cast<int32_t>(MIXR_COUNT);
                for (int32_t i = 0; i < count; ++i) {
                    snap.mixrChallenges[i].completed = true;
                }
                LOG_DEBUG("ReadMixr: %d/%zu (from CompleteMIXRs achievement fallback)", count, MIXR_COUNT);
            } else {
                LOG_DEBUG("ReadMixr: no terminals found, CompleteMIXRs = %d", completedCount.value_or(0));
            }
        }
    } else {
        LOG_DEBUG("ReadMixr: %u/%zu completed (%u terminals found in %d levels)",
                  mixrCompleted, MIXR_COUNT, mixrFound, levels->count);
    }
}
