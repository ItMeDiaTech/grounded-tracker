use std::collections::HashSet;

/// Extract collected SCAB scheme theme IDs from decompressed HostPlayer.csav data.
/// Themes appear as FStrings matching `/Game/UI/GlobalColor/GlobalColorTheme_*`.
pub fn extract_theme_ids(data: &[u8]) -> Vec<String> {
    let prefix = b"GlobalColorTheme_";
    let mut found = HashSet::new();

    let mut i = 0;
    while i < data.len().saturating_sub(prefix.len()) {
        if &data[i..i + prefix.len()] == prefix {
            let name_start = i + prefix.len();
            let name_end = data[name_start..]
                .iter()
                .position(|&b| !b.is_ascii_alphanumeric() && b != b'_')
                .map(|p| name_start + p)
                .unwrap_or(data.len().min(name_start + 64));

            if name_end > name_start {
                let name = String::from_utf8_lossy(&data[name_start..name_end]).to_string();
                if !name.is_empty() {
                    found.insert(name);
                }
            }
            i = name_end;
        } else {
            i += 1;
        }
    }

    let mut result: Vec<String> = found.into_iter().collect();
    result.sort();
    result
}
