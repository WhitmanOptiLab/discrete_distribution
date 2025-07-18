#!/bin/bash

# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]

# Script to test variations of the weighted random selections



echo "Number of tests: $1" # first argument is num of tests
echo "Weightnum: $2" # second argument is number of weights in data structure
echo "normal_static_leafsum, normal_static_leafsum_split, normal_static_sideways_fenwick, normal_changing_leafsum, normal_changing_leafsum_split, normal_changing_sideways_fenwick, uniform_static_leafsum, uniform_static_leafsum_split, uniform_static_sideways_fenwick, uniform_changing_leafsum, uniform_changing_leafsum_split, uniform_changing_sideways_fenwick, weibull_static_leafsum, weibull_static_leafsum_split, weibull_static_sideways_fenwick, weibull_changing_leafsum, weibull_changing_leafsum_split, weibull_changing_sideways_fenwick"  > results_$2.csv

# Normal distribution static weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<int>" "-DWEIGHTNUM=$2" -o test0 normal_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<int>" "-DWEIGHTNUM=$2" -o test1 normal_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test2 normal_static.cpp




# Normal distribution changing weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<int>" "-DWEIGHTNUM=$2" -o test3 normal_changing.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<int>" "-DWEIGHTNUM=$2" -o test4 normal_changing.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test5 normal_changing.cpp


# Uniform distribution static weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<int>" "-DWEIGHTNUM=$2" -o test6 uniform_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<int>" "-DWEIGHTNUM=$2" -o test7 uniform_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test8 uniform_static.cpp


# Uniform distribution changing weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<int>" "-DWEIGHTNUM=$2" -o test9 uniform_changing.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<int>" "-DWEIGHTNUM=$2" -o test10 uniform_changing.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test11 uniform_changing.cpp


# Weibull distribution static weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<int>" "-DWEIGHTNUM=$2" -o test12 weibull_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<int>" "-DWEIGHTNUM=$2" -o test13 weibull_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test14 weibull_static.cpp


# Weibull distribution changing weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<int>" "-DWEIGHTNUM=$2" -o test15 weibull_changing.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<int>" "-DWEIGHTNUM=$2" -o test16 weibull_changing.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test17 weibull_changing.cpp


sum0=0
sum1=0
sum2=0
sum3=0
sum4=0
sum5=0
sum6=0
sum7=0
sum8=0
sum9=0
sum10=0
sum11=0
sum12=0
sum13=0
sum14=0
sum15=0
sum16=0
sum17=0


for ((i=1; i<=$1; i++)); do
    # Run your program and capture the output in a variable
    #echo "starting test 0"
    result0=$(./test0)
    #echo "starting test 2"
    result1=$(./test1)
    #echo "starting test 2"
    result2=$(./test2)
    #echo "starting test 3"
    result3=$(./test3)
    #echo "starting test 4"
    result4=$(./test4)
    #echo "starting test 5"
    result5=$(./test5)
    #echo "starting test 6"
    result6=$(./test6)
    #echo "starting test 7"
    result7=$(./test7)
    #echo "starting test 8"
    result8=$(./test8)
    #echo "starting test 9"
    result9=$(./test9)
    #echo "starting test 10"
    result10=$(./test10)
    #echo "starting test 11"
    result11=$(./test11)
    #echo "starting test 12"
    result12=$(./test12)
    #echo "starting test 13"
    result13=$(./test13)
    #echo "starting test 14"
    result14=$(./test14)
    #echo "starting test 15"
    result15=$(./test15)
    #echo "starting test 16"
    result16=$(./test16)
    #echo "starting test 17"
    result17=$(./test17)
    
    

    # append test to results.csv
    echo "$result0, $result1, $result2, $result3, $result4, $result5, $result6, $result7, $result8, $result9, $result10, $result11, $result12, $result13, $result14, $result15, $result16, $result17" >> results_$2.csv

    
    # Add the result to the sum
    sum0=$(awk "BEGIN { printf \"%.6f\", $sum0 + $result0 }")
    sum1=$(awk "BEGIN { printf \"%.6f\", $sum1 + $result1 }")
    sum2=$(awk "BEGIN { printf \"%.6f\", $sum2 + $result2 }")
    sum3=$(awk "BEGIN { printf \"%.6f\", $sum3 + $result3 }")
    sum4=$(awk "BEGIN { printf \"%.6f\", $sum4 + $result4 }")
    sum5=$(awk "BEGIN { printf \"%.6f\", $sum5 + $result5 }")
    sum6=$(awk "BEGIN { printf \"%.6f\", $sum6 + $result6 }")
    sum7=$(awk "BEGIN { printf \"%.6f\", $sum7 + $result7 }")
    sum8=$(awk "BEGIN { printf \"%.6f\", $sum8 + $result8 }")
    sum9=$(awk "BEGIN { printf \"%.6f\", $sum9 + $result9 }")
    sum10=$(awk "BEGIN { printf \"%.6f\", $sum10 + $result10 }")
    sum11=$(awk "BEGIN { printf \"%.6f\", $sum11 + $result11 }")
    sum12=$(awk "BEGIN { printf \"%.6f\", $sum12 + $result12 }")
    sum13=$(awk "BEGIN { printf \"%.6f\", $sum13 + $result13 }")
    sum14=$(awk "BEGIN { printf \"%.6f\", $sum14 + $result14 }")
    sum15=$(awk "BEGIN { printf \"%.6f\", $sum15 + $result15 }")
    sum16=$(awk "BEGIN { printf \"%.6f\", $sum16 + $result16 }")
    sum17=$(awk "BEGIN { printf \"%.6f\", $sum17 + $result17 }")
    
    
