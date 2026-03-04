#include "overlay_ui.h"
#include "dx11_hook.h"
#include <imgui.h>
#include <mutex>
#include <cstdio>

static std::mutex g_stateMutex;
static ProgressSnapshot g_state;
static bool g_hasData = false;
static bool g_expanded[10] = {};

namespace OverlayUI {

void UpdateState(const ProgressSnapshot& snap) {
    std::lock_guard lock(g_stateMutex);
    g_state = snap;
    g_hasData = true;
}

static uint32_t CountBosses(const std::vector<ProgressSnapshot::BossInfo>& items) {
    uint32_t count = 0;
    for (auto& item : items) if (item.defeated) count++;
    return count;
}

static uint32_t CountCards(const std::vector<ProgressSnapshot::CreatureCardInfo>& items) {
    uint32_t count = 0;
    for (auto& item : items) if (item.collected) count++;
    return count;
}

static uint32_t CountLandmarks(const std::vector<ProgressSnapshot::LandmarkInfo>& items) {
    uint32_t count = 0;
    for (auto& item : items) if (item.discovered) count++;
    return count;
}

static uint32_t CountMixr(const std::vector<ProgressSnapshot::MixrInfo>& items) {
    uint32_t count = 0;
    for (auto& item : items) if (item.completed) count++;
    return count;
}

static uint32_t CountSchemes(const std::vector<ProgressSnapshot::ScabSchemeInfo>& items) {
    uint32_t count = 0;
    for (auto& item : items) if (item.collected) count++;
    return count;
}

static uint32_t CountCollectibles(const std::vector<ProgressSnapshot::CollectibleInfo>& items) {
    uint32_t count = 0;
    for (auto& item : items) if (item.collected) count++;
    return count;
}

static const ImVec4 kGold  = ImVec4(0.83f, 0.66f, 0.26f, 1.0f);
static const ImVec4 kGreen = ImVec4(0.2f, 0.8f, 0.4f, 1.0f);

// Renders one category row: triangle toggle + label + count + progress bar.
// Returns true if the category is expanded (caller should render detail).
static bool CategoryRow(int index, const char* label, uint32_t collected, uint32_t total) {
    bool complete = (collected >= total && total > 0);
    float frac = (total > 0) ? static_cast<float>(collected) / static_cast<float>(total) : 0.0f;

    // Triangle toggle
    ImGui::PushID(index);
    const char* arrow = g_expanded[index] ? "v" : ">";
    if (ImGui::SmallButton(arrow)) {
        g_expanded[index] = !g_expanded[index];
    }
    ImGui::SameLine();

    // Label
    ImGui::Text("%-16s", label);
    ImGui::SameLine(180.0f);

    // Count
    char countText[32];
    snprintf(countText, sizeof(countText), "%u/%u", collected, total);
    if (complete) ImGui::PushStyleColor(ImGuiCol_Text, kGold);
    ImGui::Text("%s", countText);
    if (complete) ImGui::PopStyleColor();
    ImGui::SameLine(240.0f);

    // Progress bar
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, complete ? kGold : kGreen);
    ImGui::ProgressBar(frac, ImVec2(-1, 14), "");
    ImGui::PopStyleColor();

    ImGui::PopID();
    return g_expanded[index];
}

// Shows bullet list of uncollected items by name. Gold "All complete!" when 100%.
template<typename T, typename NameFn, typename DoneFn>
static void UncollectedList(const std::vector<T>& items, NameFn getName, DoneFn isDone) {
    bool allDone = true;
    ImGui::Indent(20.0f);
    for (auto& item : items) {
        if (!isDone(item)) {
            allDone = false;
            ImGui::BulletText("%s", getName(item));
        }
    }
    if (allDone) {
        ImGui::PushStyleColor(ImGuiCol_Text, kGold);
        ImGui::Text("  All complete!");
        ImGui::PopStyleColor();
    }
    ImGui::Unindent(20.0f);
}

