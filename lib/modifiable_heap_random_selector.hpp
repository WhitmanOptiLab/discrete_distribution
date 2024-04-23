#ifndef MODIFIABLE_HEAP_RANDOM_SELECTOR
#define MODIFIABLE_HEAP_RANDOM_SELECTOR

#include <utility>
#include <vector>
#include <functional>
#include <type_traits>

#include "completetree.hpp"
#include "heap.hpp"
#include "indexed_collection.hpp"
#include "weightsum_tree.hpp"

namespace dense {
namespace stochastic {

namespace {
  enum class ignore_me{};
}

  template <
    typename I = int, size_t precision = std::numeric_limits<Real>::digits
  >
  class fast_random_selector : 
    //Extends a complete tree...
    protected complete_tree<
      typename std::conditional< std::is_enum<I>::value, 
        typename std::underlying_type< 
          typename std::conditional<std::is_enum<I>::value, I, ignore_me>::type>::type,
        I>::type,
      std::tuple<I, Real, Real> >,

    //... using the heap mix-in
    protected heap< fast_random_selector<I, precision>, 
      typename std::conditional< std::is_enum<I>::value, 
        typename std::underlying_type< 
          typename std::conditional<std::is_enum<I>::value, I, ignore_me>::type>::type,
        I>::type >,

    //... and the weightsum tree mix-in
    protected weightsum_tree< fast_random_selector<I, precision>,
      typename std::conditional< std::is_enum<I>::value, 
        typename std::underlying_type< 
          typename std::conditional<std::is_enum<I>::value, I, ignore_me>::type>::type,
        I>::type,
      precision>,

    //... AND the indexed collection mix-in
    protected indexed_collection<  fast_random_selector<I, precision>, 
      I, 
      typename std::conditional< std::is_enum<I>::value, 
        typename std::underlying_type< 
          typename std::conditional<std::is_enum<I>::value, I, ignore_me>::type>::type,
        I>::type,
      std::tuple<I, Real, Real>>
  {

    private:

      template <typename E>
      using underlying_if_enum = typename std::conditional<
        std::is_enum<E>::value,
        typename std::underlying_type<
          typename std::conditional<std::is_enum<E>::value, E, ignore_me>::type
        >::type,
        E
      >::type;

    public:

      using size_type = std::ptrdiff_t;
      using index_type = I;
      using This = fast_random_selector<index_type, precision>;
      using node_type = underlying_if_enum<index_type>;
      using value_type = std::tuple<index_type, Real, Real>;
      using iterator = value_type*;
      using const_iterator = value_type const*;
      using reference = value_type&;
      using const_reference = value_type const&;
      using BaseTree = complete_tree<node_type, value_type>;
      using Heap = heap<This, node_type>;
      using WeightSum = weightsum_tree<This, node_type, precision>;
      using Index = indexed_collection<This, index_type, node_type, value_type>;

      friend Heap;
      friend Index;
      friend WeightSum;
   
      fast_random_selector() = delete;

      template<typename InputIt>
      __attribute__ ((noinline)) fast_random_selector(InputIt first, InputIt last) :
          BaseTree(),
          Heap(),
          WeightSum(),
          Index(static_cast<index_type>(last-first))
      {
        // add weights to BaseTree
        InputIt it = first;
        for (index_type i = 0; it != last; ++it, ++i) {
          double w = *it;
          BaseTree::add_entry(std::tuple<index_type, Real, Real>(i, Real(w), 0.0));
        }

        // Heap BaseTree by weight
        std::make_heap(BaseTree::begin(), BaseTree::end(), 
            [](const std::tuple<index_type, Real, Real> &a,
            const std::tuple<index_type, Real, Real> &b){
            return std::get<1>(a) < std::get<1>(b);
          });
        //Compute WeightSum weights and map indexes
        WeightSum::compute_weights();
      }

      fast_random_selector(fast_random_selector const&) = default;

      fast_random_selector(fast_random_selector &&) = default;

      fast_random_selector& operator=(fast_random_selector const&) = default;

      fast_random_selector& operator=(fast_random_selector &&) = default;

      ~fast_random_selector() = default;
      
      
      //Methods of WeightSum we want to make available
      template<class URNG>
      index_type __attribute__ ((noinline)) operator()(URNG& g) { 
        return id_of(WeightSum::operator()(g));
      }

      void __attribute__ ((noinline)) update_weight(index_type i, Real new_weight) {
        auto node = Index::node_for_index(i);
        Real old_weight = weight_of(node);
        WeightSum::update_weight(Index::node_for_index(i), new_weight);
        if (old_weight > new_weight)
          Heap::sift_up(node);
        else
          Heap::sift_down(node);
      }

      Real get_weight(index_type i) {
        return weight_of(Index::node_for_index(i));
      }

      Real total_weight() const { return WeightSum::total_weight(); }

    private:
      //Must call WeightSum::compute_weights() after this, before using random selection
      void add_entry(value_type&& v) {
        BaseTree::add_entry(v);
      }

      void add_entry(const value_type& v) {
        BaseTree::add_entry(v);
      }

      Real& weight_of(node_type n) {
        return std::get<1>(BaseTree::value_of(n));
      }
      
      const Real& weight_of(node_type n) const {
        return const_cast<This*>(this)->weight_of(n);
      }

      Real& weightsum_of(node_type n) {
        return std::get<2>(BaseTree::value_of(n));
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
        std::swap(BaseTree::value_of(a).second, BaseTree::value_of(b).second);
        WeightSum::swap(a, b);
        Index::swap(a, b);
      }

      void swap_with_child(node_type a, node_type b) {
        Index::swap(a, b);
        WeightSum::swap_with_child(a, b);
      }

      index_type& id_of(node_type p) { return std::get<0>(BaseTree::value_of(p)); }

      void special(node_type i){
        Index::associate(id_of(i), i);
      }
  };
}
}

#endif
