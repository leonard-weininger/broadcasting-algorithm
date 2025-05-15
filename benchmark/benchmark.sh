#!/bin/bash

# Usage message
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <n> <k> [seed]"
    exit 1
fi

# Required parameters
n="$1"
k="$2"

# Optional seed
seed="$3"

# Get the directory of the script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Create a temp file for the graph output
graph_file=$(mktemp "/tmp/graph.XXXXXX.txt")

# Clean up temp file on exit
trap "rm -f '$graph_file'" EXIT

# Run generate with or without seed
if [ -n "$seed" ]; then
    "$SCRIPT_DIR/../build/generate" "$n" "$k" "$seed" --verbose 2>&1 > "$graph_file"
else
    "$SCRIPT_DIR/../build/generate" "$n" "$k" --verbose 2>&1 > "$graph_file"
fi
echo -e "[INFO]"

# Run solver and handle output
"$SCRIPT_DIR/../build/solve" --verbose --no-output < "$graph_file" 2>&1

