//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 normal_changing.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 normal_changing.cpp

#include "../../../lib/exponentialAddRemove.hpp"
#include "../../../lib/incremental_exponential.hpp"
#include <sys/time.h>
#include <iostream>
#include <random>
using namespace dense::stochastic;

int main() {
    int WEIGHTNUM = 10;
  std::uniform_real_distribution<float> d(1,10); 
  std::uniform_int_distribution<int> randomIndex(0, WEIGHTNUM - 1);

  std::default_random_engine generator;
  std::vector<float> weights = {};
  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(d(generator));
  }	            
  dense::stochastic::incremental_exponential<> selector(weights.begin(), weights.end());

  //start time
  struct timeval start, end;
  gettimeofday(&start, NULL);
  

  for(int i=0; i<100000;i++){
    selector.push_back(d(generator));
  }

   // end time
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << "incremental exponential adding time"<<elapsedtime_sec << std::endl;


           
  dense::stochastic::exponential_leaf_sum_tree_alex<> alex_selector(weights.begin(), weights.end());

  gettimeofday(&start, NULL);
  

  for(int i=0; i<100000;i++){
    alex_selector.push_back(d(generator));
  }

   // end time
  gettimeofday(&end, NULL);
   elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << "alex version adding time"<<elapsedtime_sec << std::endl;

  }

  
 

  
