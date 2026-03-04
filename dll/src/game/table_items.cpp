#include "table_items.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"
#include <string>

// Classification patterns — matches the Rust categories
static bool StartsWith(const std::string& s, const std::string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

static bool Contains(const std::string& s, const std::string& sub) {
    return s.find(sub) != std::string::npos;
}

enum class ItemCategory { Wendell, Ominent, Burgl, Stuff, Unknown };

static ItemCategory ClassifyItem(const std::string& rowName) {
    // Ominent
    if (StartsWith(rowName, "OminentInfiltrator_")) return ItemCategory::Ominent;
    if (StartsWith(rowName, "Note_OminentSpy_")) return ItemCategory::Ominent;
    if (rowName == "Ominent_Disk") return ItemCategory::Ominent;

    // BURG.L
    if (StartsWith(rowName, "TechChip_")) return ItemCategory::Burgl;
    if (StartsWith(rowName, "AudioLogBurgle")) return ItemCategory::Burgl;

    // Stuff
    if (StartsWith(rowName, "BossNote")) return ItemCategory::Stuff;
    if (StartsWith(rowName, "DuperDisc")) return ItemCategory::Stuff;
    if (StartsWith(rowName, "PasswordPiece")) return ItemCategory::Stuff;
    if (StartsWith(rowName, "Key_")) return ItemCategory::Stuff;
    if (rowName == "EmbiggeningCocktailFinal") return ItemCategory::Stuff;
    if (rowName == "KeyZiplineAscender") return ItemCategory::Stuff;
    if (StartsWith(rowName, "Recipe")) return ItemCategory::Stuff;
    if (rowName == "SCABManual") return ItemCategory::Stuff;
    if (rowName == "ScannerBracelet") return ItemCategory::Stuff;

    // Wendell (broad — everything audio/note that isn't Ominent or BURG.L)
    if (StartsWith(rowName, "AudioLogWendell")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "AudioLog_Hedge_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "AudioLog_Haze_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "AudioLog_Wendell_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "PondNote")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_Haze_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_Goodbye_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_Mant_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_StorageBox_")) return ItemCategory::Wendell;
    if (Contains(rowName, "Lab_Note")) return ItemCategory::Wendell;
    if (rowName == "NoteStumpLabEntrance") return ItemCategory::Wendell;
    if (rowName == "HedgeOverlookNote") return ItemCategory::Wendell;
    if (StartsWith(rowName, "HedgeOverlookNote")) return ItemCategory::Wendell;
    if (rowName == "HedgeLabNote") return ItemCategory::Wendell;
    if (rowName == "HedgeStorageNote") return ItemCategory::Wendell;
    if (StartsWith(rowName, "NoteRaisin")) return ItemCategory::Wendell;
    if (rowName == "FieldStationMixrNote") return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_HiddenKid_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_Wendell_")) return ItemCategory::Wendell;
    if (Contains(rowName, "GeoTentNote")) return ItemCategory::Wendell;
    if (rowName == "GrilledScience") return ItemCategory::Wendell;

    return ItemCategory::Unknown;
}

// --- Master item lists from frontend data files ---