void Render() {
    std::lock_guard lock(g_stateMutex);

    ImGui::SetNextWindowSize(ImVec2(380, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

    char title[128];
    snprintf(title, sizeof(title), "GROUNDED: S RANK TRACKER [%.1f%%]###SRankTracker",
             g_hasData ? g_state.overallPercent : 0.0f);

    bool open = true;
    if (ImGui::Begin(title, &open, flags)) {
        if (!open) {
            ImGui::End();
            DX11Hook::ToggleOverlay();
            return;
        }

        if (!g_hasData) {
            ImGui::Text("Waiting for game data...");
        } else {
            // 0: Bosses
            uint32_t bossCount = CountBosses(g_state.bosses);
            if (CategoryRow(0, "Bosses", bossCount, static_cast<uint32_t>(g_state.bosses.size()))) {
                UncollectedList(g_state.bosses,
                    [](const ProgressSnapshot::BossInfo& b) { return b.name.c_str(); },
                    [](const ProgressSnapshot::BossInfo& b) { return b.defeated; });
            }

            // 1: Creature Cards
            uint32_t cardCount = CountCards(g_state.creatureCards);
            if (CategoryRow(1, "Creature Cards", cardCount, static_cast<uint32_t>(g_state.creatureCards.size()))) {
                UncollectedList(g_state.creatureCards,
                    [](const ProgressSnapshot::CreatureCardInfo& c) { return c.name.c_str(); },
                    [](const ProgressSnapshot::CreatureCardInfo& c) { return c.collected; });
            }

            // 2: Landmarks
            uint32_t landmarkCount = CountLandmarks(g_state.landmarks);
            if (CategoryRow(2, "Landmarks", landmarkCount, static_cast<uint32_t>(g_state.landmarks.size()))) {
                UncollectedList(g_state.landmarks,
                    [](const ProgressSnapshot::LandmarkInfo& l) { return l.name.c_str(); },
                    [](const ProgressSnapshot::LandmarkInfo& l) { return l.discovered; });
            }

            // 3: Milk Molars
            uint32_t molarCollected = g_state.milkMolars.regularCollected + g_state.milkMolars.megaCollected;
            uint32_t molarTotal = g_state.milkMolars.regularTotal + g_state.milkMolars.megaTotal;
            if (CategoryRow(3, "Milk Molars", molarCollected, molarTotal)) {
                ImGui::Indent(20.0f);
                bool allDone = (molarCollected >= molarTotal && molarTotal > 0);
                if (allDone) {
                    ImGui::PushStyleColor(ImGuiCol_Text, kGold);
                    ImGui::Text("  All complete!");
                    ImGui::PopStyleColor();
                } else {
                    ImGui::Text("  Regular: %u/%u | Mega: %u/%u | Spent: %u",
                        g_state.milkMolars.regularCollected, g_state.milkMolars.regularTotal,
                        g_state.milkMolars.megaCollected, g_state.milkMolars.megaTotal,
                        g_state.milkMolars.totalSpent);
                }
                ImGui::Unindent(20.0f);
            }

            // 4: MIX.R
            uint32_t mixrCount = CountMixr(g_state.mixrChallenges);
            if (CategoryRow(4, "MIX.R", mixrCount, static_cast<uint32_t>(g_state.mixrChallenges.size()))) {
                UncollectedList(g_state.mixrChallenges,
                    [](const ProgressSnapshot::MixrInfo& m) { return m.name.c_str(); },
                    [](const ProgressSnapshot::MixrInfo& m) { return m.completed; });
            }

            // 5: SCA.B Schemes
            uint32_t schemeCount = CountSchemes(g_state.scabSchemes);
            if (CategoryRow(5, "SCA.B Schemes", schemeCount, static_cast<uint32_t>(g_state.scabSchemes.size()))) {
                UncollectedList(g_state.scabSchemes,
                    [](const ProgressSnapshot::ScabSchemeInfo& s) { return s.name.c_str(); },
                    [](const ProgressSnapshot::ScabSchemeInfo& s) { return s.collected; });
            }

            // 6: Wendell
            uint32_t wendellCount = CountCollectibles(g_state.wendell);
            if (CategoryRow(6, "Wendell", wendellCount, static_cast<uint32_t>(g_state.wendell.size()))) {
                UncollectedList(g_state.wendell,
                    [](const ProgressSnapshot::CollectibleInfo& c) { return c.name.c_str(); },
                    [](const ProgressSnapshot::CollectibleInfo& c) { return c.collected; });
            }

            // 7: Ominent
            uint32_t ominentCount = CountCollectibles(g_state.ominent);
            if (CategoryRow(7, "Ominent", ominentCount, static_cast<uint32_t>(g_state.ominent.size()))) {
                UncollectedList(g_state.ominent,
                    [](const ProgressSnapshot::CollectibleInfo& c) { return c.name.c_str(); },
                    [](const ProgressSnapshot::CollectibleInfo& c) { return c.collected; });
            }

            // 8: BURG.L Chips
            uint32_t burglCount = CountCollectibles(g_state.burglChips);
            if (CategoryRow(8, "BURG.L Chips", burglCount, static_cast<uint32_t>(g_state.burglChips.size()))) {
                UncollectedList(g_state.burglChips,
                    [](const ProgressSnapshot::CollectibleInfo& c) { return c.name.c_str(); },
                    [](const ProgressSnapshot::CollectibleInfo& c) { return c.collected; });
            }

            // 9: Stuff
            uint32_t stuffCount = CountCollectibles(g_state.stuff);
            if (CategoryRow(9, "Stuff", stuffCount, static_cast<uint32_t>(g_state.stuff.size()))) {
                UncollectedList(g_state.stuff,
                    [](const ProgressSnapshot::CollectibleInfo& c) { return c.name.c_str(); },
                    [](const ProgressSnapshot::CollectibleInfo& c) { return c.collected; });
            }

            ImGui::Separator();
            ImGui::TextDisabled("[F9] Toggle | Polling: 2s");
        }
    }
    ImGui::End();
}

} // namespace OverlayUI
