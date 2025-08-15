#!/bin/bash
# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]

# Script to test variations of the weighted random selections
echo "Number of tests: $1"
echo "Weightnum: $2"


echo "normal_changing_multi_incremental_LS, normal_changing_multi_sf_jump, normal_wrs_less_storage, uniform_changing_multi_incremental_LS, uniform_changing_multi_sf_jump, uniform_wrs_less_storage, weibull_changing_multi_incremental_LS, weibull_changing_multi_sf_jump, weibull_wrs_less_storage" > resultsC5_$2.csv



# Normal distribution changing_multi weight tests
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=$2" -o test0 normal_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=$2" -o test1 normal_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=wrsLessStorage<>"                "-DWEIGHTNUM=$2" -o test2 normal_changing_multi.cpp

g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=$2" -o test3 uniform_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=$2" -o test4 uniform_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=wrsLessStorage<>"                "-DWEIGHTNUM=$2" -o test5 uniform_changing_multi.cpp

g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=$2" -o test6 weibull_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=$2" -o test7 weibull_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=wrsLessStorage<>"                "-DWEIGHTNUM=$2" -o test8 weibull_changing_multi.cpp







# Initialize sums
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
    result0=$(./test0)
    result1=$(./test1)
    result2=$(./test2)
    result3=$(./test3)
    result4=$(./test4)
    result5=$(./test5)
    result6=$(./test6)
    result7=$(./test7)
    result8=$(./test8)
   

    echo "$result0, $result1, $result2, $result3, $result4, $result5, $result6, $result7, $result8" >> resultsC5_$2.csv

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

average0=$(awk "BEGIN { printf \"%.6f\", $sum0 / $1 }")
average1=$(awk "BEGIN { printf \"%.6f\", $sum1 / $1 }")
average2=$(awk "BEGIN { printf \"%.6f\", $sum2 / $1 }")
average3=$(awk "BEGIN { printf \"%.6f\", $sum3 / $1 }")
average4=$(awk "BEGIN { printf \"%.6f\", $sum4 / $1 }")
average5=$(awk "BEGIN { printf \"%.6f\", $sum5 / $1 }")
average6=$(awk "BEGIN { printf \"%.6f\", $sum6 / $1 }")
average7=$(awk "BEGIN { printf \"%.6f\", $sum7 / $1 }")
average8=$(awk "BEGIN { printf \"%.6f\", $sum8 / $1 }")



echo " " >> resultsC5_$2.csv
echo "$average0, $average1, $average2, $average3, $average4, $average5, $average6, $average7, $average8" >> resultsC5_$2.csv
echo "Test completed. Results saved in resultsC5_$2.csv"

