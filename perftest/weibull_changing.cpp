//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 weibull_changing.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 weibull_changing.cpp

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

double generate_weibull_between(double shape, double scale, double min, double max, std::default_random_engine& gen) {
  std::weibull_distribution<double> dist(shape, scale);
  double x;
  do {
    x = dist(gen);
  } while (x < min || x > max);
  return x;
}

int main() {
  std::weibull_distribution<float> d(0.5); 
  std::default_random_engine generator;
  std::vector<float> weights = {};
  std::uniform_real_distribution<float> d2(0.99, 1.01);
  std::uniform_int_distribution<int> randomIndex(0,WEIGHTNUM-1);



  
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
    int index = selector(generator);
    selector.update_weight(index, std::max<float>(0.0, d(generator)));
    for(int i = 0;i<4;i++){
      selector.update_weight(randomIndex(generator),std::max<float>(0.0, d(generator)));
    }
  }
  
  // end time
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << elapsed.count() << std::endl;
  
}
