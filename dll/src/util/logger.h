#pragma once

#include <string>
#include <mutex>
#include <fstream>
#include <cstdarg>

class Logger {
public:
    static void Init(const std::wstring& logPath);
    static void Shutdown();

    void Log(const char* level, const std::string& message);
    void LogF(const char* level, const char* fmt, ...);

    static Logger& Instance();

private:
    std::recursive_mutex m_mutex;
    std::ofstream m_file;
    bool m_initialized = false;
};

// Printf-style variadic macros (VS2019 compatible)
#define LOG_DEBUG(...) Logger::Instance().LogF("DEBUG", __VA_ARGS__)
#define LOG_INFO(...)  Logger::Instance().LogF("INFO",  __VA_ARGS__)
#define LOG_WARN(...)  Logger::Instance().LogF("WARN",  __VA_ARGS__)
#define LOG_ERROR(...) Logger::Instance().LogF("ERROR", __VA_ARGS__)
