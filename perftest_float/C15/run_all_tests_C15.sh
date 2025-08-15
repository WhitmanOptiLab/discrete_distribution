#!/bin/bash

compile=(
#'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=sideways_fenwick_selector<>"       "-DWEIGHTNUM=${weightNums[$i]}" -o test$i normal_changing.cpp'
#'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=sideways_fenwick_selector_bitcast<>"       "-DWEIGHTNUM=${weightNums[$i]}" -o test$i normal_changing.cpp'
#'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=${weightNums[$i]}" -o test$i normal_changing.cpp'
#'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=leaf_sum_tree<>"          "-DWEIGHTNUM=${weightNums[$i]}" -o test$i normal_changing.cpp'
#'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=leaf_sum_tree_split<>"    "-DWEIGHTNUM=${weightNums[$i]}" -o test$i normal_changing.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
#'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=nonuniform_int_distribution" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i normal_changing.cpp'
#'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=wrsLessStorage<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
)

names=(
  # 'normal_SFintrinsics'
  # 'normal_SFbitcast'
  #'normal_incrementalLeafSum'
  # 'normal_leafsum'
  # 'normal_leafsum_split'
   'uniform_SFjump'
  # 'normal_weightsum'
  # 'uniform_lowStorageWeightsum'
)

for ((idx=0; idx<${#compile[@]}; idx++)); do
  #./testscriptChanging1 10 "${compile[$idx]}" "${names[$idx]}"
  bash testscriptChanging15.sh 10 "${compile[$idx]}" "${names[$idx]}"
done