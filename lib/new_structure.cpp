#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <map>
#include <iomanip>
#include "leaf_sum_tree_selector.hpp"

using Tree = dense::stochastic::leaf_sum_tree<>;
using Real = double;
using Pos = size_t;
std::default_random_engine generator;

size_t node_of(size_t p, size_t size) {
  int depth_added = 0;
  depth_added = static_cast<int>(log2(p)+1);
  if (p == 0) depth_added = 0;
  size_t node = 2*p + 1;
  int depthDifference = std::max(static_cast<int>((log2(size)-1)) - depth_added, 0);
  node = node<<depthDifference;
  if (node<=(size)/2 && depthDifference >= 0){
    node = node << 1;
  }
  return node;
}

void test_id(){
  auto tree = Tree({0,1,2,3,4,5,6,7,8,9,10});
  int id = tree.id_of(30);
  std::cout << id << std::endl;
  id = tree.node_of(0);
  std::cout << id << std::endl;
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
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;

}

int main (){
  test_id();

  }