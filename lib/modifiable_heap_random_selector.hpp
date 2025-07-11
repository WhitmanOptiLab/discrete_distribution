#ifndef MODIFIABLE_HEAP_RANDOM_SELECTOR
#define MODIFIABLE_HEAP_RANDOM_SELECTOR

#include <utility>
#include <vector>
#include <functional>
#include <type_traits>
#include <iostream>

#include "completetree.hpp"
#include "heap.hpp"
#include "weightsum_tree.hpp"

namespace dense {
namespace stochastic {


  template <
    typename I = size_t, size_t precision = std::numeric_limits<Real>::digits
  >
  class fast_random_selector :
    //Extends a complete tree...
    protected complete_tree<I, std::pair<Real, Real> >,

    //... using the heap mix-in
    protected heap< fast_random_selector<I, precision>, I>,

    //... and the weightsum tree mix-in
    protected weightsum_tree< fast_random_selector<I, precision>, I, precision>
  {


    public:

      using size_type = std::ptrdiff_t;
      using index_type = I;
      using This = fast_random_selector<index_type, precision>;
      using node_type = index_type;
      using value_type = std::pair<Real, Real>;
      using entry_type = std::tuple<index_type, Real, Real>;
      using iterator = value_type*;
      using const_iterator = value_type const*;
      using reference = value_type&;
      using const_reference = value_type const&;
      using BaseTree = complete_tree<node_type, value_type>;
      using Heap = heap<This, node_type>;
      using WeightSum = weightsum_tree<This, node_type, precision>;

      friend Heap;
      friend WeightSum;

      fast_random_selector() = delete;

      template<typename InputIt>
      fast_random_selector(InputIt first, InputIt last) :
          BaseTree(),
          Heap(),
          WeightSum(),
		  node_to_index(),
		  index_to_node()
      {
        size_t n = static_cast<size_t>(last - first);
        node_to_index.resize(n+1);
        index_to_node.resize(n+1);
        InputIt it = first;
        for (index_type i = 0; it != last; ++it, ++i) {
          double w = *it;
          //Heap::push will add entries through the add_entry method, which
          //  will create index associations
          Heap::push(std::tuple<index_type, Real, Real>(i, Real(w), 0.0));
        }
        WeightSum::compute_weights();
      }

      fast_random_selector(fast_random_selector const&) = default;

      fast_random_selector(fast_random_selector &&) = default;

      fast_random_selector& operator=(fast_random_selector const&) = default;

      fast_random_selector& operator=(fast_random_selector &&) = default;

      ~fast_random_selector() = default;


      //Methods of WeightSum we want to make available
      template<class URNG>
      index_type operator()(URNG& g) {
        auto item = WeightSum::operator()(g);
        //std::cout<< "selected item "<< item<<std::endl;
        return id_of(item);
      }

      void update_weight(index_type i, Real new_weight) {
        auto node = index_to_node[i];
        Real old_weight = weight_of(node);
        WeightSum::update_weight(index_to_node[i], new_weight);
        if (old_weight < new_weight)
          Heap::sift_up(node);
        else
          Heap::sift_down(node);
      }

      Real get_weight(index_type i) {
        return weight_of(index_to_node[i]);
      }

      Real total_weight() const { return WeightSum::total_weight(); }

    private:

      void map_node(index_type i, node_type n) {
    	index_to_node[i] = n;
    	node_to_index[n] = i;
	    }

      void swap_indexes(node_type a, node_type b) {
        std::swap(node_to_index[a], node_to_index[b]);
		    index_to_node[node_to_index[a]] = a;
		    index_to_node[node_to_index[b]] = b;
      }

      //Must call WeightSum::compute_weights() after this, before using random selection
      void add_entry(entry_type&& e) {
        value_type v = {std::get<1>(e),std::get<2>(e)};
        BaseTree::add_entry(v);
        auto newp = BaseTree::last();
		    index_type i = std::get<0>(e);
		    map_node(i, newp);
      }

      void add_entry(const entry_type& e) {
        value_type v = {std::get<1>(e),std::get<2>(e)};
        BaseTree::add_entry(v);
        auto newp = BaseTree::last();
		    index_type i = std::get<0>(e);
		    map_node(i, newp);
      }

      Real& weight_of(node_type n) {
    	return this->value_of(n).first;
	    }

      const Real& weight_of(node_type n) const {
        return const_cast<This*>(this)->weight_of(n);
      }

      Real& weightsum_of(node_type n) {
        return this->value_of(n).second;
      }

      const Real& weightsum_of(node_type n) const {
        return const_cast<This*>(this)->weightsum_of(n);
      }

      void remove_last_entry() {
        auto last = BaseTree::value_of(BaseTree::last());
        WeightSum::update_weight(BaseTree::last(), 0);
        BaseTree::remove_last_entry();
      }

      fast_random_selector const& const_this() const {
        return static_cast<This const&>(*this);
      }

      //Max-heap property
      bool less(node_type a, node_type b) const {
        return weight_of(a) > weight_of(b);
      }

      void swap(node_type a, node_type b) {
        WeightSum::swap(a, b);
        swap_indexes(a, b);
      }

      void swap_with_child(node_type a, node_type b) {
        swap_indexes(a, b);
        WeightSum::swap_with_child(a, b);
      }

      index_type& id_of(node_type node) {
        return node_to_index[node];
      }

      std::vector<index_type> node_to_index;
      std::vector<node_type> index_to_node;


  };
}
}

#endif
