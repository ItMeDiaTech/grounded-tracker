use crate::parser::extractor::extract_save_progress;
use notify_debouncer_mini::{new_debouncer, DebouncedEventKind};
use std::path::{Path, PathBuf};
use std::sync::{Arc, Mutex};
use std::time::Duration;
use tauri::{AppHandle, Emitter, Manager};

pub struct WatcherState {
    pub watching: bool,
    pub active_save: Option<PathBuf>,
    pub last_update: Option<String>,
    pub error: Option<String>,
}

impl Default for WatcherState {
    fn default() -> Self {
        Self {
            watching: false,
            active_save: None,
            last_update: None,
            error: None,
        }
    }
}

pub fn get_save_base_dir() -> Option<PathBuf> {
    dirs::home_dir().map(|h| h.join("Saved Games").join("Grounded"))
}

pub fn find_save_folders(base_dir: &PathBuf) -> Vec<PathBuf> {
    let mut folders = Vec::new();
    if let Ok(entries) = std::fs::read_dir(base_dir) {
        for entry in entries.flatten() {
            let path = entry.path();
            if path.is_dir() && path.join("World.csav").exists() {
                folders.push(path);
            }
        }
    }
    // Sort by modification time, most recent first
    folders.sort_by(|a, b| {
        let a_time = std::fs::metadata(a.join("World.csav"))
            .and_then(|m| m.modified())
            .unwrap_or(std::time::SystemTime::UNIX_EPOCH);
        let b_time = std::fs::metadata(b.join("World.csav"))
            .and_then(|m| m.modified())
            .unwrap_or(std::time::SystemTime::UNIX_EPOCH);
        b_time.cmp(&a_time)
    });
    folders
}

/// Parse save data from `save_dir` and emit results to the frontend.
/// Updates watcher state on success/failure and emits `save-updated`,
/// `watcher-status`, or `parse-error` events accordingly.
pub(crate) fn parse_and_emit(
    app_handle: &AppHandle,
    state: &Arc<Mutex<WatcherState>>,
    save_dir: &Path,
) {
    let parse_result = std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| {
        extract_save_progress(save_dir)
    }));

    match parse_result {
        Ok(Ok(progress)) => {
            let now = chrono::Local::now().format("%Y-%m-%d %H:%M:%S").to_string();
            {
                let mut s = state.lock().unwrap();
                s.last_update = Some(now.clone());
                s.error = None;
            }
            let _ = app_handle.emit("save-updated", &progress);
            let _ = app_handle.emit(
                "watcher-status",
                crate::models::WatcherStatus {
                    watching: true,
                    active_save: Some(save_dir.to_string_lossy().to_string()),
                    last_update: Some(now),
                    error: None,
                },
            );
        }
        Ok(Err(e)) => {
            let mut s = state.lock().unwrap();
            s.error = Some(e.to_string());
            let _ = app_handle.emit("parse-error", e.to_string());
        }
        Err(panic_info) => {
            let msg = if let Some(s) = panic_info.downcast_ref::<String>() {
                format!("Parser panicked: {}", s)
            } else if let Some(s) = panic_info.downcast_ref::<&str>() {
                format!("Parser panicked: {}", s)
            } else {
                "Parser panicked with unknown error".to_string()
            };
            let mut s = state.lock().unwrap();
            s.error = Some(msg.clone());
            let _ = app_handle.emit("parse-error", msg);
        }
    }
}

