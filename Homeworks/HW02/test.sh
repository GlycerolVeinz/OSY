#!/bin/bash

line="123 some text +456 another text"

# Remove the number from the start of the line
cleaned_line="${line#"${line%%[0-9]*}"}"

echo "Original line: $line"
echo "Line with number removed from the start: $cleaned_line"
