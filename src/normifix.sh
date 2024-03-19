#!/bin/zsh

# The directory to start searching from, replace '/path/to/your/directory' with your actual directory path
root_dir=$1

# Iterate over each file found by find
find "$root_dir"  -type f \( -name "*.c" -o -name "*.h" \)| while read file; do
  # Use a temporary file to store the formatted content
  temp_file=$(mktemp)

  # Format the file and write the output to the temporary file
  python3 -m c_formatter_42 < "$file" > "$temp_file"

  # Replace the original file with the formatted file
  mv "$temp_file" "$file"

  echo "Processed $file"
done

