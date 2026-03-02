use serde::Serialize;

#[derive(Debug, thiserror::Error)]
pub enum AppError {
    #[error("Save directory not found: {0}")]
    SaveDirNotFound(String),

    #[error("No save folders found")]
    NoSaveFolders,

    #[error("Failed to read file: {0}")]
    FileRead(#[from] std::io::Error),

    #[error("Decompression failed: {0}")]
    Decompression(String),

    #[error("Watcher error: {0}")]
    Watcher(String),
}

impl Serialize for AppError {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::Serializer,
    {
        serializer.serialize_str(&self.to_string())
    }
}
