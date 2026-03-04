#include "logger.h"
#include <Windows.h>
#include <chrono>
#include <filesystem>
#include <cstdio>

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

void Logger::Init(const std::wstring& logPath) {
    auto& inst = Instance();
    std::lock_guard lock(inst.m_mutex);

    if (inst.m_initialized) return;

    // Rotate: delete old log
    std::filesystem::path p(logPath);
    if (std::filesystem::exists(p)) {
        std::error_code ec;
        std::filesystem::remove(p, ec);
    }

    inst.m_file.open(p, std::ios::out | std::ios::trunc);
    inst.m_initialized = inst.m_file.is_open();

    if (inst.m_initialized) {
        inst.Log("INFO", "Logger initialized");
    }
}

void Logger::Shutdown() {
    auto& inst = Instance();
    std::lock_guard lock(inst.m_mutex);

    if (inst.m_file.is_open()) {
        inst.m_file.close();
    }
    inst.m_initialized = false;
}

void Logger::Log(const char* level, const std::string& message) {
    std::lock_guard lock(m_mutex);

    // Timestamp
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::tm tm;
    localtime_s(&tm, &time);

    char line[2048];
    snprintf(line, sizeof(line),
        "[%04d-%02d-%02d %02d:%02d:%02d.%03d] [%s] %s",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
        tm.tm_hour, tm.tm_min, tm.tm_sec,
        static_cast<int>(ms.count()),
        level, message.c_str());

    // Write to file
    if (m_initialized && m_file.is_open()) {
        m_file << line << std::endl;
    }

    // Also output to debugger
    OutputDebugStringA(line);
    OutputDebugStringA("\n");
}

void Logger::LogF(const char* level, const char* fmt, ...) {
    char msgBuf[2048];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, sizeof(msgBuf), fmt, args);
    va_end(args);
    Log(level, std::string(msgBuf));
}
