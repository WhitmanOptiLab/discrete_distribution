#!/bin/bash

compile=(
#  'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
# 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
# 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=wrsLessStorage<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
#'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
# 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_exponential<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree_alex<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'





)

names=(
  # 'uniform_incrementalLeafSum'
  # 'uniform_SFjump'
  # 'uniform_lowStorageWeightsum'
  # 'uniform_kary'
  # 'uniform_incrementalKary'
  'uniform_kary_alexVersion'
)

for ((idx=0; idx<${#compile[@]}; idx++)); do
  #./testscriptChanging1 10 "${compile[$idx]}" "${names[$idx]}"
  bash testscriptJustUpdate.sh 10 "${compile[$idx]}" "${names[$idx]}"
done