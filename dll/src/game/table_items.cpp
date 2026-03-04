#include "table_items.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../util/logger.h"
#include <string>

// Classification patterns — matches the Rust categories
// Wendell: AudioLogWendell*, AudioLog_Hedge_*, AudioLog_Haze_*, Note_* variants, etc.
// Ominent: OminentInfiltrator_*, Note_OminentSpy_*, Ominent_Disk
// BURG.L: TechChip_*, AudioLogBurgle*
// Stuff: BossNote*, DuperDisc*, PasswordPiece*, Key_*, EmbiggeningCocktailFinal,
//        KeyZiplineAscender, Recipe*, SCABManual, ScannerBracelet

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
    if (rowName == "PondNote") return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_Haze_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_Goodbye_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_Mant_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_StorageBox_")) return ItemCategory::Wendell;
    if (Contains(rowName, "Lab_Note")) return ItemCategory::Wendell;
    if (rowName == "NoteStumpLab") return ItemCategory::Wendell;
    if (rowName == "HedgeOverlookNote") return ItemCategory::Wendell;
    if (rowName == "HedgeLabNote") return ItemCategory::Wendell;
    if (rowName == "HedgeStorageNote") return ItemCategory::Wendell;
    if (rowName == "NoteRaisin") return ItemCategory::Wendell;
    if (rowName == "FieldStationMixrNote") return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_HiddenKid_")) return ItemCategory::Wendell;
    if (StartsWith(rowName, "Note_Wendell_")) return ItemCategory::Wendell;
    if (rowName == "GeoTentNote") return ItemCategory::Wendell;
    if (rowName == "GrilledScience") return ItemCategory::Wendell;

    return ItemCategory::Unknown;
}

void ReadTableItemsFromMemory(uintptr_t gameState, ProgressSnapshot& snap) {
    snap.wendell.clear();
    snap.ominent.clear();
    snap.burglChips.clear();
    snap.stuff.clear();

    if (!gameState) return;

    // TODO: Read table items from UE4 memory after SDK generation
    // GameState -> TableItems (TArray<FTableItemState>) -> RowName + Collected
    // Classify each item and populate the snapshot
    LOG_DEBUG("ReadTableItems — stub, offsets not populated");
}
