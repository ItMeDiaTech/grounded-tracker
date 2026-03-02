# Architecture

Developer reference for the Grounded: S Rank Tracker.

## System Overview

```
┌─────────────────────────────────────────────────────────────────┐
│  Frontend (React 19 + TypeScript)                               │
│                                                                 │
│  App.tsx                                                        │
│   ├── Header              (overall progress, watcher status)    │
│   ├── ErrorBanner         (parse error display)                 │
│   ├── CategorySection × N (collapsible per-category views)      │
│   │   ├── BossGrid / CreatureGrid / LandmarkGrid                │
│   │   ├── MilkMolarSection / BrainpowerSection                  │
│   │   └── ItemGrid (MIX.R, SCAB, BURG.L)                       │
│   └── LoadingScreen       (initial load state)                  │
│                                                                 │
│  useSaveSync hook                                               │
│   ├── invoke("get_save_progress") ──── initial fetch            │
│   ├── invoke("get_watcher_status") ─── initial status           │
│   ├── listen("save-updated") ───────── live SaveProgress        │
│   ├── listen("parse-error") ────────── error string             │
│   └── listen("watcher-status") ─────── WatcherStatus            │
└───────────────────────────┬─────────────────────────────────────┘
                            │ Tauri IPC (invoke / events)
┌───────────────────────────┴─────────────────────────────────────┐
│  Backend (Rust via Tauri v2)                                    │
│                                                                 │
│  commands.rs ─── get_save_progress(path?)  → SaveProgress       │
│                  get_save_folders()         → Vec<SaveFolderInfo>│
│                  set_active_save(path)      → () [TODO]         │
│                  get_watcher_status()       → WatcherStatus      │
│                                                                 │
│  watcher.rs ──── background thread                              │
│                  watches %USERPROFILE%\Saved Games\Grounded\    │
│                  2-second debounce via notify-debouncer-mini     │
│                  emits save-updated / parse-error / watcher-status│
│                                                                 │
│  parser/ ─────── binary save file parsers                       │
│  categories/ ─── category-specific extractors                   │
│  models.rs ───── shared types                                   │
│  errors.rs ───── AppError enum                                  │
└───────────────────────────┬─────────────────────────────────────┘
                            │
┌───────────────────────────┴─────────────────────────────────────┐
│  File System                                                    │
│                                                                 │
│  %USERPROFILE%\Saved Games\Grounded\                            │
│   └── <SaveFolder>/                                             │
│       ├── World.csav        (608 KB → 2.83 MB decompressed)     │
│       ├── HostPlayer.csav   (6.7 KB → 33.5 KB decompressed)    │
│       └── Player_*.csav     (per-player data)                   │
└─────────────────────────────────────────────────────────────────┘
```

## Data Flow

### Startup

1. `lib.rs` registers Tauri commands and starts the watcher thread
2. Watcher finds the most recent save folder (sorted by `World.csav` mtime)
3. Initial parse: `extract_save_progress(save_dir)` decompresses and parses both files
4. Watcher emits `watcher-status` (watching state) and `save-updated` (parsed data) events
5. Frontend `useSaveSync` hook receives events, updates React state

### Live Updates

1. `notify` file watcher detects `.csav` change in the active save folder
2. `notify-debouncer-mini` groups rapid changes with a 2-second window
3. Watcher re-runs `extract_save_progress()` on the full save folder
4. On success: emits `save-updated` with new `SaveProgress`
5. On error: emits `parse-error` with the error message
6. Frontend re-renders affected categories

### On-Demand Fetch

The frontend can also invoke `get_save_progress` directly (e.g., on initial load or manual refresh), bypassing the event system. This returns the same `SaveProgress` struct.

## Backend Modules

### Parser Pipeline (`src-tauri/src/parser/`)

The extraction pipeline in `extractor.rs` orchestrates all parsing:

```
extract_save_progress(save_dir)
│
├── decompress_csav(World.csav)
│   └── Reads 8-byte header + Oodle Kraken decompression
│
├── decompress_csav(HostPlayer.csav)
│
├── Raw extraction (parser modules):
│   ├── extract_achievement_flags(host_data) → (flags: HashSet, counters: HashMap)
│   ├── extract_poi_ids(host_data)           → Vec<String>
│   ├── extract_theme_ids(host_data)         → Vec<String>
│   ├── extract_upgrade_levels(host_data)    → HashMap<String, u32>
│   └── extract_creature_card_ids(world_data)→ Vec<String>
│
├── Category assembly (categories modules):
│   ├── bosses::extract(flags)               → CategoryProgress<BossItem>
│   ├── creature_cards::extract(card_ids, flags) → CategoryProgress<CreatureCardItem>
│   ├── landmarks::extract(poi_ids)          → CategoryProgress<LandmarkItem>
│   ├── milk_molars::extract(flags, upgrades)→ MilkMolarProgress
│   ├── mixr::extract(flags)                 → CategoryProgress<MixrItem>
│   ├── scab_schemes::extract(theme_ids, flags) → CategoryProgress<ScabSchemeItem>
│   ├── burgl_quests::extract(flags)         → CategoryProgress<BurglQuestItem>
│   └── brainpower::extract(flags)           → BrainpowerProgress
│
└── Calculate overall_percent (weighted average of 8 active categories)
```

