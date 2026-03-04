#include "landmarks.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"
#include <string>

// 108 landmark entries — matches src/data/landmarks.ts (S Rank requirement)
static const struct { const char* id; const char* name; const char* zone; } LANDMARK_DATA[] = {
    // Grasslands (24)
    {"POI_4Leaf_Clover", "Four Leaf Clover", "Grasslands"},
    {"POI_Ant_Hill_Abandoned", "Old Ant Hill", "Grasslands"},
    {"POI_Battletoad", "Rash", "Grasslands"},
    {"POI_Can_Chop", "Chop Can", "Grasslands"},
    {"POI_Can_Jabby", "Jabby Cola", "Grasslands"},
    {"POI_Can_Welp", "Welp Flavordrink", "Grasslands"},
    {"POI_Field_Station_Cave", "Larva Tunnels Field Station", "Grasslands"},
    {"POI_Field_Station_Grasslands_Chop_Can", "Chop Can Field Station", "Grasslands"},
    {"POI_Field_Station_Grasslands_Intro", "Start Field Station", "Grasslands"},
    {"POI_Field_Station_Grasslands_Porch_Branch", "Fallen Branch Field Station", "Grasslands"},
    {"POI_Field_Station_Grasslands_Rooty_Flood", "Flower Bed Field Station", "Grasslands"},
    {"POI_Frankenline", "Frankenline", "Grasslands"},
    {"POI_GardenGnome", "Garden Gnome", "Grasslands"},
    {"POI_Juicebox_ArmedRaspberry", "Armed Raspberry Punch-O", "Grasslands"},
    {"POI_Juicebox_LemonCrime", "Lemon Crime Punch-O", "Grasslands"},
    {"POI_Juicebox_Tropicop", "Tropicop Punch-O", "Grasslands"},
    {"POI_MarshBaseball", "Baseball", "Grasslands"},
    {"POI_MilkCarton", "Squirts Carton", "Grasslands"},
    {"POI_Mint_Container", "Candy Dispenser", "Grasslands"},
    {"POI_MysteriousLab", "Mysterious Lab", "Grasslands"},
    {"POI_Oak_Lab", "Oak Lab", "Grasslands"},
    {"POI_Shrinkpad", "Mysterious Machine", "Grasslands"},
    {"POI_YokedGirthHead", "Yoked Girth Head", "Grasslands"},
    {"POI_Defense_Point_RedAnthill", "MIX.R Central Grasslands", "Grasslands"},

    // Hedge (8)
    {"POI_FlyingDisc", "Flingman Flying Disc", "Hedge"},
    {"POI_Field_Station_Hedge", "Hedge Field Station", "Hedge"},
    {"POI_Can_FizzamChuberry", "Fizzam Can (Chuberry)", "Hedge"},
    {"POI_Hedge_Ascent", "Hedge Ascent", "Hedge"},
    {"POI_Hedge_Lab", "Hedge Lab", "Hedge"},
    {"POI_Hedge_Lab_Destroyed_Entrance", "Hedge Lab Breach", "Hedge"},
    {"POI_Juicebox_Apricop", "Apricop Punch-O", "Hedge"},
    {"POI_Defense_Point_Hedge", "MIX.R Hedge", "Hedge"},

    // Pond (10)
    {"POI_BottomPondBackEntry", "Depths Mouth", "Pond"},
    {"POI_BottomPondLabEntrance", "Pond Hatch", "Pond"},
    {"POI_BottomPondTrex", "Sunken T-Rex", "Pond"},
    {"POI_Can_CalvoRootBeer", "Calvo Can", "Pond"},
    {"POI_Field_Station_UpperPond", "Upper Pond Field Station", "Pond"},
    {"POI_PondLabDome", "Pond Dome", "Pond"},
    {"POI_PondLabExit", "Pond Lab", "Pond"},
    {"POI_PondPagoda", "Pond Pagoda Tent", "Pond"},
    {"POI_PondPlantPot", "Sunken Pot", "Pond"},
    {"POI_WeddingRing", "Wedding Ring", "Pond"},

    // Haze (5)
    {"POI_Defense_Point_Haze", "MIX.R Haze", "Haze"},
    {"POI_ExposedPipe_Haze", "Exposed Pipe (Haze)", "Haze"},
    {"POI_Field_Station_Haze", "Haze Field Station", "Haze"},
    {"POI_Haze_Lab", "Haze Lab", "Haze"},
    {"POI_Juicebox_PeachFuzz", "Peach Fuzz Punch-O", "Haze"},

    // Picnic Table (6)
    {"POI_ExposedPipe_WeedArea", "Exposed Pipe", "Picnic Table"},
    {"POI_Field_Station_PicnicTable", "Picnic Table Field Station", "Picnic Table"},
    {"POI_MilkMolarBottle", "Milk Molar Bottle", "Picnic Table"},
    {"POI_PicnicTable_Cooler", "Cooler Box", "Picnic Table"},
    {"POI_PicnicTable_Shovel", "Leaning Shovel", "Picnic Table"},
    {"POI_Picnic_Dungeon", "Minotaur Maze's End", "Picnic Table"},

    // Trash Heap (6)
    {"POI_BAH_TrashEntrance", "Black Anthill Descent", "Trash Heap"},
    {"POI_BAH_Lab", "Black Anthill Lab", "Trash Heap"},
    {"POI_Can_CrushedLaBise", "Crushed La Bise Can", "Trash Heap"},
    {"POI_Field_Station_Trash", "Trash Heap Field Station", "Trash Heap"},
    {"POI_Trash_CRT", "Trashed AARTZ 3000", "Trash Heap"},
    {"POI_YokedGirthPackaging", "Yoked Girth Blister Pack", "Trash Heap"},

    // Sandbox (4)
    {"POI_BAH_SandboxEntrance", "Black Anthill Descent (Sandbox)", "Sandbox"},
    {"POI_Defense_Point_Sandbox", "MIX.R Sandbox", "Sandbox"},
    {"POI_Field_Station_Sandbox", "Sandbox Field Station", "Sandbox"},
    {"POI_Sandbox_LabOutpost", "Sandbox Lab Outpost", "Sandbox"},

    // Upper Grasslands (41)
    {"POI_AntHill_Jungle", "Jungle Anthill", "Upper Grasslands"},
    {"POI_AntHill_Lawnmower", "Lawnmower Anthill", "Upper Grasslands"},
    {"POI_Ashtray", "Dirty Ashtray", "Upper Grasslands"},
    {"POI_Bat", "Bomber Baseball Bat", "Upper Grasslands"},
    {"POI_Bike_Thor", "Samsquanch Dirt Bike", "Upper Grasslands"},
    {"POI_Can_FizzamBlueberry", "Fizzam Can (Blueberry)", "Upper Grasslands"},
    {"POI_Can_FizzamPamplemousse", "Fizzam Can (Pamplemousse)", "Upper Grasslands"},
    {"POI_Cap_Gun", "Buster Cap Gun", "Upper Grasslands"},
    {"POI_Castle_Moldorc_Vista", "Castle Moldorc Playset", "Upper Grasslands"},
    {"POI_CharcoalHotSpring", "Charcoal Hot Springs", "Upper Grasslands"},
    {"POI_CootieCatcher", "Cootie Catcher", "Upper Grasslands"},
    {"POI_CrackedDirt_Glove", "Abandoned Gardening Glove", "Upper Grasslands"},
    {"POI_Defense_Point_Super_Jungle", "Super MIX.R (Jungle)", "Upper Grasslands"},
    {"POI_Defense_Point_Super_Lawnmower", "Super MIX.R (Lawnmower)", "Upper Grasslands"},
    {"POI_Defense_Point_Super_MiniHegde", "Super MIX.R (Mini Hedge)", "Upper Grasslands"},
    {"POI_Defense_Point_WesternStep", "MIX.R Western Step", "Upper Grasslands"},
    {"POI_Field_Station_BrickWall", "BBQ Field Station", "Upper Grasslands"},
    {"POI_Field_Station_EastSteps", "East Steps Field Station", "Upper Grasslands"},
    {"POI_Field_Station_JavaMatic", "JavaMatic Field Station", "Upper Grasslands"},
    {"POI_Field_Station_Jungle", "Jungle Field Station", "Upper Grasslands"},
    {"POI_Field_Station_Junkyard", "Junkyard Field Station", "Upper Grasslands"},
    {"POI_Field_Station_Oak", "Oak Field Station", "Upper Grasslands"},
    {"POI_Field_Station_StorageBox", "Storage Box Field Station", "Upper Grasslands"},
    {"POI_Field_Station_Stump", "Stump Field Station", "Upper Grasslands"},
    {"POI_Field_Station_Termite_Wood_Pile", "Woodpile Field Station", "Upper Grasslands"},
    {"POI_JavaMatic", "JavaMatic", "Upper Grasslands"},
    {"POI_JavamaticOutPost", "JavaMatic Storage Facility", "Upper Grasslands"},
    {"POI_Lawnmower", "Lawnmower", "Upper Grasslands"},
    {"POI_SCAB_ShedPorch", "Full-Sized SCA.B", "Upper Grasslands"},
    {"POI_ShristyToyCar", "Christy Doll Car", "Upper Grasslands"},
    {"POI_StumpLab", "Stump Lab Outpost", "Upper Grasslands"},
    {"POI_TazTLarge", "Taz.T! Statue", "Upper Grasslands"},
    {"POI_Upperyard_Ascent", "Upper Yard Ascent", "Upper Grasslands"},
    {"POI_Wheelbarrow", "Wheelbarrow", "Upper Grasslands"},
    {"POI_WoodPileEntrance", "Wood Pile Entrance", "Upper Grasslands"},
    {"POI_WormHoleContainer", "Wormhole Container", "Upper Grasslands"},
    {"POI_YogiSwoleHead", "Yogi Swole Head", "Upper Grasslands"},
    {"POI_BrawnyBoxEntrance", "Brawny Boy Bin Entrance", "Upper Grasslands"},
    {"POI_RottedStump", "Stump", "Upper Grasslands"},
    {"POI_WaspNest", "Wasp Nest", "Upper Grasslands"},
    {"POI_NewGamePlusMachine", "MIX.R", "Upper Grasslands"},

    // Undershed (4)
    {"POI_Field_Station_Undershed", "Undershed Field Station", "Undershed"},
    {"POI_Undershed_Entrance", "Undershed Entrance", "Undershed"},
    {"POI_Undershed_Lab", "Undershed Lab", "Undershed"},
    {"POI_Undershed_Sinkhole", "Undershed Sinkhole", "Undershed"},
};
static constexpr size_t LANDMARK_COUNT = sizeof(LANDMARK_DATA) / sizeof(LANDMARK_DATA[0]);

