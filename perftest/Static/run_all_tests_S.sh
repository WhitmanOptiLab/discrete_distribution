#!/bin/bash

compile=(
# 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=sf_jump_bad_branches<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
#  'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=incremental_leaf_sum_tree<>"             "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
# 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=old_sideways_fenwick_selector<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
# 'g++ -std=c++20 -I../../lib -O3 "-DWRSLIB=wrsLessStorage<>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 #'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=sampling::DynamicProposalArray" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 #'g++ -std=c++20 -I../../lib -O3 -march=native "-DWRSLIB=BucketMethod" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'

 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=complete_exponential_leaf_sum_tree<size_t,double,16>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=complete_exponential_leaf_sum_tree<size_t,double,32>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=complete_exponential_leaf_sum_tree<size_t,double,64>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=complete_exponential_leaf_sum_tree<size_t,double,128>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'

 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=simd_highway_horizontal<size_t,double,16>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=simd_highway_horizontal<size_t,double,32>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=simd_highway_horizontal<size_t,double,64>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=simd_highway_horizontal<size_t,double,128>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'

 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=simd_highway_vertical<size_t,double,16>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=simd_highway_vertical<size_t,double,32>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=simd_highway_vertical<size_t,double,64>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -O3 -march=native "-DWRSLIB=simd_highway_vertical<size_t,double,128>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp'

 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -L../../vcpkg/installed/x64-linux/lib -O3 -march=native "-DWRSLIB=allsimd_highway_horizontal<size_t,double,16>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp -lhwy'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -L../../vcpkg/installed/x64-linux/lib -O3 -march=native "-DWRSLIB=allsimd_highway_horizontal<size_t,double,32>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp -lhwy'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -L../../vcpkg/installed/x64-linux/lib -O3 -march=native "-DWRSLIB=allsimd_highway_horizontal<size_t,double,64>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp -lhwy'
 'g++ -std=c++20 -I../../lib -I../../vcpkg/installed/x64-linux/include/boost/align -L../../vcpkg/installed/x64-linux/lib -O3 -march=native "-DWRSLIB=allsimd_highway_horizontal<size_t,double,128>" "-DWEIGHTNUM=${weightNums[$i]}" -o test$i uniform_static.cpp -lhwy'

)

names=(
  # 'uniform_SFjumpBadBranches'
  # 'uniform_incrementalLeafSum'
  # 'uniform_SFjump'
  # 'uniform_lowStorageWeightsum'
  #'DynamicProposalArray'
  #'BucketSampling'

  '16-aryTree_no_SIMD'
  '32-aryTree_no_SIMD'
  '64-aryTree_no_SIMD'
  '128-aryTree_no_SIMD'

  '16-aryTree_horizontal_SIMD'
  '32-aryTree_horizontal_SIMD'
  '64-aryTree_horizontal_SIMD'
  '128-aryTree_horizontal_SIMD'

  '16-aryTree_vertical_SIMD'
  '32-aryTree_vertical_SIMD'
  '64-aryTree_vertical_SIMD'
  '128-aryTree_vertical_SIMD'

  '16-aryTree_horizontal_allSIMD'
  '32-aryTree_horizontal_allSIMD'
  '64-aryTree_horizontal_allSIMD'
  '128-aryTree_horizontal_allSIMD'

)

for ((idx=0; idx<${#compile[@]}; idx++)); do
  bash testscriptStatic.sh 30 "${compile[$idx]}" "${names[$idx]}"
done