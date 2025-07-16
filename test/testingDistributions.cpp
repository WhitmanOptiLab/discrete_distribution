//#include "../lib/new_modifiable_heap_random_selector.hpp"
//#include"../lib/modifiable_heap_random_selector"
#include"../lib/sideways_fenwick_selector.hpp"
#include <iostream>
#include <fstream>
#include <random>

using namespace dense::stochastic;

int main(){
    int numWeights = 9;
    int numOperations=10000;
    double percentWeightMin=.8;
    double percentWeightMax=1.2;

    std::ofstream outputFile;


    //Generating a normally distributed vector of weights
    std::default_random_engine generator(10);
    std::normal_distribution<double> distribution(5.0,2.0);
    std::uniform_real_distribution<double> uniformDistribution(percentWeightMin,percentWeightMax);
    std::vector<double> weights;

    outputFile.open("test/adjustingWeightsHeapOutput.csv");
    //outputFile<<"starting distribution, "<<std::endl;
    for (int i=0;i<numWeights;i++){
        //double num = distribution(generator);
        weights.push_back(/*distribution(generator)*/i+1);

        //outputFile<<weights[i]<<", "; //outputing the inditial weights
    }
    //outputFile<<std::endl<<std::endl;

    //WOULD START TIMING HERE
    //Constructing the heap random selector
    sideways_fenwick_selector<int> selector(weights.begin(), weights.end());

    // for(int i=0;i<numWeights;i++){
    //     selector.update_weight(i,selector.get_weight(i)*2);
    // }
    // //selector.update_weight(1,10);

    // std::cout<<std::endl<<"getting weights all elements should be doubled"<<std::endl;
    // for(int i=0;i<numWeights;i++){
    //     std::cout<<selector.get_weight(i)<<", ";
    // }
    // std::cout<<std::endl;

    

    //selecting weights
    for (int i=0;i<numOperations;i++){
        outputFile<<selector(generator)<<std::endl;

    }
    // //WOULD STOP TIMING HERE

    // //outputing the ending distribution
    // outputFile<<"ending distribution, "<<std::endl;
    
    // for (int i=0;i<numWeights;i++){
    //     outputFile << selector.get_weight(i)<<", ";
    // }  
    outputFile.close();
}
