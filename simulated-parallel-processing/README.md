# ⚙️ Simulated Parallel Processing

## Overview
The **Simulated Parallel Processing** project is a C-based system that models how a **multi-core CPU** performs concurrent execution using **processes**, **pipes**, and **signals**.  
It demonstrates **inter-process communication (IPC)**, **signal-driven coordination**, and **non-blocking task scheduling**, providing an educational but realistic view of how operating systems and schedulers manage parallel workloads.

---

## 🎯 Learning Objectives
- Understand **process creation and management** using `fork()`.
- Implement **two-way communication** between processes with `pipe()`.
- Handle **signals safely and atomically** using `sigaction()` and `volatile sig_atomic_t`.
- Design a **non-blocking scheduling loop** for task distribution and aggregation.
- Practice **robust process cleanup** and resource management.

---

## 🧩 System Architecture
```
simulated-parallel-processing/
│
├── src/                # Core source files (main.c, process.c, ipc.c, signal.c)
├── include/            # Header files (process.h, ipc.h, signal.h)
├── Makefile            # Build configuration
└── README.md           # Project documentation
```

### Core Components
| Component | Description |
|------------|-------------|
| `main.c` | Entry point. Creates subprocesses (cores), assigns tasks, and aggregates results. |
| `process.c` | Defines core logic for task handling, result generation, and cleanup. |
| `ipc.c` | Implements inter-process communication using pipes. |
| `signal.c` | Configures real-time signals and defines safe handlers. |

---

## ⚙️ Build & Run
### Compile
```bash
make
```
### Run
```bash
./main <num_tasks> <max_bits>
```
Example:
```bash
./main 30 5
```
This runs the simulator with **30 tasks**, each extracting up to **5 bits**.

---

## 🧠 Simulation Logic

### Part 1: Creating Cores & Pipes
- The **main process** initializes three cores via `fork()`.
- Each core is assigned **two pipes** — one for input and one for output.
- Unused pipe ends are closed in both parent and child processes.
- Implemented in `create_core()` and `initialize_cores()`.

### Part 2: Task Definition & Scheduling
- Each task follows format: `{task_id}_{n}`  
  where `task_id` is unique and `n` is a randomized integer between 1 and `max_bits`.
- Cores simulate computation using `no_interrupt_sleep()` to ensure no signal interruption.
- Tasks are scheduled to **first available core** using a linear search in `schedule_tasks()`.

### Part 3: Assigning Tasks
- Tasks are sent via `write()` to `main_to_core[i]` pipes.
- Cores block on `read()` until new data arrives.
- Task execution involves extracting leftmost bits via `extract_leftmost_bits()`.

### Part 4: Returning Results
- Upon task completion, cores send results back via `core_to_main[i]` pipes.
- Each core triggers a **unique real-time signal** (`SIGRTMIN + i`) using `kill()`.
- The main process registers handlers via `sigaction()`.
- Handlers increment `volatile sig_atomic_t` counters for each core.
- Results are then safely read in the main loop (outside of signal context).

### Part 5: Post-Processing & Cleanup
- Once all tasks are complete:
  - Main process closes all pipes.
  - Cores detect EOF and terminate cleanly.
  - `wait()` ensures proper child reaping.
- Results are printed per core in the required format:
```
Core 1: 0, 3, 0, 2
Core 2: 0, 5, 3
Core 3: 2
```

---

## 🧩 Technical Highlights
- **Language:** C (POSIX Compliant)
- **Concurrency Mechanism:** `fork()`, `pipe()`, `sigaction()`, and `kill()`
- **Signal Type:** Real-time (`SIGRTMIN + i`) — prevents loss from rapid signal bursts.
- **Safety:** Atomic counters (`volatile sig_atomic_t`) prevent race conditions.
- **Robustness:** Full error checking for all system calls; proper cleanup ensures no leaks or zombies.
- **Logging:** Colored terminal output (`[MAIN]`, `[CORE x]`) for better visibility.

---

## 🧪 Example Log Output
```
[MAIN] Core 1 created (PID 4123)
[MAIN] Core 2 created (PID 4124)
[MAIN] Core 3 created (PID 4125)
[MAIN] Assigning Task 0_3 to Core 1
[CORE 1] Received Task 0_3
[CORE 1] Processed Result: 0
[MAIN] Received Result from Core 1
...
Core 1: 0, 3, 0, 2
Core 2: 0, 5, 3
Core 3: 2
```

---

## 🚀 Takeaway
This project bridges **theory and practice** in parallel computing and operating systems.  
It strengthens understanding of **IPC**, **signal safety**, and **process scheduling**, demonstrating how low-level C mechanisms emulate **multi-core CPU behavior**.  

---

**Author:** Quan Hoang Truong  
**Project:** Simulated Parallel Processing  
**Category:** Systems Programming / Operating Systems  
**Language:** C  
**Year:** 2025
