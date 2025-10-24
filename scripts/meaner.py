#!/usr/bin/env python3
"""
meaner.py
Reads a C++ file containing `inline const PParam ... = S(x,y);` and
`inline const std::array<PParam, N> NAME = { S(...), CS(...), ... };`
computes means for mobility and PSQT arrays, subtracts the mean from
each table entry and adds that mean (rounded) to the corresponding
material constant. King tables are subtracted but NOT added to a material.
Prints a clear debug summary and writes an adjusted output file.
"""
import re
import sys
import statistics
from pathlib import Path

# Mapping: table_name -> material_constant_name (None = don't add anywhere)
MOBILITY_TO_MAT = {
    "KNIGHT_MOBILITY": "KNIGHT_MAT",
    "BISHOP_MOBILITY": "BISHOP_MAT",
    "ROOK_MOBILITY":   "ROOK_MAT",
    "QUEEN_MOBILITY":  "QUEEN_MAT",
    "KING_MOBILITY":   None,
}

PSQT_TO_MAT = {
    "PAWN_PSQT":   "PAWN_MAT",
    "KNIGHT_PSQT": "KNIGHT_MAT",
    "BISHOP_PSQT": "BISHOP_MAT",
    "ROOK_PSQT":   "ROOK_MAT",
    "QUEEN_PSQT":  "QUEEN_MAT",
    "KING_PSQT":   None,
}


def parse_constants(text):
    # matches lines like: inline const PParam   ROOK_MAT   = S(445, 386);
    pat = re.compile(
        r"(inline\s+const\s+PParam\s+)(\w+)(\s*=\s*)(?:S|CS)\(\s*(-?\d+)\s*,\s*(-?\d+)\s*\)(\s*;)",
        re.M,
    )
    consts = {}
    for m in pat.finditer(text):
        name = m.group(2)
        x = int(m.group(4))
        y = int(m.group(5))
        consts[name] = (x, y)
    return consts


def parse_arrays(text):
    # matches: inline const std::array<PParam, 6> NAME = { ... };
    arr_pat = re.compile(
        r"inline\s+const\s+std::array\s*<\s*PParam\s*,\s*\d+\s*>\s+(\w+)\s*=\s*\{([^}]*)\}\s*;",
        re.S,
    )
    arrays = {}
    for m in arr_pat.finditer(text):
        name = m.group(1)
        body = m.group(2)
        # find entries preserving S vs CS
        entry_pat = re.compile(r"(CS|S)\(\s*(-?\d+)\s*,\s*(-?\d+)\s*\)")
        entries = []
        for em in entry_pat.finditer(body):
            macro = em.group(1)  # 'S' or 'CS'
            x = int(em.group(2))
            y = int(em.group(3))
            entries.append({"macro": macro, "x": x, "y": y})
        arrays[name] = entries
    return arrays


def compute_mean_round(entries):
    xs = [e["x"] for e in entries]
    ys = [e["y"] for e in entries]
    if not xs:
        return (0, 0)
    mean_x = round(statistics.mean(xs))
    mean_y = round(statistics.mean(ys))
    return (mean_x, mean_y)


def adjust_table(entries, mean):
    mx, my = mean
    for e in entries:
        e["x"] = e["x"] - mx
        e["y"] = e["y"] - my


def rebuild_array_text(entries, per_line=8):
    # Keep original macro per entry. Format them neatly with linebreaks.
    tokens = []
    for e in entries:
        tokens.append(f'{e["macro"]}({e["x"]}, {e["y"]})')
    if not tokens:
        return ""
    lines = []
    for i in range(0, len(tokens), per_line):
        lines.append("    " + ", ".join(tokens[i : i + per_line]) + ("," if i + per_line < len(tokens) else ""))
    return "\n".join(lines)


def main():
    if len(sys.argv) < 2:
        print("Usage: meaner.py input.cpp [output.cpp]")
        sys.exit(1)
    in_path = Path(sys.argv[1])
    out_path = Path(sys.argv[2]) if len(sys.argv) > 2 else in_path.with_name(in_path.stem + "_adjusted.hpp")
    text = in_path.read_text(encoding="utf-8")

    constants = parse_constants(text)
    arrays = parse_arrays(text)

    print("Parsed constants:", sorted(constants.keys()))
    # debug: list arrays
    print("Parsed arrays:", sorted(arrays.keys()))

    summary = []

    # process mobility tables
    for tab, mat in MOBILITY_TO_MAT.items():
        if tab not in arrays:
            continue
        entries = arrays[tab]
        mean = compute_mean_round(entries)
        adjust_table(entries, mean)
        # add mean to material if required
        old_mat = constants.get(mat) if mat else None
        if mat:
            if mat in constants:
                constants[mat] = (constants[mat][0] + mean[0], constants[mat][1] + mean[1])
            else:
                print(f"WARNING: material {mat} not found for table {tab}; skipping add.")
        summary.append((tab, "mobility", mean, old_mat, constants.get(mat)))

    # process PSQT tables
    for tab, mat in PSQT_TO_MAT.items():
        if tab not in arrays:
            continue
        entries = arrays[tab]
        mean = compute_mean_round(entries)
        adjust_table(entries, mean)
        old_mat = constants.get(mat) if mat else None
        if mat:
            if mat in constants:
                constants[mat] = (constants[mat][0] + mean[0], constants[mat][1] + mean[1])
            else:
                print(f"WARNING: material {mat} not found for table {tab}; skipping add.")
        summary.append((tab, "psqt", mean, old_mat, constants.get(mat)))

    # Replace constants in text (preserve prefix/suffix whitespace)
    const_pat = re.compile(
        r"(inline\s+const\s+PParam\s+)"
        r"(\w+)"
        r"(\s*=\s*)"
        r"(?:S|CS)\(\s*-?\d+\s*,\s*-?\d+\s*\)"
        r"(\s*;)",
        re.M,
    )

    def const_repl(m):
        prefix, name, middle, suffix = m.group(1), m.group(2), m.group(3), m.group(4)
        if name in constants:
            x, y = constants[name]
            return f"{prefix}{name}{middle}S({x}, {y}){suffix}"
        return m.group(0)

    text = const_pat.sub(const_repl, text)


    text = re.sub(
        r"(inline\s+const\s+PParam\s+)(\w+)(\s*=\s*)(?:S|CS)\(\s*-?\d+\s*,\s*-?\d+\s*\)(\s*;)",
        const_repl,
        text,
        flags=re.M,
    )

    # Replace arrays: callback constructs new { ... } content.
    def array_repl(m):
        name = m.group(1)
        if name not in arrays:
            return m.group(0)
        entries = arrays[name]
        new_body = rebuild_array_text(entries, per_line=8)
        # keep the same template for std::array with updated size
        return f"inline const std::array<PParam, {len(entries)}> {name} = {{\n{new_body}\n}};"

    text = re.sub(
        r"inline\s+const\s+std::array\s*<\s*PParam\s*,\s*\d+\s*>\s+(\w+)\s*=\s*\{[^}]*\}\s*;",
        array_repl,
        text,
        flags=re.S,
    )

    out_path.write_text(text, encoding="utf-8")

    # Print a friendly summary
    print("\n=== Adjustment summary ===")
    for tab, kind, mean, old_mat, new_mat in summary:
        print(f"{tab} ({kind}): mean = ({mean[0]}, {mean[1]})")
        if old_mat is None and new_mat is None:
            print("  -> no material to update (king or unmapped).")
        else:
            print(f"  -> material BEFORE: {old_mat}, AFTER: {new_mat}")
    print("\nWrote adjusted file to", out_path)


if __name__ == "__main__":
    main()
