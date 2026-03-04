"""Quick test client for mock_pipe_server.py using ctypes."""
import subprocess
import sys
import time
import struct
import json
import os
import ctypes
import ctypes.wintypes

kernel32 = ctypes.windll.kernel32
kernel32.CreateFileW.restype = ctypes.wintypes.HANDLE
kernel32.ReadFile.restype = ctypes.wintypes.BOOL
kernel32.CloseHandle.restype = ctypes.wintypes.BOOL

PIPE_PATH = r"\\.\pipe\dt-grounded"
GENERIC_READ = 0x80000000
OPEN_EXISTING = 3
INVALID_HANDLE = ctypes.wintypes.HANDLE(-1).value


def read_exact(handle, n):
    """Read exactly n bytes from the pipe handle."""
    buf = ctypes.create_string_buffer(n)
    read_bytes = ctypes.wintypes.DWORD()
    ok = kernel32.ReadFile(handle, buf, n, ctypes.byref(read_bytes), None)
    if not ok:
        raise OSError(f"ReadFile failed: error {ctypes.GetLastError()}")
    if read_bytes.value != n:
        raise OSError(f"Short read: expected {n}, got {read_bytes.value}")
    return buf.raw[:n]


def main():
    # Start server
    server_script = os.path.join(os.path.dirname(__file__), "mock_pipe_server.py")
    proc = subprocess.Popen(
        [sys.executable, "-u", server_script],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    time.sleep(1)

    if proc.poll() is not None:
        out, err = proc.communicate(timeout=5)
        print(f"Server failed to start!\nstdout: {out}\nstderr: {err}")
        sys.exit(1)

    print("Server started, connecting...")

    handle = kernel32.CreateFileW(PIPE_PATH, GENERIC_READ, 0, None, OPEN_EXISTING, 0, None)
    if handle == INVALID_HANDLE:
        print(f"Failed to connect: error {ctypes.GetLastError()}")
        proc.terminate()
        sys.exit(1)

    print("Connected to pipe!")

    # Read length-prefixed message
    length_bytes = read_exact(handle, 4)
    length = struct.unpack("<I", length_bytes)[0]
    print(f"JSON payload: {length} bytes")

    json_bytes = read_exact(handle, length)
    data = json.loads(json_bytes)

    print(f"Overall:   {data['overallPercent']}%")
    print(f"Bosses:    {data['bosses']['collectedCount']}/{data['bosses']['totalCount']}")
    print(f"Creatures: {data['creatureCards']['collectedCount']}/{data['creatureCards']['totalCount']}")
    print(f"Landmarks: {data['landmarks']['collectedCount']}/{data['landmarks']['totalCount']}")
    mm = data['milkMolars']
    print(f"Molars:    reg={mm['regularCollected']}/{mm['regularTotal']}, mega={mm['megaCollected']}/{mm['megaTotal']}")
    print(f"MIX.R:     {data['mixrChallenges']['collectedCount']}/{data['mixrChallenges']['totalCount']}")
    print(f"SCA.B:     {data['scabSchemes']['collectedCount']}/{data['scabSchemes']['totalCount']}")
    print(f"Wendell:   {data['wendell']['collectedCount']}/{data['wendell']['totalCount']}")
    print(f"Ominent:   {data['ominent']['collectedCount']}/{data['ominent']['totalCount']}")
    print(f"BURG.L:    {data['burglChips']['collectedCount']}/{data['burglChips']['totalCount']}")
    print(f"Stuff:     {data['stuff']['collectedCount']}/{data['stuff']['totalCount']}")

    # Verify schema
    required = [
        "saveName", "savePath", "lastModified", "overallPercent",
        "bosses", "creatureCards", "landmarks", "milkMolars",
        "mixrChallenges", "scabSchemes", "wendell", "ominent",
        "burglChips", "stuff",
    ]
    missing = [k for k in required if k not in data]
    if missing:
        print(f"\nMISSING keys: {missing}")
        sys.exit(1)
    else:
        print("\nAll 10 categories + metadata present. Schema matches SaveProgress!")

    kernel32.CloseHandle(handle)
    proc.terminate()
    proc.wait(timeout=5)


if __name__ == "__main__":
    main()
