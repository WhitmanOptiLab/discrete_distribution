#ifndef LEAF_SUM_TREE
#define LEAF_SUM_TREE
#include "completetree.hpp"
#include <limits>
#include <random>
#include <cassert>
#include <ostream>
#include <istream>

namespace dense {
namespace stochastic {



//Class to randomly select an index where each index's probability of being
//selected is weighted by a given vector. Supports O(log(n)) selection and
//update while using 2n memory space, along with O(1) addition and removal
//while size stays within the range of the nearest powers of two. If size
//has to expand beyond that or can contract below that, then the structure
//has to be rebuilt, resulting in an O(2n) cost. Construction itself always
//takes O(2n) time.
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
  leaf_sum_tree() : BaseTree(1), leaf_start_(1), leaf_end_(0) {}

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
    leaf_start_ = next_power_of_two(n);
    leaf_end_ = n;
    BaseTree::resize(2 * leaf_start_, 0.0); //double the size of the weights list (rounded to next power of two)
    //copy weights to leaves
    InputIt it = first;
    for (size_t i = 0; it != last; ++it, ++i) {
      BaseTree::value_of(leaf_start_ + i) = std::max(Real(*it),0.0);
    }

    //build sums from leaves up
    for (std::ptrdiff_t i = leaf_start_ - 1; i >= 1; --i) {
      BaseTree::value_of(i) = BaseTree::value_of(2 * i) + BaseTree::value_of(2 * i + 1);
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
    Real target = std::generate_canonical<Real, precision, URNG>(g) * total_weight();
    if (target == 0) return 0;

    PosType node = BaseTree::root();
    if (leaf_end_ > 65536){
    while (node < leaf_start_) {
      //if (BaseTree::value_of(node) == 0) std::cout << node << " " << target << " " << BaseTree::value_of(1) << std::endl;
      //std::cout << "Target: " << target << std::endl;

      PosType left = left_of(node);
      //std::cout << node << " left weight: " << weightsum_of(left) << ", right weight: " << weightsum_of(BaseTree::right_of(node)) << std::endl;
      Real left_sum = weightsum_of(left);

      bool go_left = (target < left_sum);
      node = go_left ? left : right_of(node);
      target -= go_left ? 0 : left_sum;
      //std::cout << std::endl;
    }
    //std::cout << "Returning node " << node << ", which has weight: " << weightsum_of(node) << std::endl;
    }
    else{
      while (node < leaf_start_) {
        PosType left = left_of(node);
        PosType right = right_of(node);
        Real left_sum = weightsum_of(left);

        unsigned int go_left = static_cast<unsigned int>(target < left_sum);

        node = left * go_left + right * (1 - go_left);
        target -= (1 - go_left) * left_sum;
      }

    }

    return id_of(node);
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
    for (size_t i = leaf_start_, j = 0; i < BaseTree::size() && j < probabilities.size(); ++i, ++j){
      probabilities[j] = BaseTree::value_of(i)/total_weight;
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
    *this = leaf_sum_tree(p.weights()); //reconstruct the tree from new weights
  }

  //Returns the minimum index possible (always 0)
  static constexpr result_type min() { return 0; }

  //Returns the highest index possible
  result_type max() const { return leaf_end_-1;}



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
    assert(i >= 0 && i < static_cast<PosType>(leaf_end_));
    return BaseTree::value_of(leaf_start_ + i);
  }

  void push_back(std::vector<Real> weights) {
    push_back(weights.begin(), weights.end());
  }

  //Adds a weight to the tree. If the tree is large enough to handle it, the weight is just added, but otherwise the tree has to double its size (costly).
  void push_back(Real& new_weight) {
    size_t new_leaf_end = leaf_end_ + 1;
    if (new_leaf_end > leaf_start_) {
      std::vector<Real> all_weights;
      all_weights.reserve(new_leaf_end);
      for(size_t i = 0; i < leaf_end_; ++i) {
        all_weights.push_back(get_weight(i));
      }
      all_weights.push_back(new_weight);

      leaf_start_ = next_power_of_two(new_leaf_end);
      leaf_end_ = new_leaf_end;
      BaseTree::resize(2 * leaf_start_, 0.0);
      for(size_t i = 0; i < leaf_end_; ++i) {
        weightsum_of(leaf_start_ + i) = all_weights[i];
      }
      //Full tree rebuild
      for (std::ptrdiff_t i = leaf_start_ - 1; i >= 1; --i) {
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
      }
    } else {
      update_weight(leaf_end_, new_weight);
      ++leaf_end_;
    }
  }

  //Adds a weight to the tree. If the tree is large enough to handle it, the weight is just added, but otherwise the tree has to double its size (costly).
  void push_back(Real&& new_weight) {
    size_t new_leaf_end = leaf_end_ + 1;
    if (new_leaf_end > leaf_start_) {
      std::vector<Real> all_weights;
      all_weights.reserve(new_leaf_end);
      for(size_t i = 0; i < leaf_end_; ++i) {
        all_weights.push_back(get_weight(i));
      }
      all_weights.push_back(new_weight);

      leaf_start_ = next_power_of_two(new_leaf_end);
      leaf_end_ = new_leaf_end;
      BaseTree::resize(2 * leaf_start_, 0.0);
      for(size_t i = 0; i < leaf_end_; ++i) {
        weightsum_of(leaf_start_ + i) = all_weights[i];
      }
      //Full tree rebuild
      for (std::ptrdiff_t i = leaf_start_ - 1; i >= 1; --i) {
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
      }
    } else {
      update_weight(leaf_end_, new_weight);
      ++leaf_end_;
    }
  }

