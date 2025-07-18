//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 normal_changing.cpp
//g++ -I../lib -O3 "-DWRSLIB=leaf_sum_tree_split<int>" "-DWEIGHTNUM=100000" -o test1 normal_changing.cpp


#include "sideways_fenwick.hpp"


#include "leaf_sum_tree_selector.hpp"
#include "leaf_sum_tree_split.hpp"
#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace dense::stochastic;

double generate_normal_between(double mean, double stddev, double min, double max, std::default_random_engine& gen) {
  std::normal_distribution<double> dist(mean, stddev);
  double x;
  do {
    x = dist(gen);
  } while (x < min || x > max);
  return x;
}

int main() {
  std::normal_distribution<float> d(5,2);
  std::uniform_real_distribution<float> d2(0.99, 1.01);
  std::uniform_int_distribution<int> randomIndex(0,WEIGHTNUM-1);
  std::default_random_engine generator;
  std::vector<float> weights = {};


  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(d(generator));
  }	      

  float minweight = *std::min_element(weights.begin(), weights.end());
  for(int i = 0; i < WEIGHTNUM; i++){
    weights[i] -= minweight;
  }	      

  //start time
  WRSLIB selector(weights.begin(), weights.end());
    auto start = std::chrono::steady_clock::now();



  
  for (int i = 0; i < 1000000; i++) {
    int index = selector(generator);
    selector.update_weight(index, std::max<float>(0.0, d(generator)));
    for(int j = 0;j<4;j++){
      int randI = randomIndex(generator);
      //std::cout<<"about to update weights "<<randI<<std::endl;
      selector.update_weight(randI,std::max<float>(0.0, d(generator)));
      //std::cout<<"just updated weights "<<randI<<std::endl;
    }
  }
  
  // end time
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << elapsed.count() << std::endl;
  
}
