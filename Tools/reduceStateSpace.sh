#!/bin/bash

# Keep all statespace-related things in here
OUTLOCATION="$(realpath -L "../Rebeca/statespaces/")/"

OBSERVABLE="$(realpath -L "../Rebeca/statespaces/observable_actions.txt")"
WC_OBSERV=$(wc -w <<< "$OBSERVABLE") # Should be 1, counts lines

# May be empty on first run
TAU="$(realpath -L "../Rebeca/statespaces/tau_transitions.txt")" 

# Check if no arguments were passed
if [ $# -eq 0 ]; then
    echo "No arguments provided. Please provide a .statespace file as an argument."
    exit 1
fi

# Check that a file for observable states exists
if [ $WC_OBSERV -ne 1 ]; then
    echo "Something wrong with the observable states"
fi


# Get the file name and extension
FILE=$1
FILEPATH="$(realpath -L $1)"
EXTENSION="${FILE##*.}"
FILENAME="${FILE%.*}"

# Check if the file extension is 'statespace'
if [ $EXTENSION != "statespace" ]; then
    echo "Invalid file type. Please provide an .statespace file."
    exit 1
fi

# Path to helper programs
CAST=$(realpath -L "Cast/cast")
EXTRACTION=$(realpath -L "Extraction/extraction")

AUTFILE="$(basename $FILENAME).aut"

# Replace .aut-file
if [[ -f $OUTLOCATION$AUTFILE ]]; then
    rm $OUTLOCATION$AUTFILE
fi
touch $OUTLOCATION$AUTFILE

$CAST $FILE > $OUTLOCATION$AUTFILE
TESTAUTFILE=$(wc -w <<< "$OUTLOCATION$AUTFILE")

# Check that something was written out to the .aut-file
if [ $TESTAUTFILE -eq 0 ]; then
    echo "Something went wrong during casting"
fi

# Extraction
$EXTRACTION $OUTLOCATION$AUTFILE $TAU -s $OBSERVABLE