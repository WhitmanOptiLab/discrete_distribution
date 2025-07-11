//#include "../lib/new_modifiable_heap_random_selector.hpp"
#include"../lib/random_selector.hpp"
#include <iostream>
#include <fstream>
#include <random>

using namespace dense::stochastic;

int main(){
    int numWeights = 100;
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
    nonuniform_int_distribution<int> selector(weights.begin(), weights.end());

    //selecting weights
    for (int i=0;i<numOperations;i++){
        int selectedItemIndex = selector(generator);
        selector.update_weight(selectedItemIndex,distribution(generator));

    }
    //WOULD STOP TIMING HERE

    //outputing the ending distribution
    outputFile<<"ending distribution, "<<std::endl;
    
    for (int i=0;i<selector.get_leaf_start()+weights.size();i++){
        outputFile << selector.weightsum_of(i)<<", ";
    }  
    outputFile.close();











}