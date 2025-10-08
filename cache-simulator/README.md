# 🧠 Cache Simulator

## Overview
The **Cache Simulator** project is a C-based program designed to model the behavior of a cache memory system. It demonstrates how cache organization and replacement policies impact CPU performance. The simulator supports multiple configurations and implements essential concepts such as **bit extraction**, **cache allocation**, and **Least Recently Used (LRU)** eviction.

This project was developed to deepen understanding of how data structures, memory allocation, and bit-level operations influence performance in modern computer architectures.

---

## 🎯 Learning Objectives
- Build and manage **C data structures** for cache simulation.
- Apply **bit-level operations** to interpret memory addresses.
- Implement **dynamic memory allocation** using `malloc` and linked lists.
- Design and execute the **LRU replacement algorithm**.
- Develop and test a multi-file **C project architecture**.

---

## 🧩 Project Architecture
```
cache-simulator/
│
├── include/        # Header files (cache.h, cpu.h, lru.h, trace.h)
├── src/            # Core source files (cache.c, cpu.c, lru.c, cache_sim.c)
├── test/           # Public unit tests and sample traces
├── obj/            # Generated object files after compilation
├── lib/            # External libraries (if any)
├── Makefile        # Build configuration
└── trace/          # Memory access traces (cat.trace, ls.trace, wc.trace)
```

### Key Files
| File | Description |
|------|--------------|
| `src/cache.c` | Core cache implementation — allocation, bit extraction, and access logic. |
| `src/lru.c` | Implements the **Least Recently Used (LRU)** policy for line eviction. |
| `src/cache_sim.c` | Main simulator driver for testing and trace execution. |
| `include/cache.h` | Structure definitions for Cache, Set, Line, and Block. |

---

## ⚙️ Compilation & Execution
### Build the project
```bash
$ make
```
This compiles all source files and generates:
- `cache_app`: Main simulator executable.
- `cache_test`: Unit test runner.

### Run tests
```bash
$ ./cache_test
```
Expected successful output:
```
[  PASSED  ] 8 tests.
hits: 787666  misses: 22104  evictions: 22088
hrate: 0.9727  mrate: 0.0273
```

### Run with trace
```bash
$ ./cache_app 2 2 10 test/wc.trace
```
Simulates a 2-way set associative cache with 4 sets and 1024-byte blocks.

---

## 🧮 Core Functionalities

### 1. **Cache Allocation**
Implements dynamic structures for:
- Blocks (`make_block`)
- Lines (`make_lines`)
- Sets (`make_sets`)
- Cache (`make_cache`)

Each level is created using `malloc`, ensuring proper initialization and pointer management.

### 2. **Bit Extraction**
The function `get_bits()` isolates **set index** and **block offset** from 32-bit memory addresses using C bitwise operations (`&, |, <<, >>`).

### 3. **LRU Replacement Policy**
`lru_fetch()` determines whether a cache access results in a **HIT**, **COLD MISS**, or **CONFLICT MISS**, updating the linked list accordingly to maintain recency order.

---

## 🧠 Debugging & Testing
Debug using **gdb**:
```bash
$ gdb --args cache_app arg1 arg2 arg3 arg4
```
Breakpoints can be set for unit tests or specific cache functions for detailed memory analysis.

---

## 📊 Sample Output
```
====================================================
[==========] Running 8 tests from 1 test suite.
[----------] 8 tests from ProjectTests
[       OK ] ProjectTests.test_cache_make (0 ms)
...
hits: 787666  misses: 22104  evictions: 22088
[  PASSED  ] 8 tests.
====================================================
```

---

## 🧱 Technical Highlights
- **Language:** C (ANSI C99 Standard)
- **Build System:** GNU Make
- **Testing Framework:** GoogleTest (C++)
- **Key Skills Demonstrated:**
  - Bitwise computation
  - Dynamic memory management
  - Data structure design (arrays + linked lists)
  - Cache architecture principles

---

## 🚀 Takeaway
This project provided hands-on experience with **low-level memory systems**, bridging theory and practice in **computer architecture**. It reinforced skills in **C programming**, **algorithmic thinking**, and **performance simulation**, all essential for systems programming and backend engineering roles.

---

**Author:** Quan Hoang Truong  
**Project:** Cache Simulator  
**Category:** Systems Programming / Computer Architecture  
**Language:** C  
**Year:** 2025
