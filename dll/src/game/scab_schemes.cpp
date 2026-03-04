#include "scab_schemes.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../util/logger.h"

// 55 SCA.B scheme mappings — matches src-tauri/src/categories/scab_schemes.rs
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

void ReadScabSchemesFromMemory(uintptr_t playerState, ProgressSnapshot& snap) {
    snap.scabSchemes.clear();
    snap.scabSchemes.reserve(SCHEME_COUNT);

    for (size_t i = 0; i < SCHEME_COUNT; ++i) {
        snap.scabSchemes.push_back({
            SCHEME_DATA[i].id,
            SCHEME_DATA[i].name,
            false
        });
    }

    if (!playerState) return;

    // TODO: Read collected themes from UE4 memory after SDK generation
    // PlayerState -> ThemesComponent -> CollectedThemes (TArray<FName>)
    LOG_DEBUG("ReadScabSchemes — stub, offsets not populated");
}
