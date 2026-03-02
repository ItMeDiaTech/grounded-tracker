/// Shared binary parsing helpers for UE4 FArchive data.

/// Read a UE4 FString at the given position: u32 length prefix + `length` bytes.
/// Returns the string (without null terminator) and the position after the FString.
pub fn read_fstring(data: &[u8], pos: usize) -> Option<(String, usize)> {
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
pub fn read_u32(data: &[u8], pos: usize) -> Option<u32> {
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

/// Find the first occurrence of `needle` in `haystack`.
pub fn find_bytes(haystack: &[u8], needle: &[u8]) -> Option<usize> {
    haystack
        .windows(needle.len())
        .position(|window| window == needle)
}
