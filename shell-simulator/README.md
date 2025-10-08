# 🧮 Project 1: Shell

## Overview
The **Shell Project** is a C/C++ implementation of a **minimal Unix-like command-line shell**, designed to demonstrate process creation, execution, and inter-process communication.  
This project replicates the behavior of a simplified shell (`tsh`), capable of handling user input, executing commands via `fork()`, `exec()`, and `wait()`, and chaining commands through pipes.  
The primary goal is to gain hands-on experience with **system calls**, **process control**, and **pipe-based communication** in a real Unix environment.

---

## 🎯 Learning Objectives
- Implement a basic shell using **process management system calls** (`fork`, `exec`, `wait`).
- Understand **inter-process communication (IPC)** through `pipe()` and `dup2()`.
- Manage **multiple processes** and handle **pipelined command execution**.
- Apply robust **memory management** and resource cleanup.
- Build, test, and debug complex C/C++ applications in a modular structure.

---

## 🧩 Project Architecture
```
project_shell/
│
├── include/        # Header files (e.g., process.h, shell.h)
├── src/            # Core logic for shell and process management
├── test/           # Unit tests and example commands
├── obj/            # Compiled object files
├── lib/            # External or support libraries (optional)
├── Makefile        # Build configuration
└── README.md       # Documentation
```

### Key Executables
| Executable | Description |
|-------------|-------------|
| `tsh_app` | Main shell executable — reads commands, runs them, and manages pipes. |
| `tsh_test` | Test suite to verify functionality and correctness. |

---

## ⚙️ Build & Run
### Build
```bash
make
```
### Run the Shell
```bash
./tsh_app
```
Example interaction:
```
tsh> ls
foo.txt
tsh> quit
```
Supports piping:
```
tsh> ls -laFh | grep Makefile ; quit
-rw-r--r--. 1 user user 1.3K Sep 11 14:34 Makefile
```

---

## 🧠 Core Functionality

### 1. **Main Shell Loop**
- Displays prompt (`display_prompt()`).
- Reads user input dynamically via `read_input()`.
- Parses input with `parse_input()` into a list of `Process` objects.
- Executes commands using `run_commands()`.
- Cleans up resources after each command (`cleanup()`).

### 2. **Command Parsing**
- Tokenizes input by `"|"` and `";"` delimiters.
- Detects pipelines and command boundaries.
- Stores tokens in structured `Process` objects for execution.

### 3. **Process Execution**
- Each command is executed in a **child process** using `fork()`.
- Child replaces its image with the target command via `execvp()`.
- The parent uses `wait()` to synchronize child completion.
- Piping implemented using `pipe()` + `dup2()` for connected commands.

### 4. **Quit Handling**
- `isQuit()` detects “quit” and terminates the shell loop gracefully.

### 5. **Debugging**
- Enable debug messages by setting `DEBUG = -DDEBUGMODE` in Makefile.
- Use `debug("msg")` to trace code execution with file and line info.

---

## 🧪 Testing & Debugging
### Run Unit Tests
```bash
./tsh_test
```
Example:
```
[==========] Running 5 tests from 1 test suite.
[ RUN      ] shell.test_pipe_execution
[       OK ] shell.test_pipe_execution (3 ms)
[----------] 5 tests from shell (8 ms total)
[  PASSED  ] 5 tests.
```
### Debugging with GDB
```bash
gdb ./tsh_app
(gdb) break run_commands
(gdb) run
```
Helpful resource: [GDB tutorial](https://www.cs.cmu.edu/~gilpin/tutorial/).

---

## 🧱 Technical Highlights
- **Language:** C++ (with POSIX system calls)
- **Key System Calls:** `fork()`, `execvp()`, `wait()`, `pipe()`, `dup2()`, `close()`
- **Memory Safety:** All allocations are cleaned up in `cleanup()`
- **Error Handling:** Robust error messages for failed syscalls
- **Testing:** Integrated with GoogleTest for structured validation
- **Debugging:** Optional compile-time debugging via macros

---

## 🚀 Takeaway
The **Shell Project** provides hands-on experience with **Unix process management** and **low-level system programming**.  
It bridges theory and practical systems engineering by recreating how real shells manage **processes**, **I/O redirection**, and **pipelines** — the core foundation of modern operating systems and command-line tools.

---

**Author:** Quan Hoang Truong  
**Project:** Shell  
**Category:** Systems Programming / Operating Systems  
**Language:** C++  
**Year:** 2025