// Wendell tapes (67 items) — src/data/wendell.ts
static const struct { const char* id; const char* name; const char* sub; } WENDELL_DATA[] = {
    {"AudioLogWendellA01", "A New Idea", "Time At Ominent"},
    {"AudioLogWendellA02", "Prototype Complete", "Time At Ominent"},
    {"AudioLogWendellA03", "SPAC.R Operational", "Time At Ominent"},
    {"AudioLogWendellA04", "Endless Possibilities", "Time At Ominent"},
    {"AudioLogWendellA05", "Recognition At Last!", "Time At Ominent"},
    {"AudioLogWendellA06", "Yet Another Failure", "Time At Ominent"},
    {"AudioLogWendellB01", "Fired!", "Time At Ominent"},
    {"AudioLogWendellB02", "An Auspicious Beginning", "SPAC.R Development"},
    {"AudioLogWendellB03", "Raw Science", "SPAC.R Development"},
    {"AudioLogWendellB04", "Embiggen Test 45", "SPAC.R Development"},
    {"AudioLogWendellB05", "Embiggen Test 50", "SPAC.R Development"},
    {"NoteRaisinHedge", "I'm Tiny!", "SPAC.R Development"},
    {"AudioLogWendellB06", "Embiggening Logistics", "SPAC.R Development"},
    {"AudioLogWendellB07", "SPAC.R Safety Test", "SPAC.R Development"},
    {"HedgeStorageNote", "Lab Modules", "SPAC.R Development"},
    {"HedgeLabNote", "A Little Exhausted", "SPAC.R Development"},
    {"AudioLog_Hedge_01", "Kaiju Children", "Hedge"},
    {"AudioLog_Hedge_02", "Spider Infestation", "Hedge"},
    {"AudioLog_Hedge_03", "Zip It", "Hedge"},
    {"AudioLog_Hedge_04", "Reboot", "Hedge"},
    {"AudioLog_Hedge_05", "Damage Control", "Hedge"},
    {"AudioLog_Hedge_06", "Re-Reboot", "Hedge"},
    {"HedgeOverlookNote", "Project ZIP.P", "Hedge"},
    {"HedgeOverlookNote_2", "Evacuation", "Hedge"},
    {"NoteRaisinHaze", "Pond Research Note", "Pond"},
    {"AudioLogWendellPond01", "Great Cabbage of Belgium", "Pond"},
    {"AudioLogWendellPond02", "Side Effects", "Pond"},
    {"PondNote01", "HAMS Test: Day 1", "Pond"},
    {"PondNote02", "Fight With Trudy", "Pond"},
    {"PondNote03", "HAMS Test: Day 7", "Pond"},
    {"PondNote04", "HAMS Test: Day 15", "Pond"},
    {"PondNote05", "Muscle Sprouts Test Kitchen", "Pond"},
    {"PondNote06", "HAMS Test: Day 36", "Pond"},
    {"PondNote07", "HAMS Test: Day 91", "Pond"},
    {"AudioLog_Haze_01", "Pipe Problems", "Haze"},
    {"AudioLog_Haze_02", "Trudy Blues", "Haze"},
    {"AudioLog_Haze_03", "Big Brain Time", "Haze"},
    {"Note_Haze_02", "Phase III Results", "Haze"},
    {"Note_Haze_03", "Blazed Earth Protocol", "Haze"},
    {"AudioLogWendellBAH01", "Under Siege", "Black Anthill"},
    {"AudioLogWendellBAH02", "Lab Lockdown", "Black Anthill"},
    {"BlackAntHill_Lab_Note01", "Extra Precautions", "Black Anthill"},
    {"BlackAntHill_Lab_Note02", "MIX.R Trial Summary", "Black Anthill"},
    {"BlackAntHill_Lab_Note03", "Security Briefing", "Black Anthill"},
    {"BlackAnthill_Lab_Note05", "Super Chip Protocol", "Black Anthill"},
    {"Note_StorageBox_01", "MIX.R Relocation Program", "Black Anthill"},
    {"AudioLogWendellU01", "Going Dark", "Undershed"},
    {"AudioLogWendellU02", "Raw Storage", "Undershed"},
    {"AudioLogWendellU03", "Last Resort", "Undershed"},
    {"AudioLogWendellU04", "Project MANT", "Undershed"},
    {"Note_Mant_01", "Project MANT: Condensed Science", "Undershed"},
    {"Note_Mant_02", "Project MANT: ZIP.P Fiber", "Undershed"},
    {"Note_Mant_03", "Project MANT: Test Drive 1", "Undershed"},
    {"Note_Mant_04", "Muscle Sprout Bonding: Test 18", "Undershed"},
    {"GrilledScience", "Grilled Science", "Undershed"},
    {"AudioLog_Wendell_SCAB", "Goodbye Wendell", "Misc"},
    {"Oak_Lab_Note1", "Imminent Collapse", "Misc"},
    {"NoteStumpLabEntrance", "CCTV Report", "Misc"},
    {"Note_HiddenKid_Message", "Exiled", "Misc"},
    {"Note_Goodbye_Trudy", "Goodbye", "Misc"},
    {"Note_Wendell_NewGamePlusMachine", "Research Notes: Quantum Toast", "Misc"},
    {"SPACR_GeoTentNote_01", "All Of These Eyes", "Misc"},
    {"OakTree_GeoTentNote01", "Local Fauna Report", "Misc"},
    {"Trash_GeoTentNote_01", "New Lab Reconnaissance", "Misc"},
    {"Sandbox_GeoTentNote_01", "Inventory Inspection", "Misc"},
    {"PicnicTable_GeoTentNote_01", "REMINDER: DO NOT OPEN THE BRAWNY BOY BIN!", "Misc"},
    {"FieldStationMixrNote", "Mixing Rigs", "Misc"},
};
static constexpr size_t WENDELL_COUNT = sizeof(WENDELL_DATA) / sizeof(WENDELL_DATA[0]);

