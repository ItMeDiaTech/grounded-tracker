use clap::Parser;
use oozextract::Extractor;
use std::fs;
use std::path::PathBuf;

#[derive(Parser)]
#[command(name = "save-dumper")]
#[command(about = "Decompress and analyze Grounded .csav save files")]
struct Cli {
    /// Path to a .csav file
    file: PathBuf,

    /// Dump raw hex output
    #[arg(short = 'x', long)]
    hex: bool,

    /// Search for ASCII strings of minimum length
    #[arg(short, long, default_value_t = 4)]
    strings_min: usize,

    /// Dump strings found in the decompressed data
    #[arg(short = 'S', long)]
    strings: bool,

    /// Write decompressed output to file
    #[arg(short, long)]
    output: Option<PathBuf>,

    /// Print header info only
    #[arg(long)]
    header_only: bool,

    /// Dump UE4 FString patterns (length-prefixed strings)
    #[arg(long)]
    fstrings: bool,

    /// Show byte statistics
    #[arg(long)]
    stats: bool,

    /// Dump achievement flags with completion state
    #[arg(long)]
    achievements: bool,

    /// Dump PartyComponent bestiary entries
    #[arg(long)]
    party_component: bool,
}

fn decompress_csav(data: &[u8]) -> Result<Vec<u8>, Box<dyn std::error::Error>> {
    if data.len() < 8 {
        return Err("File too small: need at least 8-byte header".into());
    }

    let decompressed_size = u32::from_le_bytes(data[0..4].try_into()?) as usize;
    let compressed_size = u32::from_le_bytes(data[4..8].try_into()?) as usize;

    println!("=== CSAV Header ===");
    println!("Decompressed size: {} bytes ({:.2} KB)", decompressed_size, decompressed_size as f64 / 1024.0);
    println!("Compressed size:   {} bytes ({:.2} KB)", compressed_size, compressed_size as f64 / 1024.0);
    println!("Compression ratio: {:.1}x", decompressed_size as f64 / compressed_size as f64);
    println!("First data byte:   0x{:02X}", data[8]);

    let actual_compressed = data.len() - 8;
    if compressed_size != actual_compressed {
        eprintln!(
            "WARNING: Header says {} compressed bytes, but file has {} (diff: {})",
            compressed_size,
            actual_compressed,
            (compressed_size as isize - actual_compressed as isize).abs()
        );
    }

    let mut output = vec![0u8; decompressed_size];
    let mut extractor = Extractor::new();
    extractor
        .read_from_slice(&data[8..], &mut output)
        .map_err(|e| format!("Oodle decompression failed: {}", e))?;

    println!("Decompression: OK");
    Ok(output)
}

fn dump_hex(data: &[u8], max_bytes: usize) {
    let limit = data.len().min(max_bytes);
    for (i, chunk) in data[..limit].chunks(16).enumerate() {
        let offset = i * 16;
        print!("{:08x}  ", offset);

        for (j, byte) in chunk.iter().enumerate() {
            print!("{:02x} ", byte);
            if j == 7 {
                print!(" ");
            }
        }

        // Pad if short line
        let padding = 16 - chunk.len();
        for _ in 0..padding {
            print!("   ");
        }
        if chunk.len() <= 8 {
            print!(" ");
        }

        print!(" |");
        for byte in chunk {
            if byte.is_ascii_graphic() || *byte == b' ' {
                print!("{}", *byte as char);
            } else {
                print!(".");
            }
        }
        println!("|");
    }

    if data.len() > max_bytes {
        println!("... ({} more bytes)", data.len() - max_bytes);
    }
}

fn find_strings(data: &[u8], min_len: usize) -> Vec<(usize, String)> {
    let mut results = Vec::new();
    let mut current = String::new();
    let mut start = 0;

    for (i, &byte) in data.iter().enumerate() {
        if byte.is_ascii_graphic() || byte == b' ' {
            if current.is_empty() {
                start = i;
            }
            current.push(byte as char);
        } else {
            if current.len() >= min_len {
                results.push((start, current.clone()));
            }
            current.clear();
        }
    }

    if current.len() >= min_len {
        results.push((start, current));
    }

    results
}

