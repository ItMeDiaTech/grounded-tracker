#include "creature_cards.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"
#include <algorithm>
#include <string>

// 68 creature card mappings — matches src-tauri/src/categories/creature_cards.rs
// IDs are our internal identifiers; FName matching uses case-insensitive contains
static const struct { const char* id; const char* name; } CARD_DATA[] = {
    {"ant_red_worker", "Red Worker Ant"},
    {"ant_red_soldier", "Red Soldier Ant"},
    {"ant_black_worker", "Black Worker Ant"},
    {"ant_black_soldier", "Black Soldier Ant"},
    {"ant_fire_worker", "Fire Worker Ant"},
    {"ant_fire_soldier", "Fire Soldier Ant"},
    {"ant_fire_major", "Fire Major Ant"},
    {"ant_flying", "Flying Ant"},
    {"aphid", "Aphid"},
    {"bee", "Bee"},
    {"beetle_ladybug", "Ladybug"},
    {"beetle_ladybug_larva", "Ladybug Larva"},
    {"beetle_rhinoceros", "Rhinoceros Beetle"},
    {"beetle_roly_poly", "Roly Poly"},
    {"beetle_dung", "Stinkbug"},
    {"black_ox_beetle", "Black Ox Beetle"},
    {"black_widow", "Black Widow"},
    {"broodmother", "Hedge Broodmother"},
    {"butterfly", "Butterfly"},
    {"crow", "Crow"},
    {"dust_mite", "Dust Mite"},
    {"firefly", "Firefly"},
    {"flea", "Flea"},
    {"gnat", "Gnat"},
    {"grasshopper", "Grasshopper"},
    {"grub", "Grub"},
    {"infected_ladybug", "Infected Ladybug"},
    {"infected_mite", "Infected Mite"},
    {"infected_weevil", "Infected Weevil"},
    {"infected_wolf_spider", "Infected Wolf Spider"},
    {"infected_broodmother", "Infected Broodmother"},
    {"koi_fish", "Koi Fish"},
    {"larva", "Larva"},
    {"lawn_mite", "Lawn Mite"},
    {"mantis", "Mantis"},
    {"midge", "Midge"},
    {"mosquito", "Mosquito"},
    {"moth", "Moth"},
    {"orb_weaver", "Orb Weaver"},
    {"orb_weaver_baby", "Orb Weaver Jr."},
    {"pill_bug", "Pill Bug"},
    {"pupa", "Pupa"},
    {"scarab", "Green Shield Bug"},
    {"schmector", "Director Schmector"},
    {"silverfish", "Silverfish"},
    {"slug", "Slug"},
    {"snail", "Snail"},
    {"spider_jumping", "Jumping Spider"},
    {"spider_wolf", "Wolf Spider"},
    {"spider_wolf_baby", "Wolf Spider Spiderling"},
    {"spiderling", "Spiderling"},
    {"stinkbug", "Stinkbug"},
    {"tadpole", "Tadpole"},
    {"termite_worker", "Termite Worker"},
    {"termite_soldier", "Termite Soldier"},
    {"termite_king", "Termite King"},
    {"tick", "Tick"},
    {"assistant_manager", "Assistant Manager"},
    {"water_boatman", "Water Boatman"},
    {"water_flea", "Water Flea"},
    {"wasp", "Wasp"},
    {"wasp_queen", "Wasp Queen"},
    {"weevil", "Weevil"},
    {"widow_spiderling", "Black Widow Spiderling"},
    {"worm", "Worm"},
    {"mosquito_nymph", "Mosquito Nymph"},
    {"diving_bell_spider", "Diving Bell Spider"},
    {"tiger_mosquito", "Tiger Mosquito"},
};
static constexpr size_t CARD_COUNT = sizeof(CARD_DATA) / sizeof(CARD_DATA[0]);

// Convert string to lowercase for matching
static std::string ToLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

// Try to match a resolved FName against our card IDs
// The game's FName might be e.g. "Ant_Red_Worker" while our ID is "ant_red_worker"
static int FindCardIndex(const std::string& fname) {
    std::string fnameLower = ToLower(fname);
    // Replace spaces/dashes with underscores for flexible matching
    for (auto& c : fnameLower) {
        if (c == ' ' || c == '-') c = '_';
    }

    for (size_t i = 0; i < CARD_COUNT; ++i) {
        if (fnameLower == CARD_DATA[i].id) return static_cast<int>(i);
    }

    // Try substring matching — FName might have prefixes like "CreatureCard_"
    for (size_t i = 0; i < CARD_COUNT; ++i) {
        if (fnameLower.find(CARD_DATA[i].id) != std::string::npos) return static_cast<int>(i);
    }

    return -1;
}

void ReadCreatureCardsFromMemory(uintptr_t playerState, ProgressSnapshot& snap) {
    // Populate all cards as not-collected by default
    snap.creatureCards.clear();
    snap.creatureCards.reserve(CARD_COUNT);

    for (size_t i = 0; i < CARD_COUNT; ++i) {
        snap.creatureCards.push_back({
            CARD_DATA[i].id,
            CARD_DATA[i].name,
            false
        });
    }

    if (!playerState) return;

    // Approach 1: Read InspectedItems from PlayerState
    // TArray<FDataTableRowHandle_NetCrc> — 12 bytes each: FName(8) + CRC(4)
    auto arr = ReadTArray(playerState, Offsets::PlayerState_InspectedItems);
    if (arr && arr->count > 0) {
        static constexpr size_t ROW_HANDLE_SIZE = 12;
        uint32_t matched = 0;

        static bool loggedOnce = false;
        if (!loggedOnce) {
            LOG_INFO("=== InspectedItems Dump (%d entries) ===", arr->count);
        }

        for (int32_t i = 0; i < arr->count; ++i) {
            uintptr_t elemAddr = arr->data + i * ROW_HANDLE_SIZE;
            std::string rowName = ResolveFNameAt(elemAddr, 0);
            if (rowName.empty()) continue;

            if (!loggedOnce) {
                LOG_INFO("  InspectedItem[%d]: '%s'", i, rowName.c_str());
            }

            int idx = FindCardIndex(rowName);
            if (idx >= 0) {
                snap.creatureCards[idx].collected = true;
                matched++;
            }
        }

        if (!loggedOnce) {
            LOG_INFO("=== End InspectedItems Dump (matched %u/%zu cards) ===",
                     matched, CARD_COUNT);
            loggedOnce = true;
        }

        if (matched > 0) {
            LOG_DEBUG("CreatureCards: %u/%zu from InspectedItems (%d total inspected)",
                      matched, CARD_COUNT, arr->count);
            return;  // InspectedItems worked, skip bestiary fallback
        }
    }

    // Approach 2: Try BestiaryComponent on GameState
    // Read the stats array and try to resolve creature names
    // This is exploratory — FBestiaryStat layout is unknown
    // GameState is reachable from the engine, but we don't have it here.
    // For now, log diagnostics if InspectedItems didn't match anything.
    LOG_DEBUG("CreatureCards: InspectedItems yielded no matches, "
              "creature card tracking requires further investigation");
}
