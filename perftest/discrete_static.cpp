//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=nonuniform_int_distribution<int>" "-DWEIGHTNUM=10000000" -o test0 normal_static.cpp
//g++ -I../lib -O3 "-DWRSLIB=heap_random_selector<int>" "-DWEIGHTNUM=100000" -o test1 normal_static.cpp
//g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=100" -o test2 normal_static.cpp

#include <iostream>
#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>

int random(int low, int high)
{
    return low + rand() % (high - low + 1);
}

int main() {
  std::default_random_engine generator;
  std::vector<float> weights = {};
  int sum = 0;
  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(random(1,10));
  }	      

  //start time
  struct timeval start, end;
  gettimeofday(&start, NULL);
  WRSLIB selector(weights.begin(), weights.end());
  for (int i = 0; i < 100000; i++){
    sum += selector(generator);
  }
  std::cout<<sum<<std::endl;

  //end time
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << "Time: " << elapsedtime_sec << std::endl;
}
 
