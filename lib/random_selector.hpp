#ifndef RANDOM_SELECTOR
#define RANDOM_SELECTOR
#include "completetree.hpp"
#include "weightsum_tree.hpp"
#include <limits>
#include <utility>
#include <random>
#include <cassert>

namespace dense {
namespace stochastic {


//Class to randomly select an index where each index's probability of being 
//  selected is weighted by a given vector.  
template <class IntType = size_t, size_t precision = std::numeric_limits<Real>::digits>
class nonuniform_int_distribution : protected complete_tree<IntType, Real>,
                                    public weightsum_tree<nonuniform_int_distribution<IntType, precision>, IntType, precision> {
 public:
  using This = nonuniform_int_distribution<IntType, precision>;
  using BaseTree = complete_tree<IntType, Real>;
  using WeightSum = weightsum_tree<This, IntType, precision>;
  friend WeightSum;
  using PosType = typename BaseTree::position_type;
  static PosType left_of(PosType i) { return BaseTree::left_of(i);}
  static PosType right_of(PosType i) { return BaseTree::right_of(i);}
  static PosType parent_of(PosType i) { return BaseTree::parent_of(i);}

  //Weights can be of any type, but most be convertable to Real values
  nonuniform_int_distribution() = delete;
  nonuniform_int_distribution(PosType p) : BaseTree(p), WeightSum(*this) {};

  template< class InputIt >
  nonuniform_int_distribution(InputIt first, InputIt last) :
    BaseTree(),
    WeightSum(*this)
  {
    size_t n = std::distance(first, last);
    leaf_count = next_power_of_two(n);
    leaf_start = leaf_count;
    BaseTree::resize(2 * leaf_count, 0.0);  // index 0 is unused
    // Step 1: copy weights to leaves
    InputIt it = first;
    for (size_t i = 0; it != last; ++it, ++i) {
      BaseTree::value_of(leaf_start + i) = Real(*it);
    }

    // Step 2: build sums from leaves up
    for (size_t i = leaf_start - 1; i >= 1; --i) {
      BaseTree::value_of(i) = BaseTree::value_of(2 * i) + BaseTree::value_of(2 * i + 1);
    }
  }

  Real total_weight() const {
    return BaseTree::value_of(1);  // root
  }

  Real& weight_of(PosType i) {
    return BaseTree::value_of(leaf_start + i);
  }

  Real get_weight(PosType i) const {
    assert(i >= 0 && i < static_cast<PosType>(leaf_count));
    return BaseTree::value_of(leaf_start + i);
  }

  Real& weightsum_of(PosType p) {
    return BaseTree::value_of(p);
  }
  const Real& weightsum_of(PosType p) const {
    return const_cast<This*>(this)->weightsum_of(p);
  }

  PosType id_of(PosType p) { return leaf_start + p; }

  size_t get_leaf_count() const { return leaf_count; }

  size_t get_leaf_start() const { return leaf_start; }

private:
  size_t leaf_count;
  size_t leaf_start;

  static size_t next_power_of_two(size_t n) {
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
  }

};

}
}
#endif
