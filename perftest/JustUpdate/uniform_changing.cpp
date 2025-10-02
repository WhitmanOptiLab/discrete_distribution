//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 uniform_changing.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 uniform_changing.cpp

// #include "random_selector.hpp"
// #include "modifiable_heap_random_selector.hpp"
// #include "no_weight_storage_modifiable_heap_random_selector.hpp"
// #include "leaf_sum_tree_selector.hpp"
// #include "leaf_sum_tree_split.hpp"
// #include "sideways_fenwick_selector.hpp"
// #include "old_sideways_fenwick_selector.hpp"
// #include "sideways_fenwick_selector_bitcast.hpp"
// #include "incremental_leaf_sum_tree.hpp"
// #include "bucket_alias.hpp"
// #include "wrsLessStorage.hpp"
// #include "exponential_leafsum.hpp"
// #include "SF_jump_bad_branches.hpp"



//#include "XoshiroCpp.hpp"


// #include <sys/time.h>
// #include <iostream>
// #include <random>
// #include <vector>
// #include <algorithm>
#include "sideways_fenwick.hpp"
#include "dynamicProposalArray.hpp"
#include "bucket_alias.hpp"
#include "leaf_sum_tree_selector.hpp"
#include "leaf_sum_tree_split.hpp"
#include "ternary_tree_selector.hpp"
#include "exponential.hpp"
#include "XoshiroCpp.hpp"
#include "incremental_exponential.hpp"
#include "exponentialAddRemove.hpp"


using namespace dense::stochastic;

int main() {
  std::uniform_real_distribution<double> d(1,10);
  //std::default_random_engine generator;
  std::vector<double> weights = {};
  std::uniform_real_distribution<double> d2(0.99, 1.01);
  std::uniform_int_distribution<int> randomIndex(0,WEIGHTNUM-1);
  XoshiroCpp::Xoshiro256Plus generator;

  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(d(generator));
  }

  //start time
  auto start = std::chrono::steady_clock::now();
  WRSLIB selector(weights.begin(), weights.end());
  
  for (int i = 0; i < 1000000; i++) {
        selector.update_weight(randomIndex(generator), std::max<double>(0.00001, d(generator)));

    // int index = selector(generator);
    // selector.update_weight(index, std::max<double>(0.00001,d(generator)));
  }
  
  //std::cout << selector.size() << std::endl;
  
  // end time
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << elapsed.count() << std::endl;
  
}

 