fn find_fstrings(data: &[u8]) -> Vec<(usize, String)> {
    let mut results = Vec::new();

    if data.len() < 4 {
        return results;
    }

    for i in 0..data.len() - 4 {
        let len = u32::from_le_bytes(data[i..i + 4].try_into().unwrap()) as usize;

        // Reasonable FString length: 1-1024 chars, followed by null terminator
        if len == 0 || len > 1024 {
            continue;
        }

        if i + 4 + len > data.len() {
            continue;
        }

        // Check if the last byte is a null terminator
        if data[i + 4 + len - 1] != 0 {
            continue;
        }

        // Check if all other bytes are printable ASCII
        let str_bytes = &data[i + 4..i + 4 + len - 1];
        if str_bytes.iter().all(|b| b.is_ascii_graphic() || *b == b' ' || *b == b'/' || *b == b'_' || *b == b'.') {
            if let Ok(s) = std::str::from_utf8(str_bytes) {
                if s.len() >= 2 {
                    results.push((i, s.to_string()));
                }
            }
        }
    }

    results
}

fn print_stats(data: &[u8]) {
    println!("\n=== Byte Statistics ===");
    println!("Total size: {} bytes ({:.2} KB)", data.len(), data.len() as f64 / 1024.0);

    let mut freq = [0u64; 256];
    for &b in data {
        freq[b as usize] += 1;
    }

    let null_count = freq[0];
    let ascii_printable: u64 = (0x20u8..=0x7E).map(|b| freq[b as usize]).sum();
    let high_bytes: u64 = (0x80u8..=0xFF).map(|b| freq[b as usize]).sum();

    println!("Null bytes:      {} ({:.1}%)", null_count, null_count as f64 / data.len() as f64 * 100.0);
    println!("ASCII printable: {} ({:.1}%)", ascii_printable, ascii_printable as f64 / data.len() as f64 * 100.0);
    println!("High bytes:      {} ({:.1}%)", high_bytes, high_bytes as f64 / data.len() as f64 * 100.0);

    // Look for common UE4 markers
    let patterns: &[(&str, &[u8])] = &[
        ("None", b"None"),
        ("/Script/", b"/Script/"),
        ("/Game/", b"/Game/"),
        ("BoolProperty", b"BoolProperty"),
        ("IntProperty", b"IntProperty"),
        ("StrProperty", b"StrProperty"),
        ("ArrayProperty", b"ArrayProperty"),
        ("StructProperty", b"StructProperty"),
        ("MapProperty", b"MapProperty"),
        ("ObjectProperty", b"ObjectProperty"),
        ("NameProperty", b"NameProperty"),
        ("FloatProperty", b"FloatProperty"),
        ("EnumProperty", b"EnumProperty"),
        ("TextProperty", b"TextProperty"),
        ("SoftObjectProperty", b"SoftObjectProperty"),
        ("SetProperty", b"SetProperty"),
    ];

    println!("\n=== UE4 Pattern Search ===");
    for (name, pattern) in patterns {
        let count = data
            .windows(pattern.len())
            .filter(|w| *w == *pattern)
            .count();
        if count > 0 {
            println!("{}: {} occurrences", name, count);
        }
    }
}

/// Find an FString by matching [u32 len][string bytes][null terminator].
fn find_fstring_at(data: &[u8], name: &str) -> Option<usize> {
    let len = (name.len() + 1) as u32;
    let len_bytes = len.to_le_bytes();
    let mut needle = Vec::with_capacity(4 + name.len() + 1);
    needle.extend_from_slice(&len_bytes);
    needle.extend_from_slice(name.as_bytes());
    needle.push(0);
    data.windows(needle.len()).position(|w| w == needle.as_slice())
}

