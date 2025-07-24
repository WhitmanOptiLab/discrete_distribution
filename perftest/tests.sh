#!/bin/bash

# HOW TO USE:
# Ex:
# bash testscript.sh [num tests] [wightnum]

# Script to test variations of the weighted random selections

echo "Number of tests: $1"
echo "Weightnum: $2"

echo "normal_static_wrs, normal_static_heap, normal_static_discrete, normal_static_fenwick, normal_static_leafsum, normal_static_leaf_sum_split, normal_static_sideways_fenwick, normal_changing_wrs, normal_changing_heap, normal_changing_fenwick, normal_changing_leafsum, normal_changing_leaf_sum_split, normal_changing_sideways_fenwick, uniform_static_wrs, uniform_static_heap, uniform_static_discrete, uniform_static_fenwick, uniform_static_leafsum, uniform_static_leaf_sum_split, uniform_static_sideways_fenwick, uniform_changing_wrs, uniform_changing_heap, uniform_changing_fenwick, uniform_changing_leafsum, uniform_changing_leaf_sum_split, uniform_changing_sideways_fenwick, weibull_static_wrs, weibull_static_heap, weibull_static_discrete, weibull_static_fenwick, weibull_static_leafsum, weibull_static_leaf_sum_split, weibull_static_sideways_fenwick, weibull_changing_wrs, weibull_changing_heap, weibull_changing_fenwick, weibull_changing_leafsum, weibull_changing_leaf_sum_split, weibull_changing_sideways_fenwick" > results_$2.csv

# Normal distribution static weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>"       "-DWEIGHTNUM=$2" -o test0 normal_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<int>"             "-DWEIGHTNUM=$2" -o test1 normal_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>"       "-DWEIGHTNUM=$2" -o test2 normal_static.cpp
g++ -std=c++20 -I../fenwick -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test3 normal_static.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree<int>"          "-DWEIGHTNUM=$2" -o test4 normal_static.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree_split<int>"    "-DWEIGHTNUM=$2" -o test27 normal_static.cpp
g++ -std=c++20 -I../sideways-fenwick -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test33 normal_static.cpp

# Normal distribution changing weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>"       "-DWEIGHTNUM=$2" -o test5 normal_changing.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<int>"             "-DWEIGHTNUM=$2" -o test6 normal_changing.cpp
g++ -std=c++20 -I../fenwick -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test7 normal_changing.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree<int>"          "-DWEIGHTNUM=$2" -o test8 normal_changing.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree_split<int>"    "-DWEIGHTNUM=$2" -o test28 normal_changing.cpp
g++ -std=c++20 -I../sideways-fenwick -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test34 normal_changing.cpp

# Uniform distribution static weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>"       "-DWEIGHTNUM=$2" -o test9 uniform_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<int>"             "-DWEIGHTNUM=$2" -o test10 uniform_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>"       "-DWEIGHTNUM=$2" -o test11 uniform_static.cpp
g++ -std=c++20 -I../fenwick -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test12 uniform_static.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree<int>"          "-DWEIGHTNUM=$2" -o test13 uniform_static.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree_split<int>"    "-DWEIGHTNUM=$2" -o test29 uniform_static.cpp
g++ -std=c++20 -I../sideways-fenwick -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test35 uniform_static.cpp

# Uniform distribution changing weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>"       "-DWEIGHTNUM=$2" -o test14 uniform_changing.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<int>"             "-DWEIGHTNUM=$2" -o test15 uniform_changing.cpp
g++ -std=c++20 -I../fenwick -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test16 uniform_changing.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree<int>"          "-DWEIGHTNUM=$2" -o test17 uniform_changing.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree_split<int>"    "-DWEIGHTNUM=$2" -o test30 uniform_changing.cpp
g++ -std=c++20 -I../sideways-fenwick -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test36 uniform_changing.cpp

# Weibull distribution static weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>"       "-DWEIGHTNUM=$2" -o test18 weibull_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<int>"             "-DWEIGHTNUM=$2" -o test19 weibull_static.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>"       "-DWEIGHTNUM=$2" -o test20 weibull_static.cpp
g++ -std=c++20 -I../fenwick -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test21 weibull_static.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree<int>"          "-DWEIGHTNUM=$2" -o test22 weibull_static.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree_split<int>"    "-DWEIGHTNUM=$2" -o test31 weibull_static.cpp
g++ -std=c++20 -I../sideways-fenwick -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test37 weibull_static.cpp

