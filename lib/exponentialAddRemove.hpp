#ifndef EXPONENTIAL_LEAF_SUM_TREE
#define EXPONENTIAL_LEAF_SUM_TREE
#include <vector>
#include <limits>
#include <random>
#include <cassert>
#include <iostream>
#include <queue>
#include <cmath>
#include <stdexcept>
#include <string>
#include <bit>

namespace dense {
namespace stochastic {

  template <class int_type = size_t, class Real = double, size_t fanout = 8>
class kary_complete_tree {
public:

  // Compute minimal complete k-ary tree size to store exactly n leaves
  // without needing bounds checks during selection.
  std::pair<size_t, size_t> minimal_tree_shape(size_t n) {
    // Returns {total_nodes_excluding_root, leaf_start_index}
    if (n == 0) return {0, 0}; // no internal nodes, no leaves

    size_t k = std::countr_zero(fanout);
    size_t max_leaf = size_t(1) << (((std::bit_width(n - 1) + k - 1) / k) * k);

    size_t leaf_start = 0;
    size_t level = 0;
    //size_t max_leaf = std::pow(fanout, std::ceil(std::log(n) / std::log(fanout))); //highest possible # of leaves in this tree structure;
    while (true) {
        level = first_child_of(level);
        if (level >= max_leaf) break;
        leaf_start += level - leaf_start;
    }

    int total_nodes = (-1*(0-leaf_start)+n);
    return {total_nodes, leaf_start};
  }


  static_assert((fanout & (fanout - 1)) == 0, "fanout must be power of two");

  using position_type = int_type;

  kary_complete_tree() : data_(1, Real(0)), leaf_start_(0), leaf_count_(0) {}

  explicit kary_complete_tree(size_t leaf_count) {
      resize(leaf_count);
  }

    // Resize to accommodate exactly n leaves
    void resize(size_t n) {
        if (n == 0) {
            data_.assign(1, Real(0));
            leaf_start_ = 0;
            leaf_count_ = 0;
            return;
        }

        auto [total_nodes, leaf_start] = minimal_tree_shape(n);

        // allocate all internal nodes + leaves (root is kept separately by the derived class)
        data_.resize(total_nodes);
        leaf_count_ = n;
        leaf_start_ = leaf_start;
        //std::cout << "base tree leafstart: " << leaf_start_ << std::endl;
    }



    size_t size() const {
        return data_.size();
    }

    Real& value_of(position_type p) {
        if(p >= data_.size()) {
          throw std::runtime_error("trying to access position " + std::to_string(p) + " in tree of size " + std::to_string(data_.size()));
        }

        return data_[p];
    }

    const Real& value_of(position_type p) const {
        assert(p < data_.size());
        return data_[p];
    }

    // Tree navigation (0-based indexing)
    position_type root() const {
        return 0;
    }

    position_type parent_of(position_type i) const {
        assert(i > 0);
        return (i >> log2_fanout) - 1;
    }

    position_type first_child_of(position_type i) const {
        return (i + 1) << log2_fanout;
    }

    position_type child_index(position_type parent, size_t child_num) const {
        return first_child_of(parent) + child_num;
    }

    bool is_leaf(position_type i) const {
        return i >= leaf_start_;
    }

    size_t leaf_start() const {
        return leaf_start_;
    }

    size_t leaf_count() const {
        return leaf_count_;
    }

    std::vector<Real> &data() {return data_;}

private:
    std::vector<Real> data_;
    size_t leaf_start_;
    size_t leaf_count_;
    size_t max_leaf_;

    static constexpr size_t log2_fanout = [] {
        size_t v = fanout;
        size_t r = 0;
        while (v > 1) {
            v >>= 1;
            ++r;
        }
        return r;
    }();
};


//Class to randomly select an index where each index's probability of being
//selected is weighted by a given vector. Supports O(log(n)) selection and
//update while using about O(n) memory space.
// Uses exponential tree with fixed fanout.
template <
  class int_type = size_t,
  class Real = double,
  size_t fanout = 8,
  size_t precision = std::numeric_limits<Real>::digits
>
class exponential_leaf_sum_tree : protected kary_complete_tree<int_type, Real, fanout> {
  using This = exponential_leaf_sum_tree<int_type, Real, fanout, precision>;
  using BaseTree = kary_complete_tree<int_type, Real, fanout>;
  using PosType = typename BaseTree::position_type;

public:
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
    friend class exponential_leaf_sum_tree<int_type, Real, fanout, precision>;
  };

