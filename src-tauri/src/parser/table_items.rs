/// Extract ALL row names from the Table_AllItems list in World.csav's PartyComponent.
///
/// Same binary structure as creature_cards parser: find marker, skip flag byte,
/// read u32 count, iterate FString pairs (table_path + row_name).
/// Returns every row name (not just Bestiary entries).
use crate::parser::binary::{find_bytes, read_fstring, read_u32};

pub fn extract_table_item_names(data: &[u8]) -> Vec<String> {
    let marker = b"/Script/Maine.PartyComponent\0";

    let Some(marker_pos) = find_bytes(data, marker) else {
        return Vec::new();
    };

    let mut pos = marker_pos + marker.len();

    // Skip 1 flag byte (0x1e)
    if pos >= data.len() {
        return Vec::new();
    }
    pos += 1;

    // Read u32 array count
    let Some(count) = read_u32(data, pos) else {
        return Vec::new();
    };
    pos += 4;

    if count > 500 {
        return Vec::new();
    }

    let mut names = Vec::with_capacity(count as usize);

    for _ in 0..count {
        // Read table_path FString (skip it)
        let Some((_, new_pos)) = read_fstring(data, pos) else {
            break;
        };
        pos = new_pos;

        // Read row_name FString
        let Some((row_name, new_pos)) = read_fstring(data, pos) else {
            break;
        };
        pos = new_pos;

        names.push(row_name);
    }

    names
}
