#ifndef EXPONENTIAL_LEAF_SUM_TREE
#define EXPONENTIAL_LEAF_SUM_TREE
#include <vector>
#include <limits>
#include <random>
#include <cassert>
#include <iostream>
#include <queue>

namespace dense {
namespace stochastic {

  template <class int_type = size_t, class Real = double, size_t fanout = CHILDNUM>
class kary_complete_tree {
public:

    // Compute minimal complete k-ary tree size to store exactly n leaves
    // without needing bounds checks during selection.
    static std::pair<size_t, size_t> minimal_tree_shape(size_t n) {
    // returns {total_nodes, leaf_start_index}
    if (n == 0) return {1, 0};

    size_t leaves = n;
    size_t total_nodes = n;  // count leaves
    std::vector<size_t> level_counts;
    level_counts.push_back(n);

    // Climb up until we reach the root
    while (leaves > 1) {
        size_t parents = (leaves + fanout - 1) / fanout; // ceil
        total_nodes += parents;
        level_counts.push_back(parents);
        leaves = parents;
    }

    // leaf_start is total_nodes - n (since leaves are last in array)
    size_t leaf_start = total_nodes - n;
    return {total_nodes, leaf_start};
}

    static_assert((fanout & (fanout - 1)) == 0, "fanout must be power of two");

    using position_type = int_type;

    kary_complete_tree() : data_(1, Real(0)), leaf_start_(0), leaf_count_(0) {}

    explicit kary_complete_tree(size_t leaf_count) {
        resize(leaf_count);
    }

    // Resize to accommodate at least n leaves
    void resize(size_t n) {
        if (n == 0) {
            data_.assign(1, Real(0));
            leaf_start_ = 0;
            leaf_count_ = 0;
            return;
        }

        auto [total_nodes, leaf_start] = minimal_tree_shape(n);
        data_.assign(total_nodes, Real(0));
        leaf_count_ = n;
        leaf_start_ = leaf_start;
    }

    size_t size() const {
        return data_.size();
    }

    Real& value_of(position_type p) {
        assert(p < data_.size());
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
        return (i - 1) >> log2_fanout;
    }

    position_type first_child_of(position_type i) const {
        return (i << log2_fanout) + 1;
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

private:
    std::vector<Real> data_;
    size_t leaf_start_;
    size_t leaf_count_;

    size_t log2_fanout = [] {
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
  size_t fanout = CHILDNUM,
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
    leaf_end_ = n;

    auto [total_nodes, leaf_start] = BaseTree::minimal_tree_shape(n);
BaseTree::resize(n); // will now use minimal_tree_shape internally
leaf_start_ = leaf_start;

    // Copy weights to leaves, pad with zeros
    InputIt it = first;
    for (size_t i = 0; i < leaf_start_; ++i) {
      if (i < n) {
        weightsum_of(leaf_start_ + i) = std::max(Real(*it), Real(0));
        ++it;
        }
    }

    // Build sums bottom-up from leaves to root
    for (PosType i = leaf_start_; i > 0; --i) {
      Real sum = Real(0);
      PosType first_child = BaseTree::first_child_of(i - 1); // parent's children start
      for (size_t c = 0; c < fanout; ++c) {
        PosType child = first_child + c;
        if (child >= total_nodes) break;
        sum += weightsum_of(child);
      }
      weightsum_of(i - 1) = sum;
    }
    // Compute root separately (i=0)
    {
      Real sum = Real(0);
      PosType first_child = BaseTree::first_child_of(0);
      for (size_t c = 0; c < fanout; ++c) {
        PosType child = first_child + c;
        if (child >= total_nodes) break;
        sum += weightsum_of(child);
      }
      weightsum_of(0) = sum;
    }
  }

  // No-op reset
  void reset() {}

  bool operator==(const This& other) const {
    return this->param() == other.param();
  }

  bool operator!=(const This& other) const {
    return !(*this == other);
  }

  // Select weighted random leaf index
  template<class URNG>
  result_type operator()(URNG& g) const {
    Real total = total_weight();
    if (total <= Real(0)) return 0;

    Real target = std::generate_canonical<Real, precision, URNG>(g) * total;
    if (target == Real(0)) return 0;

    PosType node = 0; // root index

    while (node < leaf_start_) {
      PosType first_child = BaseTree::first_child_of(node);
      Real cumulative = Real(0);

      size_t chosen_child = fanout; // invalid by default
      for (size_t c = 0; c < fanout; ++c) {
        PosType child = first_child + c;
        if (child >= BaseTree::size()) break;
        Real w = weightsum_of(child);
        if (target < cumulative + w) {
          target -= cumulative;
          node = child;
          chosen_child = c;
          break;
        }
        cumulative += w;
      }
      if (chosen_child == fanout) {
        // All children zero or no suitable child found
        break;
      }
    }

    // node is leaf, return index relative to leaf_start_
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
    for (size_t i = 0; i < leaf_end_; ++i) {
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

    while (i > 0) {
      i = BaseTree::parent_of(i);
      weightsum_of(i) += diff;
    }
  }

  Real get_weight(PosType i) const {
    assert(i < leaf_end_);
    return weightsum_of(leaf_start_ + i);
  }

  size_t size() const { return leaf_end_; }

  Real total_weight() const noexcept {
    if (BaseTree::size() == 0) return Real(0);
    return weightsum_of(0);
  }

  // Print tree for debugging
  void print_tree(std::ostream& os = std::cout) const {
    size_t total_nodes = BaseTree::size();

    if (total_nodes == 0) {
      os << "(empty tree)\n";
      return;
    }

    std::queue<PosType> q;
    q.push(0); // root

    size_t level = 0;

    while (!q.empty()) {
      size_t level_size = q.size();
      os << "Level " << level << ": ";

      for (size_t i = 0; i < level_size; ++i) {
        PosType node = q.front();
        q.pop();

        os << "[" << node << "]=" << weightsum_of(node) << "  ";

        PosType first_child = BaseTree::first_child_of(node);
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

private:
  Real& weightsum_of(PosType p) { return BaseTree::value_of(p); }
  const Real& weightsum_of(PosType p) const { return BaseTree::value_of(p); }

  size_t leaf_end_;   // number of leaves requested by user
  size_t leaf_start_; // index of first leaf in data_
};
}
}
#endif