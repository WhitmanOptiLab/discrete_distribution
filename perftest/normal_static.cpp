#include "random_selector.hpp"
#include "heap_random_selector.hpp"
#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
using namespace dense::stochastic;

int main() {
  std::normal_distribution<float> d(5,2); 
  std::default_random_engine generator;
  std::vector<float> weights = {};
  int sum = 0;
  
  for(int i = 0; i < 10000000; i++){
    weights.push_back(d(generator));
  }	      

  float minweight = *std::min_element(weights.begin(), weights.end());

  for(int i = 0; i < 10000000; i++){
    weights[i] -= minweight;
  }	      

  //start time
  struct timeval start, end;
  gettimeofday(&start, NULL);
  //WRSLIB is intentionally undeclared. It is intended to be specified at compilation as shown below
  //
  //g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" -o test0 normal_static.cpp
  //g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" -o test1 normal_static.cpp
  //g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" -o test2 normal_static.cpp

  WRSLIB selector(weights.begin(), weights.end()); 
  for (int i = 0; i < 100000; i++) {
    sum += selector(generator);
  }
  
  // end time
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << elapsedtime_sec << std::endl;
  if(sum == 0 ) {std::cout << "hit"; } // Added conditional so compiler must compute sum
}