  // Default constructor
  exponential_leaf_sum_tree() : BaseTree(1), leaf_end_(0), leaf_start_(0) {}

  // Construct from vector
  explicit exponential_leaf_sum_tree(const std::vector<Real>& weights)
    : exponential_leaf_sum_tree(weights.begin(), weights.end()) {}

  // Construct from initializer list
  exponential_leaf_sum_tree(const std::initializer_list<Real>& il)
    : exponential_leaf_sum_tree(il.begin(), il.end()) {}

  // Construct from iterator pair
  template<class InputIt>
  exponential_leaf_sum_tree(InputIt first, InputIt last)
    : BaseTree()
  {
    size_t n = std::distance(first, last);

    // Round up leaves to nearest full complete k-ary tree level (power of fanout)
    size_t k = std::countr_zero(fanout);
    max_leaf_ = size_t(1) << (((std::bit_width(n - 1) + k - 1) / k) * k);
    leaf_start_ = BaseTree::minimal_tree_shape(n).second;
    BaseTree::resize(n); // <-- FIX: pass leaves, not total nodes
    leaf_end_ = n;

    // Copy weights to leaves, pad with zeros
    InputIt it = first;
    for (size_t i = leaf_start_; i < leaf_start_ + n; ++i) {
        weightsum_of(i) = std::max(Real(*it), Real(0));
        //std::cout << i << std::endl;
        ++it;
    }
    //fill the rest of the unused leaves with 0
    //for(size_t i = leaf_start_ + n; i < BaseTree::size() - 1; ++i) {
    //    weightsum_of(i) = Real(0);
    //}


    // Build sums bottom-up from leaves to root (excluding root)
    for (ptrdiff_t i = leaf_start_ - 1; i >= 0; --i) {
        Real sum = 0;
        PosType first_child = (i + 1) * fanout; // -1-index adjustment
        for (size_t c = 0; c < fanout; ++c) {
            PosType child = first_child + c;
            if (child >= leaf_start_+n) break;
            sum += weightsum_of(child);
        }
        weightsum_of(i) = sum;
    }

    // Compute root separately
    Real sum = 0;
    PosType first_child = 0 * fanout; // root's first child in array
    for (size_t c = 0; c < fanout; ++c) {
        PosType child = first_child + c;
        if (child >= max_leaf_) break;
        sum += weightsum_of(child);
    }
    total_weight_ = sum;
}

  // No-op reset
void reset() {}

bool operator==(const This& other) const {
    return this->param() == other.param();
}

bool operator!=(const This& other) const {
    return !(*this == other);
}

  // Select weighted random leaf index, fully uniform, no special root
template<class URNG>
result_type operator()(URNG& g) const {
    Real total = total_weight();
    if (total <= Real(0)) return 0;
    Real target = std::generate_canonical<Real, precision, URNG>(g) * total;
    if (target == Real(0)) return 0;
    PosType first_child = 0;

    // Start at the top internal node (index 0)
    PosType node = 0;

    while (true) {  // while node is an internal
        Real cumulative = 0;
        //std::cout << target << std::endl;

        size_t chosen_child = fanout; // sentinel
        for (size_t c = 0; c < fanout; ++c) {
            PosType child = first_child + c;
            if (child >= BaseTree::size()) break;
            //std::cout << "Cumulative: " << cumulative << std::endl;
            //std::cout << child << " value: " << weightsum_of(child) << std::endl;
            Real w = weightsum_of(child);
            if (target < cumulative + w) {
              //std::cout << "Scueses!!" << std::endl;
                target -= cumulative;
                node = child;          // move to next node at end of loop
                chosen_child = c;
                break;
            }
            cumulative += w;
        }


        if (chosen_child == fanout) {
            // all children zero or none fit, stop at current node
            break;
        }
        first_child = BaseTree::first_child_of(node); // first child in array
        if (first_child >= BaseTree::size()) break;

        // otherwise, node has been updated to chosen_child
    }

    // node is now a leaf
    return static_cast<result_type>(node - leaf_start_);
}


