#pragma once

#include <Windows.h>
#include <cstdint>
#include <optional>
#include <string>

namespace Memory {

// Scan a module's .text section for a byte pattern
// Pattern uses "??" for wildcards, e.g. "48 8B 05 ?? ?? ?? ?? 48 85 C0"
uintptr_t PatternScan(HMODULE module, const std::string& pattern);

// Check if a pointer is valid (committed, accessible memory)
bool IsValidPointer(const void* ptr);

// Safely read a value at base + offset, returns nullopt on failure
template<typename T>
std::optional<T> SafeRead(uintptr_t base, ptrdiff_t offset = 0) {
    auto* addr = reinterpret_cast<const T*>(base + offset);
    if (!IsValidPointer(addr)) return std::nullopt;

    __try {
        return *addr;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return std::nullopt;
    }
}

// Safely read a pointer and return as uintptr_t
std::optional<uintptr_t> SafeReadPtr(uintptr_t base, ptrdiff_t offset = 0);

// Resolve a RIP-relative address (instruction at addr, 4-byte displacement)
uintptr_t ResolveRIPRelative(uintptr_t addr);

} // namespace Memory
