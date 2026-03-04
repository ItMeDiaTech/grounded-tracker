#include "overlay_ui.h"
#include <imgui.h>
#include <mutex>
#include <format>

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
    auto text = std::format("{}/{}", current, total);

    ImGui::Text("%-16s", label);
    ImGui::SameLine(140.0f);
    ImGui::Text("%s", text.c_str());
    ImGui::SameLine(210.0f);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
        (current >= total) ? ImVec4(0.83f, 0.66f, 0.26f, 1.0f) : ImVec4(0.2f, 0.8f, 0.4f, 1.0f));
    ImGui::ProgressBar(frac, ImVec2(-1, 14), "");
    ImGui::PopStyleColor();
}

static uint32_t CountCollected(const auto& items) {
    uint32_t count = 0;
    for (auto& item : items) {
        if constexpr (requires { item.defeated; }) {
            if (item.defeated) count++;
        } else if constexpr (requires { item.discovered; }) {
            if (item.discovered) count++;
        } else if constexpr (requires { item.completed; }) {
            if (item.completed) count++;
        } else if constexpr (requires { item.collected; }) {
            if (item.collected) count++;
        }
    }
    return count;
}

void Render() {
    std::lock_guard lock(g_stateMutex);

    ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

    auto title = std::format("GROUNDED: S RANK TRACKER [{:.1f}%]###SRankTracker",
                              g_hasData ? g_state.overallPercent : 0.0f);

    if (ImGui::Begin(title.c_str(), nullptr, flags)) {
        if (!g_hasData) {
            ImGui::Text("Waiting for game data...");
        } else {
            // Main categories
            ProgressBar("Bosses", CountCollected(g_state.bosses),
                        static_cast<uint32_t>(g_state.bosses.size()));
            ProgressBar("Creature Cards", CountCollected(g_state.creatureCards),
                        static_cast<uint32_t>(g_state.creatureCards.size()));
            ProgressBar("Landmarks", CountCollected(g_state.landmarks),
                        static_cast<uint32_t>(g_state.landmarks.size()));
            ProgressBar("Milk Molars",
                        g_state.milkMolars.regularCollected + g_state.milkMolars.megaCollected,
                        g_state.milkMolars.regularTotal + g_state.milkMolars.megaTotal);
            ProgressBar("MIX.R", CountCollected(g_state.mixrChallenges),
                        static_cast<uint32_t>(g_state.mixrChallenges.size()));
            ProgressBar("SCA.B Schemes", CountCollected(g_state.scabSchemes),
                        static_cast<uint32_t>(g_state.scabSchemes.size()));

            ImGui::Separator();

            // Collectibles row
            ImGui::Columns(2, nullptr, false);
            auto wendellCollected = CountCollected(g_state.wendell);
            auto ominentCollected = CountCollected(g_state.ominent);
            auto burglCollected = CountCollected(g_state.burglChips);
            auto stuffCollected = CountCollected(g_state.stuff);

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
