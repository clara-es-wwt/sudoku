<!-- forge wuz here -->

# Repository File Descriptions

A cross-platform, command-line Sudoku game written in C++17 with no third-party dependencies.
The game supports English and Chinese, two key-mapping modes (WASD and Vim), three difficulty
levels, undo, and save/load of game progress.

---

## Root-level files

| File | Description |
|------|-------------|
| `CMakeLists.txt` | Top-level CMake project file. Sets the C++17 standard, configures compiler flags for Debug/Release builds, handles per-platform charset options (Windows UTF-8), defines the output directory (`bin/`), and adds the `src/` and `tests/` subdirectories. |
| `README.md` | Project overview in Chinese. Documents features, build instructions, runtime usage (`-l` to load a save file, `-h` for help), keyboard controls, and the directory structure. |
| `LICENSE` | Project license file. |
| `.gitignore` | Tells Git to ignore the `build/`, `bin/`, `.vscode/settings.json`, and `.cache/` paths. |
| `.clang-format` | Clang-Format configuration. Bases the C++ style on the Google style guide. |
| `.clang-tidy` | Clang-Tidy static-analysis configuration. Enables most checks while disabling Fuchsia-, Google-, Abseil-, and LLVM-specific ones. Enforces naming conventions (CamelCase for classes/functions, lower_case for variables/parameters, UPPER_CASE for constants). |
| `DESCRIPTION.md` | This file — a summary of every file in the repository. |

---

## `src/` — Game source code

| File | Description |
|------|-------------|
| `src/CMakeLists.txt` | CMake build script for the `src/` subdirectory. Lists the six `.cpp` source files and produces the `sudoku` executable. |
| `src/main.cpp` | Program entry point. Calls `SetSystemEnv()` to configure the terminal, then branches on command-line arguments: no args → ask language/difficulty, generate a new board; `-l <file>` → load a saved game; anything else → print help. After setup it calls `scene.play()` to start the interactive loop. |
| `src/common.h` | Shared type definitions used across the whole project: the `UNSELECTED` sentinel, the `Difficulty`, `State`, and `KeyMode` enums, the `KeyMap`/`Normal`/`Vim` structs (which map key characters to movement directions), and the `point_t` / `point_value_t` structs that represent a grid coordinate and a cell's value+state. Also defines `CPointSort`, a comparator for `point_t`. |
| `src/block.h` | Header for `CBlock`. Declares the class interface: `isValid()`, `isFull()`, `print()`, `push_back()`, and `getNumberValue()`. |
| `src/block.cpp` | Implements `CBlock`, a container for exactly 9 `point_value_t*` pointers that can represent a row, a column, or a 3×3 box. `isValid()` checks that no two filled cells share the same digit. `isFull()` checks that all 9 cells are non-zero. `print()` renders one row of the board to stdout using ANSI colour codes: the cursor cell is highlighted blue, cells whose value matches the cursor's value are highlighted yellow, and player-entered (erased-then-filled) digits are shown in green. |
| `src/color.h` | Defines the `Color` namespace. Contains the `Code` enum of ANSI colour/attribute codes (bold, foreground, background) and the `Modifier` class, which formats an escape sequence `\033[mode;bg;fgm` and streams it via `operator<<`. |
| `src/display_symbol.h` | Defines four `constexpr char[]` Unicode box-drawing characters used when rendering the grid: `CORNER` (╋), `LINE` (━), `PIPE` (┃), and `ARROW` (^, marks the cursor column on underlines). |
| `src/command.h` | Header for `CCommand`. Declares the command-pattern class that wraps a single cell edit: stores a pointer to the owning `CScene`, the affected `point_t`, the previous value, and the new value. Exposes `execute()` (apply the edit) and `undo()` (revert it). |
| `src/command.cpp` | Implements `CCommand`. `execute(nInputValue)` asks the scene for the current cursor position, calls `setCurValue()` to write the digit, and records the previous value for undo. `undo()` calls `setPointValue()` on the scene to restore the previous digit. |
| `src/i18n.h` | Header for the `I18n` singleton. Declares the `Language` enum (`ENGLISH`, `CHINESE`), the `Key` enum of all user-visible string identifiers, and the `I18n` class with `SetLanguage()` and `Get()` methods. |
| `src/i18n.cpp` | Implements `I18n`. Defines two static `Dict` maps (English and Chinese) that map every `Key` to its localised string. The singleton defaults to English; `SetLanguage()` switches the active dictionary pointer. |
| `src/input.h` | Header declaring the three startup-input functions: `inputDifficulty()` (returns the number of cells to erase), `inputKeyMode()` (returns `KeyMode::NORMAL` or `KeyMode::VIM`), and `InputLanguage()` (sets the active language in `I18n`). |
| `src/input.cpp` | Implements the three startup-input functions. Each loops until the user enters a valid choice, printing the appropriate i18n prompt and an error message on bad input. `inputDifficulty()` maps Easy/Normal/Hard to 20/35/50 erased cells. |
| `src/scene.h` | Header for `CScene`, the central game-state class. Declares the 9×9 cell array (`_map[81]`), three sets of `CBlock` views (rows, columns, 3×3 boxes), the cursor position, the undo stack (`_vCommand`), and the key-map pointer. Public interface: `generate()`, `show()`, `play()`, `save()`, `load()`, `eraseRandomGrids()`, `isComplete()`, `setCurValue()`, `setPointValue()`, `getCurPoint()`, `setMode()`. |
| `src/scene.cpp` | Implements `CScene`. Key responsibilities: **`init()`** — wires each `point_value_t` cell into the row, column, and box `CBlock` views. **`generate()`** — produces a valid, fully-filled board by seeding the three diagonal 3×3 boxes with shuffled digits and then filling the rest with a backtracking algorithm. **`eraseRandomGrids()`** — randomly selects `count` cells and marks them `State::ERASED` (player-editable). **`show()`** — clears the screen and redraws the board using `CBlock::print()` and `printUnderline()`. **`play()`** — the main event loop: reads single keystrokes via `_getch()`, dispatches to movement, digit entry (via `CCommand`), undo, save-on-quit, and win-check logic. **`save()`/`load()`** — serialise/deserialise the full game state (cell values+states, cursor, command history) to/from a plain-text file using `std::filesystem` and `std::fstream`. |
| `src/system_env.hpp` | Provides `SetSystemEnv()`, called once at startup. On Windows it calls `SetConsoleOutputCP(CP_UTF8)` so Unicode box-drawing characters render correctly. On all platforms it disables `stdio`/`cin`/`cout` synchronisation for faster I/O. |
| `src/utility.inl` | Header-only utility functions included wherever needed. Provides: `random(begin, end)` (cryptographically seeded uniform integer distribution), `get_unit()` (returns `{1…9}`), `shuffle_unit()` (returns a randomly shuffled `{1…9}`), `message()` (prints a string to stdout), `cls()` (clears the terminal via ANSI escape), and a portable `_getch()` implementation for Linux/macOS that switches the terminal to raw mode to read a single character without waiting for Enter. |