# Weibull distribution changing weight tests
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>"       "-DWEIGHTNUM=$2" -o test23 weibull_changing.cpp
g++ -std=c++20 -I../lib -O3 "-DWRSLIB=fast_random_selector<int>"             "-DWEIGHTNUM=$2" -o test24 weibull_changing.cpp
g++ -std=c++20 -I../fenwick -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=$2" -o test25 weibull_changing.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree<int>"          "-DWEIGHTNUM=$2" -o test26 weibull_changing.cpp
g++ -std=c++20 -I../leaf-weightsum -O3 "-DWRSLIB=leaf_sum_tree_split<int>"    "-DWEIGHTNUM=$2" -o test32 weibull_changing.cpp
g++ -std=c++20 -I../sideways-fenwick -O3 "-DWRSLIB=sideways_fenwick_selector<int>" "-DWEIGHTNUM=$2" -o test38 weibull_changing.cpp

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
sum9=0
sum10=0
sum11=0
sum12=0
sum13=0
sum14=0
sum15=0
sum16=0
sum17=0
sum18=0
sum19=0
sum20=0
sum21=0
sum22=0
sum23=0
sum24=0
sum25=0
sum26=0
sum27=0
sum28=0
sum29=0
sum30=0
sum31=0
sum32=0
sum33=0
sum34=0
sum35=0
sum36=0
sum37=0
sum38=0

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
    result9=$(./test9)
    result10=$(./test10)
    result11=$(./test11)
    result12=$(./test12)
    result13=$(./test13)
    result14=$(./test14)
    result15=$(./test15)
    result16=$(./test16)
    result17=$(./test17)
    result18=$(./test18)
    result19=$(./test19)
    result20=$(./test20)
    result21=$(./test21)
    result22=$(./test22)
    result23=$(./test23)
    result24=$(./test24)
    result25=$(./test25)
    result26=$(./test26)
    result27=$(./test27)
    result28=$(./test28)
    result29=$(./test29)
    result30=$(./test30)
    result31=$(./test31)
    result32=$(./test32)
    result33=$(./test33)
    result34=$(./test34)
    result35=$(./test35)
    result36=$(./test36)
    result37=$(./test37)
    result38=$(./test38)

    echo "$result0, $result1, $result2, $result3, $result4, $result27, $result33, $result5, $result6, $result7, $result8, $result28, $result34, $result9, $result10, $result11, $result12, $result13, $result29, $result35, $result14, $result15, $result16, $result17, $result30, $result36, $result18, $result19, $result20, $result21, $result22, $result31, $result37, $result23, $result24, $result25, $result26, $result32, $result38" >> results_$2.csv

    sum0=$(awk "BEGIN { printf \"%.6f\", $sum0 + $result0 }")
    sum1=$(awk "BEGIN { printf \"%.6f\", $sum1 + $result1 }")
    sum2=$(awk "BEGIN { printf \"%.6f\", $sum2 + $result2 }")
    sum3=$(awk "BEGIN { printf \"%.6f\", $sum3 + $result3 }")
    sum4=$(awk "BEGIN { printf \"%.6f\", $sum4 + $result4 }")
    sum27=$(awk "BEGIN { printf \"%.6f\", $sum27 + $result27 }")
    sum33=$(awk "BEGIN { printf \"%.6f\", $sum33 + $result33 }")
    sum5=$(awk "BEGIN { printf \"%.6f\", $sum5 + $result5 }")
    sum6=$(awk "BEGIN { printf \"%.6f\", $sum6 + $result6 }")
    sum7=$(awk "BEGIN { printf \"%.6f\", $sum7 + $result7 }")
    sum8=$(awk "BEGIN { printf \"%.6f\", $sum8 + $result8 }")
    sum28=$(awk "BEGIN { printf \"%.6f\", $sum28 + $result28 }")
    sum34=$(awk "BEGIN { printf \"%.6f\", $sum34 + $result34 }")
    sum9=$(awk "BEGIN { printf \"%.6f\", $sum9 + $result9 }")
    sum10=$(awk "BEGIN { printf \"%.6f\", $sum10 + $result10 }")
    sum11=$(awk "BEGIN { printf \"%.6f\", $sum11 + $result11 }")
    sum12=$(awk "BEGIN { printf \"%.6f\", $sum12 + $result12 }")
    sum13=$(awk "BEGIN { printf \"%.6f\", $sum13 + $result13 }")
    sum29=$(awk "BEGIN { printf \"%.6f\", $sum29 + $result29 }")
    sum35=$(awk "BEGIN { printf \"%.6f\", $sum35 + $result35 }")
    sum14=$(awk "BEGIN { printf \"%.6f\", $sum14 + $result14 }")
    sum15=$(awk "BEGIN { printf \"%.6f\", $sum15 + $result15 }")
    sum16=$(awk "BEGIN { printf \"%.6f\", $sum16 + $result16 }")
    sum17=$(awk "BEGIN { printf \"%.6f\", $sum17 + $result17 }")
    sum30=$(awk "BEGIN { printf \"%.6f\", $sum30 + $result30 }")
    sum36=$(awk "BEGIN { printf \"%.6f\", $sum36 + $result36 }")
    sum18=$(awk "BEGIN { printf \"%.6f\", $sum18 + $result18 }")
    sum19=$(awk "BEGIN { printf \"%.6f\", $sum19 + $result19 }")
    sum20=$(awk "BEGIN { printf \"%.6f\", $sum20 + $result20 }")
    sum21=$(awk "BEGIN { printf \"%.6f\", $sum21 + $result21 }")
    sum22=$(awk "BEGIN { printf \"%.6f\", $sum22 + $result22 }")
    sum31=$(awk "BEGIN { printf \"%.6f\", $sum31 + $result31 }")
    sum37=$(awk "BEGIN { printf \"%.6f\", $sum37 + $result37 }")
    sum23=$(awk "BEGIN { printf \"%.6f\", $sum23 + $result23 }")
    sum24=$(awk "BEGIN { printf \"%.6f\", $sum24 + $result24 }")
    sum25=$(awk "BEGIN { printf \"%.6f\", $sum25 + $result25 }")
    sum26=$(awk "BEGIN { printf \"%.6f\", $sum26 + $result26 }")
    sum32=$(awk "BEGIN { printf \"%.6f\", $sum32 + $result32 }")
    sum38=$(awk "BEGIN { printf \"%.6f\", $sum38 + $result38 }")
