#include <random>
#include <vector>
#include <iostream>
#include <sys/time.h>
using namespace std;

double testFunct(double x) {
    return x;
}

int main() {
    vector<double> weights = {};
    vector<double> weights2 = {1.0, 3.0, 100, 2.0, 1.0, 0, 7};
    timeval time;
    gettimeofday(&time, NULL);
    std::default_random_engine generator(time.tv_usec);
    for(int i = 0; i < 100; i++) {
        
        
        discrete_distribution<>::param_type myParameters = {1.0, 2.0, 3.0};
        
        discrete_distribution<> selector(weights.begin(), weights.end());
        discrete_distribution<> selector2(weights2.begin(), weights2.end());
        // discrete_distribution<> selector({1.0, 3.0, 2.0, 1.0, 0, 3});
        // discrete_distribution<> selector3(5, 5, 10, testFunct);
        // discrete_distribution<> selector4(myParameters);
        // discrete_distribution<> selector2(weights.begin(), weights.end());
        
        
        
        //vector<double> weights2 = selector.param();
        std::uniform_real_distribution<float> d(.9, 1.1);
        //cout << d(generator) << endl;
        //cout << d(generator) << endl;

        vector<double> probabilities = selector2.probabilities();
        for(auto p : probabilities) {
            cout << p << " ";
        }
        vector<double> probabilities2 = selector2.param().probabilities();

        for(auto p : probabilities2) {
            cout << p << " ";
        }

        // for(auto p : selector.probabilities()) {
        //     cout << p << endl;
        // }

        int index = selector(generator);
        std::cout << "Index: " << index << std::endl;
        //cout << index << endl;
        
        
        // for(int j = 0; j < weights.size(); j++) {
        //     uniform_int_distribution<> add(weights[j] * -0.1, weights[j] * 0.1);
        //     weights[j] += add(generator);
            
        // }
        // if (index < 0 || index >= weights.size()) {
        //     std::cout << "Error, index out of range" << std::endl;
        // } else {
        //     counts[index]++;
        // }
        // }
        //cout << endl;
        

    }
    
}   