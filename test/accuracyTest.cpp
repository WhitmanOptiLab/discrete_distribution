//#include "../lib/new_modifiable_heap_random_selector.hpp"
#include"../fenwick/random_selector.hpp"
#include <iostream>
#include <fstream>
#include <random>

using namespace dense::stochastic;

int main(){
    int numWeights = 10;
    int numOperations=100000000;
    double percentWeightMin=.8;
    double percentWeightMax=1.2;

    std::ofstream outputFile;


    //Generating a normally distributed vector of weights
    std::default_random_engine generator(10);
    std::normal_distribution<double> distribution(5.0,2.0);
    std::uniform_real_distribution<float> d(1,10);
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
    int startindex = selector.id_of(0);

    for (int i = 0; i < 100; i++) {
        //std::cout << startindex << std::endl;
        //selector.update_weight(startindex, std::max<float>(0.0, distribution(generator)));
        startindex = selector(generator);
    }
    //WOULD STOP TIMING HERE

    //outputing the ending distribution
    outputFile<<"ending distribution, "<<std::endl;

    for (int i=0;i<numWeights;i++){
        outputFile << selector.get_weight(i)<<", ";
    }
    outputFile.close();











}