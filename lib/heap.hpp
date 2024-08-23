#ifndef DENSE_SIM_HEAP
#define DENSE_SIM_HEAP
#include "completetree.hpp"
#include <limits>
#include <utility>
#include <random>
#include <algorithm>

namespace dense {
namespace stochastic {


template <class PosType = int>
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
    _tree().add_entry(entry);
    sift_up(_tree().last());
  }

  template<typename ValType>
  void update(PosType i, ValType new_weight) {
    _tree().value_of(i) = new_weight;
    update_position(i);
  }

 protected:
  //Required interface functions for tree
  virtual void swap_with_child(PosType parent, PosType child) = 0;
  virtual void swap(PosType first, PosType second) = 0;
  virtual void remove_last_entry() = 0;
  virtual std::ptrdiff_t size() const = 0;
  virtual bool empty() = 0;
  virtual PosType root() const = 0;
  virtual PosType last() const = 0;
  virtual PosType parent_of(PosType n) const = 0;
  virtual PosType left_of(PosType n) const = 0;
  virtual PosType right_of(PosType n) const = 0;
  virtual bool less(PosType first, PosType second) const = 0;

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

  PosType min_child_of(PosType node) const {
    auto left = _tree().left_of(node);
    auto right = _tree().right_of(node);
    return (right < _tree().size() && _tree().less(right, left)) ? right : left;
  }

 private:
  heap<PosType>& _tree() { return *this; }
  const heap<PosType>& _tree() const { return *this; }
};

}
}
#endif
