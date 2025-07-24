#!/bin/bash
# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]

# Script to test variations of the weighted random selections
echo "Number of tests: $1"
echo "Weightnum: $2"

touch results_C5_$2.csv
echo "normal_changing_multi_wrs, normal_changing_multi_heap normal_changing_multi_fenwick, normal_changing_multi_leafsum, normal_changing_multi_leaf_sum_split, normal_changing_multi_sideways_fenwick, normal_changing_multi_weightsum, uniform_changing_multi_wrs, uniform_changing_multi_heap, uniform_changing_multi_fenwick, uniform_changing_multi_leafsum, uniform_changing_multi_leaf_sum_split, uniform_changing_multi_sideways_fenwick, uniform_changing_multi_weightsum, weibull_changing_multi_wrs, weibull_changing_multi_heap, weibull_changing_multi_fenwick, weibull_changing_multi_leafsum, weibull_changing_multi_leaf_sum_split, weibull_changing_multi_sideways_fenwick, weibull_changing_multi_weightsum" >> results_C5_$2.csv

# Normal distribution changing_multi weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>"       "-DWEIGHTNUM=$2" -o test0 normal_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<int>"             "-DWEIGHTNUM=$2" -o test1 normal_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<int>"          "-DWEIGHTNUM=$2" -o test4 normal_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<int>"    "-DWEIGHTNUM=$2" -o test5 normal_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test6 normal_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test7 normal_changing_multi.cpp
#g++ -std=c++20 -I../lib -O3 "-DWRSLIB=low_storage_selector<int>" "-DWEIGHTNUM=$2" -o test7 normal_changing_multi.cpp

#g++ -std=c++20 -I/home/nfs/burnsa/Documents/GitHub/discrete_distribution/lib -O3 ... 

# Uniform distribution changing_multi weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>"       "-DWEIGHTNUM=$2" -o test8 uniform_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<int>"             "-DWEIGHTNUM=$2" -o test9 uniform_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<int>"          "-DWEIGHTNUM=$2" -o test12 uniform_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<int>"    "-DWEIGHTNUM=$2" -o test13 uniform_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test14 uniform_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=low_storage_selector<int>"                "-DWEIGHTNUM=$2" -o test15 uniform_changing_multi.cpp

# Weibull distribution changing_multi weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>"       "-DWEIGHTNUM=$2" -o test16 weibull_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<int>"             "-DWEIGHTNUM=$2" -o test17 weibull_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree<int>"          "-DWEIGHTNUM=$2" -o test20 weibull_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<int>"    "-DWEIGHTNUM=$2" -o test21 weibull_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test22 weibull_changing_multi.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=low_storage_selector<int>"                "-DWEIGHTNUM=$2" -o test23 weibull_changing_multi.cpp

# Initialize sums
sum0=0
sum1=0
sum3=0
sum4=0
sum5=0
sum6=0
sum7=0
sum8=0
sum9=0
sum11=0
sum12=0
sum13=0
sum14=0
sum15=0
sum16=0
sum17=0
sum19=0
sum20=0
sum21=0
sum22=0
sum23=0


