#ifndef _SUDOKU_BOARD_VALIDATOR_H_
#define _SUDOKU_BOARD_VALIDATOR_H_

#include <array>
#include <algorithm>

// Added 2026-07-16: standalone utility to check whether a 9x9 Sudoku board is fully filled and valid.

/**
 * isBoardComplete - returns true only when every cell of the 9x9 board
 * contains a digit 1-9 and every row, column, and 3x3 box contains each
 * digit exactly once.
 *
 * @param board  A 9x9 array of ints; 0 (or any value outside 1-9) means empty.
 * @return       true if the board is complete and valid, false otherwise.
 */
inline bool isBoardComplete(const std::array<std::array<int, 9>, 9>& board)
{
    // Helper: check that a group of 9 values is a permutation of 1..9
    auto isValidGroup = [](std::array<int, 9> group) -> bool {
        std::sort(group.begin(), group.end());
        for (int i = 0; i < 9; ++i) {
            if (group[i] != i + 1)
                return false;
        }
        return true;
    };

    for (int i = 0; i < 9; ++i) {
        // Check row i
        std::array<int, 9> row = board[i];
        if (!isValidGroup(row))
            return false;

        // Check column i
        std::array<int, 9> col;
        for (int j = 0; j < 9; ++j)
            col[j] = board[j][i];
        if (!isValidGroup(col))
            return false;

        // Check 3x3 box i (boxes numbered 0..8, row-major)
        std::array<int, 9> box;
        int boxRow = (i / 3) * 3;
        int boxCol = (i % 3) * 3;
        int idx = 0;
        for (int r = boxRow; r < boxRow + 3; ++r)
            for (int c = boxCol; c < boxCol + 3; ++c)
                box[idx++] = board[r][c];
        if (!isValidGroup(box))
            return false;
    }

    return true;
}

#endif // _SUDOKU_BOARD_VALIDATOR_H_
