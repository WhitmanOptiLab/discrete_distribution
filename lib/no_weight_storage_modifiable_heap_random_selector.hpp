#ifndef LOW_STORAGE_SELECTOR 
#define LOW_STORAGE_SELECTOR

#include <utility>
#include <vector>
#include <functional>
#include <type_traits>

#include "completetree.hpp"
#include "heap.hpp"
#include "weightsum_tree.hpp"
//#include "indexed_collection.hpp"

namespace dense {
namespace stochastic {

// namespace {
//    class ignore_me{};enum
// }

  template <
    typename I = int, size_t precision = std::numeric_limits<Real>::digits
  >
  class low_storage_selector :
    //Extends a complete tree...
    protected complete_tree<I, Real>,

    //... using the heap mix-in
    protected heap< low_storage_selector<I, precision>, I>,

    //... and the weightsum tree mix-in
    protected weightsum_tree< low_storage_selector<I, precision>, I, precision>

    //... AND the indexed collection mix-in
    //protected indexed_collection<  low_storage_selector<I, precision>, I, I, std::tuple<I, Real, Real>>
  

  {
      
 
    public:

      using size_type = std::ptrdiff_t;
      using index_type = I;
      using This = low_storage_selector<index_type, precision>;
      using node_type = index_type;
      using value_type =  Real;
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

      low_storage_selector() = delete;

      template<typename InputIt>
      low_storage_selector(InputIt first, InputIt last) :
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
          push(std::tuple<index_type, Real, Real>(i, Real(w), 0.0)); //TODO - change construction
          //std::cout<<"after construction cycle tree is ";
          //this->printTree();
          //std::cout<<std::endl;
        }
        //WeightSum::compute_weights();
      }

    

      low_storage_selector(low_storage_selector const&) = default;

      low_storage_selector(low_storage_selector &&) = default;

      low_storage_selector& operator=(low_storage_selector const&) = default;

      low_storage_selector& operator=(low_storage_selector &&) = default;

      ~low_storage_selector() = default;

      // Real sum_weights(PosType i) {
      //   if (i > _tree().last()) return 0.0;
      //   Real& weightsum = _tree().weightsum_of(i);
      //   Real self_weight = weightsum; // <- original weight, not actually a weightsum yet
      //   Real left_sum  = sum_weights(Tree::left_of(i));
      //   Real right_sum = sum_weights(Tree::right_of(i));

      //   Real total = self_weight + left_sum + right_sum;
      //   weightsum = total;
      //   return total;
    //}


      //Methods of WeightSum we want to make available
      template<class URNG>
      index_type operator()(URNG& g) {
        return id_of(WeightSum::operator()(g));
      }

      void update_weight(index_type i, Real new_weight) {
        auto node = index_to_node[i];
        auto origNode = node;
        Real old_weight = weight_of(node);
        Real difference = new_weight - old_weight;
        while (node!=BaseTree::root()){
            this->weightsum_of(node)+=difference;
            node = BaseTree::parent_of(node);
        }
        this->weightsum_of(node)+=difference;
        if (difference>0) //changed this bc I'm pretty sure the original is wrong - I'm confused
          Heap::sift_up(origNode);
        else
          Heap::sift_down(origNode);
        this->add_to_total_weight(difference);
        //std::cout<<"total weight now "<<this->total_weight();
      }

      Real get_weight(index_type i) {
        return weight_of(index_to_node[i]);
      }

      Real total_weight() const { return WeightSum::total_weight(); }

    private:

     void swap_with_child(node_type parent, node_type child){
      //std::cout<<"___________swap______________"<<std::endl<<"initial counditions "<<std::endl;
        //std::cout<<"tree before swap: ";
        //this->printTree();
        //std::cout<<std::endl;
        //std::cout<<"child in position "<<child<< " now has a weight of "<<this->weight_of(child)<<std::endl;
        //std::cout<<"parent in position "<<parent<<"now has a weight of "<<this->weight_of(parent)<<std::endl;

        Real weight_diff = this->weight_of(parent) - this->weight_of(child);
        this->weightsum_of(child)+=weight_diff;
        swap_indexes(parent, child);
        //std::cout<<"swap occured";
        //std::cout<<"original parent in position "<<child<< " now has a weight of "<<this->weight_of(child)<<std::endl;
        //std::cout<<"original child in position "<<parent<< " now has a weight of "<<this->weight_of(parent)<<std::endl;

     }

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
        value_type v = std::get<1>(e);//{std::get<1>(e),std::get<2>(e)};
        BaseTree::add_entry(v);
        auto newp = BaseTree::last();
		    index_type i = std::get<0>(e);
		    map_node(i, newp);
      }

      void add_entry(const entry_type& e) {
        value_type v = std::get<1>(e);//{std::get<1>(e),std::get<2>(e)};
        BaseTree::add_entry(v);
        auto newp = BaseTree::last();
		    index_type i = std::get<0>(e);
		    map_node(i, newp);
      }

      //NOTE this used to return a reference - now it doesn't - make sure to correct everywhere
      Real weight_of(node_type n) const{ //JUST ADDED - MIGHT DELETE
      node_type end = this->last();
      Real leftVal;
      node_type left = BaseTree::left_of(n);
      Real rightVal;
      node_type right = BaseTree::right_of(n);
      if (left<=end){
        leftVal = this->value_of(left);
        if (right<=end){
          rightVal = this->value_of(right);
        }
        else{
          rightVal=0;


        }
      }
      else{
        leftVal = 0;
        rightVal=0;
      }
    
      
    	return this->value_of(n) - leftVal - rightVal;//this->value_of(BaseTree::left_of(n) - this->value_of(BaseTree::right_of(n)));
	    }

    //   const Real& weight_of(node_type n) const {
    //     return const_cast<This*>(this)->weight_of(n);
    //   }

      Real& weightsum_of(node_type n) {
        return this->value_of(n);
      }

      void push(entry_type entry) {
        //std::cout<<std::endl<<std::endl<<"################  adding item  ###################"<<std::endl;
        //std::cout<<std::setprecision(7)<<"the weight being added is"<<std::get<1>(entry)<<std::endl;
        this->add_entry(entry);
        Real weight = std::get<1>(entry);
        this->add_to_total_weight(weight);
        node_type node = this->last();
        while(node!=this->root()){
          node = BaseTree::parent_of(node);
          this->weightsum_of(node)+=weight;
        }
        // if(this->last()!=this->root()){
        //   this->weightsum_of(node)+=std::get<1>(entry);
        // }
        //std::cout<<"element added to location"<< this->last()<< " is"<<std::to_string(this->at(this->last()))<<std::endl<<std::endl;
        //std::cout<<"after pushing tree is ";

        this->sift_up(this->last());
        //this->printTree();
        //std::cout<<std::endl;
      }
      
      const Real& weightsum_of(node_type n) const {
        return const_cast<This*>(this)->weightsum_of(n);
      }

      void remove_last_entry() {
        //auto last = BaseTree::value_of(BaseTree::last());
        /*WeightSum::*/update_weight(this->last(), 0);
        BaseTree::remove_last_entry();
      }

      low_storage_selector const& const_this() const {
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

      // void swap_with_child(node_type a, node_type b) {
      //   swap_indexes(a, b);
      //   WeightSum::swap_with_child(a, b);
      // }

      index_type& id_of(node_type node) {
        return node_to_index[node];
      }

      std::vector<index_type> node_to_index;
      std::vector<node_type> index_to_node;


  };
}
}

#endif
