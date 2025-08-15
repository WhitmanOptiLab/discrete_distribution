#!/bin/bash
# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]
#$1 is number of tests
#$2 is compilation command
# Script to test variations of the weighted random selections
echo "Number of tests: $1"
echo "Compilation command: $2"
echo "Name: $3"

weightNums=(16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432 67108864)
declare -A sum
declare -A result
declare -A average

echo -n  ""> resultsS_$3.csv

for i in "${!weightNums[@]}";
do
  eval "$2"
  echo -n "${weightNums[$i]}, ">>resultsS_$3.csv
done
echo "">>resultsS_$3.csv

for j in "${!weightNums[@]}";
do 
    sum[$j]=0
done



for ((i=1; i<=$1; i++)); do
    for j in "${!weightNums[@]}";do
        result[$j]=$(./test$j)
        echo -n "${result[$j]}, ">>resultsS_$3.csv
        sum[$j]=$(awk "BEGIN { printf \"%.6f\", ${sum[$j]} + ${result[$j]} }")
    done
    echo "" >>resultsS_$3.csv

done
for j in "${!weightNums[@]}";do
        average[$j]=$(awk "BEGIN { printf \"%.6f\", ${sum[$j]} /$1 }")
done


echo " " >> resultsS_$3.csv
for j in "${!weightNums[@]}";do
    echo -n "${average[$j]} ,">> resultsS_$3.csv
done
echo "Test completed. Results saved in resultsS_$3.csv"

