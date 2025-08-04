#ifndef LEAF_SUM_TREE
#define LEAF_SUM_TREE
#include "completetree.hpp"
#include <limits>
#include <random>
#include <cassert>
#include <ostream>
#include <istream>
#include <iostream>

namespace dense {
namespace stochastic {



//Class to randomly select an index where each index's probability of being
//selected is weighted by a given vector. Supports O(log(n)) selection and
//update while using 2n memory space, along with O(1) addition and removal.
template <class int_type = size_t, size_t precision = std::numeric_limits<Real>::digits>




class leaf_sum_tree : protected complete_tree<int_type, Real> {
using This = leaf_sum_tree<int_type, precision>;
using BaseTree = complete_tree<int_type, Real>;
using PosType = typename BaseTree::position_type;
using result_type = int_type;



class Param {
  public:
    Param(const std::vector<Real>& weights) : weights_(weights) {}
    Param(std::vector<Real>&& weights) : weights_(std::move(weights)) {}

    std::vector<Real> weights() const { return weights_; }

    bool operator==(const Param& other) const { return weights_ == other.weights_; }
    bool operator!=(const Param& other) const { return !(*this == other); }

  private:
    std::vector<Real> weights_;

    friend class leaf_sum_tree<int_type, precision>;
};


//Methods accessible to the end user
public:

  using param_type = Param;


////MEMBER FUNCTIONS////

  //Empty constructor, allows you to add later items to it
  leaf_sum_tree() : BaseTree(1), leaf_end_(0) {}

  //Param-compatible constructor, turns a vector of weights into iterators for the main constructor
  leaf_sum_tree(const std::vector<Real>& weights)
  : leaf_sum_tree(weights.begin(), weights.end()) {}

  //Two-entry compatible list constructor
  leaf_sum_tree(const std::initializer_list<Real>& il)
  : leaf_sum_tree(il.begin(), il.end()) {}

  //Main constructor - takes iterators to the start and end of an array of weights
  template< class InputIt >
  leaf_sum_tree(InputIt first, InputIt last) :
    BaseTree()
  {
    size_t n = std::distance(first, last);
    leaf_end_ = n;
    BaseTree::resize(2 * leaf_end_, 0.0); //double the size of the weights list (rounded to next power of two)
    //copy weights to leaves
    InputIt it = first;
    for (size_t i = 0; it != last; ++it, ++i) {
      weightsum_of(node_of(i)) = std::max(Real(*it),0.0);
    }

    //build sums from leaves up
    for (std::ptrdiff_t i = leaf_end_ - 1; i >= 1; --i) {
      weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
    }
  }

  //Reset doesn't need to do anything other than be callable
  void reset(){}

  //Compare operations
  bool operator==(const leaf_sum_tree& other) const {
    return this->param() == other.param();
  }

  bool operator!=(const leaf_sum_tree& other) const {
    return !(*this == other);
  }






////GENERATION////

  template<class URNG>
  PosType operator()(URNG& g) {
    Real target = std::generate_canonical<Real, precision, URNG>(g) * total_weight(); //Generate a target number between 0 and the sum of all the weights
    //if (target == 0) return 0;
    PosType node = BaseTree::root(); //node starts at 1
    while (node < leaf_end_) { //While the node is in the internal half of the tree:
      PosType left = left_of(node);
      Real left_sum = weightsum_of(left); //Get the value in the node's left child

      bool go_left = (target < left_sum); //If the target is less than the left child's sum, then move to the left child's node
      node = go_left ? left : right_of(node); //Else, if the larget is larger than the left child, then move to the right child's node, and...
      if (!go_left) target -= left_sum; //Subtract the left child's sum from the target.

    } //Repeat until the target is contained within one of the weights

    return (id_of(node));
  }

  //Param_type compatible
  template<class URNG>
  result_type operator()(URNG& g, const param_type& param) const {
    leaf_sum_tree temp(param.weights_);
    return temp(g); // Call the main operator() on temp
  }



////CHARACTERISTICS////

  //Returns a list of the probabilities of landing on each of the indexes
  std::vector<double> probabilities(){
    std::vector<double> probabilities(leaf_end_);
    double total_weight = this->total_weight();
    for (size_t i = leaf_end_, j = 0; i < BaseTree::size() && j < probabilities.size(); ++i, ++j){
      probabilities[j] = weightsum_of(i)/total_weight;
    }
    return probabilities;
  }

