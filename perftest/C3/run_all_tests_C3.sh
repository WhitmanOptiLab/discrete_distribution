#!/bin/bash

compile=(
 'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=wrsLessStorage<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=exponential_leaf_sum_tree<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=incremental_exponential<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=exponential_leaf_sum_tree_alex<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'

'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=BucketMethod" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=sampling::DynamicProposalArray" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'
# 'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=sampling::DynamicProposalArrayAlex" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_changing.cpp'



)

names=(
  'uniform_incrementalLeafSum'
  'uniform_SFjump'
  'uniform_lowStorageWeightsum'
  'uniform_kary'
  'uniform_incrementalKary'
  'uniform_kary_alexVersion'
  'uniform_bucketMethod'
  'uniform_dynamicProposalArray'
  # 'uniform_dynamicProposalArrayAlex'
)

for ((idx=0; idx<${#compile[@]}; idx++)); do
  bash testscriptChanging3.sh 10 "${compile[$idx]}" "${names[$idx]}"
done