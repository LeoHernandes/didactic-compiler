#!/bin/bash

make
mkdir -p tests/asm_output
mkdir -p tests/exec_output
test_number=1
for file in $(ls tests | grep kwj | grep -v '\.s'); do
    echo $file
    ./etapa6 < tests/$file > tests/asm_output/$file.s
    gcc tests/asm_output/$file.s -o tests/exec_output/$file
    ./tests/exec_output/$file
    result=$?
    expected=$(sed "$test_number q;d" tests/results)
    if [ $result -eq $expected ]; then
        echo "✅ Correct!"
        echo ""
    else
        echo "❌ Wrong! Result: $result | Expected: $expected\n" 
        echo ""
    fi
    ((test_number++))
done


