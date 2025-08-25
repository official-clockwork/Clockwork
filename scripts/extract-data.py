import os
import random
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor, as_completed
from typing import List, Optional, Dict, Any
import chess
import chess.pgn
import io
from tqdm import tqdm


def extract_positions_from_game(game_data: str, n_positions: int = 5) -> List[str]:
    """Extract N sampled positions from a game, labeled with final result (w/d/b)."""
    try:
        pgn_io = io.StringIO(game_data)
        game = chess.pgn.read_game(pgn_io)
        if not game:
            return []

        # Map result to label
        result = game.headers.get("Result", "")
        if result == "1-0":
            outcome = "w"
        elif result == "0-1":
            outcome = "b"
        elif result == "1/2-1/2":
            outcome = "d"
        else:
            return []  # Skip unfinished/invalid games

        board = game.board()
        positions = []
        move_number = 0

        # Collect all candidate positions
        all_positions = []
        for move in game.mainline_moves():
            board.push(move)
            move_number += 1
            if not board.is_check() and any(board.legal_moves):
                all_positions.append(board.fen())

        if not all_positions:
            return []

        # Sample up to n_positions
        if len(all_positions) > n_positions:
            all_positions = random.sample(all_positions, n_positions)

        # Label with outcome
        return [f"{fen};{outcome}" for fen in all_positions]

    except Exception:
        return []


def count_games_in_file(file_path: Path) -> int:
    try:
        with open(file_path, "r", encoding="utf-8") as f:
            content = f.read()
        return content.count("[Event ")
    except Exception:
        return 0


def process_pgn_file(file_path: Path, positions_per_game: int = 5, pbar: Optional[tqdm] = None) -> List[str]:
    results = []
    try:
        with open(file_path, "r", encoding="utf-8") as f:
            content = f.read()

        games = []
        current_game = []
        for line in content.split("\n"):
            if line.strip().startswith("[Event ") and current_game:
                games.append("\n".join(current_game))
                current_game = [line]
            else:
                current_game.append(line)
        if current_game:
            games.append("\n".join(current_game))

        for game_data in games:
            if game_data.strip():
                positions = extract_positions_from_game(game_data, positions_per_game)
                results.extend(positions)
            if pbar:
                pbar.update(1)

    except Exception as e:
        print(f"Error processing file {file_path}: {e}")
        if pbar:
            expected_games = count_games_in_file(file_path)
            pbar.update(expected_games)

    return results


def extract_positions_from_folder(folder_path: str, positions_per_game: int = 5, max_workers: int = 4) -> List[str]:
    folder = Path(folder_path)
    if not folder.exists():
        raise FileNotFoundError(f"Folder {folder_path} does not exist")

    pgn_files = list(folder.glob("*.pgn"))
    if not pgn_files:
        print(f"No PGN files found in {folder_path}")
        return []

    print(f"Found {len(pgn_files)} PGN files")

    total_games = 0
    for pgn_file in tqdm(pgn_files, desc="Counting games"):
        total_games += count_games_in_file(pgn_file)

    print(f"Total games to process: {total_games}")

    all_results = []
    with tqdm(total=total_games, desc="Processing games", unit="games") as pbar:
        with ThreadPoolExecutor(max_workers=max_workers) as executor:
            future_to_file = {
                executor.submit(process_pgn_file, pgn_file, positions_per_game, pbar): pgn_file
                for pgn_file in pgn_files
            }
            for future in as_completed(future_to_file):
                try:
                    all_results.extend(future.result())
                except Exception as e:
                    print(f"Error: {e}")

    return all_results


def save_positions_to_file(positions: List[str], output_file: str = "sampled_positions.txt"):
    with open(output_file, "w", encoding="utf-8") as f:
        for line in positions:
            f.write(line + "\n")
    print(f"Saved {len(positions)} positions to {output_file}")


def main():
    folder_path = input("Enter the path to the folder containing PGN files: ").strip()
    try:
        positions_per_game = int(input("Enter number of positions per game (default 10): ") or "10")
    except ValueError:
        positions_per_game = 10

    try:
        max_workers = int(input("Enter maximum number of worker threads (default 4): ") or "4")
    except ValueError:
        max_workers = 4

    try:
        positions = extract_positions_from_folder(
            folder_path, positions_per_game=positions_per_game, max_workers=max_workers
        )
        if positions:
            output_file = f"sampled_positions_{len(positions)}.txt"
            save_positions_to_file(positions, output_file)
            print(f"\nSummary:")
            print(f"- Total positions saved: {len(positions)}")
            print(f"- File: {output_file}")
        else:
            print("No valid games found!")
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    try:
        import chess
        import chess.pgn
        from tqdm import tqdm
    except ImportError:
        print("Please install required dependencies:")
        print("pip install python-chess tqdm")
        exit(1)

    main()
