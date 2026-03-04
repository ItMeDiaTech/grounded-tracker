#pragma once

#include <string>
#include <format>
#include <mutex>
#include <fstream>

class Logger {
public:
    static void Init(const std::wstring& logPath);
    static void Shutdown();
    static void Log(const char* level, const std::string& message);

    static Logger& Instance();

private:
    std::mutex m_mutex;
    std::ofstream m_file;
    bool m_initialized = false;
};

#define LOG_DEBUG(fmt, ...) Logger::Instance().Log("DEBUG", std::format(fmt __VA_OPT__(,) __VA_ARGS__))
#define LOG_INFO(fmt, ...)  Logger::Instance().Log("INFO",  std::format(fmt __VA_OPT__(,) __VA_ARGS__))
#define LOG_WARN(fmt, ...)  Logger::Instance().Log("WARN",  std::format(fmt __VA_OPT__(,) __VA_ARGS__))
#define LOG_ERROR(fmt, ...) Logger::Instance().Log("ERROR", std::format(fmt __VA_OPT__(,) __VA_ARGS__))