pub fn start_watcher(app_handle: AppHandle) {
    let state = Arc::new(Mutex::new(WatcherState::default()));
    app_handle.manage(state.clone());

    std::thread::spawn(move || {
        let base_dir = match get_save_base_dir() {
            Some(dir) if dir.exists() => dir,
            Some(dir) => {
                let mut s = state.lock().unwrap();
                s.error = Some(format!("Save directory not found: {}", dir.display()));
                let _ = app_handle.emit(
                    "watcher-status",
                    crate::models::WatcherStatus {
                        watching: false,
                        active_save: None,
                        last_update: None,
                        error: s.error.clone(),
                    },
                );
                return;
            }
            None => {
                let mut s = state.lock().unwrap();
                s.error = Some("Could not determine home directory".into());
                let _ = app_handle.emit(
                    "watcher-status",
                    crate::models::WatcherStatus {
                        watching: false,
                        active_save: None,
                        last_update: None,
                        error: s.error.clone(),
                    },
                );
                return;
            }
        };

        // Find the most recent save folder (may be None)
        let save_folders = find_save_folders(&base_dir);
        let active_save = save_folders.first().cloned();

        // Update state and do initial parse if a save exists
        {
            let mut s = state.lock().unwrap();
            s.active_save = active_save.clone();
            s.watching = true;
        }

        if let Some(ref save_dir) = active_save {
            parse_and_emit(&app_handle, &state, save_dir);
        } else {
            // No saves yet — emit waiting status
            let _ = app_handle.emit(
                "watcher-status",
                crate::models::WatcherStatus {
                    watching: true,
                    active_save: None,
                    last_update: None,
                    error: Some("Waiting for first save".into()),
                },
            );
        }

        // Watch the base directory for changes in any save folder
        let (tx, rx) = std::sync::mpsc::channel();

        let mut debouncer = match new_debouncer(Duration::from_secs(2), tx) {
            Ok(d) => d,
            Err(e) => {
                let mut s = state.lock().unwrap();
                s.error = Some(format!("Watcher setup failed: {}", e));
                return;
            }
        };

        if let Err(e) = debouncer
            .watcher()
            .watch(&base_dir, notify::RecursiveMode::Recursive)
        {
            let mut s = state.lock().unwrap();
            s.error = Some(format!("Failed to watch directory: {}", e));
            return;
        }

        // Event loop — folder-aware switching
        loop {
            match rx.recv() {
                Ok(Ok(events)) => {
                    // Collect .csav paths from events
                    let csav_paths: Vec<&Path> = events
                        .iter()
                        .filter(|e| {
                            e.kind == DebouncedEventKind::Any
                                && e.path
                                    .extension()
                                    .map(|ext| ext == "csav")
                                    .unwrap_or(false)
                        })
                        .map(|e| e.path.as_path())
                        .collect();

                    if csav_paths.is_empty() {
                        continue;
                    }

                    let current_active = state.lock().unwrap().active_save.clone();

                    // Check if any changed path is from a different folder
                    let has_different_folder = csav_paths.iter().any(|p| {
                        p.parent()
                            .map(|parent| Some(parent) != current_active.as_deref())
                            .unwrap_or(false)
                    });

                    if has_different_folder || current_active.is_none() {
                        // Re-evaluate which save folder is newest
                        let newest = find_save_folders(&base_dir).into_iter().next();

                        if let Some(ref new_save) = newest {
                            let switched = current_active.as_ref() != Some(new_save);
                            {
                                let mut s = state.lock().unwrap();
                                s.active_save = Some(new_save.clone());
                            }
                            if switched {
                                let _ = app_handle.emit(
                                    "watcher-status",
                                    crate::models::WatcherStatus {
                                        watching: true,
                                        active_save: Some(
                                            new_save.to_string_lossy().to_string(),
                                        ),
                                        last_update: None,
                                        error: None,
                                    },
                                );
                            }
                            parse_and_emit(&app_handle, &state, new_save);
                        }
                        // If newest is None, saves were deleted — keep waiting
                    } else if let Some(ref save_dir) = current_active {
                        // Same folder — just re-parse
                        parse_and_emit(&app_handle, &state, save_dir);
                    }
                }
                Ok(Err(e)) => {
                    let _ = app_handle.emit("parse-error", format!("Watch error: {:?}", e));
                }
                Err(_) => break,
            }
        }
    });
}
