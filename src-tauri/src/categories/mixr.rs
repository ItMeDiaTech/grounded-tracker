use std::collections::HashSet;

use crate::models::{CategoryProgress, MixrItem};

const MIXR_NAMES: &[&str] = &[
    "Central Grasslands MIX.R",
    "Hedge MIX.R",
    "Haze MIX.R",
    "Sandbox MIX.R",
    "Northern Grasslands MIX.R",
    "Upper Grasslands Super MIX.R",
    "Lawnmower Super MIX.R",
    "Moldorc Super MIX.R",
];

pub fn extract_mixr(
    achievements: &HashSet<String>,
    progress: &mut CategoryProgress<MixrItem>,
) {
    let all_complete = achievements.contains("CompleteMIXRs");

    let items: Vec<MixrItem> = MIXR_NAMES
        .iter()
        .enumerate()
        .map(|(i, name)| MixrItem {
            id: format!("mixr_{}", i),
            name: name.to_string(),
            completed: all_complete,
        })
        .collect();

    progress.total_count = MIXR_NAMES.len() as u32;
    progress.collected_count = if all_complete { progress.total_count } else { 0 };
    progress.items = items;
}
