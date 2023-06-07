#include "random_selector.hpp"
#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
using namespace dense::stochastic;

int main() {
  std::normal_distribution<float> d(5,2); 
  std::default_random_engine generator(42);
  std::vector<float> weights = {};
  
  for(int i = 0; i < 100; i++){
    weights.push_back(d(generator));
  }	      

  float minweight = *std::min_element(weights.begin(), weights.end());
  for(int i = 0; i < 100; i++){
    weights[i] -= minweight;
  }	      

  //start time
  struct timeval start, end;
  gettimeofday(&start, NULL);
  nonuniform_int_distribution<int> selector(weights.begin(), weights.end()); 
  for (int i = 0; i < 100000; i++) {
    int index = selector(generator);
    selector.update_weight(index, std::max<float>(0.0, d(generator)-minweight));
  }
  
  // end time
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << "Time: " << elapsedtime_sec << std::endl;
  
}
