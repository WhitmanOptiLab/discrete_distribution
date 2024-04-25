#ifndef WEIGHTSUM_TREE
#define WEIGHTSUM_TREE
#include "completetree.hpp"
#include <limits>
#include <utility>
#include <random>

namespace dense {
namespace stochastic {

//Class to randomly select an index where each index's probability of being 
//  selected is weighted by a given vector.  
template <class Tree, class PosType, typename W, size_t precision = std::numeric_limits<W>::digits>
class weightsum_tree {
 public:
  //Weights can be of any type, but most be convertable to W values
  weightsum_tree() = default;

  void compute_weights() {
    _total_weight = sum_weights(_tree().root());
  }

  void swap_with_child(PosType parent, PosType child) {
    W weight_diff = _tree().weight_of(parent) - _tree().weight_of(child);
    _tree().weightsum_of(child) += weight_diff;
    std::swap(_tree().weight_of(parent), _tree().weight_of(child));
  }

  void swap(PosType i, PosType j) {
    W old_i_weight = _tree().weight_of(i);
    update_weight(i, _tree().weight_of(j));
    update_weight(j, old_i_weight);
  }

  void update_weight(PosType i, W new_weight) {
    W weight_diff = new_weight - _tree().weight_of(i);
    _tree().weight_of(i) = new_weight;
    while (i != _tree().root()) {
      _tree().weightsum_of(i) += weight_diff;
      i = Tree::parent_of(i);
    }
    _tree().weightsum_of(i) += weight_diff;
    _total_weight += weight_diff;
  }

  W total_weight() const { return _total_weight; }

  template<class URNG>
  PosType __attribute__ ((noinline)) operator()(URNG& g) {

    W target = std::generate_canonical<W, precision, URNG>(g)*_total_weight;
    PosType node = _tree().root();
    //Loop until target random value is less than the current node's weight
    while(_tree().weight_of(node) < target) {
      target -= _tree().weight_of(node);
      if (checked_weightsum(Tree::left_of(node)) > target) {
        node = Tree::left_of(node);
      } else {
        target -= checked_weightsum(Tree::left_of(node));
        node = Tree::right_of(node);
      }
      //Should this ever happen?  No, but floating-point rounding means it's 
      //  theoretically possible, and we need a failsafe.
      if (node > _tree().last()) {
        return _tree().last();
      }
    }
    return node;
  }

 private:
  Tree& _tree() { return *static_cast<Tree*>(this); }
  const Tree& _tree() const { return *static_cast<const Tree*>(this); }
  W checked_weightsum(PosType node) const { return node > _tree().last() ? 0 : _tree().weightsum_of(node); }
  W checked_weight(PosType node) const { return node > _tree().last() ? 0 : _tree().weight_of(node); }
  W sum_weights(PosType i) {
    if (i > _tree().last()) return 0.0f;
    _tree().special(i);  // Allows implimenting classes to insert special behavior 
    _tree().weightsum_of(i) =
      sum_weights(Tree::left_of(i)) + sum_weights(Tree::right_of(i)) 
      + _tree().weight_of(i);
    return _tree().weightsum_of(i);
  }
  bool isAncestor(PosType i, PosType j) {
    unsigned int ilz = __builtin_clz(i+1);
    unsigned int jlz = __builtin_clz(j+1);
    if (ilz > jlz) {
      return (j >> (ilz - jlz)) == i;
    } 
    return false;
  }

  W _total_weight;
};

}
}
#endif
