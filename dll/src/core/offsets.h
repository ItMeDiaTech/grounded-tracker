#pragma once

#include <cstdint>

// =============================================================================
// UE4 Offsets — Populated after Dumper-7 SDK generation
// =============================================================================
// Run Dumper-7 on Grounded, then fill these from the generated SDK headers.
// All offsets are relative to their parent object's base address.
// Set GWorld/GObjects/GNames to 0 to use pattern scanning instead.

namespace Offsets {

// --- Engine Globals (relative to module base) ---
constexpr uintptr_t GWorld = 0x0;
constexpr uintptr_t GObjects = 0x0;
constexpr uintptr_t GNames = 0x0;

// --- UWorld ---
constexpr ptrdiff_t UWorld_GameState = 0x0;       // ASurvivalGameState*
constexpr ptrdiff_t UWorld_GameInstance = 0x0;     // UGameInstance*

// --- UGameInstance ---
constexpr ptrdiff_t GameInstance_LocalPlayers = 0x0; // TArray<ULocalPlayer*>

// --- ULocalPlayer ---
constexpr ptrdiff_t LocalPlayer_PlayerController = 0x0; // APlayerController*

// --- APlayerController ---
constexpr ptrdiff_t PlayerController_PlayerState = 0x0;  // ASurvivalPlayerState*

// --- ASurvivalGameState ---
constexpr ptrdiff_t GameState_AchievementsComponent = 0x0;
constexpr ptrdiff_t GameState_BossManager = 0x0;
constexpr ptrdiff_t GameState_QuestManager = 0x0;
constexpr ptrdiff_t GameState_ItemSpawnManager = 0x0;

// --- ASurvivalPlayerState ---
constexpr ptrdiff_t PlayerState_UpgradesComponent = 0x0;
constexpr ptrdiff_t PlayerState_ThemesComponent = 0x0;
constexpr ptrdiff_t PlayerState_MolarsComponent = 0x0;
constexpr ptrdiff_t PlayerState_CreatureCardsComponent = 0x0;
constexpr ptrdiff_t PlayerState_InventoryComponent = 0x0;

// --- Achievement Component ---
constexpr ptrdiff_t AchievementComp_Achievements = 0x0;  // TMap or TArray
constexpr ptrdiff_t AchievementComp_Progress = 0x0;

// --- Boss Manager ---
constexpr ptrdiff_t BossManager_BossStates = 0x0;  // TArray<FBossState>
constexpr ptrdiff_t BossState_ID = 0x0;
constexpr ptrdiff_t BossState_Defeated = 0x0;

// --- Creature Cards ---
constexpr ptrdiff_t CreatureCards_CollectedArray = 0x0;  // TArray or TSet

// --- Landmarks / POIs ---
constexpr ptrdiff_t GameState_DiscoveredPOIs = 0x0;  // TArray<FName>

// --- Themes / SCA.B Schemes ---
constexpr ptrdiff_t ThemesComp_CollectedThemes = 0x0;  // TArray<FName>

// --- Milk Molars ---
constexpr ptrdiff_t MolarsComp_RegularCount = 0x0;
constexpr ptrdiff_t MolarsComp_MegaCount = 0x0;
constexpr ptrdiff_t MolarsComp_SpentCount = 0x0;

// --- MIX.R ---
constexpr ptrdiff_t GameState_MixrStations = 0x0;  // TArray<FMixrState>
constexpr ptrdiff_t MixrState_Completed = 0x0;

// --- Table Items (Wendell, Ominent, BURG.L, Stuff) ---
constexpr ptrdiff_t GameState_TableItems = 0x0;  // TArray<FTableItemState>
constexpr ptrdiff_t TableItem_RowName = 0x0;      // FName
constexpr ptrdiff_t TableItem_Collected = 0x0;

// --- Upgrades ---
constexpr ptrdiff_t UpgradesComp_Upgrades = 0x0;  // TArray<FUpgradeState>

} // namespace Offsets
