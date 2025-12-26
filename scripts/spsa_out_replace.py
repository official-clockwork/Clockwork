import re

# Change config as needed
VALUES_FILE = "spsa_out_140k_stc.txt"
HEADER_FILE = "src/tuned.hpp"
OUTPUT_FILE = "src/tuned.hpp"

PREFIX_TO_STRIP = "tune_"

def load_values(path):
    values = {}
    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue

            name, value = line.split(",", 1)
            name = name.strip()
            value = value.strip()

            if name.startswith(PREFIX_TO_STRIP):
                name = name[len(PREFIX_TO_STRIP):]

            values[name] = value

    return values


def replace_macro_values(text, values):
    macro_re = re.compile(
        r"""
        (?P<macro>TUNE|NO_TUNE)      # macro name
        \(
        (?P<name>\s*[\w\d_]+)\s*,   # parameter name
        (?P<value>\s*[^,]+)\s*,     # value to replace (2nd arg)
        """,
        re.VERBOSE,
    )

    def repl(match):
        name = match.group("name").strip()
        old_value = match.group("value")

        if name not in values:
            return match.group(0)

        new_value = values[name]

        return (
            f"{match.group('macro')}("
            f"{match.group('name')}, "
            f"{new_value},"
        )

    return macro_re.sub(repl, text)


def main():
    values = load_values(VALUES_FILE)

    with open(HEADER_FILE, "r") as f:
        original = f.read()

    updated = replace_macro_values(original, values)

    with open(OUTPUT_FILE, "w") as f:
        f.write(updated)


if __name__ == "__main__":
    main()