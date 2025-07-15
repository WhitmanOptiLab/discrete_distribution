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
        int numWeights = 220;
        std::default_random_engine generator(1);
        std::vector<float> weights;
        std::uniform_real_distribution<float> d(1, 10);
        

        for(int i = 0; i < numWeights; i++) {
            weights.push_back(d(generator));
        }

        
        //uncomment the selector declaration you want to use
        //first 3 are from this library, last is from C++ std
        fast_random_selector<int> HeapSelector(weights.begin(), weights.end());
        nonuniform_int_distribution<int> NonHeapSelector(weights.begin(), weights.end());

        //heap_random_selector<int> selector(weights.begin(), weights.end());
        //fast_random_selector<int> selector(weights.begin(), weights.end());
        //std::discrete_distribution<> ind(weights.begin(), weights.end());

        //std::cout << "Constructed" << std::endl;
        std::vector<unsigned int> counts(weights.size());
        double sumHeap = 0;
        double sumNonHeap = 0;
        double swapSum=0;

        
      
        for (int i = 0; i < num; i++) {
            
            int node = NonHeapSelector(generator);
            
            sumNonHeap += std::floor(std::log2(node + 1)) + 1;
            
            auto heapSelectorOutput = HeapSelector(generator);
            sumHeap += std::get<0>(heapSelectorOutput);
            double weightChange = ((d(generator) - 5.5) / 45.0) + 1; //this generates a random number (0.9, 1.1) to change weight by +/- 10%
            swapSum += HeapSelector.update_weight(std::get<1>(heapSelectorOutput), HeapSelector.get_weight(std::get<1>(heapSelectorOutput)) * weightChange); //weight changed by +/- 10%
            //update the weight of the node selected by the non-heap selector
            
            
            NonHeapSelector.update_weight(node + 1, NonHeapSelector.get_weight(node + 1) * weightChange);
            std::cout << i << " " << node << " " << std::endl;
        }
        double avgDepthNonHeap = sumNonHeap/num;
        double avgDepthHeap = sumHeap/num;
        double avgSwaps = swapSum/num;

        std::cout << "Average Depth of Non Heap selections is " << avgDepthNonHeap << std::endl;
        std::cout<<"Average Depth of Heap is slections is " <<avgDepthHeap<<std::endl;
        std::cout<< "Average Number of Swaps is "<<avgSwaps<<std::endl;
        std::cout<<"the heap component reduces the average depth of selection by "<<avgDepthNonHeap-avgDepthHeap<<" but adds "<<avgSwaps<<" swaps on average"<<std::endl;
        
  //}
}
