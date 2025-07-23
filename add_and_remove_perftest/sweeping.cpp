//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 normal_changing.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 normal_changing.cpp

#include "random_selector.hpp"
#include "modifiable_heap_random_selector.hpp"
#include "no_weight_storage_modifiable_heap_random_selector.hpp"
#include "leaf_sum_tree_selector.hpp"
#include "leaf_sum_tree_split.hpp"
#include "sideways_fenwick_selector.hpp"
#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
using namespace dense::stochastic;

int main() {
  
  std::uniform_real_distribution<float> d(1,10); 
  std::uniform_int_distribution<int> randomIndex(0, WEIGHTNUM - 1);

  std::default_random_engine generator;
  std::vector<float> weights = {};

  int initialSize=2;
  
  for(int i = 0; i < 2; i++){
    weights.push_back(d(generator));
  }	      
    

  //start time
  struct timeval start, end;
  WRSLIB selector(weights.begin(), weights.end());
  gettimeofday(&start, NULL);
  
  int sum=0;
  for (int j = 0; j < 10000000/(2*WEIGHTNUM); j++) {
    for(int i=initialSize-1;i<WEIGHTNUM;i++){
      selector.push_back(d(generator));
    }
    for(int i=WEIGHTNUM-1;i>=initialSize;i--){
      selector.pop_back();
    }
  }
  
  // end time
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << elapsedtime_sec << std::endl;
  if(sum == 0 ) {std::cout << "hit"; } // Added conditional so compiler must compute sum

  
}
