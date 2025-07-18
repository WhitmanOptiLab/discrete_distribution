//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 weibull_static.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 weibull_static.cpp
//g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=100" -o test2 weibull_static.cpp

#include "sideways_fenwick.hpp"


#include "leaf_sum_tree_selector.hpp"
#include "leaf_sum_tree_split.hpp"

#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace dense::stochastic;

int main() {
  std::weibull_distribution<float> d(0.5); 
  std::default_random_engine generator;
  std::vector<float> weights = {};
  int sum = 1;
  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(d(generator));
  }	      

  float minweight = *std::min_element(weights.begin(), weights.end());
  for(int i = 0; i < WEIGHTNUM; i++){
    weights[i] -= minweight;
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