  //Removes the last inserted weight (whatever's at the end of the input array).
  //If the new size is half or less of the tree's current memory size, then the tree will rebuild to shrink to that size.
  void pop_back() {
    if (leaf_end_ == 0) return;

    size_t item_to_remove_idx = leaf_end_ - 1;

    //First, set the weight to 0. This updates parents.
    update_weight(item_to_remove_idx, 0.0);
    --leaf_end_;

    //Now, check if we should shrink the tree's capacity. If not, there's no more work to be done! Yay! If yes, well, lots of copying.
    size_t checkSize = next_power_of_two(leaf_end_);
    if (checkSize < leaf_start_) {
      std::vector<Real> active_weights;
      active_weights.reserve(leaf_end_);
      for (size_t i = 0; i < leaf_end_; ++i) {
        active_weights.push_back(get_weight(i));
      }
      leaf_start_ = checkSize;
      BaseTree::resize(2 * leaf_start_, 0.0);
      for (size_t i = 0; i < leaf_end_; ++i) {
        weightsum_of(leaf_start_ + i) = active_weights[i];
      }
      for (std::ptrdiff_t i = leaf_start_ - 1; i >= 1; --i) {
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
    size_t new_leaf_end = leaf_end_ + count;

    if (new_leaf_end > leaf_start_) {
      //Full rebuild required :<
      std::vector<Real> new_leaves;
      new_leaves.reserve(new_leaf_end);
      for (size_t i = 0; i < leaf_end_; ++i)
        new_leaves.push_back(weightsum_of(leaf_start_ + i));
      for (; first != last; ++first)
        new_leaves.push_back(*first);

      leaf_start_ = next_power_of_two(new_leaf_end);
      leaf_end_ = new_leaf_end;
      BaseTree::resize(2 * leaf_start_, 0.0);

      for (size_t i = 0; i < leaf_end_; ++i)
        weightsum_of(leaf_start_ + i) = new_leaves[i];

      for (std::ptrdiff_t i = leaf_start_ - 1; i >= 1; --i)
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
    } else {
      //No resize! Partial tree update
      size_t pos = leaf_end_;
      for (; first != last; ++first)
        weightsum_of(leaf_start_ + pos++) = *first;

      size_t first = leaf_start_ + leaf_end_;
      size_t last  = leaf_start_ + new_leaf_end - 1;

      while (first > 1) {
        first >>= 1;
        last  >>= 1;
        for (size_t i = first; i <= last; ++i)
          weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
      }
      leaf_end_ = new_leaf_end;
    }
  }

  //Allows a number of back nodes to be deleted from the tree all at once.
  //Uses efficient tree updating - skips updating nodes multiple times, and
  //is thus more efficient than just calling the base function repeatedly.
  void pop_back(size_t count) {
    if (count > leaf_end_) count = leaf_end_;
    size_t new_leaf_end = leaf_end_ - count;

    for (size_t i = new_leaf_end; i < leaf_end_; ++i)
      weightsum_of(leaf_start_ + i) = 0.0;

    auto checkSize = next_power_of_two(new_leaf_end);
    if (checkSize < leaf_start_) {
      //Have to shrink tree for memory efficiency's sake - requires recalculating entire tree though
      std::vector<Real> kept_leaves(new_leaf_end);
      for (size_t i = 0; i < new_leaf_end; ++i)
        kept_leaves[i] = weightsum_of(leaf_start_ + i);

      leaf_start_ = checkSize;
      leaf_end_ = new_leaf_end;
      BaseTree::resize(2 * leaf_start_, 0.0);

      for (size_t i = 0; i < leaf_end_; ++i)
        weightsum_of(leaf_start_ + i) = kept_leaves[i];

      for (std::ptrdiff_t i = leaf_start_ - 1; i >= 1; --i)
        weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
    } else {
      //Not possible to shrink tree; so we just zero out deleted sections
      size_t first = leaf_start_ + new_leaf_end;
      size_t last  = leaf_start_ + leaf_end_ - 1;

      while (first > 1) {
        first >>= 1;
        last  >>= 1;
        for (size_t i = first; i <= last; ++i)
          weightsum_of(i) = weightsum_of(2 * i) + weightsum_of(2 * i + 1);
      }
      leaf_end_ = new_leaf_end;
    }
  }

  size_t size() const {return leaf_end_;}


  //Return the value stored in the root, which is the sum of all values in the tree
  Real total_weight() const noexcept {
    return BaseTree::value_of(1);  //root
  }

//Methods only for use by the class itself
private:


  //Raw value in any given tree (not user) index. Is a weight if >= leaf_start_, is a weightsum if less than leaf_start_
  Real& weightsum_of(PosType p) {
    return BaseTree::value_of(p);
  }

  //Const version
  const Real& weightsum_of(PosType p) const {
    return const_cast<This*>(this)->weightsum_of(p);
  }

  //Gets a user-visible ID from a leaf node
  PosType id_of(PosType p) {
    assert(p>=leaf_start_);
    return p-leaf_start_; }

  //Gets an internal tree node from a user-visible ID
  PosType node_of(result_type i){
    assert(i>=0 && i<=leaf_end_);
    return leaf_start_+i;}

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
  size_t leaf_start_;
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
