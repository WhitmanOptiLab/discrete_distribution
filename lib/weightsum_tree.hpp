#ifndef WEIGHTSUM_TREE
#define WEIGHTSUM_TREE
#include "completetree.hpp"
#include <limits>
#include <utility>
#include <random>
#include <iostream>

namespace dense {
namespace stochastic {

//Class to randomly select an index where each index's probability of being 
//  selected is weighted by a given vector.  
template <class Tree, class PosType, size_t precision = std::numeric_limits<Real>::digits>
class weightsum_tree {
 public:
  //Weights can be of any type, but most be convertable to Real values
  weightsum_tree() = default;

  void compute_weights() {
    _total_weight = sum_weights(_tree().root());
  }

  void swap_with_child(PosType parent, PosType child) {
    Real weight_diff = _tree().weight_of(parent) - _tree().weight_of(child);
    _tree().weightsum_of(child) += weight_diff;
    std::swap(_tree().weight_of(parent), _tree().weight_of(child));
  }

  void swap(PosType i, PosType j) {
    Real old_i_weight = _tree().weight_of(i);
    update_weight(i, _tree().weight_of(j));
    update_weight(j, old_i_weight);
  }

  void update_weight(PosType i, Real new_weight) {
    //std::cout << "Changing " << _tree().weightsum_of(i) << " to " << new_weight << " at index " << i << std::endl;
    Real weight_diff = new_weight - _tree().weightsum_of(i);
    //_tree().weightsum_of(i) = new_weight;
    while (i != _tree().root()) {
      _tree().weightsum_of(i) += weight_diff;
      i = Tree::parent_of(i);
    }
    _tree().weightsum_of(i) += weight_diff;
    _total_weight += weight_diff;
  }

  Real total_weight() const { return _total_weight; }

template<class URNG>
PosType operator()(URNG& g) {
  Real target = std::generate_canonical<Real, precision, URNG>(g) * _tree().total_weight();

  PosType node = Tree::root();

  while (node < _tree().get_leaf_start()) {
      //std::cout << "Target: " << target << std::endl;

    PosType left = Tree::left_of(node);
    //std::cout << node << " left weight: " << _tree().weightsum_of(left) << ", right weight: " << _tree().weightsum_of(Tree::right_of(node)) << std::endl;
    Real left_sum = _tree().weightsum_of(left);

    if (target < left_sum) {
      //std::cout << "Took left" << std::endl;
      node = left;
    } else {
      //std::cout << "Took right" << std::endl;
      target -= left_sum;
      node = Tree::right_of(node);
    }
    //std::cout << std::endl;
  }

  return node;
}


 private:
  Tree& _tree() { return *static_cast<Tree*>(this); }
  const Tree& _tree() const { return *static_cast<const Tree*>(this); }
  Real checked_weightsum(PosType node) const { return node > _tree().last() ? 0 : _tree().weightsum_of(node); }
  Real checked_weight(PosType node) const { return node > _tree().last() ? 0 : _tree().weight_of(node); }
  Real sum_weights(PosType i) {
  	if (i > _tree().last()) return 0.0;
    Real& weightsum = _tree().weightsum_of(i);
  	Real self_weight = weightsum; // <- original weight, not actually a weightsum yet
  	Real left_sum  = sum_weights(Tree::left_of(i));
  	Real right_sum = sum_weights(Tree::right_of(i));

  	Real total = self_weight + left_sum + right_sum;
  	weightsum = total;
  	return total;
  }

  bool isAncestor(PosType i, PosType j) {
    unsigned int ilz = __builtin_clz(i+1);
    unsigned int jlz = __builtin_clz(j+1);
    if (ilz > jlz) {
      return (j >> (ilz - jlz)) == i;
    } 
    return false;
  }

  Real _total_weight;
};

}
}
#endif
