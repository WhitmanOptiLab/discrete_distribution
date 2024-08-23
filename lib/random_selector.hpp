#ifndef RANDOM_SELECTOR
#define RANDOM_SELECTOR
#include "completetree.hpp"
#include "weightsum_tree.hpp"
#include <limits>
#include <utility>
#include <random>

namespace dense {
namespace stochastic {


//Class to randomly select an index where each index's probability of being 
//  selected is weighted by a given vector.  
template <class IntType = int, size_t precision = std::numeric_limits<Real>::digits>
class nonuniform_int_distribution : public complete_tree<IntType, std::pair<Real, Real> >,  
                                    public weightsum_tree<IntType, precision> {
 public:
  using This = nonuniform_int_distribution<IntType, precision>;
  using BaseTree = complete_tree<IntType, std::pair<Real, Real> >;
  using WeightSum = weightsum_tree<IntType, precision>;
  friend WeightSum;
  using PosType = typename BaseTree::position_type;
  virtual PosType left_of(PosType i) const { return BaseTree::left_of(i);}
  virtual PosType right_of(PosType i) const { return BaseTree::right_of(i);}
  virtual PosType parent_of(PosType i) const { return BaseTree::parent_of(i);}

  virtual void remove_last_entry() { BaseTree::remove_last_entry(); }
  virtual std::ptrdiff_t size() const { return BaseTree::size(); }
  virtual bool empty() { return BaseTree::empty(); }
  virtual PosType root() const { return BaseTree::root(); }
  virtual PosType last() const { return BaseTree::last(); }


  //Weights can be of any type, but most be convertable to Real values
  nonuniform_int_distribution() = delete;
  nonuniform_int_distribution(PosType p) : BaseTree(p), WeightSum(*this) {};

  template< class InputIt >
  __attribute__ ((noinline)) nonuniform_int_distribution(InputIt first, InputIt last) :
    BaseTree(),
    WeightSum(*this)
  {
    InputIt it = first;
    for (IntType i = 0; it != last; ++it, ++i) {
      BaseTree::emplace_entry(*it, 0.0);
    }
    WeightSum::compute_weights();
  }

  Real& weight_of(PosType p) {
    return BaseTree::value_of(p).first;
  }

  const Real& weight_of(PosType p) const {
    return BaseTree::value_of(p).first;
  }

  Real get_weight(PosType p) {
    return weight_of(p);
  }

  Real& weightsum_of(PosType p) {
    return BaseTree::value_of(p).second;
  }
  const Real& weightsum_of(PosType p) const {
    return const_cast<This*>(this)->weightsum_of(p);
  }

  PosType id_of(PosType p) { return p; }

  // Ignore
  void special(PosType i){
    return;
  }
};

}
}
#endif
