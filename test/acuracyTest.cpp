#include "lib/no_weight_storage_modifiable_heap_random_selector.hpp"
//#include "lib/modifiable_heap_random_selector.hpp"
#include <iostream>
#include <fstream>
#include <random>

using namespace dense::stochastic;

int main(){
    int numWeights = 30;
    int numOperations=100;
    double percentWeightMin=.8;
    double percentWeightMax=1.2;

    std::ofstream outputFile;


    //Generating a normally distributed vector of weights
    std::default_random_engine generator(10);
    std::normal_distribution<double> distribution(5.0,2.0);
    std::uniform_real_distribution<double> uniformDistribution(percentWeightMin,percentWeightMax);
    std::vector<double> weights;

    outputFile.open("test/adjustingWeightsHeapOutput.csv");
    outputFile<<"starting distribution, "<<std::endl;
    for (int i=0;i<numWeights;i++){
        //double num = distribution(generator);
        weights.push_back(distribution(generator));

        outputFile<<weights[i]<<", "; //outputing the inditial weights
    }
    outputFile<<std::endl<<std::endl;

    //WOULD START TIMING HERE
    //Constructing the heap random selector
    low_storage_selector<int> selector(weights.begin(), weights.end());

    // //selecting weights
    // for (int i=0;i<numOperations;i++){
    //     int selectedItemIndex = selector(generator);
    //     selector.update_weight(selectedItemIndex,selector.get_weight(selectedItemIndex)*uniformDistribution(generator));  

    // }
    //WOULD STOP TIMING HERE

    //outputing the ending distribution
    outputFile<<"ending distribution, "<<std::endl;
    for (int i=0; i<numWeights;i++){
        std::cout<<"items selected index "<<selector(generator)<<std::endl;
    }
    std::cout<<"getting ending distribution";
    for (int i=0;i<numWeights;i++){
        outputFile << selector.get_weight(i)<<", ";
    }  
    outputFile.close();











}