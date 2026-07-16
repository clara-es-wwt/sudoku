// Unit tests for isBoardComplete() — uses no external test framework,
// just plain assertions so the existing CMake/ctest setup can run them.

#include <array>
#include <cassert>
#include <iostream>
#include <string>

#include "board_validator.h"

using Board = std::array<std::array<int, 9>, 9>;

// ---------------------------------------------------------------------------
// Helper: build a known-valid solved board
// ---------------------------------------------------------------------------
static Board validBoard()
{
    return {{
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9},
    }};
}

// ---------------------------------------------------------------------------
// Test cases
// ---------------------------------------------------------------------------

static void test_valid_complete_board_returns_true()
{
    Board b = validBoard();
    assert(isBoardComplete(b) == true);
    std::cout << "PASS: valid complete board returns true\n";
}

static void test_board_with_one_empty_cell_returns_false()
{
    Board b = validBoard();
    b[4][4] = 0; // clear the centre cell
    assert(isBoardComplete(b) == false);
    std::cout << "PASS: board with one empty cell returns false\n";
}

static void test_fully_empty_board_returns_false()
{
    Board b{};
    for (auto& row : b)
        row.fill(0);
    assert(isBoardComplete(b) == false);
    std::cout << "PASS: fully empty board returns false\n";
}

static void test_duplicate_in_row_returns_false()
{
    Board b = validBoard();
    // Swap two values in the same row to create a duplicate
    std::swap(b[0][0], b[0][1]); // row 0: 3,5,... — duplicate 3 and missing 5
    assert(isBoardComplete(b) == false);
    std::cout << "PASS: duplicate in row returns false\n";
}

static void test_duplicate_in_column_returns_false()
{
    Board b = validBoard();
    // Swap two values in the same column to create a duplicate
    std::swap(b[0][0], b[1][0]); // col 0: 6,5,... — duplicate 6 and missing 5
    assert(isBoardComplete(b) == false);
    std::cout << "PASS: duplicate in column returns false\n";
}

static void test_duplicate_in_box_returns_false()
{
    Board b = validBoard();
    // Swap two values within the top-left 3x3 box to create a duplicate
    std::swap(b[0][0], b[0][1]); // top-left box: 3,5,4 — duplicate 3 and missing 5
    assert(isBoardComplete(b) == false);
    std::cout << "PASS: duplicate in 3x3 box returns false\n";
}

static void test_out_of_range_value_returns_false()
{
    Board b = validBoard();
    b[8][8] = 10; // value outside 1-9
    assert(isBoardComplete(b) == false);
    std::cout << "PASS: out-of-range cell value returns false\n";
}

static void test_negative_value_returns_false()
{
    Board b = validBoard();
    b[0][0] = -1;
    assert(isBoardComplete(b) == false);
    std::cout << "PASS: negative cell value returns false\n";
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

int main()
{
    std::cout << "=== isBoardComplete unit tests ===\n";

    test_valid_complete_board_returns_true();
    test_board_with_one_empty_cell_returns_false();
    test_fully_empty_board_returns_false();
    test_duplicate_in_row_returns_false();
    test_duplicate_in_column_returns_false();
    test_duplicate_in_box_returns_false();
    test_out_of_range_value_returns_false();
    test_negative_value_returns_false();

    std::cout << "\nAll tests passed!\n";
    return 0;
}
