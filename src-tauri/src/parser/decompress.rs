use crate::errors::AppError;
use oozextract::Extractor;

pub fn decompress_csav(data: &[u8]) -> Result<Vec<u8>, AppError> {
    if data.len() < 8 {
        return Err(AppError::Decompression(
            "File too small: need at least 8-byte header".into(),
        ));
    }

    let decompressed_size = u32::from_le_bytes(
        data[0..4]
            .try_into()
            .map_err(|_| AppError::Decompression("Invalid header".into()))?,
    ) as usize;

    let compressed_size = u32::from_le_bytes(
        data[4..8]
            .try_into()
            .map_err(|_| AppError::Decompression("Invalid header".into()))?,
    ) as usize;

    let actual_compressed = data.len() - 8;
    if compressed_size != actual_compressed {
        return Err(AppError::Decompression(format!(
            "Header says {} compressed bytes, but file has {}",
            compressed_size, actual_compressed
        )));
    }

    let mut output = vec![0u8; decompressed_size];
    let mut extractor = Extractor::new();
    extractor
        .read_from_slice(&data[8..], &mut output)
        .map_err(|e| AppError::Decompression(format!("Oodle decompression failed: {}", e)))?;

    Ok(output)
}