// Ominent items (16) — src/data/ominent.ts
static const struct { const char* id; const char* name; const char* sub; } OMINENT_DATA[] = {
    {"Note_OminentSpy_001", "Recon Journal: Entomologist", "Recon Journals"},
    {"Note_OminentSpy_002", "Recon Journal: Compliance Officer", "Recon Journals"},
    {"Note_OminentSpy_003", "Recon Journal: Intern", "Recon Journals"},
    {"Note_OminentSpy_004", "Recon Journal: Special Forces", "Recon Journals"},
    {"Note_OminentSpy_005", "Recon Journal: Health And Safety Officer", "Recon Journals"},
    {"Note_OminentSpy_006_a", "A Sticky Situation", "Recon Journals"},
    {"Note_OminentSpy_006_b", "Loyal Subjects", "Recon Journals"},
    {"Note_OminentSpy_006_c", "Recon Journal: Toxicologist", "Recon Journals"},
    {"Note_OminentSpy_007", "Recon Journal: Biomedical Engineer", "Recon Journals"},
    {"OminentInfiltrator_Polaroid_Antenna", "Suspicious Snapshot: Antenna", "Suspicious Snapshots"},
    {"OminentInfiltrator_Polaroid_SPACR", "Suspicious Snapshot: SPAC.R", "Suspicious Snapshots"},
    {"OminentInfiltrator_Polaroid_SUPERCHIP", "Suspicious Snapshot: Super Chip", "Suspicious Snapshots"},
    {"OminentInfiltrator_Polaroid_SCAB", "Suspicious Snapshot: SCA.B", "Suspicious Snapshots"},
    {"OminentInfiltrator_Polaroid_JavaMatic", "Suspicious Snapshot: Java-Matic", "Suspicious Snapshots"},
    {"OminentInfiltrator_Polaroid_Muldorc", "Suspicious Snapshot: Moldorc", "Suspicious Snapshots"},
    {"Ominent_Disk", "Ominent Data Disk", "Data"},
};
static constexpr size_t OMINENT_COUNT = sizeof(OMINENT_DATA) / sizeof(OMINENT_DATA[0]);

// BURG.L chips (16) — src/data/burgl-chips.ts
static const struct { const char* id; const char* name; const char* sub; } BURGL_DATA[] = {
    {"AudioLogBurgleA01", "Order Number 1", "Transcription Units"},
    {"AudioLogBurgleA02", "Excellent Mustache", "Transcription Units"},
    {"AudioLogBurgleA03", "First Shrinking", "Transcription Units"},
    {"AudioLogBurgleA04", "Web Samples", "Transcription Units"},
    {"AudioLogBurgleA05", "Roby's Song", "Transcription Units"},
    {"TechChip_AntHill", "Red Anthill BURG.L Chip", "Tech Chips"},
    {"TechChip_PondOutpost", "Sunken Outpost BURG.L Chip", "Tech Chips"},
    {"TechChip_Mint", "Sandbox BURG.L Chip", "Tech Chips"},
    {"TechChip_PicnicTable", "Picnic BURG.L Chip", "Tech Chips"},
    {"TechChip_Woodpile", "Woodpile BURG.L Chip", "Tech Chips"},
    {"TechChip_Stump", "Stump BURG.L Chip", "Tech Chips"},
    {"TechChip_StorageBox", "Forgotten BURG.L Chip", "Tech Chips"},
    {"TechChip_Super_Hedge", "Hedge Super Chip", "Super Tech Chips"},
    {"TechChip_Super_Pond", "Pond Super Chip", "Super Tech Chips"},
    {"TechChip_Super_Weed", "Haze Super Chip", "Super Tech Chips"},
    {"TechChip_Super_AntHill_Black", "Black Ant Super Chip", "Super Tech Chips"},
};
static constexpr size_t BURGL_COUNT = sizeof(BURGL_DATA) / sizeof(BURGL_DATA[0]);

