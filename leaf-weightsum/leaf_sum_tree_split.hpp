#ifndef LEAF_SUM_TREE_SPLIT
#define LEAF_SUM_TREE_SPLIT
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
class leaf_sum_tree_split : protected complete_tree<IntType, Real> {
 public:
  using This = leaf_sum_tree_split<IntType, precision>;
  using BaseTree = complete_tree<IntType, Real>;
  using PosType = typename BaseTree::position_type;
  static PosType left_of(PosType i) { return BaseTree::left_of(i);}
  static PosType right_of(PosType i) { return BaseTree::right_of(i);}
  static PosType parent_of(PosType i) { return BaseTree::parent_of(i);}

  //Weights can be of any type, but must be convertable to Real values
  leaf_sum_tree_split() = delete;
  leaf_sum_tree_split(PosType p) : BaseTree(p) {};

  template< class InputIt >
  leaf_sum_tree_split(InputIt first, InputIt last) :
    BaseTree()
  {
    size_t n = std::distance(first, last);
<<<<<<< Updated upstream
    leaf_count = next_power_of_two(n);
    leaf_start = leaf_count;
    BaseTree::resize(leaf_count, 0.0); //stores internal tree nodes, at the size of the weights list (rounded to next power of two)
    leaves.resize(leaf_count, 0.0); //stores leaf nodes with actual weights
=======
    leaf_start = next_power_of_two(n);
    BaseTree::resize(leaf_start, 0.0); //stores internal tree nodes, at the size of the weights list (rounded to next power of two)
    leaves.resize(leaf_start, 0.0); //stores leaf nodes with actual weights
>>>>>>> Stashed changes
    //copy weights to leaves
    InputIt it = first;
    for (size_t i = 0; it != last; ++it, ++i) {
      leaves[i] = Real(*it);
    }

    //build sums from leaves up
    for (std::ptrdiff_t i = leaf_start - 1; i >= 1; --i) {
      size_t li = 2 * i;
      size_t ri = 2 * i + 1;
<<<<<<< Updated upstream
      //std::cout << "trying to build internal tree" << std::endl;

=======
>>>>>>> Stashed changes
      Real left = (li >= leaf_start) ? leaves[li - leaf_start] : BaseTree::value_of(li);
      Real right = (ri >= leaf_start) ? leaves[ri - leaf_start] : BaseTree::value_of(ri);

      BaseTree::value_of(i) = left + right;
<<<<<<< Updated upstream
      //std::cout << "built one internal tree node at " << i << std::endl;
=======
>>>>>>> Stashed changes
    }
  }

  void update_weight(PosType i, Real new_weight) {
    Real weight_diff = new_weight - leaves[i];
    leaves[i] = new_weight;
    i += leaf_start;
    while (i > BaseTree::root()) {
      i = BaseTree::parent_of(i);
<<<<<<< Updated upstream
      //std::cout << i << std::endl;
=======
>>>>>>> Stashed changes
      weightsum_of(i) += weight_diff;
    }
  }

  template<class URNG>
  PosType operator()(URNG& g) {
    Real target = std::generate_canonical<Real, precision, URNG>(g) * total_weight();

    PosType node = BaseTree::root();

    while (node < leaf_start/2) {
<<<<<<< Updated upstream
      //std::cout << "Leaf start: " << leaf_start << std::endl;
=======
>>>>>>> Stashed changes
      //std::cout << "Node target: " << node << std::endl;
      //std::cout << "Target: " << target << std::endl;

      PosType left = BaseTree::left_of(node);
      //std::cout << left << std::endl;
      //std::cout << node << " left weight: " << weightsum_of(left) << ", right weight: " << weightsum_of(BaseTree::right_of(node)) << std::endl;
      Real left_sum = weightsum_of(left);

      bool go_left = (target < left_sum);
      node = go_left ? left : BaseTree::right_of(node);
      if (!go_left) target -= left_sum;
      //std::cout << std::endl;
    }

<<<<<<< Updated upstream
    //std::cout << "Leaf start: " << leaf_start << std::endl;
=======
>>>>>>> Stashed changes
    //std::cout << "Node target: " << node << std::endl;
    //std::cout << "Target: " << target << std::endl;

    PosType left = BaseTree::left_of(node);
    //std::cout << left << std::endl;
    //std::cout << node << " left weight: " << leaves[left-leaf_start] << ", right weight: " << leaves[BaseTree::right_of(node)-leaf_start] << std::endl;
    Real left_sum = leaves[left-leaf_start];

    bool go_left = (target < left_sum);
    node = go_left ? left : BaseTree::right_of(node);
    if (!go_left) target -= left_sum;
    //std::cout << std::endl;

    //std::cout << "Returning node " << node-leaf_start << ", which has weight: " << leaves[node-leaf_start] << std::endl;

    return node-leaf_start;
  }

  Real total_weight() const {
    return BaseTree::value_of(1);  //root
  }

  Real& weight_of(PosType i) {
    return leaves[i];
  }

  Real get_weight(PosType i) const {
<<<<<<< Updated upstream
    assert(i >= 0 && i < static_cast<PosType>(leaf_count));
=======
    assert(i >= 0 && i < static_cast<PosType>(leaf_start));
>>>>>>> Stashed changes
    return leaves[i];
  }

  Real& weightsum_of(PosType p) {
    return BaseTree::value_of(p);
  }
  const Real& weightsum_of(PosType p) const {
    return const_cast<This*>(this)->weightsum_of(p);
  }

  PosType id_of(PosType p) { return leaf_start + p; }

<<<<<<< Updated upstream
  size_t get_leaf_count() const { return leaf_count; }

  size_t get_leaf_start() const { return leaf_start; }

private:
  size_t leaf_count;
=======
  size_t get_leaf_start() const { return leaf_start; }

private:
>>>>>>> Stashed changes
  size_t leaf_start;

  static size_t next_power_of_two(size_t n) {
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
  }
<<<<<<< Updated upstream


  //Real _total_weight = 0.0;
  std::vector<Real> leaves;

=======
  std::vector<Real> leaves;
>>>>>>> Stashed changes
};

}
}
#endif
