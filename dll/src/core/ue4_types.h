#pragma once

#include <cstdint>
#include <optional>
#include <string>

// TArray view — lightweight struct for reading UE4 TArray fields
struct TArrayView {
    uintptr_t data;
    int32_t count;
};

// Read a TArray at base+offset. Validates pointer and count.
std::optional<TArrayView> ReadTArray(uintptr_t base, ptrdiff_t offset);

// FGuid — 16-byte UE4 globally unique identifier
struct FGuid {
    uint32_t A, B, C, D;
};

std::optional<FGuid> ReadFGuid(uintptr_t base, ptrdiff_t offset);
std::string FGuidToString(const FGuid& guid);

// FName resolution via engine's FName::AppendString
// Reads the FName at fnameAddr and converts to std::string
std::string ResolveFName(uintptr_t fnameAddr);

// Convenience: resolve FName at base+offset
std::string ResolveFNameAt(uintptr_t base, ptrdiff_t offset);

// Initialize the FName resolver (must be called after engine init)
bool InitFNameResolver();
