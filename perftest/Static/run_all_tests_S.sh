#!/bin/bash

compile=(
# 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=sf_jump_bad_branches<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=wrsLessStorage<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'


)

names=(
  # 'uniform_SFjumpBadBranches'
  'uniform_incrementalLeafSum'
  'uniform_SFjump'
  'uniform_lowStorageWeightsum'
  'uniform_kary'
)

for ((idx=0; idx<${#compile[@]}; idx++)); do
  #./testscriptstatic1 10 "${compile[$idx]}" "${names[$idx]}"
  bash testscriptStatic.sh 10 "${compile[$idx]}" "${names[$idx]}"
done