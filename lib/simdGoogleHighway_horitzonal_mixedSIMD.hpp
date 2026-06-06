#ifndef SIMD_HIGHWAY_HORIZONTAL_MIXEDSIMD
#define SIMD_HIGHWAY_HORIZONTAL_MIXEDSIMD
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
#include "kary_complete_tree_base.hpp"
#include "../vcpkg/installed/x64-linux/include/hwy/highway.h"


namespace hn = hwy::HWY_NAMESPACE;

namespace dense {
namespace stochastic {

// template <class int_type = size_t, class Real = double, size_t fanout = 16>
// class complete_kary_complete_tree {
// public:

//   // Compute minimal complete k-ary tree size to store exactly n leaves
//   // without needing bounds checks during selection. returns the index of the first leaf and the total number of leaves
//   // Returns {total_nodes_excluding_root, leaf_start_index}
//   std::pair<size_t, size_t> minimal_tree_shape(size_t n) {

//     if (n == 0) return {0, 0}; // no internal nodes, no leaves

//     size_t k = std::countr_zero(fanout);
//     size_t max_leaf = size_t(1) << (((std::bit_width(n - 1) + k - 1) / k) * k);

//     size_t leaf_start = 0; //the index of the first node that can contain leaves
//     size_t level = 0; //the index of the first node in the bottom row

//     while (true) {
//         level = first_child_of(level);
//         if (level >= max_leaf) break;
//         leaf_start += level - leaf_start; //PERRIN TO DO -- WHY?? can't this just be leaf_start = level
//     }

//     int total_nodes = leaf_start+n;
//     return {total_nodes, leaf_start};
//   }

//   std::pair<size_t, size_t> minimal_tree_shape_expansion(size_t n) {
//       // Returns {total_nodes_excluding_root, leaf_start_index}
//       if (n == 0) return {0, 0}; // no internal nodes, no leaves

//       size_t k = std::countr_zero(fanout);
//       size_t max_leaf = size_t(1) << (((std::bit_width(n - 1) + k - 1) / k) * k)+1;
//       //std::cout << "max_leaf: " << max_leaf << std::endl;

//       size_t leaf_start = 0;
//       size_t level = 0;
//       //size_t max_leaf = std::pow(fanout, std::ceil(std::log(n) / std::log(fanout))); //highest possible # of leaves in this tree structure;
//       while (true) {
//           level = first_child_of(level);
//           if (level >= max_leaf) break;
//           leaf_start += level - leaf_start;
//       }

//       int total_nodes = (-1*(0-leaf_start)+n);
//       return {total_nodes, leaf_start};
//   }



//   static_assert((fanout & (fanout - 1)) == 0, "fanout must be power of two");

//   using position_type = int_type;

//   //constructors
//   complete_kary_complete_tree() : data_(1, Real(0)), leaf_start_(0), leaf_count_(0) {}

//   explicit complete_kary_complete_tree(size_t leaf_count) {
//       resize(leaf_count);
//   }

//   // Resize to accommodate exactly n leaves
//     void resize(size_t n) {
//         if (n == 0) {
//             data_.assign(1, Real(0));
//             leaf_start_ = 0;
//             leaf_count_ = 0;
//             return;
//         }


//         auto [total_nodes, leaf_start] = minimal_tree_shape(n);

//         // allocate all internal nodes + leaves (root is kept separately by the derived class)
//         data_.resize(total_nodes);
//         leaf_count_ = n;
//         leaf_start_ = leaf_start;
//         //std::cout << "base tree leafstart: " << leaf_start_ << std::endl;
//     }



//     size_t size() const {
//         return data_.size();
//     }

//     Real& value_of(position_type p) {
//         if(p >= data_.size()) {
//           throw std::runtime_error("trying to access position " + std::to_string(p) + " in tree of size " + std::to_string(data_.size()));
//         }

//         return data_[p];
//     }

//     const Real& value_of(position_type p) const {
//         assert(p < data_.size());
//         return data_[p];
//     }

//     // Tree navigation (-1 based indexing). root stored seperately
 

//     position_type parent_of(position_type i) const {
//         assert(i > 0);
//         return (i >> log2_fanout) - 1;
//     }

//     position_type first_child_of(position_type i) const {
//         return (i + 1) << log2_fanout;
//     }

//     position_type child_index(position_type parent, size_t child_num) const {
//         return first_child_of(parent) + child_num;
//     }

//     bool is_leaf(position_type i) const {
//         return i >= leaf_start_;
//     }

//     size_t leaf_start() const {
//         return leaf_start_;
//     }

//     size_t leaf_count() const {
//         return leaf_count_;
//     }

//     std::vector<Real> &data() {return data_;}

// private:
//      std::vector<Real> data_;
//     size_t leaf_start_;
//     size_t leaf_count_;
//     size_t max_leaf_;

//     static constexpr size_t log2_fanout = [] { //This is the lg(fanout) with a base of 2 
//         size_t v = fanout;
//         size_t r = 0;
//         while (v > 1) {
//             v >>= 1;
//             ++r;
//         }
//         return r;
//     }();
// };


//Class to randomly select an index where each index's probability of being
//selected is weighted by a given vector. Supports O(log(n)) selection and
//update while using about O(n) memory space.
// Uses exponential tree with fixed fanout.
template <
  class int_type = size_t,
  class Real = double,
  int_type fanout = 16,
  int_type precision = std::numeric_limits<Real>::digits
>
class mixedsimd_highway_horizontal : protected complete_kary_complete_tree<int_type, Real, fanout> {
  using This = mixedsimd_highway_horizontal<int_type, Real, fanout, precision>;
  using BaseTree = complete_kary_complete_tree<int_type, Real, fanout>;
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
    friend class mixedsimd_highway_horizontal<int_type, Real, fanout, precision>;
  };

