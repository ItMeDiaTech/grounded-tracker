use crate::errors::AppError;
use crate::models::{SaveFolderInfo, SaveProgress, WatcherStatus};
use crate::parser::extractor::extract_save_progress;
use crate::watcher::{find_save_folders, get_save_base_dir, parse_and_emit, WatcherState};
use std::path::PathBuf;
use std::sync::{Arc, Mutex};
use tauri::{Emitter, State};

#[tauri::command]
pub fn get_save_progress(
    save_path: Option<String>,
) -> Result<SaveProgress, AppError> {
    let path = match save_path {
        Some(p) => PathBuf::from(p),
        None => {
            let base = get_save_base_dir()
                .ok_or_else(|| AppError::SaveDirNotFound("Home dir not found".into()))?;
            let folders = find_save_folders(&base);
            folders
                .into_iter()
                .next()
                .ok_or(AppError::NoSaveFolders)?
        }
    };

    extract_save_progress(&path)
}

#[tauri::command]
pub fn get_save_folders() -> Result<Vec<SaveFolderInfo>, AppError> {
    let base = get_save_base_dir()
        .ok_or_else(|| AppError::SaveDirNotFound("Home dir not found".into()))?;

    if !base.exists() {
        return Err(AppError::SaveDirNotFound(base.to_string_lossy().to_string()));
    }

    let folders = find_save_folders(&base);
    let infos = folders
        .into_iter()
        .map(|path| {
            let name = path
                .file_name()
                .map(|n| n.to_string_lossy().to_string())
                .unwrap_or_default();

            let last_modified = std::fs::metadata(path.join("World.csav"))
                .and_then(|m| m.modified())
                .map(|t| {
                    let dt: chrono::DateTime<chrono::Local> = t.into();
                    dt.format("%Y-%m-%d %H:%M:%S").to_string()
                })
                .unwrap_or_default();

            SaveFolderInfo {
                name,
                path: path.to_string_lossy().to_string(),
                last_modified,
            }
        })
        .collect();

    Ok(infos)
}

#[tauri::command]
pub fn set_active_save(
    path: String,
    state: State<'_, Arc<Mutex<WatcherState>>>,
    app_handle: tauri::AppHandle,
) -> Result<(), AppError> {
    let save_dir = PathBuf::from(&path);

    if !save_dir.join("World.csav").exists() {
        return Err(AppError::SaveDirNotFound(format!(
            "No World.csav in {}",
            path
        )));
    }

    {
        let mut s = state.lock().map_err(|e| AppError::Watcher(e.to_string()))?;
        s.active_save = Some(save_dir.clone());
    }

    let state_arc = state.inner().clone();
    parse_and_emit(&app_handle, &state_arc, &save_dir);

    let _ = app_handle.emit(
        "watcher-status",
        WatcherStatus {
            watching: true,
            active_save: Some(path),
            last_update: state.lock().ok().and_then(|s| s.last_update.clone()),
            error: None,
        },
    );

    Ok(())
}

#[tauri::command]
pub fn get_watcher_status(
    state: State<'_, Arc<Mutex<WatcherState>>>,
) -> Result<WatcherStatus, AppError> {
    let s = state.lock().map_err(|e| AppError::Watcher(e.to_string()))?;
    Ok(WatcherStatus {
        watching: s.watching,
        active_save: s.active_save.as_ref().map(|p| p.to_string_lossy().to_string()),
        last_update: s.last_update.clone(),
        error: s.error.clone(),
    })
}
