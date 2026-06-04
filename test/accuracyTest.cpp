//#include "../lib/new_modifiable_heap_random_selector.hpp"
//#include"../lib/no_weight_storage_modifiable_heap_random_selector.hpp"
//#include "../lib/sideways_fenwick_selector.hpp"
//#include "../lib/bucket_alias.hpp"
//#include "../lib/wrsLessStorage.hpp"
//#include "../lib/old_sideways_fenwick_selector.hpp"
#include "../lib/exponentialAddRemove.hpp"
//#include "../lib/simdGoogleHighway_vertical.hpp"
#include "../lib/simdGoogleHighway_horitzonal_ALLSIMD.hpp"
#include "../lib/simdGoogleHighway_horizontal.hpp"
#include <iostream>
#include <fstream>
#include <random>

//using namespace dense::stochastic;

int main(){
    int numWeights = 30;
    int numOperations=10;
    double percentWeightMin=.8;
    double percentWeightMax=1.2;

    std::ofstream outputFile;
 

    //Generating a normally distributed vector of weights
    std::default_random_engine generator(10);
    std::normal_distribution<double> distribution(5.0,2.0);
    std::uniform_real_distribution<float> d(1,10);
    std::uniform_real_distribution<double> uniformDistribution(percentWeightMin,percentWeightMax);
    std::vector<double> weights;

    outputFile.open("adjustingWeightsHeapOutput.csv");
    for (int i=0;i<numWeights;i++){
        weights.push_back(i);

    }

    std::cout<<"about to construct"<<std::endl;
    dense::stochastic::complete_exponential_leaf_sum_tree<size_t,float,16> selector(weights.begin(), weights.end());
    std::cout<<"constructed"<<std::endl;

    selector.printTree();

    // for(int i=numWeights; i<=numWeights+100;i++){
    //     selector.push_back(i);
    //     if(i==numWeights+1){
    //         std::cout<<"added some weights to tree   i="<<i<<std::endl;
    //         selector.printTree();
    //     }
    // }
    // std::cout<<"added weights to tree";
    // selector.printTree();


    // for(int i=numWeights+100; i>=numWeights;i--){
    //     if(i==numWeights+1){
    //         std::cout<<"removed some weights from the tree  i="<<i<<std::endl;
    //         selector.printTree();
    //     }
    //     selector.pop_back();
    // }

    // std::cout<<"removed weights - should have original tree";
    // selector.printTree();



    for(int i=0;i<1000000;i++){
        outputFile<<selector(generator)<<std::endl;
    }

    



    outputFile.close();











}