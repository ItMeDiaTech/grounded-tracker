#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct ProgressSnapshot {
    struct BossInfo {
        std::string id, name;
        bool defeated = false;
    };
    struct CreatureCardInfo {
        std::string id, name;
        bool collected = false;
    };
    struct LandmarkInfo {
        std::string id, name, zone;
        bool discovered = false;
    };
    struct MilkMolarInfo {
        uint32_t regularCollected = 0, regularTotal = 93;
        uint32_t megaCollected = 0, megaTotal = 14;
        uint32_t totalSpent = 0;
    };
    struct MixrInfo {
        std::string id, name;
        bool completed = false;
    };
    struct ScabSchemeInfo {
        std::string id, name;
        bool collected = false;
    };
    struct CollectibleInfo {
        std::string id, name, subcategory;
        bool collected = false;
    };

    std::vector<BossInfo> bosses;
    std::vector<CreatureCardInfo> creatureCards;
    std::vector<LandmarkInfo> landmarks;
    MilkMolarInfo milkMolars;
    std::vector<MixrInfo> mixrChallenges;
    std::vector<ScabSchemeInfo> scabSchemes;
    std::vector<CollectibleInfo> wendell;
    std::vector<CollectibleInfo> ominent;
    std::vector<CollectibleInfo> burglChips;
    std::vector<CollectibleInfo> stuff;
    float overallPercent = 0.0f;
    bool valid = false;
};

class GameStateReader {
public:
    bool Initialize();
    ProgressSnapshot ReadAll();

private:
    bool ResolvePointers();
    void ReadBosses(ProgressSnapshot& snap);
    void ReadCreatureCards(ProgressSnapshot& snap);
    void ReadLandmarks(ProgressSnapshot& snap);
    void ReadMilkMolars(ProgressSnapshot& snap);
    void ReadMixr(ProgressSnapshot& snap);
    void ReadScabSchemes(ProgressSnapshot& snap);
    void ReadTableItems(ProgressSnapshot& snap);
    void ReadUpgrades(ProgressSnapshot& snap);

    uintptr_t m_gameState = 0;
    uintptr_t m_playerState = 0;
    bool m_initialized = false;
};
