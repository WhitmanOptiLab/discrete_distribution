#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <map>
#include <iomanip>
#include "exponentialAddRemove.hpp"

using Tree = dense::stochastic::complete_exponential_leaf_sum_tree<size_t, double, 16>;
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
  std::vector<double> weights = {1,3,5};// = {-10,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};
//  for (int i = 0; i < 256; ++i) {
//    weights.push_back(i);
//    //std::cout << "adding " << i << std::endl;
//  }
  //std::reverse(weights.begin(),weights.end());
  auto tree = Tree(weights.begin(),weights.end());
  std::cout << "built!" << std::endl;
  //tree.print_tree();
//  for (int i = 1; i <20; ++i) {
//    tree.push_back(i);
//    tree.print_tree();
//  }
      //tree.push_back(1);
      //tree.print_tree();
    //tree.push_back(2);
    //tree.print_tree();


  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << tree(generator) << std::endl;
  std::cout << "Updating!" << std::endl;

  //tree.update_weight(0,100000);
      //std::vector<Real> weights2(1000, 1.0);

	//tree.push_back(weights2);
          tree.print_tree();

  tree.update_weight(0,256);
  //tree.push_back(100000);
  //tree.push_back(68);
  //tree.push_back(69);


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