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
    leaf_start = next_power_of_two(n);
    leaf_end = n;
    BaseTree::resize(2 * leaf_start, 0.0); //double the size of the weights list (rounded to next power of two)
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
    i=node_of(i);
    Real weight_diff = new_weight - weightsum_of(i);
    while (i >= BaseTree::root()) {
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
    
    return id_of(node);
  }

  Real total_weight() const {
    return BaseTree::value_of(1);  //root
  }

  Real& weight_of(PosType i) {
    return BaseTree::value_of(i);
  }

  Real get_weight(PosType i) const {
    assert(i >= 0 && i < static_cast<PosType>(leaf_start));
    return BaseTree::value_of(leaf_start + i);
  }

  Real& weightsum_of(PosType p) {
    return BaseTree::value_of(p);
  }
  const Real& weightsum_of(PosType p) const {
    return const_cast<This*>(this)->weightsum_of(p);
  }

  void push_back(Real& new_weight) {
    size_t new_leaf_end = leaf_end + 1;
    if (new_leaf_end > leaf_start) {
      std::vector<Real> all_weights;
      all_weights.reserve(new_leaf_end);
      for(size_t i = 0; i < leaf_end; ++i) {
        all_weights.push_back(get_weight(i));
      }
      all_weights.push_back(new_weight);

      leaf_start = next_power_of_two(new_leaf_end);
      leaf_end = new_leaf_end;
      BaseTree::resize(2 * leaf_start, 0.0);
      for(size_t i = 0; i < leaf_end; ++i) {
        weightsum_of(leaf_start + i) = all_weights[i];
      }
      //Full tree rebuild
      for (std::ptrdiff_t i = leaf_start - 1; i >= 1; --i) {
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
      }
    } else {
      update_weight(leaf_end, new_weight);
      ++leaf_end;
    }
  }

  void push_back(Real&& new_weight) {
    size_t new_leaf_end = leaf_end + 1;
    if (new_leaf_end > leaf_start) {
      std::vector<Real> all_weights;
      all_weights.reserve(new_leaf_end);
      for(size_t i = 0; i < leaf_end; ++i) {
        all_weights.push_back(get_weight(i));
      }
      all_weights.push_back(new_weight);

      leaf_start = next_power_of_two(new_leaf_end);
      leaf_end = new_leaf_end;
      BaseTree::resize(2 * leaf_start, 0.0);
      for(size_t i = 0; i < leaf_end; ++i) {
        weightsum_of(leaf_start + i) = all_weights[i];
      }
      //Full tree rebuild
      for (std::ptrdiff_t i = leaf_start - 1; i >= 1; --i) {
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
      }
    } else {
      update_weight(leaf_end, new_weight);
      ++leaf_end;
    }
  }

  void pop_back() {
    if (leaf_end == 0) return;

    size_t item_to_remove_idx = leaf_end - 1;

    //First, efficiently set the weight to 0. This updates parents.
    update_weight(item_to_remove_idx, 0.0);
    --leaf_end;

    //Now, check if we should shrink the tree's capacity. If not, there's no more work to be done! Yay! If yes, well, lots of copying.
    size_t checkSize = next_power_of_two(leaf_end);
    if (checkSize < leaf_start) {
      std::vector<Real> active_weights;
      active_weights.reserve(leaf_end);
      for (size_t i = 0; i < leaf_end; ++i) {
        active_weights.push_back(get_weight(i));
      }
      leaf_start = checkSize;
      BaseTree::resize(2 * leaf_start, 0.0);
      for (size_t i = 0; i < leaf_end; ++i) {
        weightsum_of(leaf_start + i) = active_weights[i];
      }
      for (std::ptrdiff_t i = leaf_start - 1; i >= 1; --i) {
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
      }
    }
  }

  //Allows a vector to be inserted into the tree all at once.
  //Uses efficient tree updating - skips updating nodes multiple times, and
  //is thus more efficient than just calling the base function repeatedly.
  template <typename It>
  void push_back(It first, It last) {
    size_t count = std::distance(first, last);
    size_t new_leaf_end = leaf_end + count;

    if (new_leaf_end > leaf_start) {
      //Full rebuild required :<
      std::vector<Real> new_leaves;
      new_leaves.reserve(new_leaf_end);
      for (size_t i = 0; i < leaf_end; ++i)
        new_leaves.push_back(weightsum_of(leaf_start + i));
      for (; first != last; ++first)
        new_leaves.push_back(*first);

      leaf_start = next_power_of_two(new_leaf_end);
      leaf_end = new_leaf_end;
      BaseTree::resize(2 * leaf_start, 0.0);

      for (size_t i = 0; i < leaf_end; ++i)
        weightsum_of(leaf_start + i) = new_leaves[i];

      for (std::ptrdiff_t i = leaf_start - 1; i >= 1; --i)
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
    } else {
      //No resize! Partial tree update
      size_t pos = leaf_end;
      for (; first != last; ++first)
        weightsum_of(leaf_start + pos++) = *first;

      size_t first = leaf_start + leaf_end;
      size_t last  = leaf_start + new_leaf_end - 1;

      while (first > 1) {
        first >>= 1;
        last  >>= 1;
        for (size_t i = first; i <= last; ++i)
          weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
      }
      leaf_end = new_leaf_end;
    }
  }

  //Allows a number of back nodes to be deleted from the tree all at once.
  //Uses efficient tree updating - skips updating nodes multiple times, and
  //is thus more efficient than just calling the base function repeatedly.
  void pop_back(size_t count) {
    if (count > leaf_end) count = leaf_end;
    size_t new_leaf_end = leaf_end - count;

    for (size_t i = new_leaf_end; i < leaf_end; ++i)
      weightsum_of(leaf_start + i) = 0.0;

    auto checkSize = next_power_of_two(new_leaf_end);
    if (checkSize < leaf_start) {
      //Have to shrink tree for memory efficiency's sake - requires recalculating entire tree though
      std::vector<Real> kept_leaves(new_leaf_end);
      for (size_t i = 0; i < new_leaf_end; ++i)
        kept_leaves[i] = weightsum_of(leaf_start + i);

      leaf_start = checkSize;
      leaf_end = new_leaf_end;
      BaseTree::resize(2 * leaf_start, 0.0);

      for (size_t i = 0; i < leaf_end; ++i)
        weightsum_of(leaf_start + i) = kept_leaves[i];

      for (std::ptrdiff_t i = leaf_start - 1; i >= 1; --i)
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
    } else {
      //Not possible to shrink tree; so we just zero out deleted sections
      size_t first = leaf_start + new_leaf_end;
      size_t last  = leaf_start + leaf_end - 1;

      while (first > 1) {
        first >>= 1;
        last  >>= 1;
        for (size_t i = first; i <= last; ++i)
          weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
      }
      leaf_end = new_leaf_end;
    }
  }

  PosType id_of(PosType p) { return p-leaf_start; }

  PosType node_of(IntType i){return leaf_start+i;}

  size_t get_leaf_start() const { return leaf_start; }

private:
  size_t leaf_start;
  size_t leaf_end;

  static size_t next_power_of_two(size_t n) {
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
  }
};

}
}
#endif