  template<class URNG>
  result_type operator()(URNG& g, const Param& param) const {
    exponential_leaf_sum_tree temp(param.weights_);
    return temp(g);
  }

  // Return probabilities vector
  std::vector<Real> probabilities() const {
    std::vector<Real> probs(leaf_end_, Real(0));
    Real total = total_weight();
    if (total <= Real(0)) return probs;

    for (size_t i = 0; i < leaf_end_; ++i) {
      probs[i] = weightsum_of(leaf_start_ + i) / total;
    }
    return probs;
  }

  Param param() const {
    std::vector<Real> weights(leaf_end_);
    for (size_t i = 0; i < leaf_end_; ++i) {           // <-- NOTE: loop from 0..leaf_end_-1
      weights[i] = get_weight(i);
    }
    return Param(weights);
  }

  void param(const Param& p) {
    *this = exponential_leaf_sum_tree(p.weights_);
  }

  static constexpr result_type min() { return 0; }

  result_type max() const { return leaf_end_ == 0 ? 0 : leaf_end_ - 1; }

  // Update a leaf weight and propagate change upward
  void update_weight(PosType i, Real new_weight) {
    assert(new_weight >= Real(0));
    i = leaf_start_ + i;
    Real diff = new_weight - weightsum_of(i);
    weightsum_of(i) = new_weight;
    total_weight_ += diff;

    while (i > fanout) {                 // <-- FIX: propagate to the very top internal node
      i = BaseTree::parent_of(i);
      //std::cout << i << std::endl;
      weightsum_of(i) += diff;
    }
  }

  size_t get_max() const {
    return max_leaf_;
  }


  Real get_weight(PosType i) const {
    assert(i < leaf_end_);
    return weightsum_of(leaf_start_ + i);
  }

  size_t size() const { return leaf_end_; }

  Real total_weight() const noexcept {
    if (BaseTree::size() == 0) return Real(0);
    return total_weight_;
  }

  void push_back(Real weight) {
      //max_leaf_ *= fanout;
      //leaf_start_ = BaseTree::minimal_tree_shape(leaf_end_+1).second;
      expand(leaf_end_+1);   // <-- FIX: pass leaves, not total nodes

//      // (Optional now, but if you want to keep it, the bound must be total_nodes:)
//      for (ptrdiff_t i = leaf_start_ - 1; i >= 0; --i) {
//        Real sum = 0;
//        PosType first_child = (i + 1) * fanout; // -1-index adjustment
//        for (size_t c = 0; c < fanout; ++c) {
//            PosType child = first_child + c;
//            if (child >= max_leaf_ + leaf_start_) break;  // <-- FIX bound
//            sum += weightsum_of(child);
//        }
//        weightsum_of(i) = sum;
//        //std::cout << "updated node " << i << " with sum " << sum << std::endl;
//      }
    //if(leaf_end_+leaf_start_ >= BaseTree::size()) {
    //    BaseTree::resize(BaseTree::size()*2);
    //}
    //assert(leaf_end_+leaf_start_ < BaseTree::size());

    update_weight(leaf_end_, weight);
    leaf_end_++;
  }

  void pop_back() {
    assert(leaf_end_ > 0);
    leaf_end_--;
    update_weight(leaf_end_, Real(0));

  }

// Print tree for debugging (-1-indexed)
  void print_tree(std::ostream& os = std::cout) const {
    size_t total_nodes = BaseTree::size();

    if (total_nodes == 0) {
        os << "(empty tree)\n";
        return;
    }

    std::queue<ptrdiff_t> q; // use signed for -1 root
    q.push(-1); // -1 represents the root stored separately

    size_t level = 0;

    while (!q.empty()) {
        size_t level_size = q.size();
        os << "Level " << level << ": ";

        for (size_t i = 0; i < level_size; ++i) {
            ptrdiff_t node = q.front();
            q.pop();

            Real w = (node == -1) ? total_weight_ : weightsum_of(static_cast<PosType>(node));
            os << "[" << node << "]=" << w << "  ";

            // enqueue children
            PosType first_child = (node + 1) * fanout; // -1-index adjustment
            for (size_t c = 0; c < fanout; ++c) {
                PosType child = first_child + c;
                if (child >= total_nodes) break;
                q.push(child);
            }
        }

        os << "\n";
        ++level;
    }
  }

