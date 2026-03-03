use crate::models::{CategoryProgress, LandmarkItem};

const TOTAL_LANDMARKS: u32 = 108;

pub fn extract_landmarks(
    discovered_pois: &[String],
    progress: &mut CategoryProgress<LandmarkItem>,
) {
    let items: Vec<LandmarkItem> = discovered_pois
        .iter()
        .map(|poi_id| LandmarkItem {
            id: poi_id.clone(),
            name: format_poi_name(poi_id),
            zone: String::new(), // Zone mapping done on frontend via master data
            discovered: true,
        })
        .collect();

    let collected = (items.len() as u32).min(TOTAL_LANDMARKS);
    progress.items = items;
    progress.total_count = TOTAL_LANDMARKS;
    progress.collected_count = collected;
}

/// Convert a POI ID like "POI_MarshBaseball" to "Marsh Baseball"
fn format_poi_name(id: &str) -> String {
    let name = id.strip_prefix("POI_").unwrap_or(id);
    let mut result = String::new();
    for (i, ch) in name.chars().enumerate() {
        if i > 0 && ch.is_uppercase() {
            result.push(' ');
        }
        result.push(ch);
    }
    result
}
