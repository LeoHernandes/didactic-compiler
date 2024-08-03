#!/bin/bash

ERR_UNDECLARED=10
ERR_DECLARED=11
ERR_VARIABLE=20
ERR_FUNCTION=21

make
for file in $(ls tests); do
    echo $file
    answer=$(grep '//' tests/$file)
    grep_code=$?
    answer=$(echo $answer | tr -d '/') 
    if [ $grep_code -eq 0 ]; then
        ./etapa4 < tests/$file;
        code=$?
        if [ $code -eq $ERR_UNDECLARED ] && [ "$answer" = "ERR_UNDECLARED" ]; then
            echo "✅ Correct!"
        elif [ $code -eq $ERR_DECLARED ] && [ "$answer" = "ERR_DECLARED" ]; then
            echo "✅ Correct!"
        elif [ $code -eq $ERR_VARIABLE ] && [ "$answer" = "ERR_VARIABLE" ]; then
            echo "✅ Correct!"
        elif [ $code -eq $ERR_FUNCTION ] && [ "$answer" = "ERR_FUNCTION" ]; then
            echo "✅ Correct!"
        else
            echo "❌ Wrong!: $code" 
        fi
    else
        ./etapa4 < tests/$file
        code=$?
        if [ $code -eq 0 ]; then
            echo "✅ Correct!"
        else
            echo "❌ Wrong!"
        fi
    fi
done


