#include "random_selector.hpp"
#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>

using namespace dense::stochastic;

int random(int low, int high)
{
    return low + rand() % (high - low + 1);
}

int main() {
  std::default_random_engine generator;
  std::vector<float> weights = {};
  int sum = 0;
  
  for(int i = 0; i < 10000000; i++){
    weights.push_back(random(1,10));
  }	      

  //start time
  struct timeval start, end;
  gettimeofday(&start, NULL);
  nonuniform_int_distribution<int> selector(weights); 
  for (int i = 0; i < 100000; i++) {
    sum += selector(generator);
  }
  std::cout<<sum<<std::endl;
  
  // end time
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << "Time: " << elapsedtime_sec << std::endl;
  
}
