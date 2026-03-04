#pragma once

#include "../game/game_state.h"
#include <mutex>

namespace OverlayUI {

// Update the shared state (called from poll thread)
void UpdateState(const ProgressSnapshot& snap);

// Render the overlay (called from Present hook)
void Render();

} // namespace OverlayUI
