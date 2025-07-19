//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 normal_static.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 normal_static.cpp
//g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=100" -o test2 normal_static.cpp

// For testing old vs new constructor
// g++ -I../lib -O3 "-DWRSLIB=fast_random_selector<int>" "-DSELECTNUM=1000000" normal_static.cpp
// g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DSELECTNUM=1000000" normal_static.cpp
// sudo ~/WSL2-Linux-Kernel/tools/perf/perf record ./a.out
// sudo ~/WSL2-Linux-Kernel/tools/perf/perf report

#include "random_selector.hpp"
#include "modifiable_heap_random_selector.hpp"
#include "leaf_sum_tree_selector.hpp"
#include "leaf_sum_tree_split.hpp"
#include "sideways_fenwick_selector.hpp"
#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
//#include "wrslib_selector.hpp"
using namespace dense::stochastic;

#ifndef WEIGHTNUM
#define WEIGHTNUM 1000000
#endif

#ifndef SELECTNUM
#define SELECTNUM 1000000
#endif

int main() {
  std::normal_distribution<float> d(5,2); 
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
<<<<<<< Updated upstream
<<<<<<< Updated upstream
  struct timeval start, end;
  gettimeofday(&start, NULL);
  WRSLIB selector(weights.begin(), weights.end()); 
  
  for (int i = 0; i < SELECTNUM; i++) {
=======
=======
>>>>>>> Stashed changes
  WRSLIB selector(weights.begin(), weights.end());
  auto start = std::chrono::steady_clock::now();



  for (int i = 0; i < 1000000; i++) {
>>>>>>> Stashed changes
    sum = sum + selector(generator);
  }
  
  // end time
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << elapsed.count() << std::endl;
  if(sum == 0 ) {std::cout << "hit"; } // Added conditional so compiler must compute sum
}
