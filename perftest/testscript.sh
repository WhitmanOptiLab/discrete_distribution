#!/bin/bash

# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]

# Script to test variations of the weighted random selections
echo "Number of tests: $1" # first argument is num of tests
<<<<<<< Updated upstream
echo "Number of weights: $2" # second argument is number of weights in data structure
echo "normal_static_wrs, normal_static_heap, normal_changing_wrs, normal_changing_heap, weibull_static_wrs, weibull_static_heap, weibull_changing_wrs, weibull_changing_heap" > results_$2.csv

# # Normal distribution static weight tests
# g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DSELECTNUM=$2" -o test0 normal_static.cpp
# g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DSELECTNUM=$2" -o test1 normal_static.cpp

# # Normal distribution changing weight tests
# g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DSELECTNUM=$2" -o test2 normal_changing.cpp
# g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DSELECTNUM=$2" -o test3 normal_changing.cpp

# # # Weibull distribution static weight tests
# g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DSELECTNUM=$2" -o test4 weibull_static.cpp
# g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DSELECTNUM=$2" -o test5 weibull_static.cpp
# # g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DSELECTNUM=$2" -o test12 weibull_static.cpp

# # # Weibull distribution changing weight tests
# g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DSELECTNUM=$2" -o test6 weibull_changing.cpp
# g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DSELECTNUM=$2" -o test7 weibull_changing.cpp
=======
echo "Weightnum: $2" # second argument is number of weights in data structure
echo "normal_static_wrs, normal_static_heap, normal_static_discrete, normal_static_fenwick, normal_static_leafsum, normal_changing_wrs, normal_changing_heap, uniform_static_wrs, uniform_static_heap, uniform_static_discrete, uniform_changing_wrs, uniform_changing_heap, weibull_static_wrs, weibull_static_heap, weibull_static_discrete, weibull_changing_wrs, weibull_changing_heap" > results_$2.csv
>>>>>>> Stashed changes

# Normal distribution static weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test0 normal_static.cpp
g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test1 normal_static.cpp
<<<<<<< Updated upstream
=======
g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=$2" -o test2 normal_static.cpp
g++ -I../fenwick -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test3 normal_static.cpp
g++ -I../leaf-weightsum -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test4 normal_static.cpp



>>>>>>> Stashed changes

# Normal distribution changing weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test2 normal_changing.cpp
g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test3 normal_changing.cpp

<<<<<<< Updated upstream
# # Weibull distribution static weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test4 weibull_static.cpp
g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test5 weibull_static.cpp
# g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DSELECTNUM=$2" -o test12 weibull_static.cpp
=======

# Uniform distribution static weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test5 uniform_static.cpp
g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test6 uniform_static.cpp
g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=$2" -o test7 uniform_static.cpp
>>>>>>> Stashed changes

# # Weibull distribution changing weight tests
g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test6 weibull_changing.cpp
g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DWEIGHTNUM=$2" -o test7 weibull_changing.cpp

sum0=0
sum1=0
sum2=0
sum3=0
sum4=0
sum5=0
sum6=0
sum7=0
# sum8=0
# sum9=0
# sum10=0
# sum11=0
# sum12=0
# sum13=0
# sum14=0

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
    # result8=$(./test8)
    # result9=$(./test9)
    # result10=$(./test10)
    # result11=$(./test11)
    # result12=$(./test12)
    # result13=$(./test13)
    # result14=$(./test14)

    # append test to results.csv
    # echo "$result0, $result1, $result2, $result3, $result4, $result5, $result6, $result7, $result8, $result9, $result10, $result11, $result12, $result13, $result14" >> results_$2.csv
    printf "%.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f\n" $result0 $result1 $result2 $result3 $result4 $result5 $result6 $result7 >> results_$2.csv

    # Add the result to the sum
    sum0=$(awk "BEGIN { printf \"%.6f\", $sum0 + $result0 }")
    sum1=$(awk "BEGIN { printf \"%.6f\", $sum1 + $result1 }")
    sum2=$(awk "BEGIN { printf \"%.6f\", $sum2 + $result2 }")
    sum3=$(awk "BEGIN { printf \"%.6f\", $sum3 + $result3 }")
    sum4=$(awk "BEGIN { printf \"%.6f\", $sum4 + $result4 }")
    sum5=$(awk "BEGIN { printf \"%.6f\", $sum5 + $result5 }")
    sum6=$(awk "BEGIN { printf \"%.6f\", $sum6 + $result6 }")
    sum7=$(awk "BEGIN { printf \"%.6f\", $sum7 + $result7 }")
    # sum8=$(awk "BEGIN { printf \"%.6f\", $sum8 + $result8 }")
    # sum9=$(awk "BEGIN { printf \"%.6f\", $sum9 + $result9 }")
    # sum10=$(awk "BEGIN { printf \"%.6f\", $sum10 + $result10 }")
    # sum11=$(awk "BEGIN { printf \"%.6f\", $sum11 + $result11 }")
    # sum12=$(awk "BEGIN { printf \"%.6f\", $sum12 + $result12 }")
    # sum13=$(awk "BEGIN { printf \"%.6f\", $sum13 + $result13 }")
    # sum14=$(awk "BEGIN { printf \"%.6f\", $sum14 + $result14 }")
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
# average8=$(awk "BEGIN { printf \"%.6f\", $sum8 / $1 }")
# average9=$(awk "BEGIN { printf \"%.6f\", $sum9 / $1 }")
# average10=$(awk "BEGIN { printf \"%.6f\", $sum10 / $1 }")
# average11=$(awk "BEGIN { printf \"%.6f\", $sum11 / $1 }")
# average12=$(awk "BEGIN { printf \"%.6f\", $sum12 / $1 }")
# average13=$(awk "BEGIN { printf \"%.6f\", $sum13 / $1 }")
# average14=$(awk "BEGIN { printf \"%.6f\", $sum14 / $1 }")

# echo "$average0, $average1, $average2, $average3, $average4, $average5, $average6, $average7, $average8, $average9, $average10, $average11, $average12, $average13 ,$average14" >> results_$2.csv
# echo "normal_static_wrs, normal_static_heap, normal_changing_wrs, normal_changing_heap, weibull_static_wrs, weibull_static_heap, weibull_changing_wrs, weibull_changing_heap" > results.csv
printf "%d, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f\n" $2 $average0 $average1 $average2 $average3 $average4 $average5 $average6 $average7 >> results.csv

# Print the average
echo "normal_static_wrs: $average0"
echo "normal_static_heap: $average1"
echo "normal_static_discrete: $average2"
echo "normal_changing_wrs: $average3"
# echo "normal_changing_heap: $average4"
# echo "uniform_static_wrs: $average5"
# echo "uniform_static_heap: $average6"
# echo "uniform_static_discrete: $average7"
# echo "uniform_changing_wrs: $average8"
# echo "uniform_changing_heap: $average9"
# echo "weibull_static_wrs: $average10"
echo "weibull_static_heap: $average4"
echo "weibull_static_discrete: $average5"
echo "weibull_changing_wrs: $average6"
echo "weibull_changing_heap: $average7" 
