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
  

  // TODO:
  // Set up array of weights with 1k-10k values
  // Create WRS
  // Start timing
  // A few thousand times:
  //   Random select item
  //   Change items weight up or down
  // Stop timing
  // Record runtime
  
  std::default_random_engine generator;
  std::vector<float> weights = {}; // each weight is an item in data structure
  
  for(int i = 0; i < 10000; i++){
    weights.push_back(random(1,10));
  }	      
    
  nonuniform_int_distribution<int> selector(weights); // sets up data structure
  std::cout << "Constructed" << std::endl; // shouldnt time this
  std::vector<unsigned int> counts(weights.size());
  // get start time
  struct timeval start, end;
  gettimeofday(&start, NULL);
  for (int i = 0; i < 100000; i++) {
    int index = selector(generator); // Need to look into what selector returns and how to change item weights
    std::cout<<index<<std::endl;
  }
  
  // get end time and print
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
                             double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << "Time: " << elapsedtime_sec << std::endl;
  
}
