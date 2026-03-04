#include "scab_schemes.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"
#include <algorithm>
#include <string>

// 55 SCA.B scheme mappings — matches src/data/scab-schemes.ts
static const struct { const char* id; const char* name; } SCHEME_DATA[] = {
    {"Aerobics", "Aerobics"},
    {"Broodmother", "Broodmother"},
    {"Chewgum", "Table Gum"},
    {"Controller", "Controller"},
    {"Cookie", "Cookie Sandwich"},
    {"Corporate", "Corporate Issue"},
    {"DefaultBlue", "Factory Blue"},
    {"DefaultGreen", "Factory Green"},
    {"DefaultOrange", "Factory Orange"},
    {"DefaultPink", "Factory Pink"},
    {"DefaultRed", "Factory Red"},
    {"DefaultWhite", "Factory White"},
    {"DefaultYellow", "Factory Yellow"},
    {"Dragonfly", "Dragonfly"},
    {"Easter", "Painted"},
    {"Firefighter", "Firefighter"},
    {"Funguys", "Funguys"},
    {"Ghost", "Ghostmint"},
    {"GoldCard", "Gold Card"},
    {"HalloweenCandy", "Candy Corn"},
    {"HiddenKid", "Hidden Kid"},
    {"IceCap", "Ice Cap"},
    {"IBMBoss", "Infected"},
    {"Juicebox", "Juicebox"},
    {"Lint", "Lint"},
    {"Mansion", "Mansion"},
    {"Marsh", "Shallows"},
    {"Moldorc", "Moldorc"},
    {"MoleSkin", "Moleskin"},
    {"OakTree", "Oak Tree"},
    {"OminentInfiltrator", "Ominent Infiltrator"},
    {"Picnic", "Picnic"},
    {"Playplace", "Playplace"},
    {"Pond", "Pond"},
    {"Retro", "Retro"},
    {"RottedBee", "Rotted Bee"},
    {"Sandbox", "Sandbox"},
    {"SCAB1", "SCA.B v1.02"},
    {"SCAB2", "SCA.B v2.11"},
    {"SCAB3", "SCA.B v3.09"},
    {"Shed", "Shed"},
    {"Sleek", "Sleek"},
    {"SpacemanBoy", "Spaceman Boy"},
    {"Sprinkler", "Sprinkler"},
    {"StumpLab", "Stump Lab"},
    {"Tadpole", "Tadpole"},
    {"Termite", "Termite"},
    {"Trash", "Trash Heap"},
    {"TreeFort", "Tree Fort"},
    {"Undershed", "Under Shed"},
    {"WaspQueen", "Wasp Queen"},
    {"Wendell", "Wendell"},
    {"WoodPile", "Woodpile"},
    {"Workshop", "Workshop"},
    {"Zen", "Zen"},
};
static constexpr size_t SCHEME_COUNT = sizeof(SCHEME_DATA) / sizeof(SCHEME_DATA[0]);

// Extract scheme ID from a UClass FName like "GlobalColorTheme_Aerobics_C"
// Returns the middle portion between "GlobalColorTheme_" and "_C"
static std::string ExtractSchemeId(const std::string& className) {
    const std::string prefix = "GlobalColorTheme_";
    const std::string suffix = "_C";

    if (className.size() <= prefix.size() + suffix.size()) return className;
    if (className.compare(0, prefix.size(), prefix) != 0) return className;
    if (className.compare(className.size() - suffix.size(), suffix.size(), suffix) != 0)
        return className;

    return className.substr(prefix.size(), className.size() - prefix.size() - suffix.size());
}

void ReadScabSchemesFromMemory(uintptr_t gameState, ProgressSnapshot& snap) {
    snap.scabSchemes.clear();
    snap.scabSchemes.reserve(SCHEME_COUNT);

    // Populate all schemes as not-collected
    for (size_t i = 0; i < SCHEME_COUNT; ++i) {
        snap.scabSchemes.push_back({
            SCHEME_DATA[i].id,
            SCHEME_DATA[i].name,
            false
        });
    }

    if (!gameState) return;

    // PartyComponent -> DiscoveredColorThemes (TArray<TSubclassOf<UGlobalColorTheme>>)
    // Each element is a UClass* (8 bytes)
    auto partyComp = Memory::SafeReadPtr(gameState, Offsets::GameState_PartyComponent);
    if (!partyComp) {
        LOG_DEBUG("ReadScabSchemes: PartyComponent null");
        return;
    }

    auto arr = ReadTArray(partyComp.value(), Offsets::PartyComp_DiscoveredColorThemes);
    if (!arr) {
        LOG_DEBUG("ReadScabSchemes: DiscoveredColorThemes array not readable");
        return;
    }

    uint32_t matched = 0;
    for (int32_t i = 0; i < arr->count; ++i) {
        auto classPtr = Memory::SafeReadPtr(arr->data, i * 8);
        if (!classPtr) continue;

        // UObject::NamePrivate is at offset 0x18 in UE4 4.27
        std::string className = ResolveFNameAt(classPtr.value(), 0x18);
        if (className.empty()) continue;

        std::string schemeId = ExtractSchemeId(className);

        // Match against our scheme list
        for (auto& s : snap.scabSchemes) {
            if (s.id == schemeId) {
                s.collected = true;
                matched++;
                break;
            }
        }
    }

    LOG_DEBUG("ReadScabSchemes: %u/%zu matched from %d discovered themes",
              matched, SCHEME_COUNT, arr->count);
}