---

## `tests/` — Performance tests

| File | Description |
|------|-------------|
| `tests/CMakeLists.txt` | CMake build script for the `tests/` subdirectory. Compiles all `src/` files (except `main.cpp`) together with `performance_test.cpp` into a standalone `performance_test` executable. |
| `tests/performance_test.cpp` | Stand-alone performance benchmark (no test framework required). Contains three timed test functions: **`test_generate_performance()`** — runs `CScene::generate()` 100 times and reports average/min/max milliseconds. **`test_erase_performance()`** — benchmarks `eraseRandomGrids()` at five difficulty levels (30–70 cells). **`test_complete_check_performance()`** — calls `isComplete()` 1 000 times and reports average/min/max nanoseconds. The `main()` function runs all three in sequence. |

---

## `.github/workflows/` — CI pipelines

| File | Description |
|------|-------------|
| `.github/workflows/ci-linux.yml` | GitHub Actions workflow **"Build on Linux"**. Triggers on pushes and pull requests to `master`. Runs on `ubuntu-latest` with GCC, configures CMake in Release mode, builds the project, and runs `ctest`. |
| `.github/workflows/ci-macos.yml` | GitHub Actions workflow **"Build on macOS"**. Same structure as the Linux workflow but runs on `macos-latest` with Clang/Clang++. |
| `.github/workflows/ci-windows.yml` | GitHub Actions workflow **"Build on Windows"**. Same structure but runs on `windows-latest` with MSVC (`cl`). |
| `.github/workflows/azure-webapps-node.yml` | A boilerplate GitHub Actions workflow for deploying a Node.js application to Azure App Service. This file appears to be an unused template that was never removed from the repository; it has no relation to the C++ Sudoku game. |

---

## `.vscode/` — Editor configuration

| File | Description |
|------|-------------|
| `.vscode/tasks.json` | VS Code task definitions. Provides four tasks: **CMake: configure** (`cmake -S . -B build`), **CMake: build** (`cmake --build build`, depends on configure), **CMake: clean**, and **CMake: configure & build** (composite). The build task is set as the default build group. |
| `.vscode/launch.json` | VS Code launch configuration for Windows debugging. Defines a `cppvsdbg` launch that runs the pre-built `bin/Debug/sudoku.exe` in an external terminal, with **CMake: configure & build** as the pre-launch task. |
