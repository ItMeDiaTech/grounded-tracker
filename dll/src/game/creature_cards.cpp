#include "creature_cards.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../util/logger.h"

// 68 creature card mappings — matches src-tauri/src/categories/creature_cards.rs
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

    // TODO: Read creature card collection state from UE4 memory after SDK generation
    // PlayerState -> CreatureCardsComponent -> CollectedArray -> match indices to CARD_DATA
    LOG_DEBUG("ReadCreatureCards — stub, offsets not populated");
}
