#ifndef MODIFIABLE_HEAP_RANDOM_SELECTOR
#define MODIFIABLE_HEAP_RANDOM_SELECTOR

#include <utility>
#include <vector>
#include <functional>
#include <type_traits>
#include <iostream>
#include <fstream>
#include <stdexcept>

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
    public weightsum_tree< fast_random_selector<I, precision>, I, precision>
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
        index_to_node.resize(n);
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
        return id_of(WeightSum::operator()(g));
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
        if(i > index_to_node.size() - 1) {
          throw std::out_of_range("Cannot get weight of index " + std::to_string(i) + " because current size is " + std::to_string(BaseTree::entry_count()));
        }
        return weight_of(index_to_node[i]);
      }

      Real total_weight() const { return WeightSum::total_weight(); }


      void pop_back() {
        // for(int i = 0; i < BaseTree::size(); i++) {
        //   std::cout << "{" << BaseTree::at(i).first << ", " << BaseTree::at(i).second << "}, ";
        // } //printing out weights and weightsums

        if(BaseTree::entry_count() == 0) {return;} //making sure user can't remove an entry from an empty tree

        node_type popNode = index_to_node[BaseTree::last() - 1];

        swap(popNode, (node_type)BaseTree::last());
     
         

        update_weight(node_to_index[BaseTree::last()], 0);
        

        BaseTree::pop_back();
        node_to_index.pop_back();
        index_to_node.pop_back();

      }

      void push_back(Real&& newEntry) {
        if(newEntry < 0) {
          return;
        }
        value_type v = {0,0};
        BaseTree::add_entry(v);
        
        auto newp = BaseTree::last();
		    index_type i = BaseTree::entry_count() - 1;
		    map_node(i, newp);
        update_weight(i, newEntry);
      }

      void push_back(Real& newEntry) {
        if(newEntry < 0) {
          return;
        }
        value_type v = {0,0};
        BaseTree::add_entry(v);
        node_to_index.push_back(0);
        index_to_node.push_back(0);
        auto newp = BaseTree::last();
		    index_type i = BaseTree::entry_count() - 1;
		    map_node(i, newp);
        update_weight(i,newEntry);

      }



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