  // Default constructor
  mixedsimd_highway_horizontal() : BaseTree(1), leaf_end_(0), leaf_start_(0), max_leaf_(fanout) {}

  // Construct from vector
  explicit mixedsimd_highway_horizontal(const std::vector<Real>& weights)
    : mixedsimd_highway_horizontal(weights.begin(), weights.end()) {}

  // Construct from initializer list
  mixedsimd_highway_horizontal(const std::initializer_list<Real>& il)
    : mixedsimd_highway_horizontal(il.begin(), il.end()) {}

  // Construct from iterator pair
  template<class InputIt>
  mixedsimd_highway_horizontal(InputIt first, InputIt last)
    : BaseTree()
  {
    int_type n = std::distance(first, last);
    //n = 2;

    // Round up leaves to nearest full complete k-ary tree level (power of fanout)
    if (n <= fanout){
      max_leaf_ = fanout;
    }
    else{
          int_type k = std::countr_zero(fanout);
    max_leaf_ = int_type(1) << (((std::bit_width(n - 1) + k - 1) / k) * k);
    }

    leaf_start_ = BaseTree::minimal_tree_shape(n).second;
    //std::cout << "Constructor resize!" << std::endl;
    BaseTree::resize(n);
    leaf_end_ = n;

    // Copy weights to leaves, pad with zeros
    InputIt it = first;
    for (int_type i = leaf_start_; i < leaf_start_ + n; ++i) {
        weightsum_of(i) = std::max(Real(*it), Real(0));
        //std::cout << i << std::endl;
        ++it;
    }
    //fill the rest of the unused leaves with 0. they hold 0 by default on construction of BaseTree
    //for(int_type i = leaf_start_ + n; i < BaseTree::size() - 1; ++i) {
    //    weightsum_of(i) = Real(0);
    //}


    // Build sums bottom-up from leaves to root (excluding root)
    for (ptrdiff_t i = leaf_start_ - 1; i >= 0; --i) {
        Real sum = 0;
        PosType first_child = (i + 1) * fanout; // -1-index adjustment
        for (int_type c = 0; c < fanout; ++c) {
            PosType child = first_child + c;
            if (child >= leaf_start_+n) break;
            sum += weightsum_of(child);
        }
        weightsum_of(i) = sum;
    }

    // Compute root separately
    Real sum = 0;
    PosType first_child = 0 * fanout; // root's first child in array
    if (leaf_start_ == 0){
      for (int_type c = 0; c < leaf_end_; ++c) {
        PosType child = first_child + c;
        if (child >= max_leaf_) break;
        sum += weightsum_of(child);
    }
    }
    else{
    for (int_type c = 0; c < fanout; ++c) {
        PosType child = first_child + c;
        if (child >= max_leaf_) break;
        sum += weightsum_of(child);
    }
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

    const hn::ScalableTag<Real> realBlock; //this is a "tag" telling google highway how big the simd register is and what it is filled with
    const int_type numLanes = hn::Lanes(realBlock);
    const Real* data_ptr = BaseTree::data().data();

    

    Real cummulative = 0; 
      int_type blockStart = first_child+fanout - numLanes;
      bool chosen = false;
      for (int_type i = 0; i<fanout-numLanes; i+=numLanes){
        auto nextBlock = hn::Load(realBlock,data_ptr+i);
        nextBlock = SumOfLanes(realBlock,nextBlock);
        if (target < cummulative + GetLane(nextBlock)){
          blockStart = first_child+i;

          break;
        }
        else{
          cummulative+= GetLane(nextBlock);
        }

      }

      node = std::min(numLanes+blockStart,leaf_end_+leaf_start_)-1;
      for (int_type child = blockStart; child<numLanes+blockStart; child++){
        if (child >= leaf_end_+leaf_start_) break; //PERRIN TO DO - Throw an error here? start the selection over? we should do something other than choose the fanout=th node
        Real w = weightsum_of(child);
        if (target < cummulative + w) {
          node = child;
          chosen = true;
          target -= cummulative; 

            break;
        }
        cummulative += w;

      }

      if (chosen == false){
          node = first_child;
          target -= cummulative;
          target +=weightsum_of(first_child);
      }
      first_child = BaseTree::first_child_of(node); // first child in array
      if (first_child >= BaseTree::size()){
        return  static_cast<result_type>(node - leaf_start_);
    } 
    
    while (true){
      //std::cout<<"node is "<<node<<std::endl;
      Real cummulative = 0; 
      int_type blockStart = first_child+fanout - numLanes;
      hn::Vec<hn::ScalableTag<Real>> nextBlock;
      for (int_type i = 0; i<fanout-numLanes; i+=numLanes){
        nextBlock = hn::Load(realBlock,data_ptr+first_child+i);
        auto sum = SumOfLanes(realBlock,nextBlock);
        if (target < cummulative + GetLane(sum)){
          blockStart = first_child+i; 

          break;
        }
        else{
          cummulative+= GetLane(sum);
        }

      }
      if (blockStart == first_child+fanout - numLanes){
        nextBlock = hn::Load(realBlock,data_ptr+blockStart);
      }

      //make next block contain prefix sum
      //PERRIN TO DO - make this portable
      //source https://en.algorithmica.org/hpc/algorithms/prefix/ 
      auto prefix  = hn::Add(nextBlock,hn::Slide1Up(realBlock,nextBlock));
      for (int_type i=2; i<numLanes;i=i<<1){
        prefix = hn::Add(prefix,hn::SlideUpLanes(realBlock,prefix,i));
      }
      
      // auto prefix = hn::Add(nextBlock,hn::ShiftLeftLanes<1>(nextBlock));
      // auto newprefix = hn::Add(prefix,hn::ShiftLeftLanes<2>(prefix));


      target-= cummulative;
      auto targetBlock = hn::Set(realBlock,target);
      int_type choiceIndex = hn::FindFirstTrue(realBlock,hn::Lt(targetBlock,prefix));
      node = blockStart + choiceIndex;
      

      HWY_ALIGN Real prefixArray[numLanes];
      hn::Store(prefix,realBlock,prefixArray);
      if (choiceIndex>0){
        target -= prefixArray[choiceIndex-1];
      }
      if (choiceIndex == -1){
        node = first_child;
        target -= prefixArray[numLanes -1];
        target += weightsum_of(first_child);
      }

      first_child = BaseTree::first_child_of(node); // first child in array
      //std::cout<<"first child is "<<first_child<<"    tree size is "<<BaseTree::size()<<std::endl;
      if (first_child >= BaseTree::size()){

        
        break;

      } 
    }
    //std::cout<<node<<std::endl;
    return static_cast<result_type>(node - leaf_start_);
}


  template<class URNG>
  result_type operator()(URNG& g, const Param& param) const {
    mixedsimd_highway_horizontal temp(param.weights_);
    return temp(g);
  }

  // Return probabilities vector
  std::vector<Real> probabilities() const {
    std::vector<Real> probs(leaf_end_, Real(0));
    Real total = total_weight();
    if (total <= Real(0)) return probs;

    for (int_type i = 0; i < leaf_end_; ++i) {
      probs[i] = weightsum_of(leaf_start_ + i) / total;
    }
    return probs;
  }

  Param param() const {
    std::vector<Real> weights(leaf_end_);
    for (int_type i = 0; i < leaf_end_; ++i) {           // <-- NOTE: loop from 0..leaf_end_-1
      weights[i] = get_weight(i);
    }
    return Param(weights);
  }

  void param(const Param& p) { //PERRIN TO DO --destroy old tree and associated memory
    *this = mixedsimd_highway_horizontal(p.weights_);
  }

  static constexpr result_type min() { return 0; }

  result_type max() const { return leaf_end_ == 0 ? 0 : leaf_end_ - 1; }

  // Update a leaf weight and propagate change upward
  void update_weight(PosType i, Real new_weight) {

    assert(new_weight >= Real(0));
    assert(i <= leaf_end_+leaf_start_);
    i = leaf_start_ + i;
    Real diff = new_weight - weightsum_of(i);
    weightsum_of(i) = new_weight;
    total_weight_ += diff;

    while (i >=fanout) {                 // <-- FIX: propagate to the very top int_typeernal node
      i = BaseTree::parent_of(i);
      //std::cout << i << std::endl;
      weightsum_of(i) += diff;
    }
  }

  int_type get_max() const {
    return max_leaf_;
  }


  Real get_weight(PosType i) const {
    assert(i < leaf_end_);
    return weightsum_of(leaf_start_ + i);
  }

  int_type size() const { return leaf_end_; }

  Real total_weight() const noexcept {
    if (BaseTree::size() == 0) return Real(0);
    return total_weight_;
  }

  void push_back(const std::vector<Real>& weights){
    for (Real w : weights) {
      push_back(w);
    }
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

  void pop_back(int_type count) {
    for(int_type i = 0; i < count; ++i) {
      pop_back();
    }
  }

  void pop_back() {
    assert(leaf_end_ > 0);
    leaf_end_--;
    update_weight(leaf_end_, Real(0));

  }

// Print tree for debugging (-1-indexed)
  void printTree(std::ostream& os = std::cout) const {
    int_type total_nodes = BaseTree::size();

    if (total_nodes == 0) {
        os << "(empty tree)\n";
        return;
    }

    std::queue<ptrdiff_t> q; // use signed for -1 root
    q.push(-1); // -1 represents the root stored separately

    int_type level = 0;

    while (!q.empty()) {
        int_type level_size = q.size();
        os << "Level " << level << ": ";

        for (int_type i = 0; i < level_size; ++i) {
            ptrdiff_t node = q.front();
            q.pop();

            Real w = (node == -1) ? total_weight_ : weightsum_of(static_cast<PosType>(node));
            os << "[" << node << "]=" << w << "  ";

            // enqueue children
            PosType first_child = (node + 1) * fanout; // -1-index adjustment
            for (int_type c = 0; c < fanout; ++c) {
                PosType child = first_child + c;
                if (child >= total_nodes) break;
                q.push(child);
            }
        }

        os << "\n";
        ++level;
    }
  }

  void printTreeFromNode (int_type node) const{
    int_type first = node;
    int_type last = node;

    while (last<BaseTree::size()){
      for (int_type i=first; i<=last;i++){
        std::cout<< weightsum_of(i)<<", ";
      }
      std::cout<<std::endl;
      first = BaseTree::first_child_of(first);
      last = BaseTree::first_child_of(last)+fanout-1;
      

    }
    std::cout<<std::endl;
  }

      // Expand from current leaf_count_ to new_leaf_count (must be larger), structurally, without recomputing
    // new expand: argument is NEW_LEAF_COUNT (number of leaves you want after expansion)
void expand(int_type new_leaf_count) {
    if (new_leaf_count <= leaf_end_) return; // nothing to do
    auto &data_ = BaseTree::data();
    int_type new_leaf_start = leaf_start_;
    if (new_leaf_count > max_leaf_) {
      max_leaf_ *= fanout; //PERRIN TO DO -- Can this be <<log2fanout? 
    // old shape
    auto [old_total_nodes, old_leaf_start] = BaseTree::minimal_tree_shape(leaf_end_); //PERRIN TO DO probably don't need to recalculate all this 

    // new shape
    auto [new_total_nodes, new_leaf_start] = BaseTree::minimal_tree_shape(new_leaf_count);
    leaf_start_ = new_leaf_start;

        std::vector<int_type> old_starts;
        std::vector<int_type> old_levels;

        std::vector<int_type> new_starts;
        std::vector<int_type> new_levels;

        int_type position = 0;
        int_type sum = 0;


        // Creating the level lengths and level starts for the old size
        while (true) {
            position = BaseTree::first_child_of(position);
            old_starts.push_back(sum);
            int_type level = position - sum;
            sum += level;
            old_levels.push_back(level);
            if (position >= old_total_nodes) break;
        }



//        for (int i : old_starts) {
//          std::cout << i << std::endl;
//        }
//
//        std::cout << std::endl;
//
//        for (int i : old_levels) {
//          std::cout << i << std::endl;
//        }
//        std::cout << std::endl;





        // Creating the level lengths and level starts for the new size
        position = 0;
        sum = 0;
        while (true) {
            position = BaseTree::first_child_of(position);
            new_starts.push_back(sum);
            int_type level = position - sum;
            sum += level;
            new_levels.push_back(level);
			if (position >= new_total_nodes) break;
        }

//        for (int i : new_starts) {
//          std::cout << i << std::endl;
//        }
//
//        std::cout << std::endl;
//
//        for (int i : new_levels) {
//          std::cout << i << std::endl;
//        }
//        std::cout << std::endl;



    // allocate new array (zero-initialized)
	std::vector<Real> new_data(new_total_nodes, Real(0));

    // copy each old level block into the next-deeper level of the new layout.
    // old level i -> new level (i+1). That packs the old block contiguously at the start
    // of the larger new level (the remainder stays zero).
    for (int_type i = 0; i < old_levels.size(); ++i) {
        int_type old_start = old_starts[i];
        int_type old_sz    = old_levels[i];
        int_type new_level_index = i + 1; // destination level index
        int_type new_start = new_starts[new_level_index];
        // bounds check (should not happen for reasonable expansions)
        if (old_start + old_sz > data_.size()) {
            std::cout << "Reading from: " << old_start + old_sz << ", total size: " << data_.size() << std::endl;

            throw std::runtime_error("expand(): old level range out of bounds");
        }
        if (new_start + old_sz > new_data.size()) {
			std::cout << new_starts.size() << ", reading from: " << new_level_index << std::endl;
          std::cout << "Writing to: " << new_start <<" + " << old_sz << ", total size: " << new_data.size() << std::endl;
            throw std::runtime_error("expand(): new level destination out of bounds");
        }

        std::copy_n(data_.begin() + old_start, old_sz, new_data.begin() + new_start);
    }

    // recompute only the new top internal layer (level 0) from its children (level 1)
    // top-level nodes occupy global indices new_starts[0] .. new_starts[0]+new_levels[0]-1
    // their first child indices can be computed with first_child_of(parent_index)
    int_type top_count = new_levels[0];
    int_type top_start = new_starts[0];   // usually 0
    for (int_type j = 0; j < top_count; ++j) {
        int_type parent_idx = top_start + j;
        // first child in global indexing:
        int_type first_child = BaseTree::first_child_of(parent_idx);
        Real sum = Real(0);
        for (int_type c = 0; c < fanout; ++c) {
            int_type child = first_child + c;
            if (child >= new_total_nodes) break;
            sum += new_data[child];
        }
        new_data[parent_idx] = sum;





    }
    // commit!
    data_.swap(new_data);

    }
    else if (new_leaf_count+leaf_start_ >= BaseTree::size()) {
      BaseTree::resize(std::min(new_leaf_count*2,leaf_start_+max_leaf_));
    }
}



private:
  Real& weightsum_of(PosType p) { return BaseTree::value_of(p); }
  const Real& weightsum_of(PosType p) const { return BaseTree::value_of(p); }
  int_type max_leaf_;
  int_type leaf_end_;   // number of leaves requested by user
  int_type leaf_start_; // index of first leaf in data_
  Real total_weight_ = 0;
};
}
}
#endif
