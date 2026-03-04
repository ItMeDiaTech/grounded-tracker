#include "landmarks.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../util/logger.h"

// 108 landmark entries — matches src-tauri/src/categories/landmarks.rs
// POI IDs and zones are populated from the frontend data files
// The DLL reads discovered POI list from GameState and matches by ID

void ReadLandmarksFromMemory(uintptr_t gameState, ProgressSnapshot& snap) {
    snap.landmarks.clear();

    if (!gameState) return;

    // TODO: Read discovered POIs from UE4 memory after SDK generation
    // GameState -> DiscoveredPOIs (TArray<FName>) -> convert FName to string
    // Match against known POI list from frontend data
    LOG_DEBUG("ReadLandmarks — stub, offsets not populated");
}
