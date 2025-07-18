#ifndef LEAF_SUM_TREE
#define LEAF_SUM_TREE
#include "completetree.hpp"
#include <limits>
#include <utility>
#include <random>
#include <cassert>
#include <iostream>

namespace dense {
namespace stochastic {


//Class to randomly select an index where each index's probability of being 
//  selected is weighted by a given vector.  
template <class IntType = size_t, size_t precision = std::numeric_limits<Real>::digits>
class leaf_sum_tree : protected complete_tree<IntType, Real> {
 public:
  using This = leaf_sum_tree<IntType, precision>;
  using BaseTree = complete_tree<IntType, Real>;
  using PosType = typename BaseTree::position_type;
  static PosType left_of(PosType i) { return BaseTree::left_of(i);}
  static PosType right_of(PosType i) { return BaseTree::right_of(i);}
  static PosType parent_of(PosType i) { return BaseTree::parent_of(i);}

  //Weights can be of any type, but must be convertable to Real values
  leaf_sum_tree() = delete;
  leaf_sum_tree(PosType p) : BaseTree(p) {};

  template< class InputIt >
  leaf_sum_tree(InputIt first, InputIt last) :
    BaseTree()
  {
    size_t n = std::distance(first, last);
    leaf_count = next_power_of_two(n);
    leaf_start = leaf_count;
    BaseTree::resize(2 * leaf_count, 0.0); //double the size of the weights list (rounded to next power of two)
    //copy weights to leaves
    InputIt it = first;
    for (size_t i = 0; it != last; ++it, ++i) {
      BaseTree::value_of(leaf_start + i) = Real(*it);
      //_total_weight += Real(*it);
    }

    //build sums from leaves up
    for (std::ptrdiff_t i = leaf_start - 1; i >= 1; --i) {
      //Real tempval = BaseTree::value_of(2 * i) + BaseTree::value_of(2 * i + 1);
      BaseTree::value_of(i) = BaseTree::value_of(2 * i) + BaseTree::value_of(2 * i + 1);
    }
    //compute_weights();
  }


//  void compute_weights() {
//    _total_weight = sum_weights(BaseTree::root());
//  }

  void update_weight(PosType i, Real new_weight) {
    i=node_of(i); //Perrin added - ask devon to make sure I'm understanding code correctly
    Real weight_diff = new_weight - weightsum_of(i);
    while (i != BaseTree::root()) {
      weightsum_of(i) += weight_diff;
      i = BaseTree::parent_of(i);
    }
    weightsum_of(i) += weight_diff;
    //_total_weight += weight_diff;
  }

  //Real total_weight() const { return _total_weight; }

  template<class URNG>
  PosType operator()(URNG& g) {
    Real target = std::generate_canonical<Real, precision, URNG>(g) * total_weight();

    PosType node = BaseTree::root();

    while (node < get_leaf_start()) {
      //std::cout << "Target: " << target << std::endl;

      PosType left = BaseTree::left_of(node);
      //std::cout << node << " left weight: " << weightsum_of(left) << ", right weight: " << weightsum_of(BaseTree::right_of(node)) << std::endl;
      Real left_sum = weightsum_of(left);

      bool go_left = (target < left_sum);
      node = go_left ? left : BaseTree::right_of(node);
      target -= go_left ? 0 : left_sum;
      //std::cout << std::endl;
    }
    //std::cout << "Returning node " << node << ", which has weight: " << weightsum_of(node) << std::endl;
    
    return id_of(node);//Perrin Added - talk to devon
  }

  Real total_weight() const {
    return BaseTree::value_of(1);  //root
  }

  Real& weight_of(PosType i) {
    return BaseTree::value_of(i);
  }

  Real get_weight(PosType i) const {
    assert(i >= 0 && i < static_cast<PosType>(leaf_count));
    return BaseTree::value_of(leaf_start + i);
  }

  Real& weightsum_of(PosType p) {
    return BaseTree::value_of(p);
  }
  const Real& weightsum_of(PosType p) const {
    return const_cast<This*>(this)->weightsum_of(p);
  }

  PosType id_of(PosType p) { return p-leaf_start; }

  PosType node_of(IntType i){return leaf_start+i;}

  size_t get_leaf_count() const { return leaf_count; }

  size_t get_leaf_start() const { return leaf_start; }

private:
  size_t leaf_count;
  size_t leaf_start;

  static size_t next_power_of_two(size_t n) {
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
  }
 



  //Real _total_weight = 0.0;

};

}
}
#endif
