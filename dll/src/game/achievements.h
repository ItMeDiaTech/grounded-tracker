#pragma once

#include <string>
#include <optional>
#include <cstdint>

namespace Achievements {

// Check if an achievement flag is set (reads from PlayerState's AchievementsComponent)
bool IsAchievementComplete(uintptr_t playerState, const std::string& achievementId);

// Get achievement progress value (e.g., SpendMolars count)
std::optional<int32_t> GetAchievementProgress(uintptr_t playerState, const std::string& achievementId);

// Log all achievement FNames (diagnostic — call once during init)
void LogAllAchievements(uintptr_t playerState);

} // namespace Achievements
