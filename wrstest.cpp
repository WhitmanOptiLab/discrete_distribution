#include "random_selector.hpp"
#include <sys/time.h>
#include <iostream>
#include <random>

using namespace dense::stochastic;

int main() {
  // get start time
  struct timeval start, end;
  gettimeofday(&start, NULL);

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
  std::vector<float> weights = {4, 3, 2, 1}; // each weight is an item in data structure

  nonuniform_int_distribution<int> selector(weights); // sets up data structure
  std::cout << "Constructed" << std::endl; // shouldnt time this
  std::vector<unsigned int> counts(weights.size());

  for (int i = 0; i < 100000; i++) {
    int index = selector(generator); // Need to look into what selector returns and how to change item weights
    if (index < 0 || index >= weights.size()) {
      std::cout << "Error, index out of range" << std::endl;
    } else {
      counts[index]++;
    }
  }
  
  // get end time and print
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
                             double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << "Time: " << elapsedtime_sec << std::endl;
  
  for (auto c : counts) {
    std::cout << c << ',';
  }
  std::cout << std::endl;
}
