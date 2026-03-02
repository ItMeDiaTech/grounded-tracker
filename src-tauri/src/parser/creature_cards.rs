/// Extract collected creature card IDs from decompressed World.csav data.
///
/// Finds the `/Script/Maine.PartyComponent` marker, then reads the structured
/// discovery array: a flag byte, a u32 count, then `count` entries each
/// consisting of two FStrings (table path + row name). Row names starting with
/// "Bestiary" (excluding "Gold" suffix) are creature cards.
use crate::parser::binary::{find_bytes, read_fstring, read_u32};

pub fn extract_creature_card_ids(data: &[u8]) -> Vec<String> {
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

    // Sanity cap: if count is unreasonably large, data is malformed
    if count > 500 {
        return Vec::new();
    }

    let mut bestiary_ids = Vec::new();

    for _ in 0..count {
        // Read table_path FString (skip it)
        let Some((_, new_pos)) = read_fstring(data, pos) else {
            break;
        };
        pos = new_pos;

        // Read row_name FString (this is what we care about)
        let Some((row_name, new_pos)) = read_fstring(data, pos) else {
            break;
        };
        pos = new_pos;

        if row_name.starts_with("Bestiary") && !row_name.ends_with("Gold") {
            bestiary_ids.push(row_name);
        }
    }

    bestiary_ids.sort();
    bestiary_ids
}
