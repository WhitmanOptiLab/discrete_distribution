#include "heap_random_selector.hpp"
#include "random_selector.hpp"
#include "modifiable_heap_random_selector.hpp"

#include <iostream>
#include <random>
#include <sys/time.h>

using namespace dense::stochastic;

double testFunct(double x) {
    return -x;
}

int main() {
    
    //for(int i = 110; i < 200; i++) {
        int num = 1000000;
        int numWeights = 11; 

        timeval start, end;

        std::default_random_engine generator(1);
        std::vector<float> weights;
        std::weibull_distribution<float> w(0.5);
        std::normal_distribution<float> n(5, 2);
        std::uniform_real_distribution<float> d(1, 10);
        

        for(int i = 0; i < numWeights; i++) {
            weights.push_back(d(generator));
        }
        for(int i = 0; i < weights.size(); i++) {
            std::cout << weights[i] << " ";
        }
        std::cout << std::endl;
        
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

        
        gettimeofday(&start, NULL);

        for (int i = 0; i < num; i++) {
            
            //int node = NonHeapSelector(generator);
            
            //sumNonHeap += std::floor(std::log2(node + 1)) + 1;
            
            auto heapSelectorOutput = HeapSelector(generator);
            sumHeap += std::get<0>(heapSelectorOutput);
            double weightChange = ((d(generator) - 5.5) / 45.0) + 1; //this generates a random number (0.9, 1.1) to change weight by +/- 10%
            //updating selected node
            swapSum += HeapSelector.update_weight(std::get<1>(heapSelectorOutput), HeapSelector.get_weight(std::get<1>(heapSelectorOutput)) * weightChange); //weight changed by +/- 10%
            //NonHeapSelector.update_weight(node + 1, NonHeapSelector.get_weight(node + 1) * weightChange);

            //update the weight of one other random node
            /*
            int randomIndex = (d(generator) - 1) * ((weights.size() - 1) / 9.0); //this generates a random index in the range [0, weights.size() - 1]
            swapSum += HeapSelector.update_weight(randomIndex, HeapSelector.get_weight(randomIndex) * (weightChange)); //weight changed by +/- 10%
            NonHeapSelector.update_weight(randomIndex + 1, NonHeapSelector.get_weight(randomIndex + 1) * (weightChange));
            */

            
        }

        gettimeofday(&end, NULL);

        double elapsedTime = (end.tv_sec - start.tv_sec) * 1e6; // convert seconds to microseconds
        elapsedTime += (end.tv_usec - start.tv_usec);   
        std::cout << "Elapsed time: " << (elapsedTime / 1e6) << std::endl;

        double avgDepthNonHeap = sumNonHeap/num;
        double avgDepthHeap = sumHeap/num;
        double avgSwaps = swapSum/num;

        std::cout << "Average Depth of Non Heap selections is " << avgDepthNonHeap << std::endl;
        std::cout<<"Average Depth of Heap is slections is " <<avgDepthHeap<<std::endl;
        std::cout<< "Average Number of Swaps is "<<avgSwaps<<std::endl;
        std::cout<<"the heap component reduces the average depth of selection by "<<avgDepthNonHeap-avgDepthHeap<<" but adds "<<avgSwaps<<" swaps on average"<<std::endl;
        
  //}
}
