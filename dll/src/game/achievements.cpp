#include "achievements.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../util/logger.h"

namespace Achievements {

bool IsAchievementComplete(uintptr_t gameState, const std::string& achievementId) {
    if (!gameState) return false;

    // TODO: Walk achievement component after SDK generation
    // GameState -> AchievementsComponent -> find achievement by ID -> check completed flag
    LOG_DEBUG("IsAchievementComplete('%s') — stub, offsets not populated", achievementId.c_str());
    return false;
}

std::optional<int32_t> GetAchievementProgress(uintptr_t gameState, const std::string& achievementId) {
    if (!gameState) return std::nullopt;

    // TODO: Walk achievement component after SDK generation
    LOG_DEBUG("GetAchievementProgress('%s') — stub, offsets not populated", achievementId.c_str());
    return std::nullopt;
}

} // namespace Achievements
