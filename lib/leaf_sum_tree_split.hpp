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
    leaf_start = next_power_of_two(n);
    leaf_end = n;
    BaseTree::resize(leaf_start, 0.0); //stores internal tree nodes, at the size of the weights list (rounded to next power of two)
    leaves.resize(leaf_end, 0.0); //stores leaf nodes with actual weights
    //copy weights to leaves
    InputIt it = first;
    for (size_t i = 0; it != last; ++it, ++i) {
      leaves[i] = Real(*it);
    }
    rebuild_full();
  }

  void update_weight(PosType i, Real new_weight) {
    Real weight_diff = new_weight - leaves[i];
    leaves[i] = new_weight;
    i += leaf_start;
    while (i > BaseTree::root()) {
      i = BaseTree::parent_of(i);
      weightsum_of(i) += weight_diff;
    }
  }

  template<class URNG>
  PosType operator()(URNG& g) {
    Real target = std::generate_canonical<Real, precision, URNG>(g) * total_weight();

    PosType node = BaseTree::root();

    while (node < leaf_start/2) {
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
    assert(i >= 0 && i < static_cast<PosType>(leaf_start));
    return leaves[i];
  }

  Real& weightsum_of(PosType p) {
    return BaseTree::value_of(p);
  }
  const Real& weightsum_of(PosType p) const {
    return const_cast<This*>(this)->weightsum_of(p);
  }

  void push_back(Real new_weight) {
    size_t new_leaf_end = leaf_end + 1;
    if (new_leaf_end >= leaf_start) {
      size_t old_leaf_start = leaf_start;
      leaf_start *= 2;

      BaseTree::resize(leaf_start, 0.0);
      leaves.resize(new_leaf_end, 0.0);
      leaves[leaf_end] = new_weight;
      leaf_end = new_leaf_end;
      rebuild_full();
    }
    else{
      leaves.resize(new_leaf_end, 0.0);
      update_weight(leaf_end, new_weight);
      leaf_end = new_leaf_end;
    }
  }

  //This resizes leaves before anything else - so don't touch the deleted entries, just let the .resize handle it.
  void pop_back() {
    if (leaf_end == 0) return;
    size_t new_leaf_end = leaf_end - 1;
    update_weight(new_leaf_end, 0.0);
    leaves.resize(new_leaf_end);
    auto checkSize = next_power_of_two(new_leaf_end);

    if (checkSize < leaf_start) {
      leaf_start = checkSize;
      leaf_end = new_leaf_end;
      BaseTree::resize(leaf_start, 0.0);
      rebuild_full();
    }
    else {
      leaf_end = new_leaf_end;
    }
  }

  template <typename It>
  void push_back(It first, It last) {
    size_t count = std::distance(first, last);
    size_t old_leaf_end = leaf_end;
    size_t new_leaf_end = leaf_end + count;

    if (new_leaf_end > leaf_start) {
      leaf_start = next_power_of_two(new_leaf_end);
      BaseTree::resize(leaf_start, 0.0);
      leaves.resize(new_leaf_end, 0.0);

      for (size_t i = 0; first != last; ++i, ++first)
        leaves[old_leaf_end+i] = *first;

      leaf_end = new_leaf_end;

      rebuild_full();
    }
    else{
      leaves.resize(new_leaf_end);

      for (size_t i = 0; first != last; ++i, ++first)
        leaves[old_leaf_end+i] = *first;

      leaf_end = new_leaf_end;

      rebuild_partial(old_leaf_end, new_leaf_end - 1); //Rebuild the affected part of the parent tree
    }
  }

  //This resizes leaves before anything else - so don't touch the deleted entries, just let the .resize handle it.
  void pop_back(size_t count) {
    if (count == 0) return;
    if (count > leaf_end) count = leaf_end;
    size_t old_leaf_end = leaf_end;
    size_t new_leaf_end = leaf_end - count;
    leaves.resize(new_leaf_end);

    auto checkSize = next_power_of_two(new_leaf_end);
    if (checkSize < leaf_start) {
      leaf_start = checkSize;
      leaf_end = new_leaf_end;
      BaseTree::resize(leaf_start, 0.0);
      rebuild_full();
    }
    else{

      leaf_end = new_leaf_end;

      rebuild_partial(new_leaf_end, old_leaf_end-1);
    }
  }

  //To make a new tree
  void rebuild_full() {
    for (std::ptrdiff_t i = leaf_start - 1; i >= 1; --i) {  //Iterates backwards from the last internal node up to the root
      size_t li = BaseTree::left_of(i);
      size_t ri = BaseTree::right_of(i);

      Real left = get_child_value(li);
      Real right = get_child_value(ri);

      BaseTree::value_of(i) = left + right;
    }
  }

  //To update a stretch of the tree
  void rebuild_partial(size_t first, size_t last) {
    if (last < first) return;

    size_t first_pos = leaf_start + first;
    size_t last_pos  = leaf_start + last;

    while (first_pos > 1) {
      first_pos /= 2;
      last_pos  /= 2;

      for (size_t i = first_pos; i <= last_pos; ++i) {
        size_t li = BaseTree::left_of(i);
        size_t ri = BaseTree::right_of(i);

        Real left = get_child_value(li);
        Real right = get_child_value(ri);

        BaseTree::value_of(i) = left + right;
      }
    }
  }

  //Safety checker, pretty much - enables leaves to be physically less than a full next power of two
  Real get_child_value(size_t index) const {
    if (index >= leaf_start) { //It's a leaf node position!
      if (index - leaf_start < leaf_end) {
        //It's an active leaf!
        return leaves[index - leaf_start];
      } else {
        //It's an inactive leaf beyond leaf_end, its weight is 0
        return 0.0;
      }
    } else { //It's an internal node.
      if (index < BaseTree::capacity()) {
        return BaseTree::value_of(index);
      }
      return 0.0;
    }
  }



  size_t get_leaf_start() const { return leaf_start; }

private:
  size_t leaf_start;
  size_t leaf_end;

  static size_t next_power_of_two(size_t n) {
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
  }
  std::vector<Real> leaves;
};

}
}
#endif
