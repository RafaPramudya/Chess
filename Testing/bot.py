import chess

MAX_DEPTH = 3
OUTPUT_FILE = "depth3_lines.txt"

def traverse(board, depth, line, file):
    if depth == MAX_DEPTH:
        file.write(" ".join(line) + "\n")
        return

    for move in board.legal_moves:
        board.push(move)
        traverse(
            board,
            depth + 1,
            line + [move.uci()],
            file
        )
        board.pop()

board = chess.Board()

with open(OUTPUT_FILE, "w", buffering=1) as f:
    traverse(board, 0, [], f)

print("Done. Saved depth-3 move lines.")
