from collections import Counter
import matplotlib.pyplot as plt
import sys

def count_piece_distribution(fen_files):
    """
    Reads a list of files containing FEN strings (one per line)
    Returns:
      - piece_counts: total count of each piece across all positions
      - piece_totals: list of total piece counts per FEN
    """
    piece_counts = Counter()
    piece_totals = []
    phases = []

    for fen_file in fen_files:
        try:
            with open(fen_file, "r") as f:
                for line in f:
                    fen = line.strip()
                    if not fen:
                        continue

                    board_part = fen.split(" ")[0]

                    count_this_fen = 0
                    phase_this_fen = 0
                    for ch in board_part:
                        if ch.isalpha():
                            piece_counts[ch] += 1
                            count_this_fen += 1
                            match ch:
                                case 'n' | 'N' | 'b' | 'B':
                                    phase_this_fen += 1
                                case 'r' | 'R':
                                    phase_this_fen += 2
                                case 'q' | 'Q':
                                    phase_this_fen += 4

                    piece_totals.append(count_this_fen)
                    phases.append(phase_this_fen)
        except FileNotFoundError:
            print(f"⚠️ Skipping missing file: {fen_file}")

    return piece_counts, piece_totals, phases


if __name__ == "__main__":
    # List of files
    fen_files = [
        "data/v2_filtered/gioviok_sampled_positions_337763.txt",
                                         "data/v2_filtered/giovivast_sampled_positions_2568565.txt",
                                         "data/v2_filtered/micpillar_sampled_positions_25219.txt",
                                         "data/v2_filtered/styx_sampled_positions_2101270.txt"]
    if len(sys.argv) > 1:
        fen_files = sys.argv[1:]

    piece_counts, piece_totals, phases = count_piece_distribution(fen_files)

    # --- Print counts ---
    print("Piece count distribution:")
    for piece, count in sorted(piece_counts.items()):
        print(f"{piece}: {count}")

    # --- Histogram 1: Piece types ---
    plt.figure(figsize=(10,4))
    pieces = list(piece_counts.keys())
    values = [piece_counts[p] for p in pieces]

    plt.bar(pieces, values)
    plt.xlabel("Piece")
    plt.ylabel("Count")
    plt.title("Piece Count Distribution Across All FENs")
    plt.show()

    # --- Histogram 2: Total piece counts per FEN ---
    plt.figure(figsize=(10,4))
    plt.hist(piece_totals, bins=range(0, 34), align="left", rwidth=0.8)
    plt.xlabel("Number of pieces on board")
    plt.ylabel("Number of positions")
    plt.title("Distribution of Total Piece Counts per Position")
    plt.xticks(range(0, 33))
    plt.show()

    for i in range(1, 33):
        count = sum(1 for total in piece_totals if total == i)
        print(f"Positions with {i} pieces: {count}")

    # --- Histogram 3: Phase per FEN ---
    plt.figure(figsize=(10,4))
    plt.hist(phases, bins=range(0, 26), align="left", rwidth=0.8)
    plt.xlabel("Phase of the position")
    plt.ylabel("Number of positions")
    plt.title("Distribution of Phase per Position")
    plt.xticks(range(0, 25))
    plt.show()

    for i in range(0, 25):
        count = sum(1 for phase in phases if phase == i)
        print(f"Positions with phase {i}: {count}")
