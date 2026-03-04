use crate::errors::AppError;
use crate::models::SaveProgress;
use std::fs::OpenOptions;
use std::io::Read;

const PIPE_NAME: &str = r"\\.\pipe\dt-grounded";

pub struct PipeClient {
    handle: Option<std::fs::File>,
}

impl PipeClient {
    pub fn new() -> Self {
        Self { handle: None }
    }

    /// Try to connect to the named pipe. Returns Ok(true) if connected,
    /// Ok(false) if pipe doesn't exist yet, Err on unexpected failure.
    pub fn connect(&mut self) -> Result<bool, AppError> {
        match OpenOptions::new().read(true).open(PIPE_NAME) {
            Ok(file) => {
                self.handle = Some(file);
                Ok(true)
            }
            Err(e) => {
                // File not found = pipe doesn't exist (DLL not running)
                if e.kind() == std::io::ErrorKind::NotFound {
                    Ok(false)
                } else {
                    // Other errors (access denied, etc.)
                    Err(AppError::Pipe(format!("Failed to open pipe: {}", e)))
                }
            }
        }
    }

    /// Read the next length-prefixed JSON message from the pipe.
    /// Returns Ok(Some(progress)) for data messages, Ok(None) for heartbeats.
    pub fn read_message(&mut self) -> Result<Option<SaveProgress>, AppError> {
        let handle = self
            .handle
            .as_mut()
            .ok_or_else(|| AppError::Pipe("Not connected".into()))?;

        // Read 4-byte length prefix (u32 LE)
        let mut len_buf = [0u8; 4];
        handle.read_exact(&mut len_buf).map_err(|e| {
            AppError::Pipe(format!("Failed to read length prefix: {}", e))
        })?;

        let length = u32::from_le_bytes(len_buf) as usize;

        if length == 0 || length > 1024 * 1024 {
            return Err(AppError::Pipe(format!(
                "Invalid message length: {}",
                length
            )));
        }

        // Read JSON payload
        let mut json_buf = vec![0u8; length];
        handle.read_exact(&mut json_buf).map_err(|e| {
            AppError::Pipe(format!("Failed to read JSON payload: {}", e))
        })?;

        // Check if it's a heartbeat message
        let value: serde_json::Value = serde_json::from_slice(&json_buf).map_err(|e| {
            AppError::Pipe(format!("Failed to parse JSON: {}", e))
        })?;

        if value.get("type").and_then(|t| t.as_str()) == Some("heartbeat") {
            return Ok(None);
        }

        // Deserialize as full progress data
        let progress: SaveProgress = serde_json::from_value(value).map_err(|e| {
            AppError::Pipe(format!("Failed to deserialize progress: {}", e))
        })?;

        Ok(Some(progress))
    }

    pub fn disconnect(&mut self) {
        self.handle = None;
    }

    pub fn is_connected(&self) -> bool {
        self.handle.is_some()
    }
}
