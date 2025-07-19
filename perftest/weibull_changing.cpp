//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 weibull_changing.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 weibull_changing.cpp

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
using namespace dense::stochastic;

<<<<<<< Updated upstream
<<<<<<< Updated upstream
#ifndef WEIGHTNUM
#define WEIGHTNUM 1000000
#endif

#ifndef SELECTNUM
#define SELECTNUM 1000000
#endif
=======
=======
>>>>>>> Stashed changes
double generate_weibull_between(double shape, double scale, double min, double max, std::default_random_engine& gen) {
  std::weibull_distribution<double> dist(shape, scale);
  double x;
  do {
    x = dist(gen);
  } while (x < min || x > max);
  return x;
}
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

int main() {
  std::weibull_distribution<float> d(0.5); 
  std::default_random_engine generator;
  std::vector<float> weights = {};
  std::uniform_real_distribution<float> d2(0.99, 1.01);


  
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
=======
  WRSLIB selector(weights.begin(), weights.end());
  auto start = std::chrono::steady_clock::now();

>>>>>>> Stashed changes
=======
  WRSLIB selector(weights.begin(), weights.end());
  auto start = std::chrono::steady_clock::now();

>>>>>>> Stashed changes
  
  for (int i = 0; i < SELECTNUM; i++) {
    int index = selector(generator);
    selector.update_weight(index, std::max<float>(0.0, d(generator)));
  }
  
  // end time
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << elapsed.count() << std::endl;
  
}
