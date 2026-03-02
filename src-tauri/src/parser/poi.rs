use std::collections::HashSet;

/// Extract discovered POI (Point of Interest) IDs from decompressed HostPlayer.csav data.
/// POIs appear as FStrings matching `/Game/Design/PointsOfInterest/POI_*`.
pub fn extract_poi_ids(data: &[u8]) -> Vec<String> {
    let prefix = b"/Game/Design/PointsOfInterest/POI_";
    let mut found = HashSet::new();

    let mut i = 0;
    while i < data.len().saturating_sub(prefix.len()) {
        if &data[i..i + prefix.len()] == prefix {
            let name_start = i + prefix.len() - 4; // Keep "POI_" prefix
            let name_end = data[name_start..]
                .iter()
                .position(|&b| !b.is_ascii_alphanumeric() && b != b'_')
                .map(|p| name_start + p)
                .unwrap_or(data.len().min(name_start + 128));

            if name_end > name_start {
                let name = String::from_utf8_lossy(&data[name_start..name_end]).to_string();
                if name.len() > 4 {
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
