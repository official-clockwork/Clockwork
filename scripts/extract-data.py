import random
import io
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor, as_completed
from typing import List
import chess
import chess.pgn
from tqdm import tqdm


# ------------------------------------------------------------
# Game logic
# ------------------------------------------------------------

def is_capture_or_promotion(board: chess.Board, move: chess.Move) -> bool:
    return board.is_capture(move) or move.promotion is not None


def extract_positions_from_game(game_data: str, n_positions: int = 5) -> List[str]:
    try:
        pgn_io = io.StringIO(game_data)
        game = chess.pgn.read_game(pgn_io)
        if not game:
            return []

        result = game.headers.get("Result", "")
        if result == "1-0":
            outcome = "w"
        elif result == "0-1":
            outcome = "b"
        elif result == "1/2-1/2":
            outcome = "d"
        else:
            return []

        board = game.board()
        valid_positions = []

        for node in game.mainline():
            move = node.move
            if move:
                board.push(move)

            if board.is_check() or not any(board.legal_moves):
                continue

            next_node = node.next()
            best_move = next_node.move if next_node else None

            if best_move and is_capture_or_promotion(board, best_move):
                continue

            valid_positions.append(board.fen())

        if not valid_positions:
            return []

        if len(valid_positions) > n_positions:
            valid_positions = random.sample(valid_positions, n_positions)

        return [f"{fen};{outcome}" for fen in valid_positions]

    except Exception:
        return []


def process_single_game(game_data: str, positions_per_game: int) -> List[str]:
    return extract_positions_from_game(game_data, positions_per_game)


# ------------------------------------------------------------
# PGN streaming utilities
# ------------------------------------------------------------

def stream_pgn_games(file_path: Path):
    with open(file_path, "r", encoding="utf-8") as f:
        game_lines = []
        for line in f:
            if line.startswith("[Event ") and game_lines:
                yield "".join(game_lines)
                game_lines = [line]
            else:
                game_lines.append(line)

        if game_lines:
            yield "".join(game_lines)


def count_games_in_file(file_path: Path) -> int:
    count = 0
    with open(file_path, "r", encoding="utf-8") as f:
        for line in f:
            if line.startswith("[Event "):
                count += 1
    return count


# ------------------------------------------------------------
# Main multiprocessing pipeline
# ------------------------------------------------------------

def extract_positions_from_folder(
    folder_path: str,
    positions_per_game: int = 5,
    max_workers: int = 4,
    task_buffer: int = 2000,
) -> List[str]:

    folder = Path(folder_path)
    if not folder.exists():
        raise FileNotFoundError(f"Folder does not exist: {folder}")

    pgn_files = list(folder.glob("*.pgn"))
    if not pgn_files:
        print("No PGN files found.")
        return []

    print(f"Found {len(pgn_files)} PGN files")

    total_games = sum(count_games_in_file(p) for p in pgn_files)
    print(f"Total games to process: {total_games}")

    all_results = []
    futures = []

    with ProcessPoolExecutor(max_workers=max_workers) as executor:
        with tqdm(total=total_games, desc="Processing games", unit="games") as pbar:

            for pgn_file in pgn_files:
                for game_data in stream_pgn_games(pgn_file):
                    futures.append(
                        executor.submit(
                            process_single_game,
                            game_data,
                            positions_per_game,
                        )
                    )

                    if len(futures) >= task_buffer:
                        for future in as_completed(futures):
                            all_results.extend(future.result())
                            pbar.update(1)
                        futures.clear()

            for future in as_completed(futures):
                all_results.extend(future.result())
                pbar.update(1)

    return all_results


# ------------------------------------------------------------
# Output
# ------------------------------------------------------------

def save_positions_to_file(positions: List[str], output_file: str):
    with open(output_file, "w", encoding="utf-8") as f:
        for line in positions:
            f.write(line + "\n")
    print(f"Saved {len(positions)} positions to {output_file}")


# ------------------------------------------------------------
# CLI
# ------------------------------------------------------------

def main():
    folder_path = input("Enter the path to the folder containing PGN files: ").strip()

    try:
        positions_per_game = int(input("Enter number of positions per game (default 5): ") or "5")
    except ValueError:
        positions_per_game = 5

    try:
        max_workers = int(input("Enter maximum number of worker processes (default 4): ") or "4")
    except ValueError:
        max_workers = 4

    positions = extract_positions_from_folder(
        folder_path,
        positions_per_game=positions_per_game,
        max_workers=max_workers,
    )

    if positions:
        output_file = f"sampled_positions_{len(positions)}.txt"
        save_positions_to_file(positions, output_file)
    else:
        print("No positions extracted.")


if __name__ == "__main__":
    try:
        import chess
        import chess.pgn
        from tqdm import tqdm
    except ImportError:
        print("Please install dependencies:")
        print("pip install python-chess tqdm")
        exit(1)

    main()
