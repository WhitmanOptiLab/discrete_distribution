#!/bin/bash
# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]

# Script to test variations of the weighted random selections
echo "Number of tests: $1"
echo "Weightnum: $2"

echo "exponential_LS, LS_incremental, LS, LS_split, SF_jump, SF_intrinsics" > resultsSmall_$2.csv




# Uniform distribution changing_multi weight tests
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<size_t, double, 16>"       "-DWEIGHTNUM=$2" -o test0 small_add_remove.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=$2" -o test1 small_add_remove.cpp
#g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=leaf_sum_tree<>"          "-DWEIGHTNUM=$2" -o test2 small_add_remove.cpp
#g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=leaf_sum_tree_split<>"    "-DWEIGHTNUM=$2" -o test3 small_add_remove.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=sideways_fenwick_selector<>" "-DWEIGHTNUM=$2" -o test4 small_add_remove.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=$2" -o test5 small_add_remove.cpp


# Initialize sums
sum0=0
sum1=0
#sum2=0
#sum3=0
sum4=0
sum5=0





for ((i=1; i<=$1; i++)); do
    #echo "test 0 start"
    result0=$(./test0)
    result1=$(./test1)
    #result2=$(./test2)
    #result3=$(./test3)
    result4=$(./test4)
    result5=$(./test5)
    
    
 

    #echo "$result0, $result1, $result2, $result3, $result4, $result5" >> resultsSmall_$2.csv
    echo "$result0, $result1, 0, 0, $result4, $result5" >> resultsSmall_$2.csv

    sum0=$(awk "BEGIN { printf \"%.6f\", $sum0 + $result0 }")
    sum1=$(awk "BEGIN { printf \"%.6f\", $sum1 + $result1 }")
    #sum2=$(awk "BEGIN { printf \"%.6f\", $sum2 + $result2 }")
    #sum3=$(awk "BEGIN { printf \"%.6f\", $sum3 + $result3 }")
    sum4=$(awk "BEGIN { printf \"%.6f\", $sum4 + $result4 }")
    sum5=$(awk "BEGIN { printf \"%.6f\", $sum5 + $result5 }")
    
    
done

average0=$(awk "BEGIN { printf \"%.6f\", $sum0 / $1 }")
average1=$(awk "BEGIN { printf \"%.6f\", $sum1 / $1 }")
#average2=$(awk "BEGIN { printf \"%.6f\", $sum2 / $1 }")
#average3=$(awk "BEGIN { printf \"%.6f\", $sum3 / $1 }")
average4=$(awk "BEGIN { printf \"%.6f\", $sum4 / $1 }")
average5=$(awk "BEGIN { printf \"%.6f\", $sum5 / $1 }")



echo " " >> resultsSmall_$2.csv
#echo "$average0, $average1, $average2, $average3, $average4, $average5" >> resultsSmall_$2.csv
echo "$average0, $average1, 0, 0, $average4, $average5" >> resultsSmall_$2.csv
    