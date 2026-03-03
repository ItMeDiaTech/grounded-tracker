use crate::models::{CategoryProgress, CollectibleItem};

const TOTAL_STUFF: u32 = 33;

fn is_stuff_item(name: &str) -> bool {
    // Boss Notes (4 — only the main boss notes, not ant queen notes)
    name == "BossNoteBroodmother"
        || name == "BossNoteMantis"
        || name == "BossNoteWasp"
        || name == "BossNoteIBM"
        // Super Duper (3)
        || name.starts_with("DuperDisc")
        // Password Scribbles (4)
        || name.starts_with("PasswordPiece")
        // Keys (5 — regular keys + ASST. Manager Keycard)
        || name == "Key_BlackAntHill"
        || name == "Key_MoatTreasureChest"
        || name == "Key_PondTreasureChest"
        || name == "Key_TableTreasureChest"
        || name == "Key_UpperYardTreasureChest"
        // Filled Embiggening Cell (1)
        || name == "EmbiggeningCocktailFinal"
        // ZIP.R (1)
        || name == "KeyZiplineAscender"
        // Recipes (6) + Statues (7)
        || name.starts_with("Recipe")
        // Products (2)
        || name == "SCABManual"
        || name == "ScannerBracelet"
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