done

average0=$(awk "BEGIN { printf \"%.6f\", $sum0 / $1 }")
average1=$(awk "BEGIN { printf \"%.6f\", $sum1 / $1 }")
average2=$(awk "BEGIN { printf \"%.6f\", $sum2 / $1 }")
average3=$(awk "BEGIN { printf \"%.6f\", $sum3 / $1 }")
average4=$(awk "BEGIN { printf \"%.6f\", $sum4 / $1 }")
average27=$(awk "BEGIN { printf \"%.6f\", $sum27 / $1 }")
average33=$(awk "BEGIN { printf \"%.6f\", $sum33 / $1 }")
average5=$(awk "BEGIN { printf \"%.6f\", $sum5 / $1 }")
average6=$(awk "BEGIN { printf \"%.6f\", $sum6 / $1 }")
average7=$(awk "BEGIN { printf \"%.6f\", $sum7 / $1 }")
average8=$(awk "BEGIN { printf \"%.6f\", $sum8 / $1 }")
average28=$(awk "BEGIN { printf \"%.6f\", $sum28 / $1 }")
average34=$(awk "BEGIN { printf \"%.6f\", $sum34 / $1 }")
average9=$(awk "BEGIN { printf \"%.6f\", $sum9 / $1 }")
average10=$(awk "BEGIN { printf \"%.6f\", $sum10 / $1 }")
average11=$(awk "BEGIN { printf \"%.6f\", $sum11 / $1 }")
average12=$(awk "BEGIN { printf \"%.6f\", $sum12 / $1 }")
average13=$(awk "BEGIN { printf \"%.6f\", $sum13 / $1 }")
average29=$(awk "BEGIN { printf \"%.6f\", $sum29 / $1 }")
average35=$(awk "BEGIN { printf \"%.6f\", $sum35 / $1 }")
average14=$(awk "BEGIN { printf \"%.6f\", $sum14 / $1 }")
average15=$(awk "BEGIN { printf \"%.6f\", $sum15 / $1 }")
average16=$(awk "BEGIN { printf \"%.6f\", $sum16 / $1 }")
average17=$(awk "BEGIN { printf \"%.6f\", $sum17 / $1 }")
average30=$(awk "BEGIN { printf \"%.6f\", $sum30 / $1 }")
average36=$(awk "BEGIN { printf \"%.6f\", $sum36 / $1 }")
average18=$(awk "BEGIN { printf \"%.6f\", $sum18 / $1 }")
average19=$(awk "BEGIN { printf \"%.6f\", $sum19 / $1 }")
average20=$(awk "BEGIN { printf \"%.6f\", $sum20 / $1 }")
average21=$(awk "BEGIN { printf \"%.6f\", $sum21 / $1 }")
average22=$(awk "BEGIN { printf \"%.6f\", $sum22 / $1 }")
average31=$(awk "BEGIN { printf \"%.6f\", $sum31 / $1 }")
average37=$(awk "BEGIN { printf \"%.6f\", $sum37 / $1 }")
average23=$(awk "BEGIN { printf \"%.6f\", $sum23 / $1 }")
average24=$(awk "BEGIN { printf \"%.6f\", $sum24 / $1 }")
average25=$(awk "BEGIN { printf \"%.6f\", $sum25 / $1 }")
average26=$(awk "BEGIN { printf \"%.6f\", $sum26 / $1 }")
average32=$(awk "BEGIN { printf \"%.6f\", $sum32 / $1 }")
average38=$(awk "BEGIN { printf \"%.6f\", $sum38 / $1 }")

echo " " >> results_$2.csv
echo "$average0, $average1, $average2, $average3, $average4, $average27, $average33, $average5, $average6, $average7, $average8, $average28, $average34, $average9, $average10, $average11, $average12, $average13, $average29, $average35, $average14, $average15, $average16, $average17, $average30, $average36, $average18, $average19, $average20, $average21, $average22, $average31, $average37, $average23, $average24, $average25, $average26, $average32, $average38" >> results_$2.csv