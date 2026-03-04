#include "creature_cards.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"
#include <algorithm>
#include <string>

// 68 creature card mappings — matches src/data/creature-cards.ts exactly
static const struct { const char* id; const char* name; } CARD_DATA[] = {
    // Neutral (8)
    {"ant_red_worker", "Red Worker Ant"},
    {"ant_black_worker", "Black Worker Ant"},
    {"beetle_ladybug", "Ladybug"},
    {"bee", "Bee"},
    {"firefly", "Firefly"},
    {"sickly_roly_poly", "Sickly Roly Poly"},
    {"beetle_roly_poly", "Roly Poly"},
    {"moth", "Moth"},
    // Angry (48)
    {"ant_red_soldier", "Red Soldier Ant"},
    {"lawn_mite", "Lawn Mite"},
    {"larva", "Larva"},
    {"ruzt", "RUZ.T"},
    {"water_flea", "Water Flea"},
    {"orb_weaver_jr", "Orb Weaver Jr."},
    {"spiderling", "Spiderling"},
    {"ant_black_soldier", "Black Soldier Ant"},
    {"tayzt", "TAYZ.T"},
    {"arcr", "ARC.R"},
    {"mosquito", "Mosquito"},
    {"infected_weevil", "Infected Weevil"},
    {"infected_mite", "Infected Mite"},
    {"infected_ladybug", "Infected Ladybug"},
    {"infected_larva", "Infected Larva"},
    {"infected_gnat", "Infected Gnat"},
    {"beetle_stinkbug", "Stinkbug"},
    {"orb_weaver", "Orb Weaver"},
    {"diving_bell_spider", "Diving Bell Spider"},
    {"wolf_spider", "Wolf Spider"},
    {"beetle_bombardier", "Bombardier Beetle"},
    {"ant_fire_worker", "Fire Worker Ant"},
    {"ant_fire_soldier", "Fire Soldier Ant"},
    {"dust_mite", "Dust Mite"},
    {"ladybird_larva", "Ladybird Larva"},
    {"ant_lion", "Antlion"},
    {"termite_worker", "Termite Worker"},
    {"termite_soldier", "Termite Soldier"},
    {"termite_king", "Termite King"},
    {"black_ox_beetle", "Black Ox Beetle"},
    {"ladybird", "Ladybird"},
    {"green_shield_bug", "Green Shield Bug"},
    {"infected_wolf_spider", "Infected Wolf Spider"},
    {"black_widow", "Black Widow"},
    {"spiny_water_flea", "Spiny Water Flea"},
    {"tiger_mosquito", "Tiger Mosquito"},
    {"tick", "Tick"},
    {"black_widowling", "Black Widowling"},
    {"wasp", "Wasp"},
    {"wasp_drone", "Wasp Drone"},
    {"assistant_manager", "Assistant Manager"},
    {"broodmother", "Hedge Broodmother"},
    {"mant", "Mant"},
    {"orchid_mantis", "Orchid Mantis"},
    {"director_schmector", "Director Schmector"},
    {"wasp_queen", "Wasp Queen"},
    {"infected_broodmother", "Infected Broodmother"},
    {"koi_fish", "Koi Fish"},
    // Harmless (12)
    {"aphid", "Aphid"},
    {"weevil", "Weevil"},
    {"grub", "Grub"},
    {"gnat", "Gnat"},
    {"meaty_gnat", "Meaty Gnat"},
    {"tadpole", "Tadpole"},
    {"water_boatman", "Water Boatman"},
    {"scarab", "Scarab"},
    {"red_ant_queen", "Red Ant Queen"},
    {"black_ant_queen", "Black Ant Queen"},
    {"fire_ant_queen", "Fire Ant Queen"},
    {"crow", "Crow"},
};
static constexpr size_t CARD_COUNT = sizeof(CARD_DATA) / sizeof(CARD_DATA[0]);

