#include "heap_random_selector.hpp"
#include "random_selector.hpp"
#include "modifiable_heap_random_selector.hpp"

#include <iostream>
#include <random>

using namespace dense::stochastic;

double testFunct(double x) {
    return -x;
}

int main() {
    
    //for(int i = 0; i < 100; i++) {
        
        int num = 1000000;
        std::default_random_engine generator(10);
        std::vector<float> weights;
        std::uniform_real_distribution<float> d(1, 10);
        for(int i = 0; i < num; i++) {
            weights.push_back(d(generator));
        }

        
        //uncomment the selector declaration you want to use
        //first 3 are from this library, last is from C++ std
        //fast_random_selector<int> selector(weights.begin(), weights.end());
        nonuniform_int_distribution<int> selector(weights.begin(), weights.end());

        //heap_random_selector<int> selector(weights.begin(), weights.end());
        //fast_random_selector<int> selector(weights.begin(), weights.end());
        //std::discrete_distribution<> selector(weights.begin(), weights.end());

        //std::cout << "Constructed" << std::endl;
        std::vector<unsigned int> counts(weights.size());
        double sum = 0;
        
    
        
        for (int i = 0; i < num; i++) {
            int depth = selector(generator);
            sum += std::floor(std::log2(depth)) + 1;
            //sum += depth;
        }
        double avgDepth = sum / num;

        std::cout << avgDepth << std::endl;
        
  //}
}
