#include "overlay_ui.h"
#include <imgui.h>
#include <mutex>
#include <cstdio>

static std::mutex g_stateMutex;
static ProgressSnapshot g_state;
static bool g_hasData = false;

namespace OverlayUI {

void UpdateState(const ProgressSnapshot& snap) {
    std::lock_guard lock(g_stateMutex);
    g_state = snap;
    g_hasData = true;
}

static void ProgressBar(const char* label, uint32_t current, uint32_t total) {
    float frac = (total > 0) ? static_cast<float>(current) / static_cast<float>(total) : 0.0f;
    char text[32];
    snprintf(text, sizeof(text), "%u/%u", current, total);

    ImGui::Text("%-16s", label);
    ImGui::SameLine(140.0f);
    ImGui::Text("%s", text);
    ImGui::SameLine(210.0f);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
        (current >= total) ? ImVec4(0.83f, 0.66f, 0.26f, 1.0f) : ImVec4(0.2f, 0.8f, 0.4f, 1.0f));
    ImGui::ProgressBar(frac, ImVec2(-1, 14), "");
    ImGui::PopStyleColor();
}

static uint32_t CountBosses(const std::vector<ProgressSnapshot::BossInfo>& items) {
    uint32_t count = 0;
    for (auto& item : items) if (item.defeated) count++;
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

static uint32_t CountCards(const std::vector<ProgressSnapshot::CreatureCardInfo>& items) {
    uint32_t count = 0;
    for (auto& item : items) if (item.collected) count++;
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

void Render() {
    std::lock_guard lock(g_stateMutex);

    ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

    char title[128];
    snprintf(title, sizeof(title), "GROUNDED: S RANK TRACKER [%.1f%%]###SRankTracker",
             g_hasData ? g_state.overallPercent : 0.0f);

    if (ImGui::Begin(title, nullptr, flags)) {
        if (!g_hasData) {
            ImGui::Text("Waiting for game data...");
        } else {
            // Main categories
            ProgressBar("Bosses", CountBosses(g_state.bosses),
                        static_cast<uint32_t>(g_state.bosses.size()));
            ProgressBar("Creature Cards", CountCards(g_state.creatureCards),
                        static_cast<uint32_t>(g_state.creatureCards.size()));
            ProgressBar("Landmarks", CountLandmarks(g_state.landmarks),
                        static_cast<uint32_t>(g_state.landmarks.size()));
            ProgressBar("Milk Molars",
                        g_state.milkMolars.regularCollected + g_state.milkMolars.megaCollected,
                        g_state.milkMolars.regularTotal + g_state.milkMolars.megaTotal);
            ProgressBar("MIX.R", CountMixr(g_state.mixrChallenges),
                        static_cast<uint32_t>(g_state.mixrChallenges.size()));
            ProgressBar("SCA.B Schemes", CountSchemes(g_state.scabSchemes),
                        static_cast<uint32_t>(g_state.scabSchemes.size()));

            ImGui::Separator();

            // Collectibles row
            ImGui::Columns(2, nullptr, false);
            auto wendellCollected = CountCollectibles(g_state.wendell);
            auto ominentCollected = CountCollectibles(g_state.ominent);
            auto burglCollected = CountCollectibles(g_state.burglChips);
            auto stuffCollected = CountCollectibles(g_state.stuff);

            ImGui::Text("Wendell  %u/%u", wendellCollected,
                        static_cast<uint32_t>(g_state.wendell.size()));
            ImGui::Text("BURG.L   %u/%u", burglCollected,
                        static_cast<uint32_t>(g_state.burglChips.size()));
            ImGui::NextColumn();
            ImGui::Text("Ominent  %u/%u", ominentCollected,
                        static_cast<uint32_t>(g_state.ominent.size()));
            ImGui::Text("Stuff    %u/%u", stuffCollected,
                        static_cast<uint32_t>(g_state.stuff.size()));
            ImGui::Columns(1);

            ImGui::Separator();
            ImGui::TextDisabled("[F9] Toggle | Polling: 2s | Connected");
        }
    }
    ImGui::End();
}

} // namespace OverlayUI