// Bestiary FName → card ID mapping — verified from live game data (100% save, 2026-03-04)
// All FNames sourced from OwnedKeyItems dump of 321 entries
static const struct { const char* bestiary; const char* cardId; } BESTIARY_MAP[] = {
    // --- Neutral (8 cards) ---
    {"BestiaryAntRedWorker", "ant_red_worker"},
    {"BestiaryAntBlackWorker", "ant_black_worker"},
    {"BestiaryLadybug", "beetle_ladybug"},
    {"BestiaryBee", "bee"},
    {"BestiaryFirefly", "firefly"},
    {"BestiaryRolyPolySickly", "sickly_roly_poly"},
    {"BestiaryRolyPoly", "beetle_roly_poly"},
    {"BestiaryMoth", "moth"},
    // --- Angry (48 cards) ---
    {"BestiaryAntRedSoldier", "ant_red_soldier"},
    {"BestiaryMite", "lawn_mite"},
    {"BestiaryLarva", "larva"},
    {"BestiaryTazTRuzT", "ruzt"},           // combined entry = RUZ.T card
    {"BestiaryTazTRuzTGold", "ruzt"},
    {"BestiaryTazT", "tayzt"},
    {"BestiaryTazTGold", "tayzt"},
    {"BestiaryWaterFlea", "water_flea"},
    {"BestiarySpiderOrbJr", "orb_weaver_jr"},
    {"BestiarySpiderOrbJrGold", "orb_weaver_jr"},
    {"BestiarySpiderling", "spiderling"},
    {"BestiaryAntBlackSoldier", "ant_black_soldier"},
    {"BestiaryArcR", "arcr"},
    {"BestiaryArcRGold", "arcr"},
    {"BestiaryMosquito", "mosquito"},
    {"BestiaryWeevilInfected", "infected_weevil"},
    {"BestiaryMiteInfected", "infected_mite"},
    {"BestiaryLadybugInfected", "infected_ladybug"},
    {"BestiaryLarvaInfected", "infected_larva"},
    {"BestiaryGnatInfected", "infected_gnat"},
    {"BestiaryStinkbug", "beetle_stinkbug"},
    {"BestiarySpiderOrb", "orb_weaver"},
    {"BestiarySpiderDivingBell", "diving_bell_spider"},
    {"BestiarySpiderWolf", "wolf_spider"},
    {"BestiaryBombardier", "beetle_bombardier"},
    {"BestiaryBombardierGold", "beetle_bombardier"},
    {"BestiaryAntFireWorker", "ant_fire_worker"},
    {"BestiaryAntFireSoldier", "ant_fire_soldier"},
    {"BestiaryMiteDust", "dust_mite"},
    {"BestiaryLarvaCharcoal", "ladybird_larva"},
    {"BestiaryAntlion", "ant_lion"},
    {"BestiaryTermiteWorker", "termite_worker"},
    {"BestiaryTermiteSoldier", "termite_soldier"},
    {"BestiaryTermiteKing", "termite_king"},
    {"BestiaryBlackOx", "black_ox_beetle"},
    {"BestiaryLadybugUpperyard", "ladybird"},
    {"BestiaryStinkbugGreenShield", "green_shield_bug"},
    {"BestiarySpiderWolfInfected", "infected_wolf_spider"},
    {"BestiarySpiderBlackWidow", "black_widow"},
    {"BestiaryWaterFleaSpiny", "spiny_water_flea"},
    {"BestiaryMosquitoTiger", "tiger_mosquito"},
    {"BestiaryMiteTick", "tick"},
    {"BestiarySpiderlingWidow", "black_widowling"},
    {"BestiaryWasp", "wasp"},
    {"BestiaryWaspDrone", "wasp_drone"},
    {"BestiaryArcRBoss", "assistant_manager"},   // boss ARC.R = Assistant Manager
    {"BestiaryArcRBossGold", "assistant_manager"},
    {"BestiarySpiderBossBroodmother", "broodmother"},
    {"BestiaryMant", "mant"},
    {"BestiaryMantGold", "mant"},
    {"BestiaryMantis", "orchid_mantis"},         // Mantis = Orchid Mantis
    {"BestiaryMantisGold", "orchid_mantis"},
    {"BestiarySchmector", "director_schmector"},
    {"BestiarySchmectorGold", "director_schmector"},
    {"BestiaryWaspQueen", "wasp_queen"},
    {"BestiaryBossIBM", "infected_broodmother"}, // IBM = Infected Brood Mother
    {"BestiaryKoiDummy", "koi_fish"},
    // --- Harmless (12 cards) ---
    {"BestiaryAphid", "aphid"},
    {"BestiaryWeevil", "weevil"},
    {"BestiaryGrub", "grub"},
    {"BestiaryGnat", "gnat"},
    {"BestiaryGnatMeaty", "meaty_gnat"},
    {"BestiaryTadpole", "tadpole"},
    {"BestiaryWaterBoatman", "water_boatman"},
    {"BestiaryScarab", "scarab"},
    {"BestiaryAntQueenRedDummy", "red_ant_queen"},
    {"BestiaryAntQueenBlackDummy", "black_ant_queen"},
    {"BestiaryAntQueenFireDummy", "fire_ant_queen"},
    {"BestiaryCrowDummy", "crow"},
};
static constexpr size_t BESTIARY_MAP_SIZE = sizeof(BESTIARY_MAP) / sizeof(BESTIARY_MAP[0]);

