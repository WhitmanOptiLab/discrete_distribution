#!/bin/bash

# Script to test variations of the weighted random selections
numtests=1
weightnum=100

g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$weightnum" -o test0 normal_static.cpp
g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=$weightnum" -o test1 normal_static.cpp
g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=$weightnum" -o test2 normal_static.cpp

sum0=0
sum1=0
sum2=0

for ((i=1; i<=numtests; i++)); do
    # Run your program and capture the output in a variable
    result0=$(./test0)
    result1=$(./test1)
    result2=$(./test2)

    # Add the result to the sum
    sum0=$(awk "BEGIN { printf \"%.6f\", $sum0 + $result0 }") # there must be a better way to collect the results
    sum1=$(awk "BEGIN { printf \"%.6f\", $sum1 + $result1 }")
    sum2=$(awk "BEGIN { printf \"%.6f\", $sum2 + $result2 }")
done

# Calculate the average
average0=$(awk "BEGIN { printf \"%.6f\", $sum0 / $numtests }")
average1=$(awk "BEGIN { printf \"%.6f\", $sum1 / $numtests }")
average2=$(awk "BEGIN { printf \"%.6f\", $sum2 / $numtests }")

# Print the average
echo "wrs average time: $average0"
echo "heap average time: $average1"
echo "std average time: $average2"