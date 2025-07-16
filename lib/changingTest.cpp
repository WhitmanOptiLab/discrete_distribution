#include "heap_random_selector.hpp"
#include "random_selector.hpp"
#include "modifiable_heap_random_selector.hpp"

#include <iostream>
#include <random>
#include <sys/time.h>

using namespace dense::stochastic;



int main() {
    double heapTime = 0;
    double nonHeapTime = 0;
    std::default_random_engine generator(29);
    int num = 1000000;
    int numWeights = 16;
    //for(int i = 0; i < 100; i++) {
        
         

        timeval start, end, rand;
        gettimeofday(&rand, NULL);
        
        std::vector<float> weights;
        std::weibull_distribution<float> w(0.5);
        std::normal_distribution<float> n(12, 2);
        std::uniform_real_distribution<float> d(1, 10);
        std::uniform_real_distribution<double> increment((10.0 / 11.0), (11.0 / 10.0)); // +/- 10% change in weight
        

        for(int i = 0; i < numWeights; i++) {
            weights.push_back(d(generator));
        }
        
        
        
        fast_random_selector<int> HeapSelector(weights.begin(), weights.end());
        nonuniform_int_distribution<int> NonHeapSelector(weights.begin(), weights.end());

        //heap_random_selector<int> selector(weights.begin(), weights.end());
        //fast_random_selector<int> selector(weights.begin(), weights.end());
        //std::discrete_distribution<> ind(weights.begin(), weights.end());

        //std::cout << "Constructed" << std::endl;
        double sumHeap = 0;
        double sumNonHeap = 0;
        double swapSum=0;

        //int randomIndex = (d(generator) - 1) * ((weights.size() - 1) / 9.0); //this generates a random index in the range [0, weights.size() - 1]

        
        /*
        gettimeofday(&start, NULL);
        //heap loop
        for (int i = 0; i < num; i++) {

            auto heapSelectorOutput = HeapSelector(generator);
            sumHeap += std::get<0>(heapSelectorOutput);
            //updating selected node
            //making sure that the weight does not become too large
            double weightChange = increment(generator);
            while (HeapSelector.get_weight(std::get<1>(heapSelectorOutput)) * weightChange > 25) {
                weightChange = increment(generator);
            }
            swapSum += HeapSelector.update_weight(std::get<1>(heapSelectorOutput), HeapSelector.get_weight(std::get<1>(heapSelectorOutput)) * weightChange); //weight changed by +/- 10%

            //update the weight of one other random node
            //swapSum += HeapSelector.update_weight(randomIndex, HeapSelector.get_weight(randomIndex) * (weightChange)); //weight changed by +/- 10%            
        }

        gettimeofday(&end, NULL);
        double elapsedTime = (end.tv_sec - start.tv_sec) * 1e6; // convert seconds to microseconds
        elapsedTime += (end.tv_usec - start.tv_usec);
        heapTime += elapsedTime / 1e6; // convert to seconds   
        std::cout << "HEAP elapsed time: " << (elapsedTime / 1e6) << std::endl;
        */
        

        gettimeofday(&start, NULL);
        //non-heap loop
        for(int i = 0; i < num; i++) {
            int node = NonHeapSelector(generator);
            sumNonHeap += std::floor(std::log2(node + 1)) + 1;
            double weightChange = increment(generator);
            while ((NonHeapSelector.get_weight(node + 1) * weightChange) > 25) {
                weightChange = increment(generator);
            }
            NonHeapSelector.update_weight(node + 1, NonHeapSelector.get_weight(node + 1) * weightChange); //weight changed by +/- 10%
            //update the weight of one other random node
            //NonHeapSelector.update_weight(randomIndex + 1, NonHeapSelector.get_weight(randomIndex + 1) * (weightChange));
        }

        

        gettimeofday(&end, NULL);
        double elapsedTime = (end.tv_sec - start.tv_sec) * 1e6; // convert seconds to microseconds
        elapsedTime += (end.tv_usec - start.tv_usec);
        nonHeapTime += elapsedTime / 1e6; // convert to seconds   
        std::cout << "NON-HEAP elapsed time: " << (elapsedTime / 1e6) << std::endl;

        

        double avgDepthNonHeap = sumNonHeap/num;
        double avgDepthHeap = sumHeap/num;
        double avgSwaps = swapSum/num;

        std::cout << "Average Depth of Non Heap selections is " << avgDepthNonHeap << std::endl;
        //std::cout<<"Average Depth of Heap is slections is " <<avgDepthHeap<<std::endl;
        //std::cout<< "Average Number of Swaps is "<<avgSwaps<<std::endl;
        //std::cout<<"the heap component reduces the average depth of selection by "<<avgDepthNonHeap-avgDepthHeap<<" but adds "<<avgSwaps<<" swaps on average"<<std::endl;
        
  //}
  std::cout << "TOTAL AVERAGE NON-HEAP TIME: " << nonHeapTime / 100.0 << std::endl;
  //std::cout << "TOTAL AVERAGE HEAP TIME: " << heapTime / 100.0 << std::endl;
}
