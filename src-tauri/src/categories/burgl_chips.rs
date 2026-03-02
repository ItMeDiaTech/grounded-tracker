use crate::models::{CategoryProgress, CollectibleItem};

const TOTAL_BURGL_CHIPS: u32 = 16;

fn is_burgl_chip_item(name: &str) -> bool {
    name.starts_with("TechChip_") || name.starts_with("AudioLogBurgle")
}

pub fn extract_burgl_chips(
    table_items: &[String],
    progress: &mut CategoryProgress<CollectibleItem>,
) {
    let items: Vec<CollectibleItem> = table_items
        .iter()
        .filter(|name| is_burgl_chip_item(name))
        .map(|name| CollectibleItem {
            id: name.clone(),
            name: String::new(),
            subcategory: String::new(),
            collected: true,
        })
        .collect();

    progress.collected_count = items.len() as u32;
    progress.items = items;
    progress.total_count = TOTAL_BURGL_CHIPS;
}
