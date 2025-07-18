#ifndef COMPLETE_TREE
#define COMPLETE_TREE

#include <utility>
#include <vector>
#include <functional>
#include <type_traits>
#include <sys/mman.h>
namespace dense {
namespace stochastic {

  typedef double Real;
  /* This complete tree class is meant to be subclassed to build various 
   * data structures.  Because most of those data structures require 
   * updates when values are modified, only const access to elements is 
   * granted publicly, while there are protected methods for non-const 
   * element access.
   */
  template <
    typename P,
    typename T
  >
  class complete_tree {

    public:

      using size_type = std::ptrdiff_t;
      using position_type = P;
      using entry_type = T;
      using reference = entry_type&;
      using const_reference = entry_type const&;

      complete_tree(position_type reserve = 0) {
        _tree.resize(reserve + 1, 0);
      }

      complete_tree(complete_tree const&) = default;

      complete_tree(complete_tree &&) = default;

      complete_tree& operator=(complete_tree const&) = default;

      complete_tree& operator=(complete_tree &&) = default;

      ~complete_tree() = default;

      //Tree property methods

      size_type size() const {
        return _tree.size();
      }

      size_type entries() const {
        return _tree.size() - 1;
      }

      bool empty() const {
        return _tree.empty();
      }

    public:
      //Position methods
      // Add delta to element A[i] and update tree
      void add(position_type i, entry_type delta) {
        while (i < size()) {
          value_of(i) += delta;
          i += (i & -i);  // Go to next node
        }
      }

      // Get prefix sum A[1] + A[2] + ... + A[i]
      entry_type prefix_sum(position_type i) const {
        entry_type sum = 0;
        while (i > 0) {
          sum += value_of(i);
          i -= (i & -i);  // Go to previous node
        }
        return sum;
      }

      // Get A[i] = sum(i) - sum(i - 1)
      entry_type get(position_type i) const {
        return prefix_sum(i) - prefix_sum(i - 1);
      }


      const_reference operator[](position_type i) const {
        return _tree[i];
      }

      const_reference at(position_type i) const {
        return _tree[i];
      }


      complete_tree const& const_this() const {
        return static_cast<complete_tree const&>(*this);
      }

    protected:
      reference value_of(position_type node) {
        return _tree[node];
      }
      const_reference value_of(position_type node) const {
        return _tree[node];
      }



      reference operator[](position_type i) {
        return _tree[i];
      }

      reference at(position_type i) {
        return _tree[i];
      }

      void setUpHugePages(){
        void* treeData = static_cast<void*> (_tree.data());
        const size_t size = (100 * 1024 * 1024);
        //entry_type memoryTocher;
        madvise(treeData,size,MADV_HUGEPAGE);
        // for (int i=0;i<_tree.size();i++){
        //   memoryToucher = _tree[i];
        // }
      }

      



    private:
      std::vector<entry_type> _tree;
      Real total_weight;
  };

}
}
#endif
