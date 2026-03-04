#include "achievements.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"
#include <algorithm>

namespace Achievements {

// Walk the AchievementsComponent's TArray<FAchievementStatus> looking for a match.
// Returns pointer to the matched element, or 0 if not found.
static uintptr_t FindAchievement(uintptr_t playerState, const std::string& achievementId) {
    if (!playerState) return 0;

    auto achieveComp = Memory::SafeReadPtr(playerState, Offsets::PlayerState_AchievementsComponent);
    if (!achieveComp) return 0;

    auto arr = ReadTArray(achieveComp.value(), Offsets::AchievementComp_Achievements);
    if (!arr) return 0;

    for (int32_t i = 0; i < arr->count; ++i) {
        uintptr_t elem = arr->data + i * Offsets::AchievementStatus_Size;

        // FName_NetCrc at element + 0x50 (FName is first 8 bytes, CRC is next 4)
        std::string name = ResolveFNameAt(elem, Offsets::AchievementStatus_Name);
        if (name.empty()) continue;

        // Case-insensitive comparison — engine FNames may differ in casing
        std::string nameLower = name;
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
        std::string idLower = achievementId;
        std::transform(idLower.begin(), idLower.end(), idLower.begin(), ::tolower);

        if (nameLower == idLower) return elem;
    }

    return 0;
}

bool IsAchievementComplete(uintptr_t playerState, const std::string& achievementId) {
    if (!playerState) return false;

    auto elem = FindAchievement(playerState, achievementId);
    if (!elem) return false;

    auto statValue = Memory::SafeRead<int32_t>(elem, Offsets::AchievementStatus_StatValue);
    return statValue.has_value() && statValue.value() > 0;
}

std::optional<int32_t> GetAchievementProgress(uintptr_t playerState, const std::string& achievementId) {
    if (!playerState) return std::nullopt;

    auto elem = FindAchievement(playerState, achievementId);
    if (!elem) return std::nullopt;

    return Memory::SafeRead<int32_t>(elem, Offsets::AchievementStatus_StatValue);
}

void LogAllAchievements(uintptr_t playerState) {
    if (!playerState) {
        LOG_WARN("LogAllAchievements: null playerState");
        return;
    }

    auto achieveComp = Memory::SafeReadPtr(playerState, Offsets::PlayerState_AchievementsComponent);
    if (!achieveComp) {
        LOG_WARN("LogAllAchievements: could not read AchievementsComponent");
        return;
    }

    auto arr = ReadTArray(achieveComp.value(), Offsets::AchievementComp_Achievements);
    if (!arr) {
        LOG_WARN("LogAllAchievements: could not read achievements TArray");
        return;
    }

    LOG_INFO("=== Achievement Dump (%d entries) ===", arr->count);
    for (int32_t i = 0; i < arr->count; ++i) {
        uintptr_t elem = arr->data + i * Offsets::AchievementStatus_Size;
        std::string name = ResolveFNameAt(elem, Offsets::AchievementStatus_Name);
        auto statValue = Memory::SafeRead<int32_t>(elem, Offsets::AchievementStatus_StatValue);

        LOG_INFO("  [%d] '%s' = %d", i, name.c_str(),
                 statValue.value_or(-1));
    }
    LOG_INFO("=== End Achievement Dump ===");
}

} // namespace Achievements
