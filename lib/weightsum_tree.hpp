#ifndef WEIGHTSUM_TREE
#define WEIGHTSUM_TREE
#include "completetree.hpp"
#include <limits>
#include <utility>
#include <random>

namespace dense {
namespace stochastic {

//Class to randomly select an index where each index's probability of being 
//  selected is weighted by a given vector.  
template <class Tree, class PosType, size_t precision = std::numeric_limits<Real>::digits>
class weightsum_tree {
 public:
  //Weights can be of any type, but must be convertable to Real values
  weightsum_tree() = default;

//  void compute_weights() {
//    _total_weight = prefix_sum(_tree().root());
//  }


  Real total_entries() const {
    return _tree().entries();
  }


  void update_weight(PosType i, Real new_weight) {
    Real current = _tree().get(i);
    Real delta = new_weight - current;
    _tree().add(i, delta);
    total_weight += delta;
  }

  PosType find_index(Real target) const {
    PosType pos = 0;
    PosType bit_mask = 1;
    PosType tree_size = _tree().entries();
    while ((bit_mask << 1) <= tree_size) bit_mask <<= 1;

    while (bit_mask > 0) {
      PosType next_pos = pos + bit_mask;
      if (next_pos <= tree_size) {
        Real val = _tree()[next_pos];
        if (val < target) {
          target -= val;
          pos = next_pos;
        }
      }

      bit_mask >>= 1;
    }
    return pos + 1; //Move up an index to the entry that contains target
  }


  template<class URNG>
  inline PosType operator()(URNG& g) {

    Real target = std::generate_canonical<Real, precision, URNG>(g)*total_weight;
    return find_index(target);
  }

 private:
  Tree& _tree() { return *static_cast<Tree*>(this); }
  const Tree& _tree() const { return *static_cast<const Tree*>(this); }
  Real total_weight= 0;

};

}
}
#endif
