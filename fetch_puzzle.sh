#!/usr/bin/env bash
# fetch_puzzle.sh
# Downloads a random Sudoku puzzle from the sugoku public API and saves it to
# puzzle.json so it can be inspected or fed into the game.
#
# Requirements: curl
# Usage: ./fetch_puzzle.sh [easy|medium|hard|random]

set -euo pipefail

DIFFICULTY="${1:-random}"
API_URL="https://sugoku.onrender.com/board?difficulty=${DIFFICULTY}"

echo "Fetching a '${DIFFICULTY}' puzzle from ${API_URL} ..."
curl -fsSL "${API_URL}" -o puzzle.json
echo "Puzzle saved to puzzle.json"
cat puzzle.json
echo
