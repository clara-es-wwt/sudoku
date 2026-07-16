# Agents Guide

This document provides guidance for autonomous agents working on the sudoku repository.

## Project Overview

**sudoku** is a cross-platform, command-line Sudoku game written in C++17 with no third-party dependencies. It supports English and Chinese, two key-mapping modes (WASD and Vim), three difficulty levels, undo, and save/load of game progress.

- **Language**: C++17
- **Build System**: CMake 3.12+
- **Platforms**: Linux, macOS, Windows
- **Code Style**: Google C++ style (enforced by `.clang-format` and `.clang-tidy`)

## Quick Start for Agents

### Building the Project

```bash
cmake -B build -S .
cmake --build build
```

The executable is placed in `bin/sudoku` (or `bin/sudoku.exe` on Windows).

### Running Tests

```bash
cd build
ctest
```

This runs the performance benchmark suite in `tests/performance_test.cpp`.

### Code Quality

- **Formatting**: Run `clang-format` on modified files to match the Google style guide (configured in `.clang-format`)
- **Linting**: The `.clang-tidy` configuration enforces:
  - CamelCase for classes and functions
  - lower_case for variables and parameters
  - UPPER_CASE for constants
  - Most checks enabled except Fuchsia-, Google-, Abseil-, and LLVM-specific ones

## Architecture

### Core Components

| Component | File(s) | Responsibility |
|-----------|---------|-----------------|
| **Scene** | `src/scene.h/cpp` | Central game state: board generation, rendering, event loop, save/load |
| **Block** | `src/block.h/cpp` | Container for 9 cells (row, column, or 3×3 box); validation logic |
| **Command** | `src/command.h/cpp` | Command pattern for undo/redo; wraps cell edits |
| **Input** | `src/input.h/cpp` | Startup prompts: difficulty, key mode, language selection |
| **I18n** | `src/i18n.h/cpp` | Singleton for English/Chinese localization |
| **Color** | `src/color.h` | ANSI color codes for terminal rendering |
| **Utility** | `src/utility.inl` | Random number generation, terminal control, portable `_getch()` |

### Key Data Structures

- **`point_value_t`**: Represents a cell's value (0–9) and state (ERASED, GIVEN, FILLED)
- **`point_t`**: Represents a grid coordinate (0–80 in row-major order)
- **`CBlock`**: Container of 9 `point_value_t*` pointers; used for rows, columns, and 3×3 boxes
- **`CScene`**: The 9×9 board (`_map[81]`), three sets of `CBlock` views, cursor position, undo stack

### Game Flow

1. **Startup** (`main.cpp`): Parse command-line args, prompt for language/difficulty/key mode, generate or load board
2. **Generation** (`CScene::generate()`): Seed diagonal boxes, backtrack-fill remaining cells, erase random cells
3. **Play Loop** (`CScene::play()`): Read keystrokes, dispatch to movement/digit entry/undo/save/win-check
4. **Rendering** (`CBlock::print()`, `CScene::show()`): Use ANSI colors and Unicode box-drawing characters

## Common Tasks

### Adding a Feature

1. Identify which component(s) need changes (e.g., new key binding → `input.h/cpp` and `scene.cpp`)
2. Update the relevant `.h` and `.cpp` files
3. Add i18n strings to `src/i18n.cpp` if user-facing
4. Test locally: `cmake -B build -S . && cmake --build build && ./bin/sudoku`
5. Run linting: `clang-format -i src/*.cpp src/*.h` and `clang-tidy src/*.cpp`
6. Commit with a clear message

### Fixing a Bug

1. Reproduce the issue locally
2. Add a test case to `tests/performance_test.cpp` if applicable
3. Fix the bug in the relevant source file
4. Verify the fix: rebuild and test
5. Commit with a reference to any related issue

### Refactoring

1. Ensure all tests pass before starting
2. Make focused, incremental changes
3. Verify tests still pass after each change
4. Use `git diff` to review your changes before committing

## CI/CD

The repository uses GitHub Actions for continuous integration:

- **`.github/workflows/ci-linux.yml`**: Builds on Ubuntu with GCC, runs `ctest`
- **`.github/workflows/ci-macos.yml`**: Builds on macOS with Clang, runs `ctest`
- **`.github/workflows/ci-windows.yml`**: Builds on Windows with MSVC, runs `ctest`

All three workflows must pass before merging to `master`.

### Workflow

1. Make changes and commit locally
2. Push to your branch (this opens/updates the PR and triggers CI)
3. Use `check_ci` to monitor the build status
4. If CI fails, fix the issues, commit, push, and check again
5. Once CI passes, the PR is ready for review

## File Organization

```
.
├── CMakeLists.txt              # Top-level CMake config
├── README.md                   # Project overview (Chinese)
├── DESCRIPTION.md              # File-by-file documentation
├── LICENSE                     # License file
├── .clang-format               # Code style config
├── .clang-tidy                 # Linting config
├── .gitignore                  # Git ignore rules
├── .github/workflows/          # CI/CD pipelines
├── .vscode/                    # VS Code tasks and launch configs
├── src/                        # Game source code
│   ├── main.cpp               # Entry point
│   ├── scene.h/cpp            # Game state and event loop
│   ├── block.h/cpp            # Cell container and validation
│   ├── command.h/cpp          # Undo/redo command pattern
│   ├── input.h/cpp            # Startup prompts
│   ├── i18n.h/cpp             # Localization
│   ├── color.h                # ANSI color codes
│   ├── display_symbol.h       # Unicode box-drawing characters
│   ├── common.h               # Shared type definitions
│   ├── system_env.hpp         # Terminal setup
│   └── utility.inl            # Utility functions
└── tests/                      # Performance tests
    ├── CMakeLists.txt
    └── performance_test.cpp
```

## Conventions

### Naming

- **Classes**: `CClassName` (e.g., `CScene`, `CBlock`, `CCommand`)
- **Functions**: `functionName()` (camelCase)
- **Variables**: `variable_name` (snake_case)
- **Constants**: `CONSTANT_NAME` (UPPER_CASE)
- **Member variables**: `_memberName` (leading underscore)

### Code Style

- Follow Google C++ style guide (enforced by `.clang-format`)
- Use `#pragma once` for header guards
- Prefer `constexpr` for compile-time constants
- Use `std::filesystem` and `std::fstream` for file I/O
- Avoid third-party dependencies

### Testing

- Performance tests go in `tests/performance_test.cpp`
- Use `ctest` to run all tests
- Benchmark critical paths (board generation, validation, win-check)

## Troubleshooting

### Build Fails on Windows

Ensure `SetConsoleOutputCP(CP_UTF8)` is called in `SetSystemEnv()` so Unicode characters render correctly.

### Terminal Issues on Linux/macOS

The `_getch()` implementation in `utility.inl` switches the terminal to raw mode. If the terminal gets stuck, run `stty sane` to reset it.

### Undo Stack Issues

The `_vCommand` vector in `CScene` stores all executed commands. Ensure `CCommand::execute()` and `CCommand::undo()` are symmetric to avoid state corruption.

## Resources

- **DESCRIPTION.md**: Detailed file-by-file documentation
- **README.md**: User-facing overview and usage instructions
- **.clang-format**: Code style configuration
- **.clang-tidy**: Linting rules and naming conventions
- **CMakeLists.txt**: Build configuration for all platforms

## Contact & Contribution

This is an open-source project. Contributions are welcome via pull requests. Ensure:

1. Code follows the style guide (run `clang-format`)
2. All tests pass locally and in CI
3. Commit messages are clear and descriptive
4. New features include i18n strings if user-facing
