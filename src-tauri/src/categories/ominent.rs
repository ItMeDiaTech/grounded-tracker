use crate::models::{CategoryProgress, CollectibleItem};

const TOTAL_OMINENT: u32 = 22;

fn is_ominent_item(name: &str) -> bool {
    name.starts_with("OminentInfiltrator_")
        || name.starts_with("Note_OminentSpy_")
        || name.starts_with("AudioLogOminent")
        || name == "Ominent_Disk"
}

pub fn extract_ominent(
    table_items: &[String],
    progress: &mut CategoryProgress<CollectibleItem>,
) {
    let items: Vec<CollectibleItem> = table_items
        .iter()
        .filter(|name| is_ominent_item(name))
        .map(|name| CollectibleItem {
            id: name.clone(),
            name: String::new(),
            subcategory: String::new(),
            collected: true,
        })
        .collect();

    progress.collected_count = items.len() as u32;
    progress.items = items;
    progress.total_count = TOTAL_OMINENT;
}
