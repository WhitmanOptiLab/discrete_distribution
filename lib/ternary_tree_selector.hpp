// ternary_leaf_sum_tree.hpp
#ifndef TERNARY_LEAF_SUM_TREE
#define TERNARY_LEAF_SUM_TREE

#include <vector>
#include <limits>
#include <random>
#include <cassert>
#include <cmath>
#include <iostream>

namespace dense {
namespace stochastic {

template <class int_type = size_t, size_t precision = std::numeric_limits<double>::digits>
class ternary_leaf_sum_tree {
  using Real = double;
  using PosType = size_t;
  using result_type = int_type;

public:
  class Param {
  public:
    Param(const std::vector<Real>& weights) : weights_(weights) {}
    Param(std::vector<Real>&& weights) : weights_(std::move(weights)) {}
    std::vector<Real> weights() const { return weights_; }
    bool operator==(const Param& other) const { return weights_ == other.weights_; }
    bool operator!=(const Param& other) const { return !(*this == other); }

  private:
    std::vector<Real> weights_;
    friend class ternary_leaf_sum_tree;
  };

  using param_type = Param;

#include <iostream>
#include <cmath>

  void print_tree() const {
    if (tree_.size() <= 1) return;

    auto level_start = [](int L) -> size_t {
      return 1 + (std::pow(3, L) - 1) / 2;
    };

    size_t i = 0;
    int level = 0;
    while (i < tree_.size()) {
      size_t start = level_start(level);
      size_t next_start = level_start(level + 1);
      std::cout << "Level " << level << ": ";
      for (; i < tree_.size() && i < next_start-1; ++i) {
        std::cout << tree_[i] << ' ';
      }
      std::cout << std::endl;
      ++level;
    }
  }



  ternary_leaf_sum_tree() : leaf_start_(0), leaf_end_(0) {}

  ternary_leaf_sum_tree(const std::vector<Real>& weights)
    : ternary_leaf_sum_tree(weights.begin(), weights.end()) {}

  template<class InputIt>
  ternary_leaf_sum_tree(InputIt first, InputIt last) {
    size_t n = std::distance(first, last);
    leaf_end_ = n;
    int tree_size = next_power_of_three(n);
    leaf_start_ = (tree_size)/2;
    tree_.assign(tree_size + leaf_start_, 0.0);

    InputIt it = first;
    for (size_t i = 0; it != last; ++i, ++it) {
      tree_[leaf_start_ + i] = std::max(Real(*it), 0.0);
    }

    //std::cout << tree_.size() << std::endl;
    for (std::ptrdiff_t i = leaf_start_ - 1; ; --i) {
      tree_[i] = tree_[child0(i)] + tree_[child1(i)] + tree_[child2(i)];
      if (i == 0) break;
    }
  }

  Real total_weight() const noexcept { return tree_[0]; }

  size_t size() const noexcept { return tree_.size(); }

  static constexpr result_type min() { return 0; }
  result_type max() const { return leaf_end_ - 1; }

  template<class URNG>
  result_type operator()(URNG& g) const {
    Real target = std::generate_canonical<Real, precision, URNG>(g) * total_weight();
    if (target == 0) return 0;
    PosType node = 0;
    //std::cout << "Target " << target << std::endl;
    while (node < leaf_start_) {
      Real w0 = tree_[child0(node)];
      Real w1 = tree_[child1(node)];
      //std::cout << "Target: " << target << ", Node: " << node << std::endl;
      //std::cout <<"Left: " << w0 << ", middle: " << w1 <<", right: " << tree_[child2(node)] << std::endl;
      if (target < w0) {
        node = child0(node);
      } else if ((target -= w0) < w1) {
        node = child1(node);
      } else {
        target -= w1;
        node = child2(node);
      }
    }
    //std::cout << "Returning: " << node-leaf_start_ << std::endl <<std::endl;
    return node - leaf_start_;
  }

  Real get_weight(PosType i) const {
    assert(i < leaf_end_);
    return tree_[leaf_start_ + i];
  }

  void update_weight(PosType i, Real new_weight) {
    PosType node = leaf_start_ + i;
    Real diff = new_weight - tree_[node];
    while (true) {
      tree_[node] += diff;
      if (node == 0) break;
      node = parent(node);
    }
  }

  param_type param() const {
    std::vector<Real> w(leaf_end_);
    for (size_t i = 0; i < leaf_end_; ++i)
      w[i] = get_weight(i);
    return param_type(w);
  }

  void param(const param_type& p) {
    *this = ternary_leaf_sum_tree(p.weights());
  }

private:
  std::vector<Real> tree_;
  size_t leaf_start_ = 0;
  size_t leaf_end_ = 0;

  static PosType child0(PosType i) { return 3 * i + 1; }
  static PosType child1(PosType i) { return 3 * i + 2 ; }
  static PosType child2(PosType i) { return 3 * i + 3; }
  static PosType parent(PosType i) { return (i-1) / 3; }

  static size_t next_power_of_three(size_t n) {
    size_t p = 1;
    while (p < n) p *= 3;
    return p;
  }
};

} // namespace stochastic
} // namespace dense

#endif // TERNARY_LEAF_SUM_TREE
