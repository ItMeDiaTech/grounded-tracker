use std::collections::HashMap;

/// Parse the PlayerUpgradeComponent section from decompressed HostPlayer.csav data.
///
/// Binary format after `PlayerUpgradeComponent\0`:
///   byte   flag (0x04)
///   u32    category_count
///   [category_count × entry]:
///     u32    padding (0)
///     FString  category_name ("Perks", "Stamina", "Health", "Healing", "Thirst")
///     u32    upgrade_level
///
/// Returns a map of category name → upgrade level.
pub fn extract_upgrade_levels(data: &[u8]) -> HashMap<String, u32> {
    let mut levels = HashMap::new();

    let marker = b"PlayerUpgradeComponent\0";
    let Some(marker_pos) = find_pattern(data, marker) else {
        return levels;
    };

    let mut pos = marker_pos + marker.len();

    // Skip flag byte
    if pos >= data.len() {
        return levels;
    }
    pos += 1;

    // Read category count
    let Some(count) = read_u32(data, pos) else {
        return levels;
    };
    pos += 4;

    // Sanity check — should be 0-10 categories
    if count > 10 {
        return levels;
    }

    for _ in 0..count {
        // Skip u32 padding (0)
        if pos + 4 > data.len() {
            break;
        }
        pos += 4;

        // Read FString category name
        let Some((name, new_pos)) = read_fstring(data, pos) else {
            break;
        };
        pos = new_pos;

        // Read u32 upgrade level
        let Some(level) = read_u32(data, pos) else {
            break;
        };
        pos += 4;

        levels.insert(name, level);
    }

    levels
}

/// Read a UE4 FString at the given position: u32 length prefix + `length` bytes.
/// Returns the string (without null terminator) and the position after the FString.
fn read_fstring(data: &[u8], pos: usize) -> Option<(String, usize)> {
    let len = read_u32(data, pos)? as usize;
    let start = pos + 4;
    let end = start + len;

    if end > data.len() || len == 0 {
        return None;
    }

    // FStrings are null-terminated; strip the trailing \0
    let str_end = if data[end - 1] == 0 { end - 1 } else { end };
    let s = String::from_utf8_lossy(&data[start..str_end]).to_string();
    Some((s, end))
}

/// Read a little-endian u32 at the given position.
fn read_u32(data: &[u8], pos: usize) -> Option<u32> {
    if pos + 4 > data.len() {
        return None;
    }
    Some(u32::from_le_bytes([
        data[pos],
        data[pos + 1],
        data[pos + 2],
        data[pos + 3],
    ]))
}

/// Find the first occurrence of a byte pattern in a slice.
fn find_pattern(data: &[u8], pattern: &[u8]) -> Option<usize> {
    if pattern.len() > data.len() {
        return None;
    }
    data.windows(pattern.len())
        .position(|w| w == pattern)
}