// Stuff items (33) — src/data/stuff.ts
static const struct { const char* id; const char* name; const char* sub; } STUFF_DATA[] = {
    {"ScannerBracelet", "SCA.B", "Products"},
    {"SCABManual", "SCA.B User Manual", "Products"},
    {"KeyZiplineAscender", "ZIP.R", "Products"},
    {"RecipeStatueAphid", "Aphid Figurine", "Statues"},
    {"RecipeStatueWeevil", "Weevil Figurine", "Statues"},
    {"RecipeStatueKoi", "Koi Figurine", "Statues"},
    {"RecipeStatueMant", "Mant Figurine", "Statues"},
    {"RecipeStatueWendell", "Wendell Figurine", "Statues"},
    {"RecipeStatueYokedGirth", "Yoked Girth Figurine", "Statues"},
    {"RecipeStatueMoldorc", "Moldorc Figurine", "Statues"},
    {"RecipeAntTotem", "Ant Totem Recipe", "Recipe Notes"},
    {"RecipeAbominationTotem", "Abomination Totem Recipe", "Recipe Notes"},
    {"RecipeSaltMorningStar", "Water Logged Recipe", "Recipe Notes"},
    {"RecipeBombArrows", "Water Logged Recipe", "Recipe Notes"},
    {"RecipeFridge", "Frostbitten Recipe", "Recipe Notes"},
    {"RecipeMantGong", "Glorious Recipe", "Recipe Notes"},
    {"PasswordPieceHedgeLab01", "Password Scribble: Y-", "Keys"},
    {"PasswordPieceHedgeLab02", "Password Scribble: -UR", "Keys"},
    {"PasswordPieceHedgeLab03", "Password Scribble: T19", "Keys"},
    {"PasswordPieceHedgeLab04", "Password Scribble: 58", "Keys"},
    {"Key_BlackAntHill", "Mossy Key", "Keys"},
    {"Key_MoatTreasureChest", "Melted Moat Key", "Keys"},
    {"Key_PondTreasureChest", "Sticky Key", "Keys"},
    {"Key_TableTreasureChest", "Minotaur Maze Key", "Keys"},
    {"Key_UpperYardTreasureChest", "ASST. Manager Keycard", "Keys"},
    {"EmbiggeningCocktailFinal", "Filled Embiggening Cell", "Keys"},
    {"BossNoteBroodmother", "Broodmother Research Notes", "Boss Notes"},
    {"BossNoteMantis", "Mantis Research Note", "Boss Notes"},
    {"BossNoteWasp", "Memo: O.R.C.", "Boss Notes"},
    {"BossNoteIBM", "Moldy Note", "Boss Notes"},
    {"DuperDisc1", "Duper Disc 1", "Super Duper"},
    {"DuperDisc2", "Duper Disc 2", "Super Duper"},
    {"DuperDisc3", "Duper Disc 3", "Super Duper"},
};
static constexpr size_t STUFF_COUNT = sizeof(STUFF_DATA) / sizeof(STUFF_DATA[0]);

