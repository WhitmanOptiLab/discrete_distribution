//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 uniform_changing.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 uniform_changing.cpp

//#include "sideways_fenwick.hpp"

//
//#include "leaf_sum_tree_selector.hpp"
//#include "leaf_sum_tree_split.hpp"
//#include "ternary_tree_selector.hpp"
////#include "exponential.hpp"
//#include "low_storage.hpp"
//#include "old_leaf_sum.hpp"
#include "../lib/XoshiroCpp.hpp"
//#include "incremental_exponential.hpp"
//#include "exponentialAddRemove.hpp"
#include "../lib/alexVersionBetterMemory.hpp"

#include "../lib/exponentialAddRemove.hpp"
#include "../lib/avx2_kary.hpp"

#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace dense::stochastic;

int main() {
  std::uniform_real_distribution<double> d(1,10);
  //std::default_random_engine generator;
  std::vector<double> weights = {};
  //std::uniform_real_distribution<double> d2(0.99, 1.01);
  std::uniform_int_distribution<int> randomIndex(0,WEIGHTNUM-1);
  XoshiroCpp::Xoshiro256Plus generator;

  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(d(generator));
  }

  //start time
  WRSLIB selector(weights.begin(), weights.end());
    auto start = std::chrono::steady_clock::now();



size_t sum = 0;
  for (int i = 0; i < 1000000; i++) {
    sum += selector(generator);
    //selector.update_weight(index, std::max<double>(0.0, d(generator)));
//    for(int i = 0;i<49;i++){
//      selector.update_weight(randomIndex(generator),std::max<double>(0.0, d(generator)));
//    }
  }

  //std::cout << selector.size() << std::endl;
  
  // end time
  if (sum == 0) {
    std::cout << "NO! GOD PLEASE NO! NO!!!" << std::endl;
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << elapsed.count() << std::endl;
  
}

 
