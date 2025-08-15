#!/bin/bash
# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]

# Script to test variations of the weighted random selections
echo "Number of tests: $1"
echo "Weightnum: $2"

touch resultsAddRemove_$2.csv
echo "sweeping_wrs, sweeping_heap, sweeping_leafsum, sweeping_leaf_sum_split, sweeping_sideways_fenwick, sweeping_weightsum, backAndForth_wrs, backAndForth_heap, backAndForth_leafsum, backAndForth_leaf_sum_split, backAndForth_sideways_fenwick, backAndForth_weightsum" > resultsAddRemove_$2.csv

# Normal distribution changing_multi weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<>"       "-DWEIGHTNUM=$2" -o test0 sweeping.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<>"             "-DWEIGHTNUM=$2" -o test1 sweeping.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<>"          "-DWEIGHTNUM=$2" -o test2 sweeping.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<>"    "-DWEIGHTNUM=$2" -o test3 sweeping.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<>" "-DWEIGHTNUM=$2" -o test4 sweeping.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=low_storage_selector<>" "-DWEIGHTNUM=$2" -o test5 sweeping.cpp
#g++ -std=c++20 -I../lib -O3 "-DWRSLIB=low_storage_selector<>" "-DWEIGHTNUM=$2" -o test7 sweeping.cpp

#g++ -std=c++20 -I/home/nfs/burnsa/Documents/GitHub/discrete_distribution/lib -O3 ... 

# Uniform distribution changing_multi weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<>"       "-DWEIGHTNUM=$2" -o test6 backAndForth.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<>"             "-DWEIGHTNUM=$2" -o test7 backAndForth.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<>"          "-DWEIGHTNUM=$2" -o test8 backAndForth.cpp
#g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<>"    "-DWEIGHTNUM=$2" -o test9 backAndForth.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<>" "-DWEIGHTNUM=$2" -o test10 backAndForth.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=low_storage_selector<>"                "-DWEIGHTNUM=$2" -o test11 backAndForth.cpp


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
#sum9=0
sum10=0
sum11=0




for ((i=1; i<=$1; i++)); do
    #echo "test 0 start"
    result0=$(./test0)
    result1=$(./test1)
    result2=$(./test2)
    result3=$(./test3)
    result4=$(./test4)
    result5=$(./test5)
    result6=$(./test6)
    result7=$(./test7)
    result8=$(./test8)
    #result9=$(./test9)
    result10=$(./test10)
    result11=$(./test11)
    
 

    echo "$result0, $result1, $result2, $result3, $result4, $result5, $result6, $result7, $result8, 0, $result10, $result11" >> resultsAddRemove_$2.csv

    sum0=$(awk "BEGIN { printf \"%.6f\", $sum0 + $result0 }")
    sum1=$(awk "BEGIN { printf \"%.6f\", $sum1 + $result1 }")
    sum2=$(awk "BEGIN { printf \"%.6f\", $sum2 + $result2 }")
    sum3=$(awk "BEGIN { printf \"%.6f\", $sum3 + $result3 }")
    sum4=$(awk "BEGIN { printf \"%.6f\", $sum4 + $result4 }")
    sum5=$(awk "BEGIN { printf \"%.6f\", $sum5 + $result5 }")
    sum6=$(awk "BEGIN { printf \"%.6f\", $sum6 + $result6 }")
    sum7=$(awk "BEGIN { printf \"%.6f\", $sum7 + $result7 }")
    sum8=$(awk "BEGIN { printf \"%.6f\", $sum8 + $result8 }")
    #sum9=$(awk "BEGIN { printf \"%.6f\", $sum9 + $result9 }")
    sum10=$(awk "BEGIN { printf \"%.6f\", $sum10 + $result10 }")
    sum11=$(awk "BEGIN { printf \"%.6f\", $sum11 + $result11 }")
    
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
#average9=$(awk "BEGIN { printf \"%.6f\", $sum9 / $1 }")
average10=$(awk "BEGIN { printf \"%.6f\", $sum10 / $1 }")
average11=$(awk "BEGIN { printf \"%.6f\", $sum11 / $1 }")



echo " " >> resultsAddRemove_$2.csv
echo "$average0, $average1, $average2, $average3, $average4, $average5, $average6, $average7, $average8, 0, $average10, $average11" >> resultsAddRemove_$2.csv
    

