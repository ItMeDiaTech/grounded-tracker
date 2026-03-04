#include "ue4_types.h"
#include "memory.h"
#include "offsets.h"
#include "../util/logger.h"
#include <Windows.h>
#include <cstdio>
#include <unordered_map>
#include <mutex>

// ============================================================================
// TArray Reader
// ============================================================================

std::optional<TArrayView> ReadTArray(uintptr_t base, ptrdiff_t offset) {
    if (!base) return std::nullopt;

    auto data = Memory::SafeReadPtr(base, offset + Offsets::TArray_Data);
    auto count = Memory::SafeRead<int32_t>(base, offset + Offsets::TArray_Count);

    if (!data || !count) return std::nullopt;
    if (count.value() < 0 || count.value() > 100000) return std::nullopt;

    // Empty array is valid
    if (count.value() == 0) return TArrayView{data.value(), 0};

    if (!Memory::IsValidPointer(reinterpret_cast<const void*>(data.value())))
        return std::nullopt;

    return TArrayView{data.value(), count.value()};
}

// ============================================================================
// FGuid Reader
// ============================================================================

std::optional<FGuid> ReadFGuid(uintptr_t base, ptrdiff_t offset) {
    if (!base) return std::nullopt;

    auto a = Memory::SafeRead<uint32_t>(base, offset + 0x00);
    auto b = Memory::SafeRead<uint32_t>(base, offset + 0x04);
    auto c = Memory::SafeRead<uint32_t>(base, offset + 0x08);
    auto d = Memory::SafeRead<uint32_t>(base, offset + 0x0C);

    if (!a || !b || !c || !d) return std::nullopt;

    return FGuid{a.value(), b.value(), c.value(), d.value()};
}

std::string FGuidToString(const FGuid& guid) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%08X-%08X-%08X-%08X", guid.A, guid.B, guid.C, guid.D);
    return buf;
}

// ============================================================================
// FName Resolution via FName::AppendString
// ============================================================================

// UE4 FString layout: wchar_t* Data (0x0), int32 Count (0x8), int32 Max (0xC)
struct FString {
    wchar_t* Data = nullptr;
    int32_t Count = 0;
    int32_t Max = 0;
};

// FName::AppendString signature: void __thiscall FName::AppendString(FString& out) const
using FNameAppendStringFn = void(__thiscall*)(const void* thisPtr, FString& out);

static FNameAppendStringFn g_appendString = nullptr;

// Cache resolved FNames to avoid repeated engine calls and bounded memory leak.
// FString::Data is allocated by UE4's internal allocator (FMallocBinned2) which we
// cannot free externally. Each unique FName leaks ~50 bytes; with <500 unique names
// in the game that's <25KB total — negligible for a game using gigabytes of RAM.
static std::unordered_map<uint64_t, std::string> g_nameCache;
static std::mutex g_nameCacheMutex;

bool InitFNameResolver() {
    HMODULE gameModule = GetModuleHandleW(L"Maine-Win64-Shipping.exe");
    if (!gameModule) {
        LOG_ERROR("InitFNameResolver: game module not found");
        return false;
    }

    auto moduleBase = reinterpret_cast<uintptr_t>(gameModule);
    g_appendString = reinterpret_cast<FNameAppendStringFn>(moduleBase + Offsets::AppendString);

    // Validate the function pointer
    if (!Memory::IsValidPointer(reinterpret_cast<const void*>(g_appendString))) {
        LOG_ERROR("InitFNameResolver: AppendString at 0x%llx is not valid memory",
                  reinterpret_cast<uintptr_t>(g_appendString));
        g_appendString = nullptr;
        return false;
    }

    LOG_INFO("FName::AppendString resolved at 0x%llx", reinterpret_cast<uintptr_t>(g_appendString));
    return true;
}

// SEH-protected wrapper — must not contain C++ objects with destructors
static bool CallAppendStringSEH(const void* fnamePtr, FString* outStr) {
    __try {
        g_appendString(fnamePtr, *outStr);
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

std::string ResolveFName(uintptr_t fnameAddr) {
    if (!fnameAddr || !g_appendString) return "";

    // Validate the FName memory is readable
    if (!Memory::IsValidPointer(reinterpret_cast<const void*>(fnameAddr))) return "";

    // FName is 8 bytes: ComparisonIndex (u32) + Number (u32)
    // Use both as cache key to handle numbered names (e.g., "Foo_2")
    auto key = Memory::SafeRead<uint64_t>(fnameAddr);
    if (!key) return "";

    // Check cache
    {
        std::lock_guard<std::mutex> lock(g_nameCacheMutex);
        auto it = g_nameCache.find(key.value());
        if (it != g_nameCache.end()) return it->second;
    }

    // Call engine's FName::AppendString (SEH-protected)
    FString result = {};
    if (!CallAppendStringSEH(reinterpret_cast<const void*>(fnameAddr), &result)) {
        LOG_WARN("ResolveFName: exception calling AppendString at FName 0x%llx", fnameAddr);
        return "";
    }

    std::string out;
    if (result.Data && result.Count > 0) {
        // Convert wchar_t to std::string (UE4 FNames are ASCII)
        out.reserve(result.Count);
        for (int32_t i = 0; i < result.Count - 1; ++i) {  // -1 to skip null terminator
            out.push_back(static_cast<char>(result.Data[i]));
        }
    }

    // Cache the result (Data is leaked intentionally — see comment above)
    {
        std::lock_guard<std::mutex> lock(g_nameCacheMutex);
        g_nameCache[key.value()] = out;
    }

    return out;
}

std::string ResolveFNameAt(uintptr_t base, ptrdiff_t offset) {
    if (!base) return "";
    return ResolveFName(base + offset);
}
