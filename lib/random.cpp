#include <random>
#include <iostream>
#include <sys/time.h>

using namespace std;

int main() {
    timeval rand;
    gettimeofday(&rand, NULL);
    std::default_random_engine generator(rand.tv_usec);
    std::uniform_real_distribution<double> d(0.9, 11.0/10.0);
    double max = 0;
    for(int i = 0; i < 100000; i++) {
        double initial = 1;
        for(int i = 0; i < 1000; i++) {
            double randomValue = d(generator);
            initial *= randomValue;
        }
        if(max < initial) {
            max = initial;
        }
    }
    cout << "Max value: " << max << std::endl;
    
}