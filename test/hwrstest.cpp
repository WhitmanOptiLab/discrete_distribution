#include"../lib/modifiable_heap_random_selector.hpp"
#include "../lib/random_selector.hpp"
#include "../lib/sideways_fenwick_selector.hpp"
#include "../lib/no_weight_storage_modifiable_heap_random_selector.hpp"


#include <iostream>
#include <random>

using namespace dense::stochastic;

int main() {
  std::default_random_engine generator(5);
  std::vector<float> weights = {1, 2, 3, 4, 90};

  low_storage_selector<> selector(weights.begin(), weights.end());
  std::cout << "Constructed" << std::endl;
  
  selector.update_weight(4, 10);
  



   std::vector<unsigned int> counts(10);


    
    for (int i = 0; i < 100000; i++) {
      int index = selector(generator);
      //if(index == 0) {std::cout << "4!" << std::endl;}
      
        counts[index]++;
      
    }
    for (auto c : counts) {
      std::cout << c << ',';
    }
    std::cout << std::endl;
    std::cout << std::endl;

    
  }

