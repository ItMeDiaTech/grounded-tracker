use crate::models::{CategoryProgress, CollectibleItem};

const TOTAL_STUFF: u32 = 40;

fn is_stuff_item(name: &str) -> bool {
    name.starts_with("BossNote")
        || name.starts_with("DuperDisc")
        || name.starts_with("PasswordPiece")
        || name.starts_with("Key_")
        || name.starts_with("KeyZipline")
        || name.starts_with("Recipe")
        || name.starts_with("DefensePointSuper")
        || name == "SCABManual"
        || name == "ScannerBracelet"
        || name == "GrilledScience"
}

pub fn extract_stuff(
    table_items: &[String],
    progress: &mut CategoryProgress<CollectibleItem>,
) {
    let items: Vec<CollectibleItem> = table_items
        .iter()
        .filter(|name| is_stuff_item(name))
        .map(|name| CollectibleItem {
            id: name.clone(),
            name: String::new(),
            subcategory: String::new(),
            collected: true,
        })
        .collect();

    progress.collected_count = items.len() as u32;
    progress.items = items;
    progress.total_count = TOTAL_STUFF;
}
