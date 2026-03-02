# Grounded Save File Format

## File Structure
- Extension: `.csav`
- Header: 8 bytes
  - `u32 LE` decompressed_size
  - `u32 LE` compressed_size (= file_size - 8)
- Body: Oodle Kraken compressed (first byte `0x8C`)
- Decompressed: UE4 custom FArchive binary serialization (NOT standard GVAS/property system)

## Key Files

| File | Compressed | Decompressed | Content |
|------|-----------|-------------|---------|
| World.csav | 608 KB | 2.83 MB | World state: bosses, landmarks, quests, item spawns |
| HostPlayer.csav | 6.7 KB | 33.5 KB | Player state: inventory, perks, upgrades, progress |
| Player_*.csav | 6.7 KB | 33.5 KB | Per-player data (same format as HostPlayer) |
| Options.csav | 14 bytes | ~6 bytes | Game options |
| SaveGameHeaderData.savheader | N/A | N/A | Plaintext metadata |

## Binary Format Details

No standard UE4 property types found (BoolProperty, IntProperty, etc. = 0 occurrences).
Instead uses serialized UE4 FArchive with FString paths as type markers.

### String Encoding
UE4 FString: `u32 LE length` (includes null terminator) + ASCII bytes + `0x00`

### World.csav Key Patterns (33,205 FStrings)
- `/Script/Maine.ItemSpawnManager` — item spawn system
- `/Game/Blueprints/Items/SpawnPoints/SpawnGroups/SG_*` — spawn groups
- `/Game/Blueprints/Creatures/BP_BossManager` — generic boss state
- `/Game/Blueprints/Creatures/Spider/Boss01/*` — Broodmother
- `/Game/Blueprints/Creatures/Spider/Test01/BP_BossManager_IBM` — Infected Broodmother
- `/Game/Blueprints/Creatures/Wasp/Wicklow/BP_BossManager_WaspQueen` — Wasp Queen
- `/Game/Design/Toolbox/Blueprints/BP_BAH_BossStateManager` — Black Ant Hill boss
- `/Game/Blueprints/Creatures/TazT/Boss/*` — Mantis boss mechanics
- `/Game/Blueprints/BurgleQuests/*` — BURG.L quest states
- `/Game/Design/Toolbox/Blueprints/BP_Burgle_Quest_Flag` — quest completion flags
- `/Game/Blueprints/Environment/BP_FuseBoxHatchSCAB` — SCAB collectible points
- `/Game/Art/World/ZN10_Labs/Lab_Props/BP_MicroSolution_RecipeSlot` — MIX.R recipe slots
- `/Game/Blueprints/Items/SpawnPoints/SpawnGroups/SG_NG+_MilkMolars` — NG+ milk molars
- `/Game/Blueprints/Environment/FoliageActors/UpgradeStones/BP_Rock_WeaponStone2_A` — upgrade stones
- `/Game/Blueprints/Quests/BP_LabPOITrigger` — lab POI discovery triggers

### HostPlayer.csav Key Patterns (696 FStrings)
- `/Script/Maine.InventoryComponent` — player inventory
- `/Script/Maine.EquipmentComponent` — equipped items
- `/Script/Maine.PerkComponent` — perks/mutations system
- `/Script/Maine.PlayerUpgradeComponent` — upgrade tracking
- `/Script/Maine.HealthComponent` — health state
- `/Script/Maine.StatusEffectComponent` — active effects
- `/Script/Maine.CustomPropertyComponent` — custom properties
- `MilkMolars` — milk molar count
- `Brainpower10` — brainpower level milestone
- `AllCreatureCards` — creature card collection flag
- `ObtainMutation` — mutation acquisition
- `PurchaseUpgrade` — upgrade purchase
- `UpgradeWeapons` / `UpgradeArmors` — upgrade categories
- `CreaturePEEPing` — creature scanning
- `/Game/UI/GlobalColor/GlobalColorTheme_*` — SCAB color schemes
- `/Game/Design/Perks/Stats/*` — stat tracking (kills, discoveries, etc.)
- `/Game/Design/PointsOfInterest/POI_MilkMolarBottle` — milk molar POI

