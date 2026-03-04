r"""
Mock named pipe server for testing the Tauri pipe client.

Creates \\.\pipe\dt-grounded and sends mock SaveProgress JSON
every 2 seconds, simulating the DLL's output. Uses only stdlib.

Usage:
    python mock_pipe_server.py
    python mock_pipe_server.py --interval 1
"""

import ctypes
import ctypes.wintypes
import json
import struct
import time
import sys
import random

kernel32 = ctypes.windll.kernel32

PIPE_NAME = r"\\.\pipe\dt-grounded"
PIPE_ACCESS_OUTBOUND = 0x00000002
PIPE_TYPE_BYTE = 0x00000000
PIPE_WAIT = 0x00000000
INVALID_HANDLE_VALUE = ctypes.wintypes.HANDLE(-1).value


def create_pipe():
    handle = kernel32.CreateNamedPipeW(
        PIPE_NAME,
        PIPE_ACCESS_OUTBOUND,
        PIPE_TYPE_BYTE | PIPE_WAIT,
        1,      # max instances
        65536,  # out buffer
        0,      # in buffer
        0,      # timeout
        None,   # security
    )
    if handle == INVALID_HANDLE_VALUE:
        raise OSError(f"CreateNamedPipe failed: {ctypes.GetLastError()}")
    return handle


def connect_pipe(handle):
    result = kernel32.ConnectNamedPipe(handle, None)
    if not result:
        err = ctypes.GetLastError()
        if err != 535:  # ERROR_PIPE_CONNECTED
            raise OSError(f"ConnectNamedPipe failed: {err}")


def write_pipe(handle, data: bytes) -> bool:
    written = ctypes.wintypes.DWORD()
    result = kernel32.WriteFile(
        handle,
        data,
        len(data),
        ctypes.byref(written),
        None,
    )
    return bool(result) and written.value == len(data)


def close_pipe(handle):
    kernel32.DisconnectNamedPipe(handle)
    kernel32.CloseHandle(handle)


def send_message(handle, data: dict) -> bool:
    json_bytes = json.dumps(data).encode("utf-8")
    length = struct.pack("<I", len(json_bytes))
    return write_pipe(handle, length + json_bytes)


def make_mock_data():
    """Generate mock SaveProgress JSON matching the schema."""
    def make_category(items, status_key):
        collected = sum(1 for item in items if item[status_key])
        return {
            "items": items,
            "collectedCount": collected,
            "totalCount": len(items),
        }

    bosses = [
        {"id": "killbroodmother", "name": "Hedge Broodmother", "defeated": random.random() > 0.3},
        {"id": "killinfectedbroodmother", "name": "Infected Broodmother", "defeated": random.random() > 0.5},
        {"id": "killwaspqueen", "name": "Wasp Queen", "defeated": random.random() > 0.5},
        {"id": "killdirector", "name": "Director Schmector", "defeated": random.random() > 0.6},
        {"id": "killmantis", "name": "Mantis", "defeated": random.random() > 0.7},
    ]

    creature_cards = [
        {"id": f"creature_{i}", "name": f"Creature {i}", "collected": random.random() > 0.3}
        for i in range(68)
    ]

    landmarks = [
        {"id": f"poi_{i}", "name": f"Landmark {i}", "zone": f"Zone {i % 5}", "discovered": random.random() > 0.2}
        for i in range(108)
    ]

    mixr = [
        {"id": f"mixr_{i}", "name": name, "completed": random.random() > 0.3}
        for i, name in enumerate([
            "Central Grasslands MIX.R", "Hedge MIX.R", "Haze MIX.R",
            "Sandbox MIX.R", "Northern Grasslands MIX.R",
            "Upper Grasslands Super MIX.R", "Lawnmower Super MIX.R",
            "Moldorc Super MIX.R",
        ])
    ]

    scab_schemes = [
        {"id": f"scheme_{i}", "name": f"Scheme {i}", "collected": random.random() > 0.3}
        for i in range(55)
    ]

    wendell = [
        {"id": f"wendell_{i}", "name": f"Wendell Tape {i}", "subcategory": "Audio Log", "collected": random.random() > 0.3}
        for i in range(67)
    ]

    ominent = [
        {"id": f"ominent_{i}", "name": f"Ominent Note {i}", "subcategory": "Spy Note", "collected": random.random() > 0.3}
        for i in range(16)
    ]

    burgl_chips = [
        {"id": f"burgl_{i}", "name": f"BURG.L Chip {i}", "subcategory": "Tech Chip", "collected": random.random() > 0.3}
        for i in range(16)
    ]

    stuff = [
        {"id": f"stuff_{i}", "name": f"Stuff Item {i}", "subcategory": "Key", "collected": random.random() > 0.3}
        for i in range(33)
    ]

    reg_collected = random.randint(40, 93)
    mega_collected = random.randint(5, 14)

    categories = [
        (sum(1 for b in bosses if b["defeated"]), len(bosses)),
        (sum(1 for c in creature_cards if c["collected"]), len(creature_cards)),
        (sum(1 for l in landmarks if l["discovered"]), len(landmarks)),
        (reg_collected + mega_collected, 93 + 14),
        (sum(1 for m in mixr if m["completed"]), len(mixr)),
        (sum(1 for s in scab_schemes if s["collected"]), len(scab_schemes)),
        (sum(1 for w in wendell if w["collected"]), len(wendell)),
        (sum(1 for o in ominent if o["collected"]), len(ominent)),
        (sum(1 for b in burgl_chips if b["collected"]), len(burgl_chips)),
        (sum(1 for s in stuff if s["collected"]), len(stuff)),
    ]
    overall = sum(c / t for c, t in categories if t > 0) / len(categories) * 100

    return {
        "saveName": "Mock Live Data",
        "savePath": "",
        "lastModified": time.strftime("%Y-%m-%d %H:%M:%S"),
        "overallPercent": round(overall, 1),
        "bosses": make_category(bosses, "defeated"),
        "creatureCards": make_category(creature_cards, "collected"),
        "landmarks": make_category(landmarks, "discovered"),
        "milkMolars": {
            "regularCollected": reg_collected,
            "regularTotal": 93,
            "megaCollected": mega_collected,
            "megaTotal": 14,
            "totalSpent": max(0, reg_collected + mega_collected - 10),
        },
        "mixrChallenges": make_category(mixr, "completed"),
        "scabSchemes": make_category(scab_schemes, "collected"),
        "wendell": make_category(wendell, "collected"),
        "ominent": make_category(ominent, "collected"),
        "burglChips": make_category(burgl_chips, "collected"),
        "stuff": make_category(stuff, "collected"),
    }


def main():
    interval = 2.0
    if "--interval" in sys.argv:
        idx = sys.argv.index("--interval")
        if idx + 1 < len(sys.argv):
            interval = float(sys.argv[idx + 1])

    print(f"Mock Pipe Server - {PIPE_NAME}")
    print(f"Sending updates every {interval}s")
    print("Press Ctrl+C to stop\n")

    while True:
        pipe = None
        try:
            pipe = create_pipe()
            print("Waiting for client...")
            connect_pipe(pipe)
            print("Client connected!")

            while True:
                data = make_mock_data()
                if not send_message(pipe, data):
                    print("Client disconnected")
                    break
                print(f"  Sent update: {data['overallPercent']:.1f}% overall")
                time.sleep(interval)

        except KeyboardInterrupt:
            print("\nShutting down")
            break
        except OSError as e:
            print(f"Error: {e}")
            time.sleep(1)
        finally:
            if pipe is not None:
                try:
                    close_pipe(pipe)
                except:
                    pass


if __name__ == "__main__":
    main()
