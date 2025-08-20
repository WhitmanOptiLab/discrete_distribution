//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../../lib -O3 -std=c++20 "-DWRSLIB=exponential_leaf_sum_tree" "-DWEIGHTNUM=100000" -o test1 small_add_remove.cpp

#include "no_weight_storage_modifiable_heap_random_selector.hpp"
#include "leaf_sum_tree_selector.hpp"
#include "leaf_sum_tree_split.hpp"
#include "sideways_fenwick_selector.hpp"
#include "sideways_fenwick_selector_bitcast.hpp"
#include "incremental_leaf_sum_tree.hpp"
#include "exponentialAddRemove.hpp"
#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
using namespace dense::stochastic;

int main() {
  std::uniform_real_distribution<float> d(1,10); 
  std::uniform_int_distribution<int> randomIndex(0, WEIGHTNUM - 1);

  std::default_random_engine generator;
  std::vector<float> weights = {};
  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(d(generator));
  }	            

  //constructing the selector
  WRSLIB selector = WRSLIB(weights.begin(), weights.end());
  std::cout << "constructed" << std::endl;
  selector.print_tree();
  
  std::vector<size_t> counts(WEIGHTNUM + 3, 0);

  //start time
  struct timeval start, end;
  gettimeofday(&start, NULL);
  
  

  
  for(int i = 0; i < 8; i++) {
    auto weight = d(generator);
    selector.push_back(weight);
    weights.push_back(weight);
  }

  for(int i = 0; i < 5; i++) {
    selector.pop_back();
    weights.pop_back();
  }
  

  for(int i = 0; i < 1000000; i++) {
    int index = selector(generator);
    counts[index]++;
  }

  
  // end time
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << elapsedtime_sec << std::endl;

  double totalWeight = 0.0;
  double totalCount = 0.0;

  for(int i = 0; i < WEIGHTNUM + 3; i++) {
    totalWeight += weights[i];
    totalCount += counts[i];
  }


  for(int i = 0; i < WEIGHTNUM + 3; i++) {
    std::cout << "EXPECTED proportion: " << counts[i] / totalCount << ", EXPECTED proportion: " << weights[i] / totalWeight << std::endl;
  }
  
}