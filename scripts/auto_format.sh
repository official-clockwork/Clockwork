#!/bin/bash

EXCLUDE_DIRS=(
    "./build"
    "./build-release"
    "./build-evaltune"
    "./.git"
    "./.vscode"
    "./.idea"
    "./vendor"
)

FILE_PATTERNS=(
    "*.cpp" "*.hpp"
)

find_args=()
for pattern in "${FILE_PATTERNS[@]}"; do
    if [ ${#find_args[@]} -eq 0 ]; then
        find_args+=(-iname "$pattern")
    else
        find_args+=(-o -iname "$pattern")
    fi
done

if [ ${#find_args[@]} -gt 0 ]; then
    find_args=("(" "${find_args[@]}" ")")
fi

exclude_find_args=()
for dir in "${EXCLUDE_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        exclude_find_args+=(-path "$dir" -prune -o)
    fi
done

if [ ${#find_args[@]} -gt 0 ]; then
    find . \
         "${exclude_find_args[@]}" \
         "${find_args[@]}" \
         -print0 | while IFS= read -r -d $'\0' file; do
        echo "Formatting: $file"
        clang-format -i -style=file "$file"
    done
fi
