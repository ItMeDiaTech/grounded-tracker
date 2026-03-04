mod commands;
mod connection;
mod errors;
mod models;
mod pipe_client;

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_shell::init())
        .invoke_handler(tauri::generate_handler![
            commands::get_connection_status,
            commands::reconnect,
        ])
        .setup(|app| {
            connection::start_connection_loop(app.handle().clone());
            Ok(())
        })
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
