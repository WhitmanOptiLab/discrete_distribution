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
class nonuniform_int_distribution : public complete_tree<IntType, Real>,
                                    public weightsum_tree<nonuniform_int_distribution<IntType, precision>, IntType, precision> {
 public:
  using This = nonuniform_int_distribution<IntType, precision>;
  using BaseTree = complete_tree<IntType, Real>;
  using WeightSum = weightsum_tree<This, IntType, precision>;
  friend WeightSum;
  using PosType = typename BaseTree::position_type;

  //Weights can be of any type, but most be convertable to Real values
  nonuniform_int_distribution() = delete;
  nonuniform_int_distribution(PosType p) : BaseTree(p), WeightSum(*this) {};

  template< class InputIt >
  nonuniform_int_distribution(InputIt first, InputIt last) :
    BaseTree(std::distance(first, last)),
    WeightSum(*this)
  {
    for (PosType i = 1; first != last; ++first, ++i) {
      BaseTree::add(i, *first); // Add to Fenwick tree
    }
    BaseTree::setUpHugePages();
    WeightSum::access_total_weight() = BaseTree::prefix_sum(BaseTree::entries());

  }

  Real& fenwick_weight_of(PosType p) {
    return BaseTree::value_of(p);
  }

  Real get_weight(PosType p) {
    return BaseTree::get(p+1);
  }

  PosType id_of(PosType p) { return p; }

};

}
}
#endif
