#pragma once

#include <cstdint>

// =============================================================================
// UE4 Offsets — Grounded 1.4.7 (UE4 4.27.2)
// Generated via Dumper-7 SDK on 2026-03-04
// =============================================================================

namespace Offsets {

// --- Engine Globals (relative to Maine-Win64-Shipping.exe base) ---
constexpr uintptr_t GWorld   = 0x05D80158;
constexpr uintptr_t GObjects = 0x062AFF48;
constexpr uintptr_t GNames   = 0x06271B80;

// --- ProcessEvent ---
constexpr uintptr_t ProcessEvent    = 0x0202AA50;
constexpr uintptr_t AppendString    = 0x01E340F0;
constexpr uintptr_t ProcessEventIdx = 0x00000044;

// --- UWorld (Engine_classes.hpp) ---
constexpr ptrdiff_t UWorld_PersistentLevel = 0x0030;
constexpr ptrdiff_t UWorld_AuthorityGameMode = 0x0168;
constexpr ptrdiff_t UWorld_GameState = 0x0170;        // AGameStateBase*
constexpr ptrdiff_t UWorld_GameInstance = 0x01D8;      // UGameInstance*

// --- AGameStateBase ---
constexpr ptrdiff_t GameStateBase_PlayerArray = 0x0250; // TArray<APlayerState*>

// --- ASurvivalGameState (Maine_classes.hpp:32638, size 0x06A0) ---
constexpr ptrdiff_t GameState_PartyComponent = 0x02D0;              // UPartyComponent*
constexpr ptrdiff_t GameState_MapComponent = 0x02E8;                // UMapComponent*
constexpr ptrdiff_t GameState_ItemManagerComponent = 0x02F8;        // UItemManagerComponent*
constexpr ptrdiff_t GameState_QuestManagerComponent = 0x0308;       // UQuestManagerComponent*
constexpr ptrdiff_t GameState_BurgleQuestManagerComponent = 0x03B0; // UBurgleQuestManagerComponent*
constexpr ptrdiff_t GameState_TotalPartyStatsComponent = 0x0400;    // UPlayerStatsComponent*
constexpr ptrdiff_t GameState_BestiaryComponent = 0x0408;           // UBestiaryComponent*
constexpr ptrdiff_t GameState_PersistenceComponent = 0x0420;        // UPersistenceComponent*
constexpr ptrdiff_t GameState_TrackedCollectibles = 0x0630;         // TArray<FCollectibleData>

// ABP_SurvivalGameState_C (extends ASurvivalGameState, size 0x0828)
constexpr ptrdiff_t BPGameState_CurrentColorTheme = 0x06B8;  // UGlobalColorTheme_Base_C*

// --- ASurvivalPlayerState (Maine_classes.hpp:33189, size 0x0790) ---
constexpr ptrdiff_t PlayerState_AchievementsComponent = 0x04A8;     // UAchievementsComponent*
constexpr ptrdiff_t PlayerState_PlayerStatsComponent = 0x04B0;      // UPlayerStatsComponent*
constexpr ptrdiff_t PlayerState_CurrentUITheme = 0x04C8;            // TSubclassOf<UGlobalColorTheme>
constexpr ptrdiff_t PlayerState_CurrentNightUITheme = 0x04D0;       // TSubclassOf<UGlobalColorTheme>
constexpr ptrdiff_t PlayerState_ColorSelection = 0x04DC;            // int32
constexpr ptrdiff_t PlayerState_PersonalKeyItems = 0x04E0;          // TArray<UItem*>
constexpr ptrdiff_t PlayerState_InspectedItems = 0x04F0;            // TArray<FDataTableRowHandle_NetCrc>
constexpr ptrdiff_t PlayerState_DiscoveredPOIs = 0x0670;            // TArray<UPointOfInterestDataAsset*>
constexpr ptrdiff_t PlayerState_PerkComponent = 0x06E8;             // UPerkComponent*
constexpr ptrdiff_t PlayerState_PlayerUpgradeComponent = 0x06F0;    // UPlayerUpgradeComponent*
constexpr ptrdiff_t PlayerState_UnlockedEmotes = 0x0760;            // TArray<FDataTableRowHandle_NetCrc>

// --- UAchievementsComponent (Maine_classes.hpp:575, size 0x0108) ---
constexpr ptrdiff_t AchievementComp_Achievements = 0x00D0;  // TArray<FAchievementStatus>

// FAchievementStatus struct (size 0x00A8)
constexpr ptrdiff_t AchievementStatus_Name = 0x0050;         // FName_NetCrc (12 bytes)
constexpr ptrdiff_t AchievementStatus_StatValue = 0x005C;    // int32
constexpr ptrdiff_t AchievementStatus_Data = 0x0068;         // FAchievementData (64 bytes)
constexpr size_t    AchievementStatus_Size = 0x00A8;

// EAchievementStat values of interest
constexpr int AchievementStat_MilkMolarsSpent = 10;
constexpr int AchievementStat_BrainpowerLevel = 11;
constexpr int AchievementStat_AllCreatureCards = 15;
constexpr int AchievementStat_AllBurglQuests = 18;
constexpr int AchievementStat_AllAntQueens = 19;

// --- UPlayerUpgradeComponent (Maine_classes.hpp:29462, size 0x0118) ---
constexpr ptrdiff_t UpgradeComp_PlayerUpgrades = 0x00E0;  // TArray<FPlayerUpgrade>
constexpr ptrdiff_t UpgradeComp_LegacyEffects = 0x00F0;   // TArray<FDataTableRowHandle_NetCrc>

// FPlayerUpgrade struct (size ~0x0020)
constexpr ptrdiff_t PlayerUpgrade_Name = 0x0000;      // FName_NetCrc (12 bytes)
constexpr ptrdiff_t PlayerUpgrade_Tier = 0x000C;      // int32
constexpr ptrdiff_t PlayerUpgrade_StatusEffectID = 0x0010; // FGuid (16 bytes)
constexpr size_t    PlayerUpgrade_Size = 0x0020;

// --- UPerkComponent (Maine_classes.hpp:28699, size 0x01F0) ---
constexpr ptrdiff_t PerkComp_Loadouts = 0x0108;     // TArray<FPerkLoadout>
constexpr ptrdiff_t PerkComp_ActiveLoadout = 0x0118; // int32
constexpr ptrdiff_t PerkComp_Perks = 0x01D8;        // TArray<FPerkStatus>

// FPerkStatus struct (size 0x0024)
constexpr ptrdiff_t PerkStatus_Name = 0x0000;        // FName_NetCrc (12 bytes)
constexpr ptrdiff_t PerkStatus_Tier = 0x000C;        // int32
constexpr ptrdiff_t PerkStatus_Equipped = 0x0010;    // bool
constexpr ptrdiff_t PerkStatus_StatusEffectID = 0x0014; // FGuid (16 bytes)
constexpr size_t    PerkStatus_Size = 0x0024;

// --- UBestiaryComponent (Maine_classes.hpp:9360, size 0x00D0) ---
constexpr ptrdiff_t BestiaryComp_Stats = 0x00B8;  // TArray<FBestiaryStat>

// --- UPartyComponent (Maine_classes.hpp:22259, size 0x0688) ---
constexpr ptrdiff_t PartyComp_ScienceFound = 0x0470;           // int32
constexpr ptrdiff_t PartyComp_PartyUpgradePoints = 0x047C;     // int32 (party milk molar points remaining)
constexpr ptrdiff_t PartyComp_PersonalUpgradePoints = 0x0480;  // int32 (personal milk molar points remaining)
constexpr ptrdiff_t PartyComp_OwnedKeyItems = 0x0490;          // TArray<FDataTableRowHandle_NetCrc>
constexpr ptrdiff_t PartyComp_KnownRecipes = 0x04A0;           // TArray<FRecipeKnowledge>
constexpr ptrdiff_t PartyComp_TechUnlockDatas = 0x04D0;        // TArray<FTechUnlockData>
constexpr ptrdiff_t PartyComp_UnlockedTechTrees = 0x04F0;      // TArray<FDataTableRowHandle_NetCrc>
constexpr ptrdiff_t PartyComp_DiscoveredPOIs = 0x0500;         // TArray<UPointOfInterestDataAsset*>
constexpr ptrdiff_t PartyComp_DiscoveredColorThemes = 0x0510;  // TArray<TSubclassOf<UGlobalColorTheme>>
constexpr ptrdiff_t PartyComp_ItemStackUpgrades = 0x0630;      // TArray<FItemStackUpgrade>

// --- UBurgleQuestManagerComponent (size 0x0360) ---
constexpr ptrdiff_t BurgleQuestMgr_CompletedEpicQuestCount = 0x0300;  // int32
constexpr ptrdiff_t BurgleQuestMgr_QuestSlotsAvailable = 0x0304;      // int32

// --- ABossManager (Maine_classes.hpp:10074, size 0x0398) ---
constexpr ptrdiff_t BossManager_BossActive = 0x0370;     // bool (replicated)
constexpr ptrdiff_t BossManager_MaxKillCount = 0x0374;   // int32
constexpr ptrdiff_t BossManager_BossActor = 0x037C;      // TWeakObjectPtr<ASurvivalCharacter>
constexpr ptrdiff_t BossManager_Persistence = 0x0388;    // UPersistenceComponent*

// ABP_BAH_BossStateManager_C (size 0x0260)
constexpr ptrdiff_t BossStateMgr_ConditionalBossDead = 0x0238;   // UConditionalToggleComponent*
constexpr ptrdiff_t BossStateMgr_ConditionalBossActive = 0x0240; // UConditionalToggleComponent*
constexpr ptrdiff_t BossStateMgr_Persistence = 0x0248;           // UPersistenceComponent*

// --- AMIXRTerminal (Maine_classes.hpp:28097, size 0x0270) ---
constexpr ptrdiff_t MIXRTerminal_CompletionCount = 0x0248;  // int32 (replicated)

// --- FCollectibleData (size 0x0024) ---
constexpr ptrdiff_t Collectible_Type = 0x0000;      // ECollectibleType (1 byte): Invalid=0, Color=1, Upgrade=2, UpgradeParty=3
constexpr ptrdiff_t Collectible_State = 0x0001;     // ECollectibleState (1 byte): Unknown=0, Discovered=1, Collected=2
constexpr ptrdiff_t Collectible_ID = 0x0004;        // FGuid (16 bytes)
constexpr ptrdiff_t Collectible_WorldPos = 0x0014;  // FVector (12 bytes)
constexpr size_t    Collectible_Size = 0x0024;

// --- UPointOfInterestDataAsset (size 0x0148) ---
constexpr ptrdiff_t POIData_CategoryTag = 0x0030;   // FGameplayTag
constexpr ptrdiff_t POIData_Tags = 0x0038;           // FGameplayTagContainer
constexpr ptrdiff_t POIData_DisplayName = 0x0058;    // FLocString

// --- UGlobalColorTheme_Base_C ---
constexpr ptrdiff_t ColorTheme_GlobalColorList = 0x0038;  // FGlobalColorList (size 0x0390)
constexpr ptrdiff_t ColorTheme_IdName = 0x0E78;           // FString
constexpr ptrdiff_t ColorTheme_DisplayName = 0x0E88;      // FString

// --- UGlobalUIData ---
constexpr ptrdiff_t GlobalUIData_UIThemes = 0x0458;        // TArray (unlock-eligible)
constexpr ptrdiff_t GlobalUIData_AllUIThemes = 0x0468;     // TArray (all registered)
constexpr ptrdiff_t GlobalUIData_DefaultUITheme = 0x0478;  // TSubclassOf
constexpr ptrdiff_t GlobalUIData_DefaultNightUITheme = 0x0480; // TSubclassOf

// --- UPlayerStatsComponent (size 0x0150) ---
constexpr ptrdiff_t StatsComp_Configs = 0x00E0;    // TArray<UPlayerStatConfig*>
constexpr ptrdiff_t StatsComp_PlayerStats = 0x00F0; // TMap<UPlayerStatConfig*, int32>

// --- TArray layout (UE4 standard) ---
constexpr ptrdiff_t TArray_Data = 0x0000;   // T*
constexpr ptrdiff_t TArray_Count = 0x0008;  // int32
constexpr ptrdiff_t TArray_Max = 0x000C;    // int32
constexpr size_t    TArray_Size = 0x0010;

} // namespace Offsets
