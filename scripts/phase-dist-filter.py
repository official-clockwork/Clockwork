import random
import sys

def phase_scale_factor(phase: int) -> float:
    odd_scale = min(0.9, -0.09793221 * (phase - 23) + 0.189393939)
    p16 = abs(phase - 16)
    if phase > 16:
        base = 1 - 0.875 * (p16 / 8.0) ** 2
    else:
        base = 1 - 0.95 * (p16 / 16) ** 2
    if phase % 2 > 0 and phase > 12:
        return odd_scale * base
    return base

def fen_phase(fen: str) -> int:
    phase = 0
    for chr in fen:
        match chr:
            case '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '/' | 'p' | 'P' | 'k' | 'K':
                continue
            case 'n' | 'N' | 'b' | 'B':
                phase += 1
            case 'r' | 'R':
                phase += 2
            case 'q' | 'Q':
                phase += 4
            case _:
                break

    return min(phase, 24)

if __name__ == "__main__":
    input_filename = sys.argv[1]
    output_filename = sys.argv[2]
    print(f"Filtering file {input_filename} and writing to {output_filename}")

    with open(input_filename, "r") as infile, open(output_filename, "w") as outfile:
        lines = infile.readlines()
        phase_counts = [0] * 25
        for line in lines:
            phase_counts[fen_phase(line)] += 1

        phase_keep_probs = [0] * 25
        phase_norm_const = 100
        for phase in range(0, 25):
            observed = phase_counts[phase] / len(lines)
            # this is not actually the desired probability, but it will be normalized
            desired = phase_scale_factor(phase)

            phase_keep_probs[phase] = desired / observed
            phase_norm_const = min(phase_norm_const, observed / desired)

        for phase in range(0, 25):
            phase_keep_probs[phase] = min(phase_keep_probs[phase] * phase_norm_const, 1)
            print(f"Phase {phase} has keep probability {phase_keep_probs[phase]}")

        for line in lines:
            phase = fen_phase(line)
            if random.random() < phase_keep_probs[phase]:
                outfile.write(line)

        print("Finished")