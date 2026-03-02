# Grounded: S Rank Tracker

Desktop app that live-syncs with [Grounded](https://store.steampowered.com/app/962130/Grounded/) (Steam) save files to track S Rank completion progress.

Built with Tauri v2 (Rust backend) + React 19 (TypeScript frontend).

## Features

- **Live file watching** — automatically detects save file changes and updates progress in real-time
- **Auto-detection** of save folders at `%USERPROFILE%\Saved Games\Grounded\`
- **11 tracked categories** across 374 total items
- **Overall + per-category** completion percentages
- **Boss, creature, and landmark images** sourced from the Grounded Wiki
- **Collapsible category sections** with individual item tracking

## Tech Stack

| Layer | Technology |
|-------|-----------|
| Desktop framework | [Tauri v2](https://v2.tauri.app/) |
| Backend | Rust |
| Frontend | React 19 + TypeScript |
| Build tool | Vite 6 |
| Package manager | pnpm |
| Decompression | [oozextract](https://github.com/lvlvllvlvllvlvl/oozextract) (Oodle Kraken) |
| File watching | [notify](https://docs.rs/notify) with 2-second debounce |

## Prerequisites

- [Node.js](https://nodejs.org/) 18+
- [pnpm](https://pnpm.io/)
- [Rust toolchain](https://rustup.rs/)
- [Grounded](https://store.steampowered.com/app/962130/Grounded/) (Steam) installed — needed for `oo2core_9_win64.dll`

## Getting Started

```bash
git clone https://github.com/ItMeDiaTech/grounded-tracker.git
cd dt-grounded
pnpm install
node scripts/download-wiki-images.mjs
pnpm tauri dev
```

The image download script fetches boss and creature images from the Grounded Wiki. It's idempotent and skips files that already exist.

## Commands

| Command | Description |
|---------|-------------|
| `pnpm tauri dev` | Run Tauri dev mode (Vite dev server + native window) |
| `pnpm tauri build` | Build for production |
| `pnpm dev` | Run Vite dev server only (no native window) |
| `pnpm build` | TypeScript check + Vite build |
| `node scripts/download-wiki-images.mjs` | Download wiki images to `public/images/` |
| `cargo run` in `tools/save-dumper/` | Run save file analysis tool (see [docs/save-format.md](docs/save-format.md)) |

## Project Structure

```
dt-grounded/
├── src/                          # React frontend
│   ├── main.tsx                  # Entry point
│   ├── App.tsx                   # Root component, category layout
│   ├── components/
│   │   ├── Header.tsx            # Overall progress + watcher status
│   │   ├── CategorySection.tsx   # Collapsible category container
│   │   ├── BossGrid.tsx          # Boss cards with images + location
│   │   ├── CreatureGrid.tsx      # Creature card grid
│   │   ├── LandmarkGrid.tsx      # Landmark grid with zone filtering
│   │   ├── MilkMolarSection.tsx  # Regular/mega molar breakdown
│   │   ├── BrainpowerSection.tsx # Level 1–21 progress
│   │   ├── ImageCard.tsx         # Shared image + label card
│   │   ├── ErrorBanner.tsx       # Error notification
│   │   └── LoadingScreen.tsx     # Initial load state
│   ├── hooks/
│   │   └── useSaveSync.ts        # Tauri event listeners + initial fetch
│   ├── data/                     # Master data (names, IDs, totals)
│   │   ├── bosses.ts             # 5 boss definitions
│   │   ├── creature-cards.ts     # 68 creature card definitions
│   │   ├── landmarks.ts          # 108 landmark POI definitions
│   │   ├── mixr-challenges.ts    # 9 MIX.R challenge names
│   │   └── scab-schemes.ts       # 55 SCAB scheme names
│   ├── types/
│   │   └── save-data.ts          # TypeScript interfaces (mirrors Rust)
│   └── styles/
│       ├── globals.css
│       └── animations.css
├── src-tauri/                    # Rust backend
│   ├── src/
│   │   ├── main.rs               # Window launcher
│   │   ├── lib.rs                # Tauri app setup + plugin registration
│   │   ├── commands.rs           # Tauri IPC command handlers
│   │   ├── models.rs             # Shared types (SaveProgress, etc.)
│   │   ├── errors.rs             # AppError enum (thiserror)
│   │   ├── watcher.rs            # File watcher thread
│   │   ├── parser/               # Binary save file parsers
│   │   │   ├── mod.rs
│   │   │   ├── decompress.rs     # Oodle decompression wrapper
│   │   │   ├── extractor.rs      # Main extraction pipeline
│   │   │   ├── achievements.rs   # Achievement flag extraction
│   │   │   ├── creature_cards.rs # PartyComponent bestiary parser
│   │   │   ├── poi.rs            # POI ID extraction
│   │   │   ├── themes.rs         # SCAB theme extraction
│   │   │   └── upgrades.rs       # PlayerUpgradeComponent parser
│   │   └── categories/           # Category-specific logic
│   │       ├── mod.rs
│   │       ├── bosses.rs
│   │       ├── creature_cards.rs
│   │       ├── landmarks.rs
│   │       ├── milk_molars.rs
│   │       ├── mixr.rs
│   │       ├── scab_schemes.rs
│   │       ├── burgl_quests.rs
│   │       ├── brainpower.rs
│   │       ├── data_items.rs     # Stub
│   │       └── recipes.rs        # Stub
│   └── Cargo.toml
├── tools/
│   └── save-dumper/              # CLI tool for save file analysis
├── scripts/
│   └── download-wiki-images.mjs  # Wiki image downloader
├── docs/
│   ├── save-format.md            # Save file binary format reference
│   └── architecture.md           # Developer architecture guide
├── public/
│   └── images/
│       ├── bosses/               # Boss PNG files
│       └── creatures/            # Creature PNG files
└── LICENSE
```

## S Rank Categories

| Category | Items | Source File | Parse Method |
|----------|------:|-------------|--------------|
| Bosses | 5 | World.csav | Achievement flags (KillBroodmother, etc.) |
| Creature Cards | 68 | World.csav | PartyComponent bestiary entries |
| Landmarks | 108 | HostPlayer.csav | POI ID extraction |
| Milk Molars (Regular) | 93 | HostPlayer.csav | SpendMolars achievement + upgrade levels |
| Milk Molars (Mega) | 14 | HostPlayer.csav | SpendMolars achievement + upgrade levels |
| MIX.R Challenges | 9 | HostPlayer.csav | CompleteMIXRs achievement (all-or-nothing) |
| SCAB Schemes | 55 | HostPlayer.csav | GlobalColorTheme extraction |
| BURG.L Quests | 1 | HostPlayer.csav | CompleteAllBURGLQuests achievement (binary) |
| Brainpower | 21 | HostPlayer.csav | Brainpower1–Brainpower21 flags |
| **Total** | **374** | | |

Data Items and Recipes are defined as categories but are currently stubs (0 items) and excluded from overall progress.

## Architecture Overview

```
┌──────────────────────────────────────────────────────┐
│  Frontend (React + TypeScript)                       │
│                                                      │
│  useSaveSync hook ──── invoke("get_save_progress")   │
│       │                                              │
│       └── listen("save-updated") ── SaveProgress     │
│       └── listen("parse-error")  ── error string     │
│       └── listen("watcher-status") ── WatcherStatus  │
└────────────────────┬─────────────────────────────────┘
                     │ Tauri IPC
┌────────────────────┴─────────────────────────────────┐
│  Backend (Rust)                                      │
│                                                      │
│  commands.rs ─── get_save_progress()                 │
│                  get_save_folders()                   │
│                  get_watcher_status()                 │
│                                                      │
│  watcher.rs ──── background thread                   │
│       │          watches .csav files (2s debounce)   │
│       │          emits events to frontend             │
│       │                                              │
│       └── parser/extractor.rs                        │
│               ├── decompress.rs (Oodle Kraken)       │
│               ├── achievements.rs                    │
│               ├── creature_cards.rs                  │
│               ├── poi.rs                             │
│               ├── themes.rs                          │
│               └── upgrades.rs                        │
│                      │                               │
│               categories/* (match parsed data        │
│                  against master lists → items)        │
└──────────────────────────────────────────────────────┘
```

**Startup flow:**
1. Watcher thread starts, finds the most recent save folder
2. Initial parse: decompresses World.csav + HostPlayer.csav → extracts all categories
3. Emits `watcher-status` and `save-updated` events
4. Frontend receives `SaveProgress` via event listener

**Live update flow:**
1. File watcher detects `.csav` change (2-second debounce)
2. Re-parses affected save files
3. Emits `save-updated` with new `SaveProgress`
4. React re-renders updated categories

For detailed architecture, see [docs/architecture.md](docs/architecture.md).

## Save File Format

Grounded saves use `.csav` files: an 8-byte header followed by an Oodle Kraken compressed body. The decompressed data is a custom UE4 FArchive binary serialization (not the standard GVAS property system).

For full format documentation, see [docs/save-format.md](docs/save-format.md).

## License

MIT — see [LICENSE](LICENSE).
