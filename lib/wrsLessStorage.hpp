#ifndef WRS_LOW_STORAGE
#define WRS_LOW_STORAGE
#include "completetree.hpp"
#include "weightsum_tree.hpp"
#include <limits>
#include <utility>
#include <random>
#include <iostream>

namespace dense {
namespace stochastic {


//Class to randomly select an index where each index's probability of being 
//  selected is weighted by a given vector.  
template <class IntType = size_t, size_t precision = std::numeric_limits<Real>::digits>
class wrsLessStorage : protected complete_tree<IntType, Real >,  
                                    public weightsum_tree<wrsLessStorage<IntType, precision>, IntType, precision> {
 public:
  using This = wrsLessStorage<IntType, precision>;
  using BaseTree = complete_tree<IntType, Real >;
  using WeightSum = weightsum_tree<This, IntType, precision>;
  friend WeightSum;
  using PosType = typename BaseTree::position_type;
  static PosType left_of(PosType i) { return BaseTree::left_of(i);}
  static PosType right_of(PosType i) { return BaseTree::right_of(i);}
  static PosType parent_of(PosType i) { return BaseTree::parent_of(i);}

  //Weights can be of any type, but most be convertable to Real values
  wrsLessStorage() = delete;
  wrsLessStorage(PosType p) : BaseTree(p), WeightSum(*this) {};

  template< class InputIt >
  __attribute__ ((noinline)) wrsLessStorage(InputIt first, InputIt last) :
    BaseTree(),
    WeightSum(*this)
  {
    WeightSum::add_to_total_weight(-1 * WeightSum::total_weight());
    InputIt it = first;
    for (IntType i = 0; it != last; ++it, ++i) {
      BaseTree::emplace_entry(*it);
    }
    PosType lastNonLeaf = (BaseTree::entry_count())/2;
    if (BaseTree::entry_count()%2==0){
        this->value_of(lastNonLeaf)+=(this->value_of(BaseTree::left_of(lastNonLeaf)));
    }
    else{
        this->value_of(lastNonLeaf)+=(this->value_of(BaseTree::left_of(lastNonLeaf)))+(this->value_of(BaseTree::right_of(lastNonLeaf)));
    }
    for (PosType node = lastNonLeaf-1;node>0;node--){
        this->value_of(node)+=((this->value_of(BaseTree::left_of(node))+(this->value_of(BaseTree::right_of(node)))));
    }
    WeightSum::add_to_total_weight(this->value_of(BaseTree::root()));
  }

  Real weight_of(PosType p) {
    PosType lastNonLeaf = BaseTree::entry_count()/2;
    if (p<lastNonLeaf){
        return BaseTree::value_of(p)-BaseTree::value_of(left_of(p))-BaseTree::value_of(right_of(p));
    }
    else if (p==lastNonLeaf){
        if (BaseTree::right_of(p)>BaseTree::entry_count()){
            return BaseTree::value_of(p)-BaseTree::value_of(left_of(p));
        }
        else{
            return BaseTree::value_of(p)-BaseTree::value_of(left_of(p))-BaseTree::value_of(right_of(p));
        }

    }
    else{
        return BaseTree::value_of(p);
    }

  }

  Real get_weight(IntType p) {
    return weight_of(p+1);
  }

    template<class URNG>
    IntType operator()(URNG& g) {
    //      Real randomTest = std::generate_canonical<Real, precision, URNG>(g);
    // std::cout<<"randomTest "<<randomTest;
    return id_of(WeightSum::operator()(g));
    }

  void update_weight(IntType i, Real delta) {
    i=i+1; //node index
    while (i > this->root()) {
      this->weightsum_of(i) += delta;
      i = BaseTree::parent_of(i);
    }
    this->weightsum_of(i) += delta;
    
    WeightSum::add_to_total_weight(delta);
  }
  
  Real& weightsum_of(PosType p) {
    return BaseTree::value_of(p);
  }
  const Real& weightsum_of(PosType p) const {
    return const_cast<This*>(this)->weightsum_of(p);
  }

  void push_back(Real& newEntry) {
    BaseTree::add_entry(0);
    WeightSum::update_weight(BaseTree::last(),newEntry);
  }
  void push_back(Real&& newEntry) {
    BaseTree::add_entry(0);
    WeightSum::update_weight(BaseTree::last(),newEntry);
  }
  
  void pop_back() {
  WeightSum::update_weight(BaseTree::last(), 0);
  BaseTree::pop_back();
  }

  PosType id_of(PosType p) { return p-1; }


  void update_weight_internal(PosType i, Real new_weight) {
    update_weight(i, new_weight - this->weight_of(i));
  }
};

}
}
#endif