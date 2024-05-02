#!/bin/bash

for file in $(ls tests --hide="*.dot"); do
    ./etapa3 < tests/$file | ./output2dot.sh > tests/$file.dot;
done

for i in {00..59}; do
    dot_file="tests/z${i}.dot"
    ref_dot_file="tests/z${i}.ref.dot"

   
    xdot "$dot_file" &
    xdot  "$ref_dot_file" 
   
    while pgrep -x "xdot" >/dev/null; do
        sleep 1
    done
done
