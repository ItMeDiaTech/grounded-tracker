use serde::Serialize;

#[derive(Debug, thiserror::Error)]
pub enum AppError {
    #[error("Failed to read file: {0}")]
    FileRead(#[from] std::io::Error),

    #[error("Pipe error: {0}")]
    Pipe(String),

    #[error("DLL injection failed: {0}")]
    Injection(String),

    #[error("Game not running")]
    GameNotRunning,

    #[error("Connection error: {0}")]
    Connection(String),
}

impl Serialize for AppError {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::Serializer,
    {
        serializer.serialize_str(&self.to_string())
    }
}
