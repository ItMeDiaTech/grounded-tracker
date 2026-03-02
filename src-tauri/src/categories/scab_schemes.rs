use std::collections::HashSet;

use crate::models::{CategoryProgress, ScabSchemeItem};

const TOTAL_SCAB_SCHEMES: u32 = 55;

pub fn extract_scab_schemes(
    collected_themes: &[String],
    achievements: &HashSet<String>,
    progress: &mut CategoryProgress<ScabSchemeItem>,
) {
    let all_obtained = achievements.contains("ObtainSCABs");

    let items: Vec<ScabSchemeItem> = collected_themes
        .iter()
        .map(|name| ScabSchemeItem {
            id: name.to_lowercase(),
            name: format_scheme_name(name),
            collected: true,
        })
        .collect();

    let collected = if all_obtained {
        TOTAL_SCAB_SCHEMES
    } else {
        (items.len() as u32).min(TOTAL_SCAB_SCHEMES)
    };

    progress.items = items;
    progress.total_count = TOTAL_SCAB_SCHEMES;
    progress.collected_count = collected;
}

fn format_scheme_name(raw: &str) -> String {
    let mut result = String::new();
    for (i, ch) in raw.chars().enumerate() {
        if i > 0 && ch.is_uppercase() {
            result.push(' ');
        }
        result.push(ch);
    }
    result
}
