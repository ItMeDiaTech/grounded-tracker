#include "memory.h"
#include "../util/logger.h"
#include <sstream>
#include <vector>

namespace Memory {

static std::vector<uint8_t> ParsePattern(const std::string& pattern, std::vector<bool>& mask) {
    std::vector<uint8_t> bytes;
    std::istringstream stream(pattern);
    std::string token;

    while (stream >> token) {
        if (token == "??" || token == "?") {
            bytes.push_back(0);
            mask.push_back(false);
        } else {
            bytes.push_back(static_cast<uint8_t>(std::stoul(token, nullptr, 16)));
            mask.push_back(true);
        }
    }
    return bytes;
}

uintptr_t PatternScan(HMODULE module, const std::string& pattern) {
    if (!module) return 0;

    auto* dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(module);
    auto* ntHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(
        reinterpret_cast<uintptr_t>(module) + dosHeader->e_lfanew);

    auto moduleBase = reinterpret_cast<uintptr_t>(module);
    auto moduleSize = ntHeaders->OptionalHeader.SizeOfImage;

    std::vector<bool> mask;
    auto bytes = ParsePattern(pattern, mask);

    if (bytes.empty()) return 0;

    auto* scanStart = reinterpret_cast<const uint8_t*>(moduleBase);
    auto* scanEnd = scanStart + moduleSize - bytes.size();

    for (auto* current = scanStart; current <= scanEnd; ++current) {
        bool found = true;
        for (size_t i = 0; i < bytes.size(); ++i) {
            if (mask[i] && current[i] != bytes[i]) {
                found = false;
                break;
            }
        }
        if (found) {
            return reinterpret_cast<uintptr_t>(current);
        }
    }

    return 0;
}

bool IsValidPointer(const void* ptr) {
    if (!ptr) return false;

    MEMORY_BASIC_INFORMATION mbi{};
    if (VirtualQuery(ptr, &mbi, sizeof(mbi)) == 0) return false;

    if (mbi.State != MEM_COMMIT) return false;
    if (mbi.Protect & PAGE_NOACCESS) return false;
    if (mbi.Protect & PAGE_GUARD) return false;

    return true;
}

std::optional<uintptr_t> SafeReadPtr(uintptr_t base, ptrdiff_t offset) {
    auto* addr = reinterpret_cast<const uintptr_t*>(base + offset);
    if (!IsValidPointer(addr)) return std::nullopt;

    __try {
        uintptr_t value = *addr;
        if (value == 0) return std::nullopt;
        return value;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return std::nullopt;
    }
}

uintptr_t ResolveRIPRelative(uintptr_t addr) {
    // RIP-relative: instruction at addr has a 4-byte displacement
    // Effective address = addr + 4 + *(int32_t*)addr
    // But typically the pattern scan lands at the start of the instruction,
    // and the displacement is at addr+3 (after opcode bytes).
    // Caller should pass the address of the displacement itself.
    if (!IsValidPointer(reinterpret_cast<const void*>(addr))) return 0;

    int32_t displacement = *reinterpret_cast<const int32_t*>(addr);
    return addr + 4 + displacement;
}

} // namespace Memory
