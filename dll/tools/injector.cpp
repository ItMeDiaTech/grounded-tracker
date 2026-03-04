#include <Windows.h>
#include <TlHelp32.h>
#include <cstdio>
#include <string>
#include <filesystem>

static DWORD FindProcessByName(const wchar_t* processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);

    DWORD pid = 0;
    if (Process32FirstW(snapshot, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, processName) == 0) {
                pid = pe.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &pe));
    }

    CloseHandle(snapshot);
    return pid;
}

int wmain(int argc, wchar_t* argv[]) {
    wprintf(L"Grounded Tracker — DLL Injector\n");
    wprintf(L"================================\n\n");

    // Determine DLL path
    std::wstring dllPath;
    if (argc > 1) {
        dllPath = argv[1];
    } else {
        // Look for grounded_tracker.dll next to the injector
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);
        auto dir = std::filesystem::path(exePath).parent_path();
        dllPath = (dir / L"grounded_tracker.dll").wstring();
    }

    if (!std::filesystem::exists(dllPath)) {
        wprintf(L"ERROR: DLL not found: %ls\n", dllPath.c_str());
        return 1;
    }

    wprintf(L"DLL: %ls\n", dllPath.c_str());

    // Find Grounded process
    const wchar_t* targetProcess = L"Maine-Win64-Shipping.exe";
    wprintf(L"Looking for %ls...\n", targetProcess);

    DWORD pid = FindProcessByName(targetProcess);
    if (pid == 0) {
        wprintf(L"ERROR: Grounded is not running.\n");
        wprintf(L"Launch the game first, then run this injector.\n");
        return 1;
    }

    wprintf(L"Found process PID: %lu\n", pid);

    // Open process
    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE, pid
    );

    if (!hProcess) {
        wprintf(L"ERROR: Failed to open process (error %lu).\n", GetLastError());
        wprintf(L"Try running as Administrator.\n");
        return 1;
    }

    // Allocate memory in target process for DLL path
    size_t pathSize = (dllPath.size() + 1) * sizeof(wchar_t);
    void* remoteMem = VirtualAllocEx(hProcess, nullptr, pathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!remoteMem) {
        wprintf(L"ERROR: VirtualAllocEx failed (error %lu).\n", GetLastError());
        CloseHandle(hProcess);
        return 1;
    }

    // Write DLL path to target process memory
    if (!WriteProcessMemory(hProcess, remoteMem, dllPath.c_str(), pathSize, nullptr)) {
        wprintf(L"ERROR: WriteProcessMemory failed (error %lu).\n", GetLastError());
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Get LoadLibraryW address
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    auto pLoadLibraryW = reinterpret_cast<LPTHREAD_START_ROUTINE>(
        GetProcAddress(hKernel32, "LoadLibraryW")
    );

    // Create remote thread calling LoadLibraryW with our DLL path
    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, pLoadLibraryW, remoteMem, 0, nullptr);

    if (!hThread) {
        wprintf(L"ERROR: CreateRemoteThread failed (error %lu).\n", GetLastError());
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    wprintf(L"DLL injected! Waiting for initialization...\n");

    // Wait for the thread to complete
    WaitForSingleObject(hThread, 10000);

    // Check if DLL loaded successfully
    DWORD exitCode = 0;
    GetExitCodeThread(hThread, &exitCode);

    if (exitCode == 0) {
        wprintf(L"WARNING: LoadLibraryW returned NULL — DLL may have failed to load.\n");
    } else {
        wprintf(L"SUCCESS: DLL loaded (handle: 0x%lX).\n", exitCode);
        wprintf(L"Check %%TEMP%%\\GroundedTracker.log for status.\n");
    }

    // Cleanup
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}
