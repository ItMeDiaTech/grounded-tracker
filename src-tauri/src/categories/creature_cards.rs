use std::collections::HashSet;

use crate::models::{CategoryProgress, CreatureCardItem};

const TOTAL_CREATURE_CARDS: u32 = 68;

/// Map from PartyComponent Bestiary row names to (frontend_id, display_name).
/// Row names come from World.csav's PartyComponent discovery array,
/// filtered to entries starting with "Bestiary" (excluding "Gold" suffix).
const CARD_MAPPINGS: &[(&str, &str, &str)] = &[
    // (save_row_name, frontend_id, display_name)
    ("BestiaryAntRedWorker", "ant_red_worker", "Red Worker Ant"),
    ("BestiaryAntBlackWorker", "ant_black_worker", "Black Worker Ant"),
    ("BestiaryLadybug", "beetle_ladybug", "Ladybug"),
    ("BestiaryBee", "bee", "Bee"),
    ("BestiaryFirefly", "firefly", "Firefly"),
    ("BestiaryRolyPolySickly", "sickly_roly_poly", "Sickly Roly Poly"),
    ("BestiaryRolyPoly", "beetle_roly_poly", "Roly Poly"),
    ("BestiaryMoth", "moth", "Moth"),
    ("BestiaryAntRedSoldier", "ant_red_soldier", "Red Soldier Ant"),
    ("BestiaryMite", "lawn_mite", "Lawn Mite"),
    ("BestiaryLarva", "larva", "Larva"),
    ("BestiaryTazTRuzT", "ruzt", "RUZ.T"),
    ("BestiaryWaterFlea", "water_flea", "Water Flea"),
    ("BestiarySpiderOrbJr", "orb_weaver_jr", "Orb Weaver Jr."),
    ("BestiarySpiderling", "spiderling", "Spiderling"),
    ("BestiaryAntBlackSoldier", "ant_black_soldier", "Black Soldier Ant"),
    ("BestiaryTazT", "tayzt", "TAYZ.T"),
    ("BestiaryArcR", "arcr", "ARC.R"),
    ("BestiaryMosquito", "mosquito", "Mosquito"),
    ("BestiaryWeevilInfected", "infected_weevil", "Infected Weevil"),
    ("BestiaryMiteInfected", "infected_mite", "Infected Mite"),
    ("BestiaryLadybugInfected", "infected_ladybug", "Infected Ladybug"),
    ("BestiaryLarvaInfected", "infected_larva", "Infected Larva"),
    ("BestiaryGnatInfected", "infected_gnat", "Infected Gnat"),
    ("BestiaryStinkbug", "beetle_stinkbug", "Stinkbug"),
    ("BestiarySpiderOrb", "orb_weaver", "Orb Weaver"),
    ("BestiarySpiderDivingBell", "diving_bell_spider", "Diving Bell Spider"),
    ("BestiarySpiderWolf", "wolf_spider", "Wolf Spider"),
    ("BestiaryBombardier", "beetle_bombardier", "Bombardier Beetle"),
    ("BestiaryAntFireWorker", "ant_fire_worker", "Fire Worker Ant"),
    ("BestiaryAntFireSoldier", "ant_fire_soldier", "Fire Soldier Ant"),
    ("BestiaryMiteDust", "dust_mite", "Dust Mite"),
    ("BestiaryLarvaCharcoal", "ladybird_larva", "Ladybird Larva"),
    ("BestiaryAntlion", "ant_lion", "Antlion"),
    ("BestiaryTermiteWorker", "termite_worker", "Termite Worker"),
    ("BestiaryTermiteSoldier", "termite_soldier", "Termite Soldier"),
    ("BestiaryTermiteKing", "termite_king", "Termite King"),
    ("BestiaryBlackOx", "black_ox_beetle", "Black Ox Beetle"),
    ("BestiaryLadybugUpperyard", "ladybird", "Ladybird"),
    ("BestiaryStinkbugGreenShield", "green_shield_bug", "Green Shield Bug"),
    ("BestiarySpiderWolfInfected", "infected_wolf_spider", "Infected Wolf Spider"),
    ("BestiarySpiderBlackWidow", "black_widow", "Black Widow"),
    ("BestiaryWaterFleaSpiny", "spiny_water_flea", "Spiny Water Flea"),
    ("BestiaryMosquitoTiger", "tiger_mosquito", "Tiger Mosquito"),
    ("BestiaryMiteTick", "tick", "Tick"),
    ("BestiarySpiderlingWidow", "black_widowling", "Black Widowling"),
    ("BestiaryWasp", "wasp", "Wasp"),
    ("BestiaryWaspDrone", "wasp_drone", "Wasp Drone"),
    ("BestiaryArcRBoss", "assistant_manager", "Assistant Manager"),
    ("BestiarySpiderBossBroodmother", "broodmother", "Broodmother"),
    ("BestiaryMant", "mant", "Mant"),
    ("BestiaryMantis", "orchid_mantis", "Orchid Mantis"),
    ("BestiarySchmector", "director_schmector", "Director Schmector"),
    ("BestiaryWaspQueen", "wasp_queen", "Wasp Queen"),
    ("BestiaryBossIBM", "infected_broodmother", "Infected Broodmother"),
    ("BestiaryKoiDummy", "koi_fish", "Koi Fish"),
    ("BestiaryAphid", "aphid", "Aphid"),
    ("BestiaryWeevil", "weevil", "Weevil"),
    ("BestiaryGrub", "grub", "Grub"),
    ("BestiaryGnat", "gnat", "Gnat"),
    ("BestiaryGnatMeaty", "meaty_gnat", "Meaty Gnat"),
    ("BestiaryTadpole", "tadpole", "Tadpole"),
    ("BestiaryWaterBoatman", "water_boatman", "Water Boatman"),
    ("BestiaryScarab", "scarab", "Scarab"),
    ("BestiaryAntQueenRedDummy", "red_ant_queen", "Red Ant Queen"),
    ("BestiaryAntQueenBlackDummy", "black_ant_queen", "Black Ant Queen"),
    ("BestiaryAntQueenFireDummy", "fire_ant_queen", "Fire Ant Queen"),
    ("BestiaryCrowDummy", "crow", "Crow"),
];

pub fn extract_creature_cards(
    card_ids: &[String],
    achievements: &HashSet<String>,
    progress: &mut CategoryProgress<CreatureCardItem>,
) {
    let all_collected = achievements.contains("AllCreatureCards");

    // Build a set of found card IDs for quick lookup
    let found_set: HashSet<&str> = card_ids.iter().map(|s| s.as_str()).collect();

    // Map found save IDs to frontend IDs
    let mut collected_frontend_ids: HashSet<&str> = HashSet::new();
    for &(save_pattern, frontend_id, _) in CARD_MAPPINGS {
        if all_collected || found_set.contains(save_pattern) {
            collected_frontend_ids.insert(frontend_id);
        }
    }

    // Build items list with per-card collected status
    let items: Vec<CreatureCardItem> = CARD_MAPPINGS
        .iter()
        .map(|&(_, frontend_id, display_name)| CreatureCardItem {
            id: frontend_id.to_string(),
            name: display_name.to_string(),
            collected: collected_frontend_ids.contains(frontend_id),
        })
        .collect();

    let collected_count = items.iter().filter(|c| c.collected).count() as u32;

    progress.items = items;
    progress.total_count = TOTAL_CREATURE_CARDS;
    progress.collected_count = collected_count;
}