void ReadLandmarksFromMemory(uintptr_t gameState, ProgressSnapshot& snap) {
    snap.landmarks.clear();
    snap.landmarks.reserve(LANDMARK_COUNT);

    // Populate all landmarks as not-discovered
    for (size_t i = 0; i < LANDMARK_COUNT; ++i) {
        snap.landmarks.push_back({
            LANDMARK_DATA[i].id,
            LANDMARK_DATA[i].name,
            LANDMARK_DATA[i].zone,
            false
        });
    }

    if (!gameState) return;

    // PartyComponent -> DiscoveredPOIs (TArray<UPointOfInterestDataAsset*>)
    auto partyComp = Memory::SafeReadPtr(gameState, Offsets::GameState_PartyComponent);
    if (!partyComp) {
        LOG_DEBUG("ReadLandmarks: PartyComponent null");
        return;
    }

    auto arr = ReadTArray(partyComp.value(), Offsets::PartyComp_DiscoveredPOIs);
    if (!arr) {
        LOG_DEBUG("ReadLandmarks: DiscoveredPOIs array not readable");
        return;
    }

    uint32_t matched = 0;
    for (int32_t i = 0; i < arr->count; ++i) {
        auto assetPtr = Memory::SafeReadPtr(arr->data, i * 8);
        if (!assetPtr) continue;

        // UObject::NamePrivate at offset 0x18
        std::string poiName = ResolveFNameAt(assetPtr.value(), 0x18);
        if (poiName.empty()) continue;

        // Match against master list
        for (auto& lm : snap.landmarks) {
            if (lm.id == poiName) {
                lm.discovered = true;
                matched++;
                break;
            }
        }
    }

    LOG_DEBUG("ReadLandmarks: %u/%zu matched from %d discovered POIs",
              matched, LANDMARK_COUNT, arr->count);
}
