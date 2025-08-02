import chess
import chess.pgn
import random
import re
import io
import os
from concurrent.futures import ThreadPoolExecutor, as_completed
from tqdm import tqdm
import multiprocessing

# --- Config ---
INPUT_FOLDER = "data/"              # Folder containing PGN files
OUTPUT_FENS = "filtered_positions.fen"   # Output file
POSITIONS_PER_GAME = 10
MAX_WORKERS = multiprocessing.cpu_count()

# --- Mate annotation pattern ---
mate_pattern = re.compile(r"(#-?\d+|mate\s*[+-]?\d+)", re.IGNORECASE)

def is_capture_move(move, board):
    return board.is_capture(move)

def is_mate_annotation(comment):
    return mate_pattern.search(comment) is not None

def process_game(game_text):
    try:
        game = chess.pgn.read_game(io.StringIO(game_text))
        board = game.board()
        fens = []
        node = game

        while not node.is_end():
            next_node = node.variation(0)
            move = next_node.move
            if move is None:
                break

            comment = node.comment or ""

            if not next_node.is_end():
                response_node = next_node.variation(0)
                response_move = response_node.move
                response_comment = next_node.comment or ""

                board.push(move)

                if (
                    not board.is_check()
                    and response_move is not None
                    and not is_capture_move(response_move, board)
                    and not is_mate_annotation(comment)
                    and not is_mate_annotation(response_comment)
                ):
                    fens.append(board.fen())

                board.push(response_move)
                node = response_node
            else:
                break

        if len(fens) > POSITIONS_PER_GAME:
            fens = random.sample(fens, POSITIONS_PER_GAME)

        return fens
    except Exception:
        return []

def split_games_in_file(pgn_path):
    """Yields raw PGN strings per game from a file."""
    with open(pgn_path, "r", encoding="utf-8", errors="ignore") as f:
        buffer = []
        for line in f:
            if line.strip() == "" and buffer:
                yield "".join(buffer)
                buffer = []
            else:
                buffer.append(line)
        if buffer:
            yield "".join(buffer)

def collect_all_games(folder_path):
    """Yields tuples of (game_text, filename) for all games in the folder."""
    for filename in os.listdir(folder_path):
        if filename.lower().endswith(".pgn"):
            file_path = os.path.join(folder_path, filename)
            for game_text in split_games_in_file(file_path):
                yield game_text

def main():
    all_game_texts = list(collect_all_games(INPUT_FOLDER))
    total_games = len(all_game_texts)

    with open(OUTPUT_FENS, "w") as out_file:
        with ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
            futures = [executor.submit(process_game, game_text) for game_text in all_game_texts]

            for future in tqdm(as_completed(futures), total=total_games, desc="Processing all PGN files"):
                fens = future.result()
                for fen in fens:
                    out_file.write(fen + "\n")

if __name__ == "__main__":
    main()
