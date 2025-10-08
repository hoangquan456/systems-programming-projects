# 🧠 System Programming Projects

## Overview
This repository showcases a collection of **low-level systems programming projects** written in **C and C++**, focused on building core components of operating systems — from process management and interprocess communication (IPC) to memory hierarchy simulation and shell design.  
Each project demonstrates a different facet of **Unix/Linux system-level programming**, emphasizing mastery of **system calls**, **process control**, **signals**, and **resource management**.

These projects were completed as part of an intensive systems programming course and are designed to reflect **real-world engineering skills** — from building reliable concurrent systems to simulating CPU behavior and cache architectures.

---

## 🧩 Repository Structure
```
system-programming-projects/
│
├── project1-shell/                    # Minimal Unix Shell (process, exec, pipes)
├── cache-simulator/                   # Cache memory performance simulator
├── simulated-parallel-processing/     # Multi-core processing simulation using pipes and signals
└── README.md                          # This file
```

Each project includes:
- **src/** — Source code and implementation
- **include/** — Header files for interfaces and structs
- **test/** — Unit and integration tests
- **Makefile** — Build automation and testing
- **README.md** — Technical documentation

---

## 🧠 Projects Summary

### 🧮 Shell Simulator
**Goal:** Implement a minimal Unix shell (`tsh`) using system calls.  
**Highlights:**
- Executes commands via `fork()`, `execvp()`, and `wait()`.
- Implements command parsing, tokenization, and I/O redirection with `pipe()` and `dup2()`.
- Supports multi-command pipelines and graceful termination (`quit` command).
- Features a `DEBUGMODE` flag and GDB-compatible debugging setup.

🧰 *Core Skills:* Process creation, IPC, shell design, memory management, testing.


### ⚙️ Cache Simulator
**Goal:** Build a simulator to model CPU cache behavior and analyze memory access efficiency.  
**Highlights:**
- Implements cache allocation (`make_cache`, `make_sets`, `make_lines`).
- Uses bit extraction for cache indexing and tag identification.
- Designs an LRU (Least Recently Used) eviction algorithm.
- Evaluates hit/miss ratios across various cache configurations and workloads.

🧰 *Core Skills:* Memory management, data structure allocation, bitwise operations, LRU algorithms.


### 🚀 Simulated Parallel Processing
**Goal:** Simulate multi-core parallel processing using **processes, pipes, and signals**.  
**Highlights:**
- Main process coordinates three child processes acting as CPU cores.
- Two-way communication via pipes (`pipe()`), and result notification via real-time signals (`SIGRTMIN + i`).
- Uses atomic signal handling (`volatile sig_atomic_t`) for safe, non-blocking communication.
- Demonstrates real-time scheduling, task distribution, and concurrent result aggregation.

🧰 *Core Skills:* Process synchronization, signal handling, IPC, concurrent task scheduling.

---

## 🧰 Technical Stack
| Category | Technologies & Tools |
|-----------|----------------------|
| **Languages** | C, C++ |
| **OS & Environment** | Linux / Unix |
| **Core APIs** | fork(), exec(), wait(), pipe(), dup2(), sigaction(), kill(), nanosleep() |
| **Build System** | GNU Make |
| **Debugging Tools** | GDB, custom debug macros |
| **Testing Framework** | GoogleTest (C++) |
| **Version Control** | Git |

---

## 🧪 What I Learned
- Designing modular, testable systems in **C/C++** with clear memory ownership and cleanup.
- Handling **concurrent processes** and **asynchronous signals** safely.
- Implementing **low-level communication primitives** for coordination between processes.
- Debugging and profiling performance-critical C programs.
- Building maintainable Makefiles and adhering to strict compiler warnings (`-Wall -Wextra -Werror`).

---

## 🚀 Takeaway
This repository represents a complete journey through **systems-level software engineering** — from memory simulation and concurrency to shell implementation.  
Each project mirrors **real operating system mechanisms**, built entirely from scratch without external libraries beyond standard POSIX calls.

By mastering these projects, I’ve developed the foundation for building **robust, performant, and low-level software systems** that interact directly with the operating system.

---

**Author:** Quan Hoang Truong  
**Repository:** System Programming Projects  
**Focus Areas:** OS Internals, Concurrency, Memory Systems, IPC  
**Language:** C / C++  
**Year:** 2025