      // Expand from current leaf_count_ to new_leaf_count (must be larger), structurally, without recomputing
    // new expand: argument is NEW_LEAF_COUNT (number of leaves you want after expansion)
void expand(size_t new_leaf_count) {
    if (new_leaf_count <= leaf_end_) return; // nothing to do
    auto &data_ = BaseTree::data();
    if (new_leaf_count >= max_leaf_) {
      max_leaf_ *= fanout;

    // old shape
    auto [old_total_nodes, old_leaf_start] = BaseTree::minimal_tree_shape(leaf_end_);
    size_t old_total = old_total_nodes;

    // new shape
    auto [new_total_nodes, new_leaf_start] = BaseTree::minimal_tree_shape(new_leaf_count);
    leaf_start_ = new_leaf_start;


        // Creating the level lengths and level starts for the old size
        std::vector<size_t> old_starts;
        std::vector<size_t> old_levels;
        old_starts.push_back(0);
        size_t position = fanout;
        size_t sum = fanout;
        old_levels.push_back(fanout);
        while (true) {
            position = BaseTree::first_child_of(position);
            if (position >= old_total) break;
            std::cout << position << std::endl;
            old_starts.push_back(sum);
            size_t level = position - sum;
            sum += level;
            old_levels.push_back(level);
        }
        std::cout << sum << std::endl;
        old_starts.push_back(sum);
        old_levels.push_back(position-sum);



        // Creating the level lengths and level starts for the new size
        std::vector<size_t> new_starts;
        std::vector<size_t> new_levels;
        new_starts.push_back(0);
        position = fanout;
        sum = fanout;
        new_levels.push_back(fanout);
        while (true) {
            position = BaseTree::first_child_of(position);
            if (position >= new_total_nodes) break;
            std::cout << position << std::endl;
            new_starts.push_back(sum);
            size_t level = position - sum;
            sum += level;
            new_levels.push_back(level);
        }
        std::cout << sum << std::endl;
        new_starts.push_back(sum);
            new_levels.push_back(new_leaf_count);



    // allocate new array (zero-initialized)
    std::vector<Real> new_data(new_total_nodes, Real(0));
    //std::cout << new_data.size() << std::endl;

    // copy each old level block into the next-deeper level of the new layout.
    // old level i -> new level (i+1). That packs the old block contiguously at the start
    // of the larger new level (the remainder stays zero).
    for (size_t i = 0; i < old_levels.size(); ++i) {
        size_t old_start = old_starts[i];
        size_t old_sz    = old_levels[i];
        size_t new_level_index = i + 1; // destination level index
        size_t new_start = new_starts[new_level_index];

        std::copy_n(data_.begin() + old_start, old_sz, new_data.begin() + new_start);
    }

    // recompute only the new top internal layer (level 0) from its children (level 1)
    // top-level nodes occupy global indices new_starts[0] .. new_starts[0]+new_levels[0]-1
    // their first child indices can be computed with first_child_of(parent_index)
    size_t top_count = new_levels[0];
    size_t top_start = new_starts[0];   // usually 0
    for (size_t j = 0; j < top_count; ++j) {
        size_t parent_idx = top_start + j;
        // first child in global indexing:
        size_t first_child = BaseTree::first_child_of(parent_idx);
        Real sum = Real(0);
        for (size_t c = 0; c < fanout; ++c) {
            size_t child = first_child + c;
            if (child >= new_total_nodes) break;
            sum += new_data[child];
        }
        new_data[parent_idx] = sum;
    }


    // commit
    data_.swap(new_data);
    //std::cout << data_.size() << std::endl;
    }
    else if (new_leaf_count+leaf_start_ >= BaseTree::size()) {
      BaseTree::resize(new_leaf_count*2);
    }

    //leaf_end_ = new_leaf_count;
    //leaf_start_ = new_leaf_start;
}



private:
  Real& weightsum_of(PosType p) { return BaseTree::value_of(p); }
  const Real& weightsum_of(PosType p) const { return BaseTree::value_of(p); }
  size_t max_leaf_;
  size_t leaf_end_;   // number of leaves requested by user
  size_t leaf_start_; // index of first leaf in data_
  double total_weight_ = 0;
};
}
}
#endif
