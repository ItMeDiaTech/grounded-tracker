#pragma once

#include <string>
#include <optional>
#include <cstdint>

namespace Achievements {

// Check if an achievement flag is set
bool IsAchievementComplete(uintptr_t gameState, const std::string& achievementId);

// Get achievement progress value (e.g., SpendMolars count)
std::optional<int32_t> GetAchievementProgress(uintptr_t gameState, const std::string& achievementId);

} // namespace Achievements
