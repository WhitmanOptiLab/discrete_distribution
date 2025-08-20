//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 normal_static.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 normal_static.cpp
//g++ -I../lib -O3 "-DWRSLIB=leaf_sum_tree<>" "-DWEIGHTNUM=10000" -o test2 normal_static.cpp

#include "sideways_fenwick.hpp"


#include "leaf_sum_tree_selector.hpp"
#include "leaf_sum_tree_split.hpp"
#include "ternary_tree_selector.hpp"
#include "exponential.hpp"
#include "low_storage.hpp"
#include "XoshiroCpp.hpp"



#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
#include "incremental_exponential.hpp"

//#include "wrslib_selector.hpp"
using namespace dense::stochastic;

int main() {
  std::normal_distribution<double> d(5,2); 
  XoshiroCpp::Xoshiro256Plus generator;
  std::vector<double> weights = {};
  size_t sum = 1;
  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(std::max<double>(0.0, d(generator)));
  }

  //start time
  auto start = std::chrono::steady_clock::now();
  WRSLIB selector(weights.begin(), weights.end());


  for (int i = 0; i < 1000000; i++) {
    sum = sum + selector(generator);
  }
  
  // end time
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << elapsed.count() << std::endl;
  if(sum == 0 ) {std::cout << "hit"; } // Added conditional so compiler must compute sum
}
