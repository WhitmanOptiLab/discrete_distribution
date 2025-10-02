#!/bin/bash

compile=(
# 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=sf_jump_bad_branches<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
#  'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
# 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
# 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=wrsLessStorage<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
 'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=sampling::DynamicProposalArray" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=complete_exponential_leaf_sum_tree<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=BucketMethod" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'


)

names=(
  # 'uniform_SFjumpBadBranches'
  # 'uniform_incrementalLeafSum'
  # 'uniform_SFjump'
  # 'uniform_lowStorageWeightsum'
  'DynamicProposalArray'
  '16-aryTree'
  'BucketSampling'
)

for ((idx=0; idx<${#compile[@]}; idx++)); do
  bash testscriptStatic.sh 10 "${compile[$idx]}" "${names[$idx]}"
done