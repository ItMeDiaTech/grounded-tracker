use crate::models::{CategoryProgress, CollectibleItem};

const TOTAL_WENDELL: u32 = 67;

fn is_wendell_item(name: &str) -> bool {
    name.starts_with("AudioLogWendell")
        || name.starts_with("AudioLog_Hedge_")
        || name.starts_with("AudioLog_Haze_")
        || name.starts_with("AudioLog_Wendell_")
        || name.starts_with("PondNote")
        || name.starts_with("Note_Haze_")
        || name.starts_with("Note_Goodbye_")
        || name.starts_with("Note_Mant_")
        || name.starts_with("Note_StorageBox_")
        || name.starts_with("BlackAntHill_Lab_Note")
        || name.starts_with("BlackAnthill_Lab_Note")
        || name.starts_with("Oak_Lab_Note")
        || name.starts_with("NoteStumpLab")
        || name.starts_with("HedgeOverlookNote")
        || name.starts_with("HedgeLabNote")
        || name.starts_with("HedgeStorageNote")
        || name.starts_with("NoteRaisin")
        || name.starts_with("FieldStationMixrNote")
        || name.starts_with("Note_HiddenKid_")
        || name.starts_with("Note_Wendell_")
        || name.contains("GeoTentNote")
        || name == "GrilledScience"
}

pub fn extract_wendell(
    table_items: &[String],
    progress: &mut CategoryProgress<CollectibleItem>,
) {
    let items: Vec<CollectibleItem> = table_items
        .iter()
        .filter(|name| is_wendell_item(name))
        .map(|name| CollectibleItem {
            id: name.clone(),
            name: String::new(),
            subcategory: String::new(),
            collected: true,
        })
        .collect();

    progress.collected_count = items.len() as u32;
    progress.items = items;
    progress.total_count = TOTAL_WENDELL;
}
