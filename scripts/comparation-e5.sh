#!/bin/bash

make
for file in $(ls tests); do
    echo $file
    ./etapa5 < tests/$file > tests/$file-output.iloc;
    /usr/bin/python3 ilocsim.py -x -s < tests/$file-output.iloc > tests/$file-results
    exit_code=$?
    if [ $exit_code -eq 0 ]; then
        echo "✅ Correct!"
    else
        echo "❌ Wrong!: $exit_code" 
    fi
done


