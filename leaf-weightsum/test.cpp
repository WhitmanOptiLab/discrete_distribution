#include "heap_random_selector.hpp"
#include "random_selector.hpp"
#include "modifiable_heap_random_selector.hpp"
#include <sys/time.h>

#include <iostream>
#include <random>

using namespace dense::stochastic;

double testFunct(double x) {
    return -x;
}

int main() {
    double sum = 0;
    for(int i = 0; i < 100; i++) {
        timeval start, end;
        gettimeofday(&start, NULL);

        std::default_random_engine generator(start.tv_usec);
        std::vector<float> weights = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 15};

        
        //uncomment the selector declaration you want to use
        //first 3 are from this library, last is from C++ std
        nonuniform_int_distribution<int> selector(weights.begin(), weights.end());
        nonuniform_int_distribution<int> selector1(weights.begin(), weights.end());

        //heap_random_selector<int> selector(weights.begin(), weights.end());
        //fast_random_selector<int> selector(weights.begin(), weights.end());
        //std::discrete_distribution<> selector(weights.begin(), weights.end());

        //std::cout << "Constructed" << std::endl;
        std::vector<unsigned int> counts(weights.size());

        
    
        
        for (int i = 0; i < 400000; i++) {
        int index = selector(generator);
        if (index < 0 || index >= weights.size()) {
           
        }
        for (auto c : counts) {
        //std::cout << c << ',';
        }

        //std::cout << std::endl;
        gettimeofday(&end, NULL);
        double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
        double(end.tv_usec - start.tv_usec)/1000000.0;
        //std::cout << elapsedtime_sec << std::endl;
        sum += elapsedtime_sec;
    }
    std::cout << "Average time: " << sum / 100.0 << std::endl;
  }

