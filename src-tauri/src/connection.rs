use crate::models::ConnectionStatus;
use crate::pipe_client::PipeClient;
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;
use tauri::{AppHandle, Emitter, Manager};

pub struct ConnectionState {
    pub connected: bool,
    pub game_running: bool,
    pub last_update: Option<String>,
    pub error: Option<String>,
}

impl Default for ConnectionState {
    fn default() -> Self {
        Self {
            connected: false,
            game_running: false,
            last_update: None,
            error: None,
        }
    }
}

fn is_game_running() -> bool {
    // Check if Maine-Win64-Shipping.exe is running via tasklist
    match std::process::Command::new("tasklist")
        .args(["/FI", "IMAGENAME eq Maine-Win64-Shipping.exe", "/FO", "CSV", "/NH"])
        .output()
    {
        Ok(output) => {
            let stdout = String::from_utf8_lossy(&output.stdout);
            stdout.contains("Maine-Win64-Shipping.exe")
        }
        Err(_) => false,
    }
}

fn emit_status(app: &AppHandle, state: &Arc<Mutex<ConnectionState>>) {
    if let Ok(s) = state.lock() {
        let _ = app.emit(
            "connection-status",
            ConnectionStatus {
                connected: s.connected,
                game_running: s.game_running,
                dll_injected: s.connected,
                last_update: s.last_update.clone(),
                error: s.error.clone(),
            },
        );
    }
}

pub fn start_connection_loop(app: AppHandle) {
    let state = Arc::new(Mutex::new(ConnectionState::default()));
    app.manage(state.clone());

    thread::spawn(move || {
        let mut client = PipeClient::new();

        loop {
            // 1. Check if game is running
            let game_running = is_game_running();
            {
                let mut s = state.lock().unwrap();
                s.game_running = game_running;
            }

            if !game_running {
                // Game not running — disconnect and wait
                if client.is_connected() {
                    client.disconnect();
                    let mut s = state.lock().unwrap();
                    s.connected = false;
                    s.error = None;
                }
                emit_status(&app, &state);
                thread::sleep(Duration::from_secs(3));
                continue;
            }

            // 2. Try to connect if not connected
            if !client.is_connected() {
                match client.connect() {
                    Ok(true) => {
                        let mut s = state.lock().unwrap();
                        s.connected = true;
                        s.error = None;
                        emit_status(&app, &state);
                    }
                    Ok(false) => {
                        // Pipe doesn't exist yet — DLL not injected
                        let mut s = state.lock().unwrap();
                        s.connected = false;
                        emit_status(&app, &state);
                        thread::sleep(Duration::from_secs(2));
                        continue;
                    }
                    Err(e) => {
                        let mut s = state.lock().unwrap();
                        s.connected = false;
                        s.error = Some(e.to_string());
                        emit_status(&app, &state);
                        thread::sleep(Duration::from_secs(2));
                        continue;
                    }
                }
            }

            // 3. Read message (blocking)
            match client.read_message() {
                Ok(Some(progress)) => {
                    let now = chrono::Local::now()
                        .format("%H:%M:%S")
                        .to_string();
                    {
                        let mut s = state.lock().unwrap();
                        s.last_update = Some(now);
                        s.error = None;
                    }
                    let _ = app.emit("save-updated", &progress);
                }
                Ok(None) => {
                    // Heartbeat — DLL alive but no game data yet
                    {
                        let mut s = state.lock().unwrap();
                        s.connected = true;
                        s.error = None;
                    }
                    emit_status(&app, &state);
                }
                Err(e) => {
                    // Pipe broken — disconnect and retry
                    client.disconnect();
                    let mut s = state.lock().unwrap();
                    s.connected = false;
                    s.error = Some(e.to_string());
                    emit_status(&app, &state);
                    thread::sleep(Duration::from_secs(1));
                }
            }
        }
    });
}
