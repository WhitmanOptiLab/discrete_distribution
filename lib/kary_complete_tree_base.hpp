#ifndef KARY_BASE
#define KARY_BASE
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
#include <algorithm>
#include <boost/align/aligned_allocator.hpp>




namespace dense {
namespace stochastic {
template <class int_type = size_t, class Real = double, size_t fanout = 16>
class complete_kary_complete_tree {
public:

  // Compute minimal complete k-ary tree size to store exactly n leaves
  // without needing bounds checks during selection. returns the index of the first leaf and the total number of leaves
  // Returns {total_nodes_excluding_root, leaf_start_index}
  std::pair<size_t, size_t> minimal_tree_shape(size_t n) {

    if (n == 0) return {0, 0}; // no internal nodes, no leaves

    size_t k = std::countr_zero(fanout);
    size_t max_leaf = size_t(1) << (((std::bit_width(n - 1) + k - 1) / k) * k);

    size_t leaf_start = 0; //the index of the first node that can contain leaves
    size_t level = 0; //the index of the first node in the bottom row

    while (true) {
        level = first_child_of(level);
        if (level >= max_leaf) break;
        leaf_start += level - leaf_start; //PERRIN TO DO -- WHY?? can't this just be leaf_start = level
    }

    int total_nodes = leaf_start+n;
    return {total_nodes, leaf_start};
  }

  std::pair<size_t, size_t> minimal_tree_shape_expansion(size_t n) {
      // Returns {total_nodes_excluding_root, leaf_start_index}
      if (n == 0) return {0, 0}; // no internal nodes, no leaves

      size_t k = std::countr_zero(fanout);
      size_t max_leaf = size_t(1) << (((std::bit_width(n - 1) + k - 1) / k) * k)+1;
      //std::cout << "max_leaf: " << max_leaf << std::endl;

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

  //constructors
  complete_kary_complete_tree() : data_(1, Real(0)), leaf_start_(0), leaf_count_(0) {}

  explicit complete_kary_complete_tree(size_t leaf_count) {
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

    // Tree navigation (-1 based indexing). root stored seperately
 

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

    std::vector<Real,boost::alignment::aligned_allocator<Real, 128> > &data() {return data_;}

    const std::vector<Real,boost::alignment::aligned_allocator<Real, 128> > &data() const {return data_;}

private:


    std::vector<Real,boost::alignment::aligned_allocator<Real, 128> > data_;
    size_t leaf_start_;
    size_t leaf_count_;
    size_t max_leaf_;

    static constexpr size_t log2_fanout = [] { //This is the lg(fanout) with a base of 2 
        size_t v = fanout;
        size_t r = 0;
        while (v > 1) {
            v >>= 1;
            ++r;
        }
        return r;
    }();
};
}}

#endif