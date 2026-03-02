use std::collections::HashSet;

use crate::models::{BossItem, CategoryProgress};

/// Boss achievement flag → (display name, location)
const BOSS_DATA: &[(&str, &str, &str)] = &[
    ("KillBroodmother", "Hedge Broodmother", "Hedge Lab"),
    (
        "KillInfectedBroodmother",
        "Infected Broodmother",
        "Haze Lab",
    ),
    ("KillWaspQueen", "Wasp Queen", "Upper Yard"),
    ("KillDirector", "Assistant Manager", "Black Ant Lab"),
    ("KillMantis", "Mantis", "Undershed Lab"),
];

pub fn extract_bosses(
    achievements: &HashSet<String>,
    progress: &mut CategoryProgress<BossItem>,
) {
    let items: Vec<BossItem> = BOSS_DATA
        .iter()
        .map(|(flag, name, _location)| {
            let defeated = achievements.contains(*flag);
            BossItem {
                id: flag.to_lowercase(),
                name: name.to_string(),
                defeated,
            }
        })
        .collect();

    let collected = items.iter().filter(|b| b.defeated).count() as u32;
    progress.items = items;
    progress.total_count = BOSS_DATA.len() as u32;
    progress.collected_count = collected;
}
