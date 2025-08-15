//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 weibull_changing.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 weibull_changing.cpp

#include "random_selector.hpp"
#include "modifiable_heap_random_selector.hpp"
#include "no_weight_storage_modifiable_heap_random_selector.hpp"
#include "leaf_sum_tree_selector.hpp"
#include "leaf_sum_tree_split.hpp"
#include "sideways_fenwick_selector.hpp"
#include "old_sideways_fenwick_selector.hpp"
#include "sideways_fenwick_selector_bitcast.hpp"
#include "incremental_leaf_sum_tree.hpp"
#include "bucket_alias.hpp"
#include "wrsLessStorage.hpp"

//#include "XoshiroCpp.hpp"


#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace dense::stochastic;


int main() {
  //std::ofstream debug;
  //debug.open("debug.txt");
  std::weibull_distribution<float> d(0.5); 
    std::default_random_engine generator; 

  std::vector<float> weights = {};
  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(std::max<double>(0.00001,d(generator)));
  }	      

  // for(int i = 0; i < WEIGHTNUM; i++){
  //   if (weights[i]<=0){weights[i]=.01;}
  // }	
  
  //   vector<Element> elements{};
  // for(int i=0;i<weights.size();i++){
  //   elements.emplace_back(i,i,weights[i]);
  // }

  //start time
  struct timeval start, end;
  WRSLIB selector(weights.begin(), weights.end());
  //WRSLIB selector(elements.size(),elements);
  gettimeofday(&start, NULL);

  
  for (int i = 0; i < 1000000; i++) {
    //debug<<"about to select "<<i<<std::endl;
    int index = selector(generator);
    //debug<<"selected "<<i<<std::endl;
    //debug<<"about to update "<<i<<std::endl;
    selector.update_weight(index, std::max<double>(0.00001,d(generator)));
    //debug<<"updated "<<i<<std::endl<<std::endl;
  }
  
  // end time
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << elapsedtime_sec << std::endl;
//debug.close();
  
}