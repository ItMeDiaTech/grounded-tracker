use crate::connection::ConnectionState;
use crate::errors::AppError;
use crate::models::ConnectionStatus;
use std::sync::{Arc, Mutex};
use tauri::State;

#[tauri::command]
pub fn get_connection_status(
    state: State<'_, Arc<Mutex<ConnectionState>>>,
) -> Result<ConnectionStatus, AppError> {
    let s = state
        .lock()
        .map_err(|e| AppError::Connection(e.to_string()))?;
    Ok(ConnectionStatus {
        connected: s.connected,
        game_running: s.game_running,
        dll_injected: s.connected, // connected implies DLL is injected
        last_update: s.last_update.clone(),
        error: s.error.clone(),
    })
}

#[tauri::command]
pub fn reconnect(
    state: State<'_, Arc<Mutex<ConnectionState>>>,
) -> Result<(), AppError> {
    let mut s = state
        .lock()
        .map_err(|e| AppError::Connection(e.to_string()))?;
    s.connected = false;
    s.error = None;
    Ok(())
}
