#!/bin/bash

if [ "$1" == "-m" ]; then
    echo "Executing make..."
    make
    
    # Check if make succeeded (exit code 0). If not, abort the script.
    if [ $? -ne 0 ]; then
        echo "Make failed! Fix your code before uploading."
        exit 1
    fi
fi

killall jtagd || true
jtagd --user-start
dtekv-run main.bin
