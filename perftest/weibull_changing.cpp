//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 weibull_changing.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 weibull_changing.cpp

#include "random_selector.hpp"
#include "modifiable_heap_random_selector.hpp"
#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
using namespace dense::stochastic;

int main() {
  std::weibull_distribution<float> d(0.5); 
  std::default_random_engine generator;
  std::vector<float> weights = {};
  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(d(generator));
  }	      

  float minweight = *std::min_element(weights.begin(), weights.end());
  for(int i = 0; i < WEIGHTNUM; i++){
    weights[i] -= minweight;
  }	      

  //start time
  struct timeval start, end;
  WRSLIB selector(weights.begin(), weights.end());
  gettimeofday(&start, NULL);
  
  for (int i = 0; i < 1000000; i++) {
    int index = selector(generator);
    selector.update_weight(index, std::max<float>(0.0, d(generator)-minweight));
  }
  
  // end time
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << elapsedtime_sec << std::endl;
  
}
