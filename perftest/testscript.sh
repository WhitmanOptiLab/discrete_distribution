#!/bin/bash

# Script to test variations of the weighted random selections
numtests=1
weightnum=100

echo "Number of tests: $1"
echo "Weightnum: $2"
echo "WRS, Heap, Discrete" > results.csv

g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test0 normal_static.cpp
g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=$2" -o test1 normal_static.cpp
g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=$2" -o test2 normal_static.cpp

sum0=0
sum1=0
sum2=0

for ((i=1; i<=$1; i++)); do
    # Run your program and capture the output in a variable
    result0=$(./test0)
    result1=$(./test1)
    result2=$(./test2)

    # append test to results.csv
    echo "$result0, $result1, $result2" >> results.csv

    # Add the result to the sum
    sum0=$(awk "BEGIN { printf \"%.6f\", $sum0 + $result0 }")
    sum1=$(awk "BEGIN { printf \"%.6f\", $sum1 + $result1 }")
    sum2=$(awk "BEGIN { printf \"%.6f\", $sum2 + $result2 }")
done

# Calculate the average
average0=$(awk "BEGIN { printf \"%.6f\", $sum0 / $1 }")
average1=$(awk "BEGIN { printf \"%.6f\", $sum1 / $1 }")
average2=$(awk "BEGIN { printf \"%.6f\", $sum2 / $1 }")

# Add averages to the csv
echo " , , " >> results.csv
echo "$average0, $average1, $average2" >> results.csv

# Print the average
echo "wrs average time: $average0"
echo "heap average time: $average1"
echo "std average time: $average2"