done

# Calculate the average
average0=$(awk "BEGIN { printf \"%.6f\", $sum0 / $1 }")
average1=$(awk "BEGIN { printf \"%.6f\", $sum1 / $1 }")
average2=$(awk "BEGIN { printf \"%.6f\", $sum2 / $1 }")
average3=$(awk "BEGIN { printf \"%.6f\", $sum3 / $1 }")
average4=$(awk "BEGIN { printf \"%.6f\", $sum4 / $1 }")
average5=$(awk "BEGIN { printf \"%.6f\", $sum5 / $1 }")
average6=$(awk "BEGIN { printf \"%.6f\", $sum6 / $1 }")
average7=$(awk "BEGIN { printf \"%.6f\", $sum7 / $1 }")
average8=$(awk "BEGIN { printf \"%.6f\", $sum8 / $1 }")
average9=$(awk "BEGIN { printf \"%.6f\", $sum9 / $1 }")
average10=$(awk "BEGIN { printf \"%.6f\", $sum10 / $1 }")
average11=$(awk "BEGIN { printf \"%.6f\", $sum11 / $1 }")
average12=$(awk "BEGIN { printf \"%.6f\", $sum12 / $1 }")
average13=$(awk "BEGIN { printf \"%.6f\", $sum13 / $1 }")
average14=$(awk "BEGIN { printf \"%.6f\", $sum14 / $1 }")
average15=$(awk "BEGIN { printf \"%.6f\", $sum15 / $1 }")
average16=$(awk "BEGIN { printf \"%.6f\", $sum16 / $1 }")
average17=$(awk "BEGIN { printf \"%.6f\", $sum17 / $1 }")


# Add averages to the csv
echo " " >> results.csv
echo "$average0, $average1, $average2, $average3, $average4, $average5, $average6, $average7, $average8, $average9, $average10, $average11, $average12, $average13, $average14, $average15, $average16, $average17" >> results_$2.csv

# Print the average
echo "normal_static_leafsum: $average0"
echo "normal_static_leafsum_split: $average1"
echo "normal_static_sideways_fenwick: $average2"
echo "normal_changing_leafsum: $average3"
echo "normal_changing_leafsum_split: $average4"
echo "normal_changing_sideways_fenwick: $average5"
echo "uniform_static_leafsum: $average6"
echo "uniform_static_leafsum_split: $average7"
echo "uniform_static_sideways_fenwick: $average8"
echo "uniform_changing_leafsum: $average9"
echo "uniform_changing_leafsum_split: $average10"
echo "uniform_changing_sideways_fenwick: $average11"
echo "weibull_static_leafsum: $average12"
echo "weibull_static_leafsum_split: $average13"
echo "weibull_static_sideways_fenwick: $average14"
echo "weibull_changing_leafsum: $average15"
echo "weibull_changing_leafsum_split: $average16"
echo "weibull_changing_sideways_fenwick: $average17" 















