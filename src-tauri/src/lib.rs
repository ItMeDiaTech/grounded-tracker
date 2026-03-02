mod commands;
mod errors;
mod models;
mod parser;
mod watcher;
mod categories;

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_shell::init())
        .invoke_handler(tauri::generate_handler![
            commands::get_save_progress,
            commands::get_save_folders,
            commands::set_active_save,
            commands::get_watcher_status,
        ])
        .setup(|app| {
            watcher::start_watcher(app.handle().clone());
            Ok(())
        })
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
