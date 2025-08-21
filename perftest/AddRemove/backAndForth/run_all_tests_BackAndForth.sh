#!/bin/bash

compile=(
#'g++ -std=c++20 -I../../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=${weightNums[$i]}" -o test$i backAndForth.cpp'
#'g++ -std=c++20 -I../../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i backAndForth.cpp'
#'g++ -std=c++20 -I../../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i backAndForth.cpp'
#'g++ -std=c++20 -I../../../lib -O3 "-DWRSLIB=incremental_exponential<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i backAndForth.cpp'
'g++ -std=c++20 -I../../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree_alex<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i backAndForth.cpp'




)

names=(
  # 'backAndForth_incrementalLeafSum'
  # 'backAndForth_SFjump'
  # 'backAndForth_kary'
  #'backAndForth_incrementalKary'
  backAndForth_AlexVersionKary
)

for ((idx=0; idx<${#compile[@]}; idx++)); do
  #./testscriptChanging1 10 "${compile[$idx]}" "${names[$idx]}"
  bash testscriptBackAndForth.sh 10 "${compile[$idx]}" "${names[$idx]}"
done