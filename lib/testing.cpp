#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <map>
#include <iomanip>
#include "exponentialAddRemove.hpp"

using Tree = dense::stochastic::complete_exponential_leaf_sum_tree<>;
using Real = double;
using Pos = size_t;

void test_sampling_distribution() {
    std::cout << "Test: Sampling Distribution\n";
    std::vector<Real> weights = {1.0, 3.0, 6.0};
    Tree tree(weights);
    tree.print_tree();

    std::mt19937 rng(42);
    std::map<Pos, int> counts;
    const int trials = 100'000;

    for (int i = 0; i < trials; ++i)
        counts[tree(rng)]++;

    std::vector<Real> expected = {0.1, 0.3, 0.6};
    for (Pos i = 0; i < weights.size(); ++i) {
        Real proportion = Real(counts[i]) / trials;
        std::cout << "Index " << i << ": " << std::fixed << std::setprecision(3) << proportion
                  << " (expected ~" << expected[i] << ")\n";
        assert(std::abs(proportion - expected[i]) < 0.02);
    }
}

void test_update_weight() {
    std::cout << "\nTest: Update Weight\n";
    std::vector<Real> weights = {1.0, 1.0, 1.0};
    Tree tree(weights);

    tree.update_weight(2, 7.0);
    std::mt19937 rng(123);
    std::map<Pos, int> counts;
    for (int i = 0; i < 100'000; ++i)
        counts[tree(rng)]++;

    std::vector<Real> expected = {1.0, 1.0, 7.0};
    Real total = std::accumulate(expected.begin(), expected.end(), 0.0);
    for (Pos i = 0; i < expected.size(); ++i) {
        Real expected_ratio = expected[i] / total;
        Real actual_ratio = Real(counts[i]) / 100'000;
        std::cout << "Index " << i << ": " << std::fixed << std::setprecision(3) << actual_ratio
                  << " (expected ~" << expected_ratio << ")\n";
        assert(std::abs(actual_ratio - expected_ratio) < 0.02);
    }
}

void test_push_and_pop() {
    std::cout << "\nTest: Push and Pop\n";
    Tree tree;
    tree.push_back({0.0, 1.0, 2.0,3.0});
    tree.print_tree();
    assert(tree.size() == 4);

    //tree.print_tree();
    tree.push_back(4.0);
    tree.print_tree();
    assert(tree.size() == 5);
    tree.pop_back();
    assert(tree.size() == 4);
    tree.print_tree();
    assert(tree.total_weight() == 6.0);
}

//void test_param_sampling() {
//    std::cout << "\nTest: Param Sampling\n";
//
//
//    Tree source({1.0, 1.0, 1.0});
//    source.update_weight(0, 0.0);
//    source.update_weight(1, 1.0);
//    source.update_weight(2, 9.0);
//    source.print_tree();
//
//    // Grab param snapshot
//    auto param = source.param();
//    const std::vector<Real>& pw = param.weights();
//
//    // Create a NEW tree with different weights
//    Tree target({1.0, 1.0, 1.0}); // Dummy init
//    target.param(param); // Use param setter to overwrite
//
//    std::mt19937 rng(9);
//    std::map<Pos, int> counts;
//    for (int i = 0; i < 100'000; ++i)
//        counts[target(rng)]++;
//
//    Real total = std::accumulate(pw.begin(), pw.end(), 0.0);
//    for (Pos i = 0; i < pw.size(); ++i) {
//        Real expected = pw[i] / total;
//        Real actual = Real(counts[i]) / 100'000;
//        std::cout << "Index " << i << ": " << actual << " (expected ~" << expected << ")\n";
//        assert(std::abs(actual - expected) < 0.02);
//    }
//}



void test_zero_weight_behavior() {
    std::cout << "\nTest: Zero Weight Sampling\n";
    Tree tree({0.0, 0.0, 10.0, 0.0, 0.0});
    std::mt19937 rng(123);

    for (int i = 0; i < 100; ++i)
        assert(tree(rng) == 2); // Only valid index
}

/*void test_probabilities_sum_to_one() {
    std::cout << "\nTest: Probabilities Sum\n";
    Tree tree({0.5, 1.0, 1.5});
    auto probs = tree.probabilities();

    Real sum = std::accumulate(probs.begin(), probs.end(), 0.0);
    std::cout << "Sum of probabilities: " << sum << "\n";
    assert(std::abs(sum - 1.0) < 1e-6);
}*/

int main() {
    test_sampling_distribution();
    test_update_weight();
    test_push_and_pop();
    //test_param_sampling();
    test_zero_weight_behavior();
    //test_probabilities_sum_to_one();

    std::cout << "\n✅ All tests passed!\n";
    return 0;
}