### Watcher (`watcher.rs`)

- `get_save_base_dir()` — returns `{home}/Saved Games/Grounded`
- `find_save_folders(base_dir)` — lists directories containing `World.csav`, sorted newest-first
- `start_watcher(app_handle)` — spawns the background thread, initializes file watching

State managed via `Arc<Mutex<WatcherState>>` registered as Tauri managed state.

### Commands (`commands.rs`)

| Command | Parameters | Returns |
|---------|-----------|---------|
| `get_save_progress` | `save_path: Option<String>` | `SaveProgress` |
| `get_save_folders` | none | `Vec<SaveFolderInfo>` |
| `set_active_save` | `path: String` | `()` (not yet implemented) |
| `get_watcher_status` | none (reads managed state) | `WatcherStatus` |

## Frontend Modules

### `useSaveSync` Hook (`src/hooks/useSaveSync.ts`)

Central hook that manages all backend communication:

- Calls `invoke("get_save_progress")` and `invoke("get_watcher_status")` on mount
- Sets up three Tauri event listeners: `save-updated`, `parse-error`, `watcher-status`
- Returns `{ data, status, lastError, lastSync, loading }`

### Component Hierarchy

```
App.tsx
├── LoadingScreen          (shown while loading && !data)
├── Header                 (overallPercent, saveName, lastSync, watching)
├── ErrorBanner            (shown when lastError is set)
└── CategorySection × N    (title, icon, collected, total, color, children)
    ├── BossGrid           (BossItem[] → image cards with location)
    ├── CreatureGrid       (CreatureCardItem[] → image cards)
    ├── LandmarkGrid       (LandmarkItem[] → grid with zone filtering)
    ├── MilkMolarSection   (MilkMolarProgress → regular/mega breakdown)
    ├── BrainpowerSection  (BrainpowerProgress → level 1–21 bar)
    └── ItemGrid (generic) (MixrItem[] / ScabSchemeItem[] / BurglQuestItem[])
```

### Master Data (`src/data/`)

Static definitions that the backend matches against parsed save data:

| File | Count | Content |
|------|------:|---------|
| `bosses.ts` | 5 | Boss ID, achievement flag, name, location |
| `creature-cards.ts` | 68 | Creature ID, display name (grouped: Neutral/Angry/Harmless) |
| `landmarks.ts` | 108 | POI ID, name, zone |
| `mixr-challenges.ts` | 9 | MIX.R ID, name, description |
| `scab-schemes.ts` | 55 | Scheme ID, display name |

## Key Types

Types are defined in Rust (`src-tauri/src/models.rs`) and mirrored in TypeScript (`src/types/save-data.ts`). Rust structs use `#[serde(rename_all = "camelCase")]` for frontend compatibility.

### `SaveProgress`

Top-level response from `get_save_progress`. Contains:
- `save_name`, `save_path`, `last_modified` — save folder metadata
- `overall_percent` — weighted average of active categories (0.0–100.0)
- One field per category: `bosses`, `creature_cards`, `landmarks`, `milk_molars`, `mixr_challenges`, `scab_schemes`, `burgl_quests`, `brainpower`, `data_items`, `recipes`

### `CategoryProgress<T>`

Generic wrapper for item-based categories:
- `items: Vec<T>` — all items with collected/discovered/completed status
- `collected_count: u32`
- `total_count: u32`

### Specialized Progress Types

- `MilkMolarProgress` — `regular_collected`/`regular_total` (93) + `mega_collected`/`mega_total` (14) + `total_spent`
- `BrainpowerProgress` — `current_level` (0–21) + `max_level` (21)

### `WatcherStatus`

Watcher state sent via events and `get_watcher_status`:
- `watching: bool`
- `active_save: Option<String>` — folder path
- `last_update: Option<String>` — timestamp
- `error: Option<String>`

### `SaveFolderInfo`

Returned by `get_save_folders`:
- `name` — folder name
- `path` — full path
- `last_modified` — formatted timestamp
