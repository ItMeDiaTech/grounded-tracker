use std::collections::{HashMap, HashSet};

/// Extract achievement flags from the AchievementsComponent section of
/// decompressed HostPlayer.csav data.
///
/// Returns:
/// - `HashSet<String>`: flags that are completed (u32 == 1)
/// - `HashMap<String, u32>`: progress values for each flag found
///
/// Binary format per entry in AchievementsComponent:
///   u32  entry_marker (always 1)
///   FString  flag_name (u32 len + len bytes including null)
///   u32  progress_count
///   u32  completed (1 = unlocked, 0 = locked)
///   u32  unknown (always 0)
pub fn extract_achievement_flags(data: &[u8]) -> (HashSet<String>, HashMap<String, u32>) {
    let mut completed_flags = HashSet::new();
    let mut progress_values = HashMap::new();

    // Find the AchievementsComponent marker
    let marker = b"AchievementsComponent";
    let stop_marker = b"PlayerUpgradeComponent";

    let start_offset = match find_pattern(data, marker) {
        Some(offset) => offset + marker.len(),
        None => return (completed_flags, progress_values),
    };

    // Find the end boundary (next component)
    let end_offset = find_pattern(&data[start_offset..], stop_marker)
        .map(|o| start_offset + o)
        .unwrap_or(data.len());

    let section = &data[start_offset..end_offset];

    // Pre-defined achievement flags — always present in AchievementsComponent.
    // Must read the completion u32 to determine actual state.
    const PREDEFINED_FLAGS: &[&str] = &[
        // Boss kills
        "KillBroodmother",
        "KillInfectedBroodmother",
        "KillMantis",
        "KillWaspQueen",
        "KillDirector",
        // Completion flags
        "AllCreatureCards",
        "CompleteMIXRs",
        "CompleteAllBURGLQuests",
        "Brainpower10",
        "SpendMolars",
        "ObtainSCABs",
        "100PercentGame",
        "WinGame",
        // Discovery & progress milestones
        "DiscoverSites",
        "PlugHazeCanister",
        "BlackAntHillLab",
        "HedgeLab",
        "PondLab",
        "KillWolfSpider",
        "RescueBurgL",
        "CompleteJavamatic",
        "ActivateREMIXR",
        "BeatNewGamePlus",
    ];

    for flag_name in PREDEFINED_FLAGS {
        if let Some(result) = read_flag_state(section, flag_name) {
            progress_values.insert(flag_name.to_string(), result.progress);
            if result.completed {
                completed_flags.insert(flag_name.to_string());
            }
        }
    }

    // Dynamic flags: Brainpower1..Brainpower21
    // These only appear when earned, so presence = completed.
    // BUT we must use FString-aware matching to avoid substring collisions
    // (e.g., "Brainpower1" matching inside "Brainpower10").
    for level in 1..=21u32 {
        let flag = format!("Brainpower{}", level);
        if find_fstring(section, &flag).is_some() {
            // For dynamic flags, presence means completed — but still read
            // the actual state if we can, to get the progress value.
            if let Some(result) = read_flag_state(section, &flag) {
                progress_values.insert(flag.clone(), result.progress);
                if result.completed {
                    completed_flags.insert(flag);
                }
            } else {
                // FString found but couldn't parse structure — presence = completed
                completed_flags.insert(flag);
            }
        }
    }

    (completed_flags, progress_values)
}

struct FlagState {
    progress: u32,
    completed: bool,
}

/// Read the completion state and progress value for a flag in the
/// AchievementsComponent section.
///
/// After the FString, the layout is:
///   u32 progress_count
///   u32 completed (1 = unlocked, 0 = locked)
fn read_flag_state(section: &[u8], flag_name: &str) -> Option<FlagState> {
    let fstring_pos = find_fstring(section, flag_name)?;
    let fstring_len = 4 + flag_name.len() + 1; // u32 length prefix + string bytes + null
    let after_fstring = fstring_pos + fstring_len;

    // Need at least 8 bytes after the FString for progress + completed
    if after_fstring + 8 > section.len() {
        return None;
    }

    let progress = read_u32(section, after_fstring)?;
    let completed_val = read_u32(section, after_fstring + 4)?;

    Some(FlagState {
        progress,
        completed: completed_val != 0,
    })
}

/// Find an FString in binary data by matching its full encoded form:
/// [u32 len][string bytes][null terminator]
/// where len = string.len() + 1 (for the null terminator).
///
/// This prevents substring matches (e.g., "Brainpower1" inside "Brainpower10").
fn find_fstring(data: &[u8], name: &str) -> Option<usize> {
    let len = (name.len() + 1) as u32; // +1 for null terminator
    let len_bytes = len.to_le_bytes();
    let mut needle = Vec::with_capacity(4 + name.len() + 1);
    needle.extend_from_slice(&len_bytes);
    needle.extend_from_slice(name.as_bytes());
    needle.push(0); // null terminator
    data.windows(needle.len()).position(|w| w == needle.as_slice())
}

/// Find the first occurrence of a byte pattern in a slice.
fn find_pattern(data: &[u8], pattern: &[u8]) -> Option<usize> {
    if pattern.len() > data.len() {
        return None;
    }
    data.windows(pattern.len())
        .position(|w| w == pattern)
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
