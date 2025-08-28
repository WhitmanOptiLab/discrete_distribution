#!/bin/bash

compile=(
 'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=wrsLessStorage<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=exponential_leaf_sum_tree<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=incremental_exponential<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=exponential_leaf_sum_tree_alex<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'

'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=BucketMethod" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=sampling::DynamicProposalArray" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'

 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=wrsLessStorage<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_exponential<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=exponential_leaf_sum_tree_alex<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'

'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=BucketMethod" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=sampling::DynamicProposalArray" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'

)

names=(
  'uniform_SIMD_incrementalLeafSum'
  'uniform_SIMD_SFjump'
  'uniform_SIMD_lowStorageWeightsum'
  'uniform_SIMD_kary'
  'uniform_SIMD_incrementalKary'
  'uniform_SIMD_kary_alexVersion'
  'uniform_SIMD_bucketMethod'
  'uniform_SIMD_dynamicProposalArray'

    'uniform_incrementalLeafSum'
  'uniform_SFjump'
  'uniform_lowStorageWeightsum'
  'uniform_kary'
  'uniform_incrementalKary'
  'uniform_kary_alexVersion'
  'uniform_bucketMethod'
  'uniform_dynamicProposalArray'
)

for ((idx=0; idx<${#compile[@]}; idx++)); do
  bash testscriptStatic.sh 10 "${compile[$idx]}" "${names[$idx]}"
done