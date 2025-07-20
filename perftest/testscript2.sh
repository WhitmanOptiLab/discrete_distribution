#!/bin/bash

# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]

# Script to test variations of the weighted random selections



echo "Number of tests: $1" # first argument is num of tests
echo "Weightnum: $2" # second argument is number of weights in data structure
echo "normal_static_weightsum, normal_changing_weightsum, normal_multichange, uniform_static_weightsum, uniform_changing_weightsum, uniform_multichange, weibull_static_weightsum, weibull_changing_weightsum, weibull_multichange" > results_$2.csv

# Normal distribution static weight tests
#g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test0 normal_static.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test1 normal_static.cpp
#g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=$2" -o test2 normal_static.cpp
g++ -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test0 normal_static.cpp



# Normal distribution changing weight tests
#g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test4 normal_changing.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test5 normal_changing.cpp
g++ -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test1 normal_changing.cpp

g++ -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test2 normal_changing_multi.cpp


# Uniform distribution static weight tests
#g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test7 uniform_static.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test8 uniform_static.cpp
#g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=$2" -o test9 uniform_static.cpp
g++ -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test3 uniform_static.cpp


# Uniform distribution changing weight tests
#g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test11 uniform_changing.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test12 uniform_changing.cpp
g++ -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test4 uniform_changing.cpp

g++ -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test5 uniform_changing_multi.cpp



# Weibull distribution static weight tests
#g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test14 weibull_static.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test15 weibull_static.cpp
#g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=$2" -o test16 weibull_static.cpp
g++ -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test6 weibull_static.cpp


# Weibull distribution changing weight tests
#g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test18 weibull_changing.cpp
#g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test19 weibull_changing.cpp
g++ -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test7 weibull_changing.cpp

g++ -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test8 weibull_changing_multi.cpp



sum0=0
sum1=0
sum2=0
sum3=0
sum4=0
sum5=0
sum6=0
sum7=0
sum8=0


for ((i=1; i<=$1; i++)); do
    # Run your program and capture the output in a variable
    result0=$(./test0)
    result1=$(./test1)
    result2=$(./test2)
    result3=$(./test3)
    result4=$(./test4)
    result5=$(./test5)
    result6=$(./test6)
    result7=$(./test7)
    result8=$(./test8)
    
    

    # append test to results.csv
    echo "$result0, $result1, $result2, $result3, $result4, $result5, $result6, $result7, $result8" >> results_$2.csv

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


# Add averages to the csv
echo " " >> results.csv
echo "$average0, $average1, $average2, $average3, $average4, $average5, $average6, $average7, $average8" >> results_$2.csv

# Print the average
echo "normal_static_weightsum: $average0"
echo "normal_changing_weightsum: $average1"
echo "normal_multichange: $average2"
echo "uniform_static_weightsum: $average3"
echo "uniform_changing_weightsum: $average4"
echo "uniform_multichange: $average5"
echo "weibull_static_weightsum: $average6"
echo "weibull_changing_weightsum: $average7"
echo "weibull_multichange: $average8"




