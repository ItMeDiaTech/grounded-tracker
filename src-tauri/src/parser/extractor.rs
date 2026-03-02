use crate::errors::AppError;
use crate::models::*;
use crate::parser::achievements::extract_achievement_flags;
use crate::parser::creature_cards::extract_creature_card_ids;
use crate::parser::decompress::decompress_csav;
use crate::parser::poi::extract_poi_ids;
use crate::parser::table_items::extract_table_item_names;
use crate::parser::themes::extract_theme_ids;
use crate::parser::upgrades::extract_upgrade_levels;
use std::collections::{HashMap, HashSet};
use std::fs;
use std::path::Path;

/// Extract progress data from a save folder.
/// HostPlayer.csav is the primary data source (achievements, POIs, themes).
/// World.csav provides supplementary data (creature cards).
pub fn extract_save_progress(save_dir: &Path) -> Result<SaveProgress, AppError> {
    let world_path = save_dir.join("World.csav");
    let host_player_path = save_dir.join("HostPlayer.csav");

    let mut progress = SaveProgress {
        save_name: save_dir
            .file_name()
            .map(|n| n.to_string_lossy().to_string())
            .unwrap_or_default(),
        save_path: save_dir.to_string_lossy().to_string(),
        ..Default::default()
    };

    // Get last modified time from whichever file is newer
    for path in [&host_player_path, &world_path] {
        if let Ok(metadata) = fs::metadata(path) {
            if let Ok(modified) = metadata.modified() {
                let datetime: chrono::DateTime<chrono::Local> = modified.into();
                let ts = datetime.format("%Y-%m-%d %H:%M:%S").to_string();
                if ts > progress.last_modified {
                    progress.last_modified = ts;
                }
            }
        }
    }

    // Decompress World.csav for supplementary data
    let world_decompressed = if world_path.exists() {
        let world_data = fs::read(&world_path)?;
        Some(decompress_csav(&world_data)?)
    } else {
        None
    };

    // Parse HostPlayer.csav — primary data source
    if host_player_path.exists() {
        let host_data = fs::read(&host_player_path)?;
        let decompressed = decompress_csav(&host_data)?;

        // Step 1: Extract structured data from HostPlayer
        let (achievements, achievement_progress) = extract_achievement_flags(&decompressed);
        let discovered_pois = extract_poi_ids(&decompressed);
        let collected_themes = extract_theme_ids(&decompressed);
        let upgrade_levels = extract_upgrade_levels(&decompressed);

        // Creature cards live in World.csav's PartyComponent, not HostPlayer
        let creature_card_ids = world_decompressed
            .as_deref()
            .map(extract_creature_card_ids)
            .unwrap_or_default();

        // Data items (Wendell, Ominent, BURG.L, Stuff) also in World.csav's PartyComponent
        let table_item_names = world_decompressed
            .as_deref()
            .map(extract_table_item_names)
            .unwrap_or_default();

        // Step 2: Route parsed data to category extractors
        extract_all_categories(
            &achievements,
            &achievement_progress,
            &discovered_pois,
            &collected_themes,
            &creature_card_ids,
            &upgrade_levels,
            &table_item_names,
            &mut progress,
        );
    }

    // Calculate overall percentage
    progress.overall_percent = calculate_overall_percent(&progress);

    Ok(progress)
}

fn extract_all_categories(
    achievements: &HashSet<String>,
    achievement_progress: &HashMap<String, u32>,
    discovered_pois: &[String],
    collected_themes: &[String],
    creature_card_ids: &[String],
    upgrade_levels: &HashMap<String, u32>,
    table_item_names: &[String],
    progress: &mut SaveProgress,
) {
    // Boss defeats — from achievement flags
    crate::categories::bosses::extract_bosses(achievements, &mut progress.bosses);

    // Creature cards — from per-card parser + achievement flags
    crate::categories::creature_cards::extract_creature_cards(
        creature_card_ids,
        achievements,
        &mut progress.creature_cards,
    );

    // Landmarks — from POI parser results
    crate::categories::landmarks::extract_landmarks(discovered_pois, &mut progress.landmarks);

    // Milk molars — from achievement flags + progress values + upgrade levels
    crate::categories::milk_molars::extract_milk_molars(
        achievements,
        achievement_progress,
        upgrade_levels,
        &mut progress.milk_molars,
    );

    // MIX.R — from achievement flags only (slot-count heuristic removed)
    crate::categories::mixr::extract_mixr(achievements, &mut progress.mixr_challenges);

    // SCAB schemes — from theme parser + achievement flags
    crate::categories::scab_schemes::extract_scab_schemes(
        collected_themes,
        achievements,
        &mut progress.scab_schemes,
    );

    // Data item categories — parsed from Table_AllItems in World.csav
    crate::categories::wendell::extract_wendell(table_item_names, &mut progress.wendell);
    crate::categories::ominent::extract_ominent(table_item_names, &mut progress.ominent);
    crate::categories::burgl_chips::extract_burgl_chips(table_item_names, &mut progress.burgl_chips);
    crate::categories::stuff::extract_stuff(table_item_names, &mut progress.stuff);
}

fn calculate_overall_percent(progress: &SaveProgress) -> f32 {
    let mut total_items = 0u32;
    let mut collected_items = 0u32;

    // Only count S Rank categories (exclude data_items and recipes)
    let categories: Vec<(u32, u32)> = vec![
        (
            progress.bosses.collected_count,
            progress.bosses.total_count,
        ),
        (
            progress.creature_cards.collected_count,
            progress.creature_cards.total_count,
        ),
        (
            progress.landmarks.collected_count,
            progress.landmarks.total_count,
        ),
        (
            progress.milk_molars.regular_collected + progress.milk_molars.mega_collected,
            progress.milk_molars.regular_total + progress.milk_molars.mega_total,
        ),
        (
            progress.mixr_challenges.collected_count,
            progress.mixr_challenges.total_count,
        ),
        (
            progress.scab_schemes.collected_count,
            progress.scab_schemes.total_count,
        ),
    ];

    for (collected, total) in categories {
        collected_items += collected;
        total_items += total;
    }

    if total_items == 0 {
        0.0
    } else {
        (collected_items as f32 / total_items as f32) * 100.0
    }
}
