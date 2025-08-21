#!/bin/bash

compile=(
#'g++ -std=c++20 -I../../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=${weightNums[$i]}" -o test$i sweeping.cpp'
#'g++ -std=c++20 -I../../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i sweeping.cpp'
#'g++ -std=c++20 -I../../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i sweeping.cpp'
#'g++ -std=c++20 -I../../../lib -O3 "-DWRSLIB=incremental_exponential<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i sweeping.cpp'
'g++ -std=c++20 -I../../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree_alex<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i sweeping.cpp'




)

names=(
  # 'sweeping_incrementalLeafSum'
  # 'sweeping_SFjump'
  # 'sweeping_kary'
  #'sweeping_incrementalKary'
  sweeping_AlexVersionKary
)

for ((idx=0; idx<${#compile[@]}; idx++)); do
  #./testscriptChanging1 10 "${compile[$idx]}" "${names[$idx]}"
  bash testscriptSweeping.sh 10 "${compile[$idx]}" "${names[$idx]}"
done