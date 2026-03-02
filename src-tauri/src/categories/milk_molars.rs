use std::collections::{HashMap, HashSet};

use crate::models::MilkMolarProgress;

pub fn extract_milk_molars(
    achievements: &HashSet<String>,
    achievement_progress: &HashMap<String, u32>,
    upgrade_levels: &HashMap<String, u32>,
    progress: &mut MilkMolarProgress,
) {
    progress.regular_total = 93;
    progress.mega_total = 14;

    let molars_complete = achievements.contains("SpendMolars");
    let spend_progress = achievement_progress.get("SpendMolars").copied().unwrap_or(0);

    // Total upgrade levels purchased (indirect molar spending metric)
    let total_upgrades: u32 = upgrade_levels.values().sum();
    progress.total_spent = spend_progress;

    if molars_complete {
        // SpendMolars achievement completed — all regular molars found and spent
        progress.regular_collected = progress.regular_total;
    } else {
        // Use SpendMolars progress as approximation (molars spent ≈ molars found)
        // Fall back to total upgrade levels if no spend data
        progress.regular_collected = if spend_progress > 0 {
            spend_progress.min(progress.regular_total)
        } else {
            total_upgrades.min(progress.regular_total)
        };
    }

    // Mega molars: if SpendMolars is complete, assume all mega molars found too.
    // Otherwise we can't detect individual mega molar collection.
    if molars_complete {
        progress.mega_collected = progress.mega_total;
    }
}
