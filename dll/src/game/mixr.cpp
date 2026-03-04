#include "mixr.h"
#include "achievements.h"
#include "../core/engine.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"
#include <string>

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

    // Approach: Scan PersistentLevel actors for AMIXRTerminal instances
    // UWorld -> PersistentLevel -> Actors TArray
    auto world = GetUWorld();
    if (!world) {
        LOG_DEBUG("ReadMixr: UWorld null");
        return;
    }

    auto level = Memory::SafeReadPtr(world, Offsets::UWorld_PersistentLevel);
    if (!level) {
        LOG_DEBUG("ReadMixr: PersistentLevel null");
        return;
    }

    // ULevel::Actors is a TArray<AActor*> at offset 0x0098
    static constexpr ptrdiff_t ULevel_Actors = 0x0098;
    auto actors = ReadTArray(level.value(), ULevel_Actors);
    if (!actors) {
        LOG_DEBUG("ReadMixr: Actors array not readable");
        return;
    }

    uint32_t mixrFound = 0;
    static bool loggedOnce = false;

    for (int32_t i = 0; i < actors->count; ++i) {
        auto actorPtr = Memory::SafeReadPtr(actors->data, i * 8);
        if (!actorPtr || !actorPtr.value()) continue;

        // Read actor's UClass pointer (UObject::ClassPrivate at offset 0x10)
        auto classPtr = Memory::SafeReadPtr(actorPtr.value(), 0x10);
        if (!classPtr) continue;

        // Read class FName (UObject::NamePrivate at offset 0x18)
        std::string className = ResolveFNameAt(classPtr.value(), 0x18);
        if (className.empty()) continue;

        // Check if this is a MIX.R terminal (class name contains "MIXRTerminal" or "Defense_Point")
        if (className.find("MIXRTerminal") == std::string::npos &&
            className.find("Defense_Point") == std::string::npos &&
            className.find("MIXR") == std::string::npos) {
            continue;
        }

        // Read CompletionCount
        auto completionCount = Memory::SafeRead<int32_t>(
            actorPtr.value(), Offsets::MIXRTerminal_CompletionCount);

        // Read actor name to identify which station this is
        std::string actorName = ResolveFNameAt(actorPtr.value(), 0x18);

        if (!loggedOnce) {
            LOG_INFO("MIX.R actor[%d]: class='%s' name='%s' completion=%d",
                     mixrFound, className.c_str(), actorName.c_str(),
                     completionCount.value_or(-1));
        }

        if (completionCount.has_value() && completionCount.value() > 0) {
            // Mark the corresponding station as completed
            // We match by index order since actors may appear in level order
            if (mixrFound < MIXR_COUNT) {
                snap.mixrChallenges[mixrFound].completed = true;
            }
        }

        mixrFound++;
    }

    if (!loggedOnce && mixrFound > 0) {
        LOG_INFO("MIX.R: found %u terminal actors in level", mixrFound);
        loggedOnce = true;
    }

    // If we didn't find actors (e.g., different level), log it
    if (mixrFound == 0) {
        LOG_DEBUG("ReadMixr: no MIX.R terminal actors found in PersistentLevel "
                  "(actor count: %d)", actors->count);
    }
}