  //Param! Returns a struct of the current weights
  param_type param() const {
    std::vector<Real> weights(leaf_end_);
    for (size_t i = 0; i < leaf_end_; ++i)
      weights[i] = get_weight(i);
    return param_type(weights);
  }

  //Turns an existing tree into a tree with leaves containing only the weights given in p
  void param(const param_type& p) {
    *this = leaf_sum_tree(p.weights());
  }

  //Returns the minimum index possible (always 0)
  static constexpr result_type min() { return 0; }

  //Returns the highest index possible
  result_type max() const { return leaf_end_-1;}



////NON-MEMBER FUNCTIONS////

  //WEIGHT ACCESS//

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

  //This function exists as a clone of update_weight() to skip calculating node_of(i), which for pushing or popping repeatedly, can make a big difference
  void update_weight_internal(PosType i, Real new_weight) {
    assert(new_weight >= 0);
    Real weight_diff = new_weight - weightsum_of(i);
    while (i != BaseTree::root()) {
      weightsum_of(i) += weight_diff;
      i = BaseTree::parent_of(i);
    }
    weightsum_of(i) += weight_diff;
  }

  //Gets a leaf's weight. Needed for this class because weights can be changed, and so
  //the weights in the class can be different from those originally inputted
  Real get_weight(PosType i) const {
    assert(i >= 0 && i < static_cast<PosType>(leaf_end_));
    return weightsum_of(node_of(i));
  }


  //PUSHING AND POPPING//


  //Adds a weight to the tree using update_weight_internal().
  void push_back(Real& new_weight) {
    size_t new_leaf_end = leaf_end_ + 1;
    BaseTree::add_entry(weightsum_of(leaf_end_));
    BaseTree::add_entry(0.0);
    update_weight_internal(new_leaf_end*2-1, new_weight);
    leaf_end_ = new_leaf_end;
  }

  //Adds a weight to the tree using update_weight_internal().
  void push_back(Real&& new_weight) {
    size_t new_leaf_end = leaf_end_ + 1;
    BaseTree::add_entry(weightsum_of(leaf_end_));
    BaseTree::add_entry(0.0);
    update_weight_internal(new_leaf_end*2-1, new_weight);
    leaf_end_ = new_leaf_end;
  }

  //Overload to allow giving a direct vector object reference to push_back()
  void push_back(std::vector<Real>& weights) {
    push_back(weights.begin(), weights.end());
  }

  //Overload to allow giving a direct vector object reference to push_back()
  void push_back(std::vector<Real>&& weights) {
    push_back(weights.begin(), weights.end());
  }

  //Allows a vector to be inserted into the tree all at once.
  //Uses efficient tree updating - skips updating nodes multiple times, and
  //is thus more efficient than just calling the base function repeatedly.
  template <typename It>
  void push_back(It first, It last) {
    size_t count = std::distance(first, last);
    size_t new_leaf_end = leaf_end_ + count;

    size_t pos = leaf_end_;
    if (pos == 0){
      BaseTree::add_entry(*first);
      ++first;
      pos = 1;
    }
    for (; first != last; ++first, pos += 1){
      BaseTree::add_entry(weightsum_of(pos));
      BaseTree::add_entry(*first);
    }
    size_t total = BaseTree::size();
    size_t first_parent = (total - 2 * count) >> 1;
    size_t last_parent  = (total - 1) >> 1;

    while (last_parent > 0) {
      for (size_t i = first_parent; i <= last_parent; ++i) {
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
    }
    first_parent >>= 1;
    last_parent  >>= 1;
  }

  leaf_end_ = new_leaf_end;
  }

  //Removes the last inserted weight (whatever's at the end of the input array).
  void pop_back() {
    if (leaf_end_ == 0) return;
    size_t item_to_remove_idx = BaseTree::size() - 1;
    update_weight_internal(item_to_remove_idx, 0.0);
    Real value = weightsum_of(item_to_remove_idx-1);
    weightsum_of(parent_of(item_to_remove_idx-1)) = value;
    leaf_end_ -= 1;
    BaseTree::remove_last_entry();
    BaseTree::remove_last_entry();
  }

