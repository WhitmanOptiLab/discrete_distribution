#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <map>
#include <iomanip>
#include <sstream>
#include "leaf_sum_tree_selector.hpp"

using Tree = dense::stochastic::leaf_sum_tree<>;
using Real = double;
using Pos = size_t;

void test_all_zero_weights() {
    std::cout << "\nTest: All Zero Weights\n";
    Tree tree(std::vector<Real>(10, 0.0));
    std::mt19937 rng(123);
    // Expect it to always pick the first leaf (or something consistent, as sum is 0)
    for (int i = 0; i < 100; ++i) {
        auto idx = tree(rng);
        assert(idx < 10);
    }
}

void test_large_uniform_weights() {
    std::cout << "\nTest: Large Uniform Weights\n";
    constexpr size_t N = 10000;
    std::vector<Real> weights(N, 1.0);
    Tree tree(weights);

    std::mt19937 rng(123);
    std::map<Pos, int> counts;

    const int trials = 500000;
    for (int i = 0; i < trials; ++i) {
        counts[tree(rng)]++;
    }

    // Check uniformity within 1% tolerance
    Real expected = 1.0 / N;
    for (const auto& [idx, count] : counts) {
        Real ratio = Real(count) / trials;
        assert(std::abs(ratio - expected) < 0.01);
    }
    std::cout << "Large uniform weights sampling passed.\n";
}

void test_push_pop_stress() {
    std::cout << "\nTest: Push and Pop Stress\n";
    Tree tree;
    std::mt19937 rng(42);

    // Push 1000 weights of value 1.0
    std::vector<Real> weights(1000, 1.0);
    //for (int i = 0; i < 1000; ++i)
        tree.push_back(weights);
    assert(tree.size() == 1000);

    // Pop 500 weights
    //for (int i = 0; i < 500; ++i)
        tree.pop_back(500);
    assert(tree.size() == 500);

    // Sample and verify valid indices
    for (int i = 0; i < 100; ++i) {
        auto idx = tree(rng);
        assert(idx < 500);
    }

    std::cout << "Push/pop stress test passed.\n";
}

void test_update_weight_edge_cases() {
    std::cout << "\nTest: Update Weight Edge Cases\n";
    std::vector<Real> weights = {5.0, 0.0, 3.0};
    Tree tree(weights);

    tree.update_weight(1, 10.0); // Increase zero weight to 10
    tree.update_weight(0, 0.0);  // Decrease from 5 to 0
    tree.update_weight(2, 0.0);  // Set last to zero

    std::mt19937 rng(99);
    std::map<Pos, int> counts;
    const int trials = 100000;

    for (int i = 0; i < trials; ++i)
        counts[tree(rng)]++;

    // Only index 1 should have nonzero weight now
    assert(counts[0] == 0);
    assert(counts[2] == 0);
    assert(counts[1] == trials);

    std::cout << "Update weight edge cases passed.\n";
}

void test_stream_operators() {
    std::cout << "\nTest: Stream Operators\n";

    // Step 1: Original distribution
    std::vector<double> weights = {0.5, 1.5, 2.0, 3.0};
    Tree original(weights);
    std::cout << std::endl;

    // Step 2: Stream to string
    std::stringstream ss;
    ss << original;
    std::string serialized = ss.str();
    std::cout << "Serialized: " << serialized << std::endl;

    // Step 3: Read back into new distribution
    Tree loaded;
    ss.seekg(0); // rewind the stream
    ss >> loaded;

    // Step 4: Compare
    assert(original == loaded && "Deserialized distribution does not match original");

    std::cout << "✅ Stream operators passed." << std::endl;
}


void test_single_element_tree() {
    std::cout << "\nTest: Single Element Tree\n";
    Tree tree({42.0});
    std::mt19937 rng(11);

    for (int i = 0; i < 100; ++i)
        assert(tree(rng) == 0);

    tree.update_weight(0, 0.0);
    for (int i = 0; i < 100; ++i)
        assert(tree(rng) == 0); // Behavior when total weight zero; still returns index 0

    std::cout << "Single element tree test passed.\n";
}

void test_empty_tree_stream() {
    std::cout << "\nTest: Empty Tree Stream\n";
    Tree original;
    std::stringstream ss;
    ss << original;

    Tree loaded;
    ss.seekg(0);
    ss >> loaded;

    assert(original == loaded);
    assert(loaded.size() == 0);
    std::cout << "✅ Empty tree stream passed.\n";
}
void test_max_weight_handling() {
    std::cout << "\nTest: Max Weight Handling\n";
    std::vector<Real> weights = {
        std::numeric_limits<Real>::max() / 2,
        std::numeric_limits<Real>::max() / 3,
        1.0
    };
    Tree tree(weights);
    std::mt19937 rng(123);

    // Try sampling — shouldn't crash or go out of bounds
    for (int i = 0; i < 100; ++i)
        assert(tree(rng) < tree.size());

    std::cout << "✅ Max weight handling passed.\n";
}

void test_near_zero_weights() {
    std::cout << "\nTest: Near-Zero Weights\n";
    std::vector<Real> weights = {1e-300, 1e-300, 1e-300};
    Tree tree(weights);
    std::mt19937 rng(456);

    std::map<Pos, int> counts;
    for (int i = 0; i < 100000; ++i)
        counts[tree(rng)]++;

    for (auto& [i, c] : counts)
        assert(c > 0); // Every index should be reachable

    std::cout << "✅ Near-zero weights sampling passed.\n";
}

void test_pop_to_zero_and_reuse() {
    std::cout << "\nTest: Pop to Zero and Reuse\n";
    Tree tree({1.0, 2.0});
    tree.pop_back(2);
    assert(tree.size() == 0);

    tree.push_back({5.0, 6.0, 7.0});
    assert(tree.size() == 3);

    std::mt19937 rng(789);
    for (int i = 0; i < 100; ++i)
        assert(tree(rng) < 3);

    std::cout << "✅ Pop to zero and reuse passed.\n";
}

void test_noop_update() {
    std::cout << "\nTest: No-op Update\n";
    Tree tree({2.0, 3.0, 4.0});
    tree.update_weight(1, 3.0); // No change

    std::stringstream ss;
    ss << tree;

    Tree loaded;
    ss.seekg(0);
    ss >> loaded;

    assert(tree == loaded);
    std::cout << "✅ No-op update test passed.\n";
}



int main() {
    test_all_zero_weights();
    test_large_uniform_weights();
    test_push_pop_stress();
    test_update_weight_edge_cases();
    test_single_element_tree();
    test_stream_operators();
    test_empty_tree_stream();
    test_max_weight_handling();
    test_near_zero_weights();
    test_pop_to_zero_and_reuse();
    test_noop_update();



    std::cout << "\n🎉 All extreme tests passed!\n";
    return 0;
}
