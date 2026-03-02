import { useState, useEffect } from "react";
import { listen } from "@tauri-apps/api/event";
import { invoke } from "@tauri-apps/api/core";
import type { SaveProgress, WatcherStatus } from "../types/save-data";

interface SaveSyncState {
  data: SaveProgress | null;
  status: WatcherStatus | null;
  lastError: string | null;
  lastSync: string | null;
  loading: boolean;
}

export function useSaveSync(): SaveSyncState {
  const [data, setData] = useState<SaveProgress | null>(null);
  const [status, setStatus] = useState<WatcherStatus | null>(null);
  const [lastError, setLastError] = useState<string | null>(null);
  const [lastSync, setLastSync] = useState<string | null>(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    // Listen for save data updates from the Rust watcher
    const unlistenSave = listen<SaveProgress>("save-updated", (event) => {
      setData(event.payload);
      setLastSync(new Date().toLocaleTimeString());
      setLastError(null);
      setLoading(false);
    });

    const unlistenError = listen<string>("parse-error", (event) => {
      setLastError(event.payload);
      setLoading(false);
    });

    const unlistenStatus = listen<WatcherStatus>("watcher-status", (event) => {
      setStatus(event.payload);
      if (event.payload.error) {
        setLastError(event.payload.error);
      }
      setLoading(false);
    });

    // Fetch initial data directly — bypasses event race condition
    invoke<SaveProgress>("get_save_progress")
      .then((progress) => {
        setData(progress);
        setLastSync(new Date().toLocaleTimeString());
        setLoading(false);
      })
      .catch((e) => {
        setLastError(String(e));
        setLoading(false);
      });

    // Fetch watcher status for display (doesn't control loading)
    invoke<WatcherStatus>("get_watcher_status")
      .then((s) => {
        setStatus(s);
        if (s.error) setLastError(s.error);
      })
      .catch(() => {});

    return () => {
      unlistenSave.then((f) => f());
      unlistenError.then((f) => f());
      unlistenStatus.then((f) => f());
    };
  }, []);

  return { data, status, lastError, lastSync, loading };
}
