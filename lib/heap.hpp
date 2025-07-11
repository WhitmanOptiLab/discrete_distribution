#ifndef DENSE_SIM_HEAP
#define DENSE_SIM_HEAP
#include "completetree.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <utility>
#include <random>
#include <algorithm>

namespace dense {
namespace stochastic {


template <class Tree, class PosType = int>
class heap {
 public:
  heap() = default;

  void pop() {
    if (_tree().empty()) return;
    if (_tree().root() != _tree().last()) {
      _tree().swap(_tree().root(), _tree().last());
    }
    _tree().remove_last_entry();
    sift_down(_tree().root());
  }

  template<typename T>
  void push(T entry) {
    std::cout<<std::endl<<std::endl<<"################  adding item  ###################"<<std::endl;
    //std::cout<<std::setprecision(7)<<"the weight being added is"<<std::get<1>(entry)<<std::endl;
    _tree().add_entry(entry);
    std::cout<<"element added to location"<< _tree().last()<< " is"<<std::to_string(_tree().at(_tree().last()))<<std::endl<<std::endl;
    sift_up(_tree().last());
  }

  template<typename ValType>
  void update(PosType i, ValType new_weight) {
    _tree().value_of(i) = new_weight;
    update_position(i);
  }

 protected:
  void update_position(PosType i) {
    if (i == _tree().root() || !_tree().less(i, _tree().parent_of(i))) {
      sift_down(i);
    } else {
      sift_up(i);
    }
  }

  bool sift_up(PosType node) {
    PosType start = node, parent;
    while (node != _tree().root() && _tree().less(node, parent = _tree().parent_of(node))) {
      _tree().swap_with_child(parent, node);
      node = parent;
    }
    return node != start;
  }

  bool sift_down(PosType node) {
    PosType start = node, min_child;
    while (_tree().left_of(node) < _tree().size() && _tree().less(min_child = min_child_of(node), node)) {
      _tree().swap_with_child(node, min_child);
      node = min_child;
    }
    return node != start;
  }

 private:
  PosType min_child_of(PosType node) const {
    auto left = _tree().left_of(node);
    auto right = _tree().right_of(node);
    return (right < _tree().size() && _tree().less(right, left)) ? right : left;
  }

  Tree& _tree() { return *static_cast<Tree*>(this); }
  const Tree& _tree() const { return *static_cast<const Tree*>(this); }
};

}
}
#endif
