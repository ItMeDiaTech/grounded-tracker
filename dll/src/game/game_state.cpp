#include "game_state.h"
#include "achievements.h"
#include "creature_cards.h"
#include "landmarks.h"
#include "milk_molars.h"
#include "mixr.h"
#include "scab_schemes.h"
#include "table_items.h"
#include "upgrades.h"
#include "../core/engine.h"
#include "../core/offsets.h"
#include "../util/logger.h"

bool GameStateReader::Initialize() {
    return ResolvePointers();
}

bool GameStateReader::ResolvePointers() {
    m_gameState = GetGameState();
    m_playerState = GetPlayerState();

    if (!m_gameState) {
        LOG_WARN("GameState not resolved");
    }
    if (!m_playerState) {
        LOG_WARN("PlayerState not resolved");
    }

    m_initialized = (m_gameState != 0);
    return m_initialized;
}

ProgressSnapshot GameStateReader::ReadAll() {
    ProgressSnapshot snap;

    // Re-resolve pointers each read (they can change on map load)
    if (!ResolvePointers()) {
        snap.valid = false;
        return snap;
    }

    __try {
        ReadBosses(snap);
        ReadCreatureCards(snap);
        ReadLandmarks(snap);
        ReadMilkMolars(snap);
        ReadMixr(snap);
        ReadScabSchemes(snap);
        ReadTableItems(snap);
        ReadUpgrades(snap);

        // Calculate overall percent
        // Weights mirror the Rust implementation
        float total = 0.0f;
        float earned = 0.0f;
        auto addCategory = [&](uint32_t collected, uint32_t total_count) {
            if (total_count > 0) {
                total += 1.0f;
                earned += static_cast<float>(collected) / static_cast<float>(total_count);
            }
        };

        uint32_t bossCount = 0;
        for (auto& b : snap.bosses) if (b.defeated) bossCount++;
        addCategory(bossCount, static_cast<uint32_t>(snap.bosses.size()));

        uint32_t cardCount = 0;
        for (auto& c : snap.creatureCards) if (c.collected) cardCount++;
        addCategory(cardCount, static_cast<uint32_t>(snap.creatureCards.size()));

        uint32_t landmarkCount = 0;
        for (auto& l : snap.landmarks) if (l.discovered) landmarkCount++;
        addCategory(landmarkCount, static_cast<uint32_t>(snap.landmarks.size()));

        addCategory(snap.milkMolars.regularCollected + snap.milkMolars.megaCollected,
                     snap.milkMolars.regularTotal + snap.milkMolars.megaTotal);

        uint32_t mixrCount = 0;
        for (auto& m : snap.mixrChallenges) if (m.completed) mixrCount++;
        addCategory(mixrCount, static_cast<uint32_t>(snap.mixrChallenges.size()));

        uint32_t schemeCount = 0;
        for (auto& s : snap.scabSchemes) if (s.collected) schemeCount++;
        addCategory(schemeCount, static_cast<uint32_t>(snap.scabSchemes.size()));

        uint32_t wendellCount = 0;
        for (auto& w : snap.wendell) if (w.collected) wendellCount++;
        addCategory(wendellCount, static_cast<uint32_t>(snap.wendell.size()));

        uint32_t ominentCount = 0;
        for (auto& o : snap.ominent) if (o.collected) ominentCount++;
        addCategory(ominentCount, static_cast<uint32_t>(snap.ominent.size()));

        uint32_t burglCount = 0;
        for (auto& b : snap.burglChips) if (b.collected) burglCount++;
        addCategory(burglCount, static_cast<uint32_t>(snap.burglChips.size()));

        uint32_t stuffCount = 0;
        for (auto& s : snap.stuff) if (s.collected) stuffCount++;
        addCategory(stuffCount, static_cast<uint32_t>(snap.stuff.size()));

        snap.overallPercent = (total > 0.0f) ? (earned / total * 100.0f) : 0.0f;
        snap.valid = true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        LOG_ERROR("SEH exception during ReadAll");
        snap.valid = false;
    }

    return snap;
}

// --- Delegate methods to per-category readers ---

void GameStateReader::ReadBosses(ProgressSnapshot& snap) {
    // 5 bosses — matches src-tauri/src/categories/bosses.rs
    static const struct { const char* id; const char* name; } BOSS_DATA[] = {
        {"killbroodmother", "Hedge Broodmother"},
        {"killinfectedbroodmother", "Infected Broodmother"},
        {"killwaspqueen", "Wasp Queen"},
        {"killdirector", "Director Schmector"},
        {"killmantis", "Mantis"},
    };

    snap.bosses.clear();
    for (auto& [id, name] : BOSS_DATA) {
        bool defeated = Achievements::IsAchievementComplete(m_gameState, id);
        snap.bosses.push_back({id, name, defeated});
    }
}

void GameStateReader::ReadCreatureCards(ProgressSnapshot& snap) {
    ReadCreatureCardsFromMemory(m_playerState, snap);
}

void GameStateReader::ReadLandmarks(ProgressSnapshot& snap) {
    ReadLandmarksFromMemory(m_gameState, snap);
}

void GameStateReader::ReadMilkMolars(ProgressSnapshot& snap) {
    ReadMilkMolarsFromMemory(m_gameState, m_playerState, snap);
}

void GameStateReader::ReadMixr(ProgressSnapshot& snap) {
    ReadMixrFromMemory(m_gameState, snap);
}

void GameStateReader::ReadScabSchemes(ProgressSnapshot& snap) {
    ReadScabSchemesFromMemory(m_playerState, snap);
}

void GameStateReader::ReadTableItems(ProgressSnapshot& snap) {
    ReadTableItemsFromMemory(m_gameState, snap);
}

void GameStateReader::ReadUpgrades(ProgressSnapshot& snap) {
    ReadUpgradesFromMemory(m_playerState, snap);
}
