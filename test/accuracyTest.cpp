//#include "../lib/new_modifiable_heap_random_selector.hpp"
//#include"../lib/no_weight_storage_modifiable_heap_random_selector.hpp"
//#include "../lib/sideways_fenwick_selector.hpp"
//#include "../lib/bucket_alias.hpp"
//#include "../lib/wrsLessStorage.hpp"
#include "../lib/old_sideways_fenwick_selector.hpp"
#include <iostream>
#include <fstream>
#include <random>

//using namespace dense::stochastic;

int main(){
    int numWeights = 32;
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
    //outputFile<<"starting distribution from 1 to "<<numWeights+1<<": ";
    for (int i=0;i<numWeights;i++){
        //double num = distribution(generator);
        weights.push_back(i/*distribution(generator)*/);

        //outputFile<<weights[i]<<", "; //outputing the initial weights
    }
   // outputFile<<std::endl<<std::endl;

    //WOULD START TIMING HERE
    //Constructing the heap random selector
    std::cout<<"about to construct"<<std::endl;
    dense::stochastic::old_sideways_fenwick_selector selector(weights.begin(), weights.end());
    std::cout<<"constructed"<<std::endl;

    for(int i=0; i<5;i++){
        selector.update_weight(numWeights-1-i,5);
    }

    // for(int i=0;i<numWeights;i++){
    //     selector.update_weight(i,i);
    // }

    //outputFile<<"distribution after construction: ";

    // for (int i=0;i<numWeights;i++){
    //     outputFile << selector.get_weight(i)<<", ";
    // }
    // outputFile<<std::endl;

    for(int i=0;i<100000;i++){
        outputFile<<selector(generator)<<std::endl;
    }

    // //selecting weights
    // int startindex = selector.id_of(0);

    // for (int i = 0; i < 100; i++) {
    //     //std::cout << startindex << std::endl;
    //     //selector.update_weight(startindex, std::max<float>(0.0, distribution(generator)));
    //     startindex = selector(generator);
    // }
    //WOULD STOP TIMING HERE

    //outputing the ending distribution

    // outputFile<<std::endl<<"adding weights: ";
    // for(int i=numWeights;i<numWeights+4;i++){
    //     double entry = 1.5;//distribution(generator);
    //     outputFile<<entry<<", ";
    //     selector.push_back(entry);
    // }

    


    // outputFile<<"ending distribution, "<<std::endl;

    // for (int i=0;i<numWeights+4;i++){
    //     outputFile << selector.get_weight(i)<<", ";
    // }

    // std::cout<<"about to update ";
    // outputFile<<"updating last weight to 10"<<std::endl;
    // selector.update_weight(numWeights+3,10);

    // outputFile<<std::endl<<"ending distribution: "<<std::endl;
    // std::cout<<"just updated "<<std::endl;

    // for (int i=0;i<(numWeights+4);i++){
    //     outputFile << selector.get_weight(i)<<", ";
    //     //std::cout<<"getting weight at index "<<i<<std::endl;
    // }

    // outputFile<<std::endl<<"removing last 6 weights"<<std::endl;
    // for(int i=0;i<6;i++){
    //     selector.pop_back();
    // }
    // outputFile<<std::endl<<"ending distribution: "<<std::endl;

    // for (int i=0;i<(numWeights+4-6);i++){
    //     outputFile << selector.get_weight(i)<<", ";
    //     //std::cout<<"getting weight at index "<<i<<std::endl;
    // }



    outputFile.close();











}