for ((i=1; i<=$1; i++)); do
    result0=$(./test0)
    result1=$(./test1)
    result3=$(./test3)
    result4=$(./test4)
    result5=$(./test5)
    result6=$(./test6)
    result7=$(./test7)
    result8=$(./test8)
    result9=$(./test9)
    result11=$(./test11)
    result12=$(./test12)
    result13=$(./test13)
    result14=$(./test14)
    result15=$(./test15)
    result16=$(./test16)
    result17=$(./test17)
    result19=$(./test19)
    result20=$(./test20)
    result21=$(./test21)
    result22=$(./test22)
    result23=$(./test23)

    echo "$result0, $result1, $result3, $result4, $result5, $result6, $result7, $result8, $result9, $result11, $result12, $result13, $result14, $result15, $result16, $result17, $result19, $result20, $result21, $result22, $result23" >> results_C5_$2.csv

    sum0=$(awk "BEGIN { printf \"%.6f\", $sum0 + $result0 }")
    sum1=$(awk "BEGIN { printf \"%.6f\", $sum1 + $result1 }")
    sum3=$(awk "BEGIN { printf \"%.6f\", $sum3 + $result3 }")
    sum4=$(awk "BEGIN { printf \"%.6f\", $sum4 + $result4 }")
    sum5=$(awk "BEGIN { printf \"%.6f\", $sum5 + $result5 }")
    sum6=$(awk "BEGIN { printf \"%.6f\", $sum6 + $result6 }")
    sum7=$(awk "BEGIN { printf \"%.6f\", $sum7 + $result7 }")
    sum8=$(awk "BEGIN { printf \"%.6f\", $sum8 + $result8 }")
    sum9=$(awk "BEGIN { printf \"%.6f\", $sum9 + $result9 }")
    sum11=$(awk "BEGIN { printf \"%.6f\", $sum11 + $result11 }")
    sum12=$(awk "BEGIN { printf \"%.6f\", $sum12 + $result12 }")
    sum13=$(awk "BEGIN { printf \"%.6f\", $sum13 + $result13 }")
    sum14=$(awk "BEGIN { printf \"%.6f\", $sum14 + $result14 }")
    sum15=$(awk "BEGIN { printf \"%.6f\", $sum15 + $result15 }")
    sum16=$(awk "BEGIN { printf \"%.6f\", $sum16 + $result16 }")
    sum17=$(awk "BEGIN { printf \"%.6f\", $sum17 + $result17 }")
    sum19=$(awk "BEGIN { printf \"%.6f\", $sum19 + $result19 }")
    sum20=$(awk "BEGIN { printf \"%.6f\", $sum20 + $result20 }")
    sum21=$(awk "BEGIN { printf \"%.6f\", $sum21 + $result21 }")
    sum22=$(awk "BEGIN { printf \"%.6f\", $sum22 + $result22 }")
    sum23=$(awk "BEGIN { printf \"%.6f\", $sum23 + $result23 }")
done

average0=$(awk "BEGIN { printf \"%.6f\", $sum0 / $1 }")
average1=$(awk "BEGIN { printf \"%.6f\", $sum1 / $1 }")
average3=$(awk "BEGIN { printf \"%.6f\", $sum3 / $1 }")
average4=$(awk "BEGIN { printf \"%.6f\", $sum4 / $1 }")
average5=$(awk "BEGIN { printf \"%.6f\", $sum5 / $1 }")
average6=$(awk "BEGIN { printf \"%.6f\", $sum6 / $1 }")
average7=$(awk "BEGIN { printf \"%.6f\", $sum7 / $1 }")
average8=$(awk "BEGIN { printf \"%.6f\", $sum8 / $1 }")
average9=$(awk "BEGIN { printf \"%.6f\", $sum9 / $1 }")
average11=$(awk "BEGIN { printf \"%.6f\", $sum11 / $1 }")
average12=$(awk "BEGIN { printf \"%.6f\", $sum12 / $1 }")
average13=$(awk "BEGIN { printf \"%.6f\", $sum13 / $1 }")
average14=$(awk "BEGIN { printf \"%.6f\", $sum14 / $1 }")
average15=$(awk "BEGIN { printf \"%.6f\", $sum15 / $1 }")
average16=$(awk "BEGIN { printf \"%.6f\", $sum16 / $1 }")
average17=$(awk "BEGIN { printf \"%.6f\", $sum17 / $1 }")
average19=$(awk "BEGIN { printf \"%.6f\", $sum19 / $1 }")
average20=$(awk "BEGIN { printf \"%.6f\", $sum20 / $1 }")
average21=$(awk "BEGIN { printf \"%.6f\", $sum21 / $1 }")
average22=$(awk "BEGIN { printf \"%.6f\", $sum22 / $1 }")
average23=$(awk "BEGIN { printf \"%.6f\", $sum23 / $1 }")

echo " " >> results_C5_$2.csv
echo "$average0, $average1, $average3, $average4, $average5, $average6, $average7, $average8, $average9, $average11, $average12, $average13, $average14, $average15, $average16, $average17, $average19, $average20, $average21, $average22, $average23" >> results_C5_$2.csv
    

