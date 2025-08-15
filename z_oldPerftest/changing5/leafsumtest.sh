#!/bin/bash
# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]

# Script to test variations of the weighted random selections
echo "Number of tests: $1"
echo "Weightnum: $2"


echo "uniform_4_child_leafsum, uniform_8_child_leafsum, uniform_16_child_leafsum, uniform_32_child_leafsum, uniform_64_child_leafsum, uniform_128_child_leafsum, uniform_incremental_leafsum, uniform_leafsum" > resultsC5_$2.csv



# uniform distribution changing_multi weight tests
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>"       "-DWEIGHTNUM=$2" "-DCHILDNUM=4" -o test0 uniform_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>"       "-DWEIGHTNUM=$2" "-DCHILDNUM=8" -o test1 uniform_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>"       "-DWEIGHTNUM=$2" "-DCHILDNUM=16" -o test2 uniform_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>"       "-DWEIGHTNUM=$2" "-DCHILDNUM=32" -o test3 uniform_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>"       "-DWEIGHTNUM=$2" "-DCHILDNUM=64" -o test4 uniform_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>"       "-DWEIGHTNUM=$2" "-DCHILDNUM=128" -o test5 uniform_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=$2" "-DCHILDNUM=0" -o test6 uniform_changing_multi.cpp
g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=leaf_sum_tree<>"          "-DWEIGHTNUM=$2" "-DCHILDNUM=0" -o test7 uniform_changing_multi.cpp









# Initialize sums
sum0=0
sum1=0
sum2=0
sum3=0
sum4=0
sum5=0
sum6=0
sum7=0




for ((i=1; i<=$1; i++)); do
    result0=$(./test0)
    result1=$(./test1)
    result2=$(./test2)
    result3=$(./test3)
    result4=$(./test4)
    result5=$(./test5)
    result6=$(./test6)
    result7=$(./test7)
     

    echo "$result0, $result1, $result2, $result3, $result4, $result5, $result6, $result7" >> resultsC5_$2.csv

    sum0=$(awk "BEGIN { printf \"%.6f\", $sum0 + $result0 }")
    sum1=$(awk "BEGIN { printf \"%.6f\", $sum1 + $result1 }")
    sum2=$(awk "BEGIN { printf \"%.6f\", $sum2 + $result2 }")
    sum3=$(awk "BEGIN { printf \"%.6f\", $sum3 + $result3 }")
    sum4=$(awk "BEGIN { printf \"%.6f\", $sum4 + $result4 }")
    sum5=$(awk "BEGIN { printf \"%.6f\", $sum5 + $result5 }")
    sum6=$(awk "BEGIN { printf \"%.6f\", $sum6 + $result6 }")
    sum7=$(awk "BEGIN { printf \"%.6f\", $sum7 + $result7 }")
done

average0=$(awk "BEGIN { printf \"%.6f\", $sum0 / $1 }")
average1=$(awk "BEGIN { printf \"%.6f\", $sum1 / $1 }")
average2=$(awk "BEGIN { printf \"%.6f\", $sum2 / $1 }")
average3=$(awk "BEGIN { printf \"%.6f\", $sum3 / $1 }")
average4=$(awk "BEGIN { printf \"%.6f\", $sum4 / $1 }")
average5=$(awk "BEGIN { printf \"%.6f\", $sum5 / $1 }")
average6=$(awk "BEGIN { printf \"%.6f\", $sum6 / $1 }")
average7=$(awk "BEGIN { printf \"%.6f\", $sum7 / $1 }")



echo " " >> resultsC5_$2.csv
echo "$average0, $average1, $average2, $average3, $average4, $average5, $average6, $average7" >> resultsC5_$2.csv
echo "Test completed. Results saved in resultsC5_$2.csv"

