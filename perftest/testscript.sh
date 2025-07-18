#!/bin/bash

# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]

# Script to test variations of the weighted random selections



echo "Number of tests: $1" # first argument is num of tests
echo "Weightnum: $2" # second argument is number of weights in data structure
echo "normal_static_fenwick, normal_static_discrete, normal_changing_fenwick, uniform_static_fenwick, uniform_static_discrete, uniform_changing_fenwick, weibull_static_fenwick, weibull_static_discrete, weibull_changing_fenwick" > results_$2.csv

# Normal distribution static weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test0 normal_static.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test1 normal_static.cpp
g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=$2" -o test2 normal_static.cpp

# Normal distribution changing weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test3 normal_changing.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test4 normal_changing.cpp

# Uniform distribution static weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test5 uniform_static.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test6 uniform_static.cpp
g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=$2" -o test7 uniform_static.cpp

# Uniform distribution changing weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test8 uniform_changing.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test9 uniform_changing.cpp

# Weibull distribution static weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test10 weibull_static.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test11 weibull_static.cpp
g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=$2" -o test12 weibull_static.cpp

# Weibull distribution changing weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test13 weibull_changing.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test14 weibull_changing.cpp

sum0=0
sum2=0
sum3=0

sum5=0

sum7=0
sum8=0

sum10=0

sum12=0
sum13=0


for ((i=1; i<=$1; i++)); do
    # Run your program and capture the output in a variable
    result0=$(./test0)

    result2=$(./test2)
    result3=$(./test3)

    result5=$(./test5)

    result7=$(./test7)
    result8=$(./test8)

    result10=$(./test10)

    result12=$(./test12)
    result13=$(./test13)


    # append test to results.csv
    echo "$result0, $result2, $result3, $result5, $result7, $result8, $result10, $result12, $result13" >> results_$2.csv

    # Add the result to the sum
    sum0=$(awk "BEGIN { printf \"%.6f\", $sum0 + $result0 }")
    sum2=$(awk "BEGIN { printf \"%.6f\", $sum2 + $result2 }")
    sum3=$(awk "BEGIN { printf \"%.6f\", $sum3 + $result3 }")
    sum5=$(awk "BEGIN { printf \"%.6f\", $sum5 + $result5 }")
    sum7=$(awk "BEGIN { printf \"%.6f\", $sum7 + $result7 }")
    sum8=$(awk "BEGIN { printf \"%.6f\", $sum8 + $result8 }")
    sum10=$(awk "BEGIN { printf \"%.6f\", $sum10 + $result10 }")
    sum12=$(awk "BEGIN { printf \"%.6f\", $sum12 + $result12 }")
    sum13=$(awk "BEGIN { printf \"%.6f\", $sum13 + $result13 }")
done

# Calculate the average
average0=$(awk "BEGIN { printf \"%.6f\", $sum0 / $1 }")
average2=$(awk "BEGIN { printf \"%.6f\", $sum2 / $1 }")
average3=$(awk "BEGIN { printf \"%.6f\", $sum3 / $1 }")
average5=$(awk "BEGIN { printf \"%.6f\", $sum5 / $1 }")
average7=$(awk "BEGIN { printf \"%.6f\", $sum7 / $1 }")
average8=$(awk "BEGIN { printf \"%.6f\", $sum8 / $1 }")
average10=$(awk "BEGIN { printf \"%.6f\", $sum10 / $1 }")
average12=$(awk "BEGIN { printf \"%.6f\", $sum12 / $1 }")
average13=$(awk "BEGIN { printf \"%.6f\", $sum13 / $1 }")

# Add averages to the csv
echo " " >> results.csv
echo "$average0, $average2, $average3, $average5, $average7, $average8, $average10, $average12, $average13" >> results_$2.csv

# Print the average
echo "normal_static_fenwick: $average0"
echo "normal_static_discrete: $average2"
echo "normal_changing_fenwick: $average3"
echo "uniform_static_fenwick: $average5"
echo "uniform_static_discrete: $average7"
echo "uniform_changing_fenwick: $average8"
echo "weibull_static_fenwick: $average10"
echo "weibull_static_discrete: $average12"
echo "weibull_changing_fenwick: $average13"