// Look up a bestiary FName in the mapping table
static int FindCardByBestiary(const std::string& bestiaryName) {
    for (size_t i = 0; i < BESTIARY_MAP_SIZE; ++i) {
        if (bestiaryName == BESTIARY_MAP[i].bestiary) {
            // Find the card ID in CARD_DATA
            for (size_t j = 0; j < CARD_COUNT; ++j) {
                if (std::string(CARD_DATA[j].id) == BESTIARY_MAP[i].cardId)
                    return static_cast<int>(j);
            }
        }
    }
    return -1;
}

void ReadCreatureCardsFromMemory(uintptr_t gameState, ProgressSnapshot& snap) {
    snap.creatureCards.clear();
    snap.creatureCards.reserve(CARD_COUNT);

    for (size_t i = 0; i < CARD_COUNT; ++i) {
        snap.creatureCards.push_back({
            CARD_DATA[i].id,
            CARD_DATA[i].name,
            false
        });
    }

    if (!gameState) return;

    // Creature cards are tracked via "Bestiary*" entries in PartyComponent->OwnedKeyItems
    // Same data source as table items, scan at 8-byte boundaries
    auto partyComp = Memory::SafeReadPtr(gameState, Offsets::GameState_PartyComponent);
    if (!partyComp) {
        LOG_DEBUG("CreatureCards: PartyComponent null");
        return;
    }

    auto arr = ReadTArray(partyComp.value(), Offsets::PartyComp_OwnedKeyItems);
    if (!arr || arr->count == 0) {
        LOG_DEBUG("CreatureCards: OwnedKeyItems empty or not readable");
        return;
    }

    // OwnedKeyItems is TArray<FDataTableRowHandle_NetCrc> — each element is 40 bytes (0x28).
    // RowName FName is at offset +0x08 within each element.
    uint32_t matched = 0;
    static bool loggedOnce = false;
    bool shouldLog = !loggedOnce;

    if (shouldLog) {
        LOG_INFO("=== CreatureCards Scan (from OwnedKeyItems, %d entries, stride=%zu) ===",
                 arr->count, Offsets::FDataTableRowHandle_NetCrc_Size);
    }

    for (int32_t i = 0; i < arr->count; ++i) {
        uintptr_t elem = arr->data + i * Offsets::FDataTableRowHandle_NetCrc_Size;
        std::string fname = ResolveFNameAt(elem, Offsets::FDataTableRowHandle_NetCrc_RowName);
        if (fname.empty()) continue;
        if (fname.find("Bestiary") != 0) continue;  // Only process Bestiary* entries

        int idx = FindCardByBestiary(fname);
        if (idx >= 0 && !snap.creatureCards[idx].collected) {
            snap.creatureCards[idx].collected = true;
            matched++;
        } else if (shouldLog && idx < 0) {
            LOG_DEBUG("CreatureCards: unmapped bestiary '%s'", fname.c_str());
        }
    }

    if (shouldLog) {
        LOG_INFO("=== End CreatureCards Scan (matched %u/%zu) ===", matched, CARD_COUNT);
        loggedOnce = true;
    }

    LOG_DEBUG("CreatureCards: %u/%zu from OwnedKeyItems scan", matched, CARD_COUNT);
}
