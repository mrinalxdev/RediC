# Redis Implementation in C

A lightweight implementation of Redis server from scratch in C, featuring basic data structures and persistence mechanisms similar to Redis.

## Features

### Implemented
- Basic Key-Value Operations (SET/GET)
- Data Structures
  - Lists (PUSH/POP operations)
  - Sets (SADD/SISMEMBER operations)
  - Hashes (Hash operations)
- Persistence
  - RDB Snapshots (Periodic saving of dataset to disk)
  - Manual SAVE command
- Network Communication
  - TCP/IP server implementation
  - Basic RESP protocol support
  - Multiple client connections using select()

### Planned Features
- AOF (Append Only File) persistence
- Multi-threading support
- Advanced RESP protocol features
- Command pipelining
- Make file implementation

## Technical Details

### Data Structures
- Hash table implementation for key-value storage
- Linked list implementation for List operations
- Set implementation with duplicate prevention
- Hash map implementation for complex key-value pairs

### Persistence
- RDB file format for snapshot storage
- Automatic periodic saving (every 5 minutes)
- Manual saving through SAVE command

## Building and Running

### Prerequisites
- GCC compiler
- Unix-like operating system (Linux/MacOS)

### Compilation
```bash
gcc -o redis-server main.c data_s.c storage.c utils.c rdb.c
```

### Running the Server
```bash
./redis-server
```
The server will start listening on port 6379 (default Redis port)

## Usage

### Connecting to the Server
You can use any Redis client or netcat:
```bash
nc localhost 6379
```

### Supported Commands

#### Basic Operations
```
SET key value
GET key
```

#### List Operations
```
PUSH key value
POP key
```

#### Set Operations
```
SADD key value
SISMEMBER key value
```

#### Persistence Operations
```
SAVE
```

### Examples
```
SET user1 "John Doe"
GET user1
PUSH mylist "first item"
PUSH mylist "second item"
POP mylist
SADD myset "unique value"
SISMEMBER myset "unique value"
SAVE
```

## Project Structure

- `main.c`: Server implementation and command handling
- `data_s.c/h`: Data structure implementations
- `storage.c/h`: Basic storage operations
- `utils.c/h`: Utility functions
- `rdb.c/h`: RDB persistence implementation

## Implementation Details

### Hash Table
- Fixed size hash table (256 buckets)
- Simple hash function based on first character
- Collision handling through chaining

### Network Protocol
- Basic implementation of RESP (Redis Serialization Protocol)
- Support for simple string responses
- Error handling for unknown commands

### Persistence
- Binary format for RDB files
- Version tracking for compatibility
- Automatic periodic saves
- Manual save command support

## Memory Management

The implementation uses dynamic memory allocation for:
- Storing key-value pairs
- List nodes
- Set elements
- Hash table entries

Memory is freed appropriately when:
- Items are removed from data structures
- Connections are closed
- Server is shut down

## Limitations

- Fixed hash table size
- Basic hash function implementation
- No expiration mechanism
- Limited RESP protocol support
- No replication support
- No cluster support
- No transaction support


