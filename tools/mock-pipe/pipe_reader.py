"""Connect to an existing named pipe and read messages."""
import ctypes
import ctypes.wintypes
import struct
import json
import sys

kernel32 = ctypes.windll.kernel32
kernel32.CreateFileW.restype = ctypes.wintypes.HANDLE
kernel32.ReadFile.restype = ctypes.wintypes.BOOL
kernel32.CloseHandle.restype = ctypes.wintypes.BOOL

PIPE_PATH = r"\\.\pipe\dt-grounded"
GENERIC_READ = 0x80000000
OPEN_EXISTING = 3
INVALID_HANDLE = ctypes.wintypes.HANDLE(-1).value


def read_exact(handle, n):
    buf = ctypes.create_string_buffer(n)
    read_bytes = ctypes.wintypes.DWORD()
    ok = kernel32.ReadFile(handle, buf, n, ctypes.byref(read_bytes), None)
    if not ok:
        raise OSError(f"ReadFile failed: error {ctypes.GetLastError()}")
    if read_bytes.value != n:
        raise OSError(f"Short read: expected {n}, got {read_bytes.value}")
    return buf.raw[:n]


def main():
    count = int(sys.argv[1]) if len(sys.argv) > 1 else 1

    print(f"Connecting to {PIPE_PATH}...")
    handle = kernel32.CreateFileW(PIPE_PATH, GENERIC_READ, 0, None, OPEN_EXISTING, 0, None)
    if handle == INVALID_HANDLE:
        print(f"Failed to connect: error {ctypes.GetLastError()}")
        sys.exit(1)

    print("Connected!\n")

    for i in range(count):
        length_bytes = read_exact(handle, 4)
        length = struct.unpack("<I", length_bytes)[0]
        json_bytes = read_exact(handle, length)
        data = json.loads(json_bytes)

        print(f"--- Message {i+1} ({length} bytes) ---")
        print(f"  Save:      {data['saveName']}")
        print(f"  Overall:   {data['overallPercent']}%")
        print(f"  Bosses:    {data['bosses']['collectedCount']}/{data['bosses']['totalCount']}")
        print(f"  Creatures: {data['creatureCards']['collectedCount']}/{data['creatureCards']['totalCount']}")
        print(f"  Landmarks: {data['landmarks']['collectedCount']}/{data['landmarks']['totalCount']}")
        mm = data['milkMolars']
        print(f"  Molars:    reg={mm['regularCollected']}/{mm['regularTotal']}, mega={mm['megaCollected']}/{mm['megaTotal']}")
        print(f"  MIX.R:     {data['mixrChallenges']['collectedCount']}/{data['mixrChallenges']['totalCount']}")
        print(f"  SCA.B:     {data['scabSchemes']['collectedCount']}/{data['scabSchemes']['totalCount']}")
        print(f"  Wendell:   {data['wendell']['collectedCount']}/{data['wendell']['totalCount']}")
        print(f"  Ominent:   {data['ominent']['collectedCount']}/{data['ominent']['totalCount']}")
        print(f"  BURG.L:    {data['burglChips']['collectedCount']}/{data['burglChips']['totalCount']}")
        print(f"  Stuff:     {data['stuff']['collectedCount']}/{data['stuff']['totalCount']}")
        print()

    kernel32.CloseHandle(handle)


if __name__ == "__main__":
    main()
