#include "scab_schemes.h"
#include "../core/memory.h"
#include "../core/offsets.h"
#include "../core/ue4_types.h"
#include "../util/logger.h"
#include <algorithm>
#include <string>

// 55 SCA.B scheme mappings — matches src/data/scab-schemes.ts exactly
// IDs are lowercase and must match the lowercased UClass FName middle portion
// e.g. GlobalColorTheme_Aerobics_C → "aerobics"
static const struct { const char* id; const char* name; } SCHEME_DATA[] = {
    // Default (4)
    {"default", "SCA.B Classic"},
    {"highcontrast", "High Contrast"},
    {"lcd", "SCA.B LCD"},
    {"night", "SCA.B Night"},
    // Collectible (51)
    {"aerobics", "Aerobics"},
    {"hotdoghorror", "Billy Hog Horror"},
    {"visualaid", "Bugjuice"},
    {"chubbsseries", "Chubbs Series"},
    {"condimentjockey", "Condiment Jockey"},
    {"corporate", "Corporate Issue"},
    {"cyberstrike", "Cyberstrike"},
    {"dogloaf", "Dog Loaf"},
    {"flyingv", "Flying-V"},
    {"frankenline", "Frankenline"},
    {"fright", "Fright"},
    {"icecap", "Frostbite"},
    {"frosting", "Frosting"},
    {"garbo", "Garbo"},
    {"ghost", "Ghostmint"},
    {"gingerspice", "Gingerspice"},
    {"girthscape", "Girthscape"},
    {"greystone", "Greystone"},
    {"gunmetal", "Gunmetal"},
    {"hedgeberry", "Hedgeberry"},
    {"highdensity", "High Density"},
    {"holidazzle", "Holidazzle"},
    {"superblaster", "Hyperblaster"},
    {"sankematic", "JavaMatic"},
    {"lasercorp", "Lasercorp"},
    {"legend", "Legend"},
    {"miterider", "Miterider"},
    {"moldorc", "Moldorc"},
    {"murk", "Muck"},
    {"neomauve", "Neomauve"},
    {"panfish", "Panfish"},
    {"porridge", "Porridge"},
    {"puncho", "Punch-O"},
    {"questmodule", "RPG Module"},
    {"sawdust", "Sawdust"},
    {"scab1", "SCA.B v1.02"},
    {"scab2", "SCA.B v2.11"},
    {"scab3", "SCA.B v3.09"},
    {"scabbugged", "SCA.B Bugged"},
    {"sewage", "Sewage"},
    {"marsh", "Shallows"},
    {"shinebright", "Shinebright"},
    {"supreme", "Supreme"},
    {"metamorphosis", "Syndrome"},
    {"yogiswole", "Swole"},
    {"chewgum", "Table Gum"},
    {"textbook", "Textbook"},
    {"sadstab", "The Sad Stab"},
    {"supertoast", "Toasted"},
    {"tully", "Tully"},
    {"witch", "Witchly"},
};
static constexpr size_t SCHEME_COUNT = sizeof(SCHEME_DATA) / sizeof(SCHEME_DATA[0]);

static std::string ToLowerStr(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

// Extract scheme ID from a UClass FName like "GlobalColorTheme_Aerobics_C"
// Returns the lowercased middle portion between "GlobalColorTheme_" and "_C"
static std::string ExtractSchemeId(const std::string& className) {
    const std::string prefix = "GlobalColorTheme_";
    const std::string suffix = "_C";

    if (className.size() <= prefix.size() + suffix.size()) return ToLowerStr(className);
    if (className.compare(0, prefix.size(), prefix) != 0) return ToLowerStr(className);
    if (className.compare(className.size() - suffix.size(), suffix.size(), suffix) != 0)
        return ToLowerStr(className);

    std::string mid = className.substr(prefix.size(), className.size() - prefix.size() - suffix.size());
    return ToLowerStr(mid);
}

void ReadScabSchemesFromMemory(uintptr_t gameState, ProgressSnapshot& snap) {
    snap.scabSchemes.clear();
    snap.scabSchemes.reserve(SCHEME_COUNT);

    // Populate all schemes — mark first 4 (defaults) as always collected
    for (size_t i = 0; i < SCHEME_COUNT; ++i) {
        snap.scabSchemes.push_back({
            SCHEME_DATA[i].id,
            SCHEME_DATA[i].name,
            i < 4  // first 4 are default/always unlocked
        });
    }

    if (!gameState) return;

    // PartyComponent -> DiscoveredColorThemes (TArray<TSubclassOf<UGlobalColorTheme>>)
    // Each element is a UClass* (8 bytes)
    auto partyComp = Memory::SafeReadPtr(gameState, Offsets::GameState_PartyComponent);
    if (!partyComp) {
        LOG_DEBUG("ReadScabSchemes: PartyComponent null");
        return;
    }

    auto arr = ReadTArray(partyComp.value(), Offsets::PartyComp_DiscoveredColorThemes);
    if (!arr) {
        LOG_DEBUG("ReadScabSchemes: DiscoveredColorThemes array not readable");
        return;
    }

    uint32_t matched = 0;
    static bool loggedOnce = false;
    bool shouldLog = !loggedOnce && arr->count > 0;
    if (shouldLog) {
        LOG_INFO("=== DiscoveredColorThemes Dump (%d entries) ===", arr->count);
    }

    for (int32_t i = 0; i < arr->count; ++i) {
        auto classPtr = Memory::SafeReadPtr(arr->data, i * 8);
        if (!classPtr) continue;

        // UObject::NamePrivate is at offset 0x18 in UE4 4.27
        std::string className = ResolveFNameAt(classPtr.value(), Offsets::UObject_NamePrivate);
        if (className.empty()) continue;

        std::string schemeId = ExtractSchemeId(className);

        if (shouldLog) {
            LOG_INFO("  Theme[%d]: class='%s' id='%s'", i, className.c_str(), schemeId.c_str());
        }

        // Match against our scheme list (both are lowercase)
        for (auto& s : snap.scabSchemes) {
            if (s.id == schemeId) {
                s.collected = true;
                matched++;
                break;
            }
        }
    }

    if (shouldLog) {
        LOG_INFO("=== End DiscoveredColorThemes Dump (matched %u/%zu) ===",
                 matched, SCHEME_COUNT);
        loggedOnce = true;
    }

    LOG_DEBUG("ReadScabSchemes: %u/%zu matched from %d discovered themes",
              matched, SCHEME_COUNT, arr->count);
}
