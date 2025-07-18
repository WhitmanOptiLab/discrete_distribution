//#include "../lib/new_modifiable_heap_random_selector.hpp"
#include"../lib/sideways_fenwick.hpp"
#include <random>

using namespace dense::stochastic;

int main(){
    

    


    //Generating a normally distributed vector of weights
    std::default_random_engine generator(10);
    // std::normal_distribution<double> distribution(5.0,2.0);
    //std::uniform_real_distribution<double> uniformDistribution(percentWeightMin,percentWeightMax);
    std::vector<double> weights = {1.0,1.1,2.1,2.2,3.3,4.4,5.5,6.6,7.7,8.88};

    //outputFile.open("test/adjustingWeightsHeapOutput.csv");
    //outputFile<<"starting distribution, "<<std::endl;
   //outputFile<<std::endl<<std::endl;

    //WOULD START TIMING HERE
    //Constructing the heap random selector
    sideways_fenwick_selector<int> selector(weights.begin(), weights.end());

    //selecting weights
    for (int i=0;i<10000000;i++){
        int selectedItemIndex = selector(generator);
        selector.update_weight(selectedItemIndex,169);

    }
    //WOULD STOP TIMING HERE

    //outputing the ending distribution
    


}