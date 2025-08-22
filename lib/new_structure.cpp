#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <map>
#include <iomanip>
#include "exponentialAddRemove.hpp"

using Tree = dense::stochastic::exponential_leaf_sum_tree<size_t, double, 8>;
using Real = double;
using PosType = size_t;
std::default_random_engine generator;

PosType node_of(size_t p, size_t size)  {
  PosType node = 2*p + 1;
  PosType depthDifference = __builtin_clz(node) - __builtin_clz(size);
  node = node<<depthDifference;
  if (node>(size)){
    node = node >> 1;
  }
  return node;
}



void test_id(){
  std::vector<double> weights;// = {-10,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};
  for (int i = 0; i <64; ++i) {
    weights.push_back(i);
  }
  //std::reverse(weights.begin(),weights.end());
  auto tree = Tree(weights.begin(),weights.end());
  std::cout << "built!" << std::endl;
  tree.print_tree();
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << "Updating!" << std::endl;

  tree.push_back(64);
  tree.push_back(65);
  tree.push_back(66);
  //tree.pop_back();


  tree.print_tree();
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;

}

int main (){
  test_id();
  //std::cout << node_of(2,21) << std::endl;
  }