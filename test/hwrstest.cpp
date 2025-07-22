#include"../lib/modifiable_heap_random_selector.hpp"
#include "../lib/random_selector.hpp"


#include <iostream>
#include <random>

using namespace dense::stochastic;

int main() {
  std::default_random_engine generator(5);
  std::vector<float> weights = {1, 2, 3, 4};

  fast_random_selector<> selector(weights.begin(), weights.end());
  std::cout << "Constructed" << std::endl;
  std::vector<unsigned int> counts(10);
  selector.push_entry(10);
  selector.push_entry(10);
  selector.remove_last_entry();
    
    for (int i = 0; i < 100000; i++) {
      int index = selector(generator);
      //if(index == 0) {std::cout << "4!" << std::endl;}
      if (index < 0 || index >= selector.max()) {
        std::cout << "Error, index out of range" << std::endl;
      } else {
        counts[index]++;
      }
    }
    for (auto c : counts) {
      std::cout << c * 2 << ',';
    }
    std::cout << std::endl;
    std::cout << std::endl;

    
  }

