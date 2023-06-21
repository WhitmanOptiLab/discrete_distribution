//HOW TO USE COMPILATION FLAGS:
//Enter the library you want to test after 'DWRSLIB=' and the number of weights after 'DWEIGHTNUM='.

//Ex:
//g++ -I../lib -O3 "-DWRSLIB=std::discrete_distribution<int>" "-DWEIGHTNUM=100" -o test0 discrete_changing.cpp

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
  std::vector<float> weights = {}; // each weight is an item in data structure
  std::default_random_engine generator;
  
  for(int i = 0; i < WEIGHTNUM; i++){
    weights.push_back(random(1,10));
  }	      

  //start time
  struct timeval start, end;
  gettimeofday(&start, NULL);
  WRSLIB selector(weights.begin(), weights.end());
  for (int i = 0; i < 100000; i++){
    int index = selector(generator);
    weights[index] = random(1,10);
    selector = std::discrete_distribution<>(weights.begin(), weights.end());
  }

  //end time
  gettimeofday(&end, NULL);
  double elapsedtime_sec = double(end.tv_sec - start.tv_sec) + 
    double(end.tv_usec - start.tv_usec)/1000000.0;
  std::cout << elapsedtime_sec << std::endl;
}
 
