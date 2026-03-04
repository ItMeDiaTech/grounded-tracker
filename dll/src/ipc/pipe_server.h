#pragma once

#include <Windows.h>
#include <atomic>
#include <mutex>
#include <thread>
#include "../game/game_state.h"

class PipeServer {
public:
    bool Start();
    void Stop();
    void PushUpdate(const ProgressSnapshot& snap);
    void PushHeartbeat();

private:
    HANDLE m_hPipe = INVALID_HANDLE_VALUE;
    std::atomic<bool> m_running{false};
    std::mutex m_writeMutex;
    std::thread m_acceptThread;

    void AcceptLoop();
    bool WriteMessage(const std::string& json);
};