fn read_u32_at(data: &[u8], pos: usize) -> Option<u32> {
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

fn read_fstring_at(data: &[u8], pos: usize) -> Option<(String, usize)> {
    let len = read_u32_at(data, pos)? as usize;
    let start = pos + 4;
    let end = start + len;
    if end > data.len() || len == 0 {
        return None;
    }
    let str_end = if data[end - 1] == 0 { end - 1 } else { end };
    let s = String::from_utf8_lossy(&data[start..str_end]).to_string();
    Some((s, end))
}

fn find_pattern(data: &[u8], pattern: &[u8]) -> Option<usize> {
    if pattern.len() > data.len() {
        return None;
    }
    data.windows(pattern.len())
        .position(|w| w == pattern)
}

fn dump_achievements(data: &[u8]) {
    println!("\n=== Achievement Flags ===");

    let marker = b"AchievementsComponent";
    let stop_marker = b"PlayerUpgradeComponent";

    let start_offset = match find_pattern(data, marker) {
        Some(offset) => {
            println!("AchievementsComponent found at offset 0x{:08x}", offset);
            offset + marker.len()
        }
        None => {
            println!("AchievementsComponent NOT found");
            return;
        }
    };

    let end_offset = find_pattern(&data[start_offset..], stop_marker)
        .map(|o| start_offset + o)
        .unwrap_or(data.len());

    let section = &data[start_offset..end_offset];
    println!("Section size: {} bytes", section.len());

    const FLAGS: &[&str] = &[
        "KillBroodmother", "KillInfectedBroodmother", "KillMantis",
        "KillWaspQueen", "KillDirector", "AllCreatureCards", "CompleteMIXRs",
        "CompleteAllBURGLQuests", "Brainpower10", "SpendMolars", "ObtainSCABs",
        "100PercentGame", "WinGame", "DiscoverSites", "PlugHazeCanister",
        "BlackAntHillLab", "HedgeLab", "PondLab", "KillWolfSpider",
        "RescueBurgL", "CompleteJavamatic", "ActivateREMIXR", "BeatNewGamePlus",
    ];

    println!("\n{:<30} {:>10} {:>10} {:>10}", "Flag", "Progress", "Completed", "Found");
    println!("{:-<30} {:-<10} {:-<10} {:-<10}", "", "", "", "");

    for flag_name in FLAGS {
        if let Some(pos) = find_fstring_at(section, flag_name) {
            let fstring_len = 4 + flag_name.len() + 1;
            let after = pos + fstring_len;
            let progress = read_u32_at(section, after).unwrap_or(0);
            let completed = read_u32_at(section, after + 4).unwrap_or(0);
            println!("{:<30} {:>10} {:>10} {:>10}",
                flag_name, progress, completed, "YES");
        } else {
            println!("{:<30} {:>10} {:>10} {:>10}",
                flag_name, "-", "-", "NO");
        }
    }

    // Dynamic Brainpower flags
    println!("\nDynamic Brainpower flags:");
    for level in 1..=21u32 {
        let flag = format!("Brainpower{}", level);
        if let Some(pos) = find_fstring_at(section, &flag) {
            let fstring_len = 4 + flag.len() + 1;
            let after = pos + fstring_len;
            let progress = read_u32_at(section, after).unwrap_or(0);
            let completed = read_u32_at(section, after + 4).unwrap_or(0);
            println!("  {:<28} progress={} completed={}", flag, progress, completed);
        }
    }
}

fn dump_party_component(data: &[u8]) {
    println!("\n=== PartyComponent (Bestiary) ===");

    let marker = b"/Script/Maine.PartyComponent\0";
    let Some(marker_pos) = find_pattern(data, marker) else {
        println!("PartyComponent NOT found");
        return;
    };

    println!("PartyComponent found at offset 0x{:08x}", marker_pos);
    let mut pos = marker_pos + marker.len();

    // Skip flag byte
    if pos >= data.len() {
        println!("Unexpected end of data after marker");
        return;
    }
    println!("Flag byte: 0x{:02x}", data[pos]);
    pos += 1;

    let Some(count) = read_u32_at(data, pos) else {
        println!("Failed to read array count");
        return;
    };
    pos += 4;
    println!("Array count: {}", count);

    if count > 500 {
        println!("Count too large (>500), likely malformed data");
        return;
    }

    let mut bestiary_count = 0u32;
    let mut total = 0u32;

    for i in 0..count {
        let Some((table_path, new_pos)) = read_fstring_at(data, pos) else {
            println!("  [{}] Failed to read table_path FString at 0x{:08x}", i, pos);
            break;
        };
        pos = new_pos;

        let Some((row_name, new_pos)) = read_fstring_at(data, pos) else {
            println!("  [{}] Failed to read row_name FString at 0x{:08x}", i, pos);
            break;
        };
        pos = new_pos;
        total += 1;

        if row_name.starts_with("Bestiary") {
            let is_gold = row_name.ends_with("Gold");
            let marker = if is_gold { " (Gold)" } else { "" };
            println!("  [{}] {}{}", i, row_name, marker);
            if !is_gold {
                bestiary_count += 1;
            }
        }

        // Print first few non-bestiary entries for context
        if !row_name.starts_with("Bestiary") && total <= 5 {
            println!("  [{}] {} (table: {})", i, row_name, table_path);
        }
    }

    println!("\nBestiary entries (non-Gold): {}", bestiary_count);
    println!("Total array entries parsed: {}", total);
}

fn main() {
    let cli = Cli::parse();

    let data = match fs::read(&cli.file) {
        Ok(d) => d,
        Err(e) => {
            eprintln!("Failed to read {}: {}", cli.file.display(), e);
            std::process::exit(1);
        }
    };

    println!("File: {} ({} bytes)", cli.file.display(), data.len());

    let decompressed = match decompress_csav(&data) {
        Ok(d) => d,
        Err(e) => {
            eprintln!("Decompression failed: {}", e);
            std::process::exit(1);
        }
    };

    if cli.header_only {
        return;
    }

    if let Some(ref output_path) = cli.output {
        match fs::write(output_path, &decompressed) {
            Ok(_) => println!("Wrote decompressed data to {}", output_path.display()),
            Err(e) => eprintln!("Failed to write output: {}", e),
        }
    }

    if cli.stats {
        print_stats(&decompressed);
    }

    if cli.hex {
        println!("\n=== Hex Dump (first 4096 bytes) ===");
        dump_hex(&decompressed, 4096);
    }

    if cli.strings {
        let strings = find_strings(&decompressed, cli.strings_min);
        println!("\n=== Strings (min length {}) ===", cli.strings_min);
        println!("Found {} strings", strings.len());
        for (offset, s) in &strings {
            println!("  0x{:08x}: {}", offset, s);
        }
    }

    if cli.fstrings {
        let fstrings = find_fstrings(&decompressed);
        println!("\n=== UE4 FStrings ===");
        println!("Found {} FStrings", fstrings.len());
        for (offset, s) in &fstrings {
            println!("  0x{:08x}: [{}] {}", offset, s.len(), s);
        }
    }

    if cli.achievements {
        dump_achievements(&decompressed);
    }

    if cli.party_component {
        dump_party_component(&decompressed);
    }

    // Default: show a summary if no specific output flags given
    if !cli.hex && !cli.strings && !cli.fstrings && cli.output.is_none()
        && !cli.stats && !cli.achievements && !cli.party_component
    {
        print_stats(&decompressed);

        let fstrings = find_fstrings(&decompressed);
        println!("\n=== UE4 FStrings (first 100) ===");
        println!("Found {} total FStrings", fstrings.len());
        for (offset, s) in fstrings.iter().take(100) {
            println!("  0x{:08x}: [{}] {}", offset, s.len(), s);
        }
        if fstrings.len() > 100 {
            println!("  ... and {} more", fstrings.len() - 100);
        }
    }
}
