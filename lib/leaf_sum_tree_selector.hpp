#ifndef LEAF_SUM_TREE
#define LEAF_SUM_TREE
#include "completetree.hpp"
#include <limits>
#include <random>
#include <cassert>

namespace dense {
namespace stochastic {


//Class to randomly select an index where each index's probability of being...
//...selected is weighted by a given vector.
template <class IntType = size_t, size_t precision = std::numeric_limits<Real>::digits>
class leaf_sum_tree : protected complete_tree<IntType, Real> {
using This = leaf_sum_tree<IntType, precision>;
using BaseTree = complete_tree<IntType, Real>;
using PosType = typename BaseTree::position_type;

//Methods accessible to the end user
public:



////MEMBER FUNCTIONS////

  //Empty constructor, allows you to add later items to it
  leaf_sum_tree() : BaseTree() {};

  //Main constructor - takes iterators to the start and end of an array of weights
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
    }

    //build sums from leaves up
    for (std::ptrdiff_t i = leaf_start - 1; i >= 1; --i) {
      BaseTree::value_of(i) = BaseTree::value_of(2 * i) + BaseTree::value_of(2 * i + 1);
    }
  }

  //Reset doesn't need to do anything other than be callable
  void reset();



////GENERATION////

  template<class URNG>
  PosType operator()(URNG& g) {
    Real target = std::generate_canonical<Real, precision, URNG>(g) * total_weight();

    PosType node = BaseTree::root();

    while (node < leaf_start) {
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



////CHARACTERISTICS////

  //Returns a list of the probabilities of landing on each of the indexes
  std::vector<double> probabilities(){
    std::vector<double> probabilities(leaf_end);
    double total_weight = BaseTree::value_of(1);
    for(int i = leaf_start, j = 0; i < BaseTree::size(); ++i, ++j){
      probabilities[j] = BaseTree::value_of(i)/total_weight;
    }
    return probabilities;
  }

  //TODO: param()
  //param_type param() const {}
  //void param(const param_type& params){}

  //Returns the minimum index possible (always 0)
  IntType min() const { return 0;}

  //Returns the highest index possible
  IntType max() const { return leaf_end-1;}



////NON-MEMBER FUNCTIONS////

  //Set a given leaf's weight to any positive real value
  void update_weight(PosType i, Real new_weight) {
    assert(new_weight >= 0);
    i=node_of(i);
    Real weight_diff = new_weight - weightsum_of(i);
    while (i != BaseTree::root()) {
      weightsum_of(i) += weight_diff;
      i = BaseTree::parent_of(i);
    }
    weightsum_of(i) += weight_diff;
  }

  //Gets a leaf's weight. Needed for this class because weights can be changed, and so...
  //...the weights in the class can be different from those originally inputted
  Real get_weight(PosType i) const {
    assert(i >= 0 && i < static_cast<PosType>(leaf_start));
    return BaseTree::value_of(leaf_start + i);
  }

  //Adds a weight to the tree. If the tree is large enough to handle it, the weight is just added, but otherwise the tree has to double its size (costly).
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

  //Adds a weight to the tree. If the tree is large enough to handle it, the weight is just added, but otherwise the tree has to double its size (costly).
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

  //Removes the last inserted weight (whatever's at the end of the input array).
  //If the new size is half or less of the tree's current memory size, then the tree will rebuild to shrink to that size.
  void pop_back() {
    if (leaf_end == 0) return;

    size_t item_to_remove_idx = leaf_end - 1;

    //First, set the weight to 0. This updates parents.
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



//Methods only for use by the class itself
private:

  //Return the value stored in the root, which is the sum of all values in the tree
  Real total_weight() const {
    return BaseTree::value_of(1);  //root
  }

  //Raw value in any given tree (not user) index. Is a weight if >= leaf_start, is a weightsum if less than leaf_start
  Real& weightsum_of(PosType p) {
    return BaseTree::value_of(p);
  }

  //Const version
  const Real& weightsum_of(PosType p) const {
    return const_cast<This*>(this)->weightsum_of(p);
  }

  //Gets a user-visible ID from a leaf node
  PosType id_of(PosType p) {
    assert(p>=leaf_start);
    return p-leaf_start; }

  //Gets an internal tree node from a user-visible ID
  PosType node_of(IntType i){
    assert(i>=0 && i<leaf_end);
    return leaf_start+i;}

  //Returns the next largest power of two. Used in constructing the tree, either in the constructor or when rebuilding to be larger or smaller
  static size_t next_power_of_two(size_t n) {
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
  }

  //Position functions used to navigate the tree.
  static PosType left_of(PosType i) { return BaseTree::left_of(i);}
  static PosType right_of(PosType i) { return BaseTree::right_of(i);}
  static PosType parent_of(PosType i) { return BaseTree::parent_of(i);}

  //Used for keeping track of indexing properly.
  size_t leaf_start;
  size_t leaf_end;
};

}
}
#endif