## Parsing Strategy

Since this is a custom FArchive (not property-based), we parse by:
1. Scanning for known FString markers
2. Reading structured data around those markers
3. Tracking object instances by their class path

Key: Each serialized object starts with its class path FString, followed by instance-specific binary data.

## Parser Architecture

The main extraction pipeline lives in `src-tauri/src/parser/extractor.rs`:

```
World.csav ─── decompress ──┐
                             ├── extract_achievement_flags() → HashSet<String> + HashMap<String, u32>
HostPlayer.csav ─ decompress ┤   extract_poi_ids()            → Vec<String>
                             │   extract_theme_ids()           → Vec<String>
                             │   extract_upgrade_levels()      → HashMap<String, u32>
                             │   extract_creature_card_ids()   → Vec<String>
                             │
                             └── categories/*  (match parsed data against master lists)
                                    ├── bosses        → CategoryProgress<BossItem>
                                    ├── creature_cards → CategoryProgress<CreatureCardItem>
                                    ├── landmarks     → CategoryProgress<LandmarkItem>
                                    ├── milk_molars   → MilkMolarProgress
                                    ├── mixr          → CategoryProgress<MixrItem>
                                    ├── scab_schemes  → CategoryProgress<ScabSchemeItem>
                                    ├── burgl_quests  → CategoryProgress<BurglQuestItem>
                                    └── brainpower    → BrainpowerProgress
```

**Parser modules** (`src-tauri/src/parser/`):

| Module | Input | Output | Method |
|--------|-------|--------|--------|
| `decompress.rs` | Raw `.csav` bytes | Decompressed bytes | Read 8-byte header, call oozextract |
| `achievements.rs` | HostPlayer bytes | Achievement flags + counters | Find `AchievementsComponent` marker, read 25 predefined flags + Brainpower1–21 |
| `creature_cards.rs` | World bytes | Creature card IDs | Find `PartyComponent` marker, read bestiary discovery array |
| `poi.rs` | HostPlayer bytes | POI IDs | Scan for `/Game/Design/PointsOfInterest/POI_*` prefixes |
| `themes.rs` | HostPlayer bytes | Theme names | Scan for `GlobalColorTheme_*` prefixes |
| `upgrades.rs` | HostPlayer bytes | Upgrade category → level map | Find `PlayerUpgradeComponent` marker, read structured entries |

## Tools

### save-dumper CLI

A standalone CLI tool for analyzing `.csav` files, useful for reverse-engineering the binary format.

```bash
cd tools/save-dumper/
cargo run -- <file.csav> [flags]
```

**Flags:**

| Flag | Short | Description |
|------|-------|-------------|
| `--header-only` | | Print header info only (compressed/decompressed sizes) |
| `--hex` | `-x` | Dump raw hex output |
| `--strings` | `-S` | Dump ASCII strings found in decompressed data |
| `--strings-min <N>` | | Minimum string length for `--strings` (default: 4) |
| `--fstrings` | | Dump UE4 FString patterns (length-prefixed strings) |
| `--stats` | | Show byte value statistics |
| `--achievements` | | Dump achievement flag states |
| `--party-component` | | Dump PartyComponent bestiary entries |
| `--output <path>` | `-o` | Write decompressed output to file |

**Examples:**

```bash
# View file header
cargo run -- ~/Saved\ Games/Grounded/MySave/World.csav --header-only

# Dump all FStrings from a save file
cargo run -- ~/Saved\ Games/Grounded/MySave/HostPlayer.csav --fstrings

# Extract achievement flags
cargo run -- ~/Saved\ Games/Grounded/MySave/HostPlayer.csav --achievements

# Dump creature card bestiary data
cargo run -- ~/Saved\ Games/Grounded/MySave/World.csav --party-component

# Write decompressed data to file for external analysis
cargo run -- ~/Saved\ Games/Grounded/MySave/World.csav -o world_decompressed.bin
```
