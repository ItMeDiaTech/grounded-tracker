#pragma once

// Named pipe protocol definitions
// Pipe name: \\.\pipe\dt-grounded
// Message format: [u32 LE length][JSON bytes]
// JSON schema matches SaveProgress TypeScript interface (camelCase keys)

#define PIPE_NAME L"\\\\.\\pipe\\dt-grounded"
#define PIPE_BUFFER_SIZE 65536