void ReadTableItemsFromMemory(uintptr_t gameState, ProgressSnapshot& snap) {
    snap.wendell.clear();
    snap.ominent.clear();
    snap.burglChips.clear();
    snap.stuff.clear();

    // Populate all items as not-collected from master lists
    snap.wendell.reserve(WENDELL_COUNT);
    for (size_t i = 0; i < WENDELL_COUNT; ++i)
        snap.wendell.push_back({WENDELL_DATA[i].id, WENDELL_DATA[i].name, WENDELL_DATA[i].sub, false});

    snap.ominent.reserve(OMINENT_COUNT);
    for (size_t i = 0; i < OMINENT_COUNT; ++i)
        snap.ominent.push_back({OMINENT_DATA[i].id, OMINENT_DATA[i].name, OMINENT_DATA[i].sub, false});

    snap.burglChips.reserve(BURGL_COUNT);
    for (size_t i = 0; i < BURGL_COUNT; ++i)
        snap.burglChips.push_back({BURGL_DATA[i].id, BURGL_DATA[i].name, BURGL_DATA[i].sub, false});

    snap.stuff.reserve(STUFF_COUNT);
    for (size_t i = 0; i < STUFF_COUNT; ++i)
        snap.stuff.push_back({STUFF_DATA[i].id, STUFF_DATA[i].name, STUFF_DATA[i].sub, false});

    if (!gameState) return;

    // PartyComponent -> OwnedKeyItems (TArray<FDataTableRowHandle_NetCrc>)
    // Each element is 40 bytes (FDataTableRowHandle_NetCrc_Size stride)
    auto partyComp = Memory::SafeReadPtr(gameState, Offsets::GameState_PartyComponent);
    if (!partyComp) {
        LOG_DEBUG("ReadTableItems: PartyComponent null");
        return;
    }

    auto arr = ReadTArray(partyComp.value(), Offsets::PartyComp_OwnedKeyItems);
    if (!arr) {
        LOG_DEBUG("ReadTableItems: OwnedKeyItems array not readable");
        return;
    }

    // OwnedKeyItems is TArray<FDataTableRowHandle_NetCrc> — each element is 40 bytes (0x28).
    // Layout per element: DataTable*(0x00, 8) + RowName FName(0x08, 8) + NetCrc padding(0x10, 24)
    uint32_t matched = 0;
    static bool loggedOnce = false;
    bool shouldLog = !loggedOnce && arr->count > 0;

    if (shouldLog) {
        LOG_INFO("=== OwnedKeyItems Scan (%d entries, stride=%zu) ===",
                 arr->count, Offsets::FDataTableRowHandle_NetCrc_Size);
    }

    for (int32_t i = 0; i < arr->count; ++i) {
        uintptr_t elem = arr->data + i * Offsets::FDataTableRowHandle_NetCrc_Size;
        std::string rowName = ResolveFNameAt(elem, Offsets::FDataTableRowHandle_NetCrc_RowName);
        if (rowName.empty()) continue;
        if (rowName.find("None") == 0) continue;

        if (shouldLog) {
            LOG_DEBUG("  OwnedKeyItem[%d]: '%s'", i, rowName.c_str());
        }

        ItemCategory cat = ClassifyItem(rowName);

        auto markCollected = [&](std::vector<ProgressSnapshot::CollectibleInfo>& list) -> bool {
            for (size_t j = 0; j < list.size(); ++j) {
                if (!list[j].collected && list[j].id == rowName) {
                    list[j].collected = true;
                    matched++;
                    return true;
                }
            }
            return false;
        };

        switch (cat) {
            case ItemCategory::Wendell:  markCollected(snap.wendell); break;
            case ItemCategory::Ominent:  markCollected(snap.ominent); break;
            case ItemCategory::Burgl:    markCollected(snap.burglChips); break;
            case ItemCategory::Stuff:    markCollected(snap.stuff); break;
            case ItemCategory::Unknown:
                break;
        }
    }

    if (shouldLog) {
        LOG_INFO("=== End OwnedKeyItems Scan (matched %u) ===", matched);
        loggedOnce = true;
    }

    LOG_DEBUG("ReadTableItems: %u matched from %d owned key items "
              "(W:%zu O:%zu B:%zu S:%zu)",
              matched, arr->count,
              WENDELL_COUNT, OMINENT_COUNT, BURGL_COUNT, STUFF_COUNT);
}
