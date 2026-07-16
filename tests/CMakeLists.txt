# tests目录的CMakeLists.txt文件

# ── performance_test ────────────────────────────────────────────────────────
set(PERFORMANCE_TEST_FILES
    ../src/block.cpp
    ../src/command.cpp
    ../src/i18n.cpp
    ../src/input.cpp
    ../src/scene.cpp
    performance_test.cpp
)

add_executable(performance_test
   ${PERFORMANCE_TEST_FILES}
)

# ── board_complete_test ──────────────────────────────────────────────────────
add_executable(board_complete_test
    board_complete_test.cpp
)

# Enable CTest and register the new test so `ctest` picks it up
enable_testing()
add_test(NAME board_complete_test COMMAND board_complete_test)
