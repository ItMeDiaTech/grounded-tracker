use std::collections::HashSet;

use crate::models::{CategoryProgress, MixrItem};

const MIXR_NAMES: &[&str] = &[
    "MIX.R Alpha",
    "MIX.R Beta",
    "MIX.R Gamma",
    "MIX.R Delta",
    "MIX.R Epsilon",
    "MIX.R Zeta",
    "MIX.R Eta",
    "MIX.R Theta",
    "MIX.R Iota",
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
