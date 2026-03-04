"""Inject Dumper-7.dll into a running process by PID."""
import ctypes
import ctypes.wintypes
import sys
import os

kernel32 = ctypes.windll.kernel32

# Set return types and arg types for 64-bit compatibility
kernel32.OpenProcess.restype = ctypes.wintypes.HANDLE
kernel32.VirtualAllocEx.restype = ctypes.c_uint64
kernel32.VirtualAllocEx.argtypes = [
    ctypes.wintypes.HANDLE, ctypes.c_uint64, ctypes.c_size_t,
    ctypes.wintypes.DWORD, ctypes.wintypes.DWORD,
]
kernel32.WriteProcessMemory.restype = ctypes.wintypes.BOOL
kernel32.WriteProcessMemory.argtypes = [
    ctypes.wintypes.HANDLE, ctypes.c_uint64, ctypes.c_void_p,
    ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t),
]
kernel32.CreateRemoteThread.restype = ctypes.wintypes.HANDLE
kernel32.CreateRemoteThread.argtypes = [
    ctypes.wintypes.HANDLE, ctypes.c_void_p, ctypes.c_size_t,
    ctypes.c_uint64, ctypes.c_uint64, ctypes.wintypes.DWORD,
    ctypes.POINTER(ctypes.wintypes.DWORD),
]
kernel32.GetModuleHandleW.restype = ctypes.wintypes.HMODULE
kernel32.GetProcAddress.restype = ctypes.c_uint64
kernel32.GetProcAddress.argtypes = [ctypes.wintypes.HMODULE, ctypes.c_char_p]
kernel32.WaitForSingleObject.restype = ctypes.wintypes.DWORD

PROCESS_ALL_ACCESS = 0x001F0FFF
MEM_COMMIT = 0x1000
MEM_RESERVE = 0x2000
PAGE_READWRITE = 0x04
INFINITE = 0xFFFFFFFF


def inject_dll(pid: int, dll_path: str) -> bool:
    """Inject a DLL into a process by PID."""
    # Ensure absolute path
    dll_path = os.path.abspath(dll_path)
    if not os.path.exists(dll_path):
        print(f"ERROR: DLL not found: {dll_path}")
        return False

    dll_path_w = dll_path.encode("utf-16-le") + b"\x00\x00"
    path_size = len(dll_path_w)

    print(f"Target PID: {pid}")
    print(f"DLL: {dll_path}")
    print(f"Path size: {path_size} bytes")

    # Open process
    h_process = kernel32.OpenProcess(PROCESS_ALL_ACCESS, False, pid)
    if not h_process:
        print(f"ERROR: OpenProcess failed: {ctypes.GetLastError()}")
        return False
    print("Opened process handle")

    try:
        # Allocate memory in target process
        remote_mem = kernel32.VirtualAllocEx(
            h_process, 0, path_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE
        )
        if not remote_mem:
            print(f"ERROR: VirtualAllocEx failed: {ctypes.GetLastError()}")
            return False
        print(f"Allocated remote memory at 0x{remote_mem:X}")

        # Write DLL path to target process
        written = ctypes.c_size_t(0)
        ok = kernel32.WriteProcessMemory(
            h_process, remote_mem, dll_path_w, path_size, ctypes.byref(written)
        )
        if not ok:
            print(f"ERROR: WriteProcessMemory failed: {ctypes.GetLastError()}")
            return False
        print(f"Wrote {written.value} bytes to remote memory")

        # Get LoadLibraryW address
        h_kernel32 = kernel32.GetModuleHandleW("kernel32.dll")
        load_library_addr = kernel32.GetProcAddress(h_kernel32, b"LoadLibraryW")
        if not load_library_addr:
            print(f"ERROR: GetProcAddress failed: {ctypes.GetLastError()}")
            return False
        print(f"LoadLibraryW at 0x{load_library_addr:X}")

        # Create remote thread
        thread_id = ctypes.wintypes.DWORD(0)
        h_thread = kernel32.CreateRemoteThread(
            h_process,
            None,
            0,
            load_library_addr,
            remote_mem,
            0,
            ctypes.byref(thread_id),
        )
        if not h_thread:
            print(f"ERROR: CreateRemoteThread failed: {ctypes.GetLastError()}")
            return False
        print(f"Created remote thread (TID: {thread_id.value})")

        # Wait for thread to complete
        print("Waiting for DLL to load...")
        kernel32.WaitForSingleObject(h_thread, INFINITE)
        print("DLL loaded successfully!")

        kernel32.CloseHandle(h_thread)
        return True

    finally:
        kernel32.CloseHandle(h_process)


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <PID> <DLL_PATH>")
        sys.exit(1)

    pid = int(sys.argv[1])
    dll_path = sys.argv[2]

    if inject_dll(pid, dll_path):
        print("\nInjection complete!")
    else:
        print("\nInjection FAILED!")
        sys.exit(1)
