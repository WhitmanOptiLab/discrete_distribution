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
template <class PosType, size_t precision = std::numeric_limits<Real>::digits>
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
    Real weight_diff = new_weight - _tree().weight_of(i);
    _tree().weight_of(i) = new_weight;
    while (i != _tree().root()) {
      _tree().weightsum_of(i) += weight_diff;
      i = parent_of(i);
    }
    _tree().weightsum_of(i) += weight_diff;
    _total_weight += weight_diff;
  }

  Real total_weight() const { return _total_weight; }

  template<class URNG>
  PosType __attribute__ ((noinline)) operator()(URNG& g) {

    Real target = std::generate_canonical<Real, precision, URNG>(g)*_total_weight;
    PosType node = _tree().root();
    //Loop until target random value is less than the current node's weight
    while(_tree().weight_of(node) < target) {
      target -= _tree().weight_of(node);
      if (checked_weightsum(left_of(node)) > target) {
        node = left_of(node);
      } else {
        target -= checked_weightsum(left_of(node));
        node = right_of(node);
      }
      //Should this ever happen?  No, but floating-point rounding means it's 
      //  theoretically possible, and we need a failsafe.
      if (node > _tree().last()) {
        return _tree().last();
      }
    }
    return node;
  }

  //Required interface functions for tree
  virtual void remove_last_entry() = 0;
  virtual std::ptrdiff_t size() const = 0;
  virtual bool empty() = 0;
  virtual PosType root() const = 0;
  virtual PosType last() const = 0;
  virtual PosType parent_of(PosType n) const = 0;
  virtual PosType left_of(PosType n) const = 0;
  virtual PosType right_of(PosType n) const = 0;
  virtual Real& weight_of(PosType n) = 0;
  virtual Real& weightsum_of(PosType n) = 0;
  virtual const Real& weight_of(PosType n) const = 0;
  virtual const Real& weightsum_of(PosType n) const = 0;
  virtual void special(PosType n) { return; }

 private:
  weightsum_tree<PosType, precision>& _tree() { return *this; }
  const weightsum_tree<PosType, precision>& _tree() const { return *this; }
  Real checked_weightsum(PosType node) const { return node > _tree().last() ? 0 : _tree().weightsum_of(node); }
  Real checked_weight(PosType node) const { return node > _tree().last() ? 0 : _tree().weight_of(node); }
  Real sum_weights(PosType i) {
    if (i > _tree().last()) return 0.0f;
    _tree().special(i);  // Allows implimenting classes to insert special behavior 
    _tree().weightsum_of(i) =
      sum_weights(left_of(i)) + sum_weights(right_of(i)) 
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

  Real _total_weight;
};

}
}
#endif
