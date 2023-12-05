#!/bin/bash

# Check if no arguments were passed
if [ $# -eq 0 ]; then
    echo "No arguments provided. Please provide an .aut file as an argument."
    exit 1
fi

# Get the file name and extension
FILE=$1
EXTENSION="${FILE##*.}"
FILENAME="${FILE%.*}"

# Check if the file extension is 'aut'
if [ $EXTENSION != "aut" ]; then
    echo "Invalid file type. Please provide an .aut file."
    exit 1
fi

# Run the commands
ltsconvert $FILE --out=dot $FILENAME.dot
dot -Tpdf $FILENAME.dot -o $FILENAME.pdf

echo "Converted the reduced statespace."