  //Allows a number of back nodes to be deleted from the tree all at once.
  //Uses efficient tree updating - skips updating nodes multiple times, and
  //is thus more efficient than just calling the base function repeatedly.
  void pop_back(size_t count) {
    if (count > leaf_end_) count = leaf_end_;
    size_t new_leaf_end = leaf_end_ - count;

    for (size_t i = 0; i < count; ++i) {
      PosType right_child = BaseTree::size() - 1 - 2 * i;
      weightsum_of(right_child) = 0.0;
    }

    size_t first = (BaseTree::size() - 2 * count) >> 1;
    size_t last  = (BaseTree::size() - 1) >> 1;

    while (last > 0) {
      for (size_t i = first; i <= last; ++i)
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
      first >>= 1;
      last  >>= 1;
    }

    leaf_end_ = new_leaf_end;

    for (size_t i = 0; i < 2 * count; ++i) {
      BaseTree::remove_last_entry();
    }
  }

  //SIZE METHODS//

  //Return the number of entries, aka leaves
  size_t size() const {return leaf_end_;}


  //Return the value stored in the root, which is the sum of all values in the tree
  Real total_weight() const noexcept {
    return weightsum_of(1);  //root
  }

//Methods only for use by the class itself
private:

  //Returns a user-visible ID from a leaf node. Goes through the swizzling process
  PosType node_of(size_t p) const {
  PosType node = 2*p + 1;
  PosType depthDifference = __builtin_clz(node) - __builtin_clz(BaseTree::size()-1);
  node = node<<depthDifference;
  node = (node > (BaseTree::size() - 1)) ? (node >> 1) : node; //Hopefully the compiler can recognize that this can be a CMOV instead of a branch
  return node;
  }

  //Gets an internal tree node from a user-visible ID, assuming the tree is swizzled
  PosType id_of(result_type i) const {
    PosType node = i>>std::countr_zero(i);
    return (node>>1);
  }


  //Raw value in any given tree (not user) index. Is a weight if >= leaf_start_, is a weightsum if less than leaf_start_
  Real& weightsum_of(PosType p) {
    return BaseTree::value_of(p);
  }

  //Const version
  const Real& weightsum_of(PosType p) const {
    return const_cast<This*>(this)->weightsum_of(p);
  }


  //Returns the next largest power of two. Used in constructing the tree, either in the constructor or when rebuilding to be larger or smaller
  static size_t next_power_of_two(size_t n) {
    if (n == 0) return 1;
    return 1ULL << (64 - __builtin_clzll(n - 1));
  }


  //Position functions used to navigate the tree.
  static PosType left_of(PosType i) { return BaseTree::left_of(i);}
  static PosType right_of(PosType i) { return BaseTree::right_of(i);}
  static PosType parent_of(PosType i) { return BaseTree::parent_of(i);}

  //Used for keeping track of indexing properly.
  size_t leaf_end_;
};


  //Function to return a stream output of the entirety of the given tree object. The first item in the stream is the total number of weights.
  template <typename int_type, size_t precision>
  std::ostream& operator<<(std::ostream& os, const leaf_sum_tree<int_type, precision>& dist) {
    std::ios_base::fmtflags old_flags = os.flags();
    char old_fill = os.fill();

    os.setf(std::ios_base::dec | std::ios_base::left, std::ios_base::basefield | std::ios_base::adjustfield);
    os.fill(' ');

    const auto weights = dist.param().weights();
    os << weights.size();
    for (const auto& w : weights) {
      os << ' ' << w;
    }

    os.flags(old_flags);
    os.fill(old_fill);
    return os;
  }


  //Function to construct a tree object from a string input.
  template <typename int_type, size_t precision>
  std::istream& operator>>(std::istream& is, leaf_sum_tree<int_type, precision>& dist) {
    std::ios_base::fmtflags old_flags = is.flags();
    is.setf(std::ios_base::dec, std::ios_base::basefield);

    size_t n;
    is >> n;
    if (!is) {
      is.flags(old_flags);
      return is;
    }

    std::vector<double> weights(n);
    for (size_t i = 0; i < n; ++i) {
      is >> weights[i];
      if (!is) {
        is.setstate(std::ios::failbit);
        is.flags(old_flags);
        return is;
      }
    }

    dist.param(weights);

    is.flags(old_flags);
    return is;
  }


}
}
#endif
