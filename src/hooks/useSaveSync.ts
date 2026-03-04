import { useState, useEffect } from "react";
import { listen } from "@tauri-apps/api/event";
import { invoke } from "@tauri-apps/api/core";
import type { SaveProgress, ConnectionStatus } from "../types/save-data";

interface SaveSyncState {
  data: SaveProgress | null;
  status: ConnectionStatus | null;
  lastError: string | null;
  lastSync: string | null;
  loading: boolean;
}

export function useSaveSync(): SaveSyncState {
  const [data, setData] = useState<SaveProgress | null>(null);
  const [status, setStatus] = useState<ConnectionStatus | null>(null);
  const [lastError, setLastError] = useState<string | null>(null);
  const [lastSync, setLastSync] = useState<string | null>(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    // Listen for save data updates from the pipe connection
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

    const unlistenStatus = listen<ConnectionStatus>("connection-status", (event) => {
      setStatus(event.payload);
      if (event.payload.error) {
        setLastError(event.payload.error);
      }
      setLoading(false);
    });

    // Fetch initial connection status
    invoke<ConnectionStatus>("get_connection_status")
      .then((s) => {
        setStatus(s);
        if (s.error) setLastError(s.error);
        setLoading(false);
      })
      .catch(() => {
        setLoading(false);
      });

    return () => {
      unlistenSave.then((f) => f());
      unlistenError.then((f) => f());
      unlistenStatus.then((f) => f());
    };
  }, []);

  return { data, status, lastError, lastSync, loading };
}
