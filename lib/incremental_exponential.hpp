#ifndef INCREMENTAL_EXPONENTIAL_LEAF_SUM_TREE
#define INCREMENTAL_EXPONENTIAL_LEAF_SUM_TREE
#include <vector>
#include <limits>
#include <random>
#include <cassert>
#include <iostream>
#include <queue>

namespace dense {
namespace stochastic {

  template <class int_type = size_t, class Real = double, size_t fanout = 16>
class kary_layered_complete_tree {
public:

    static_assert((fanout & (fanout - 1)) == 0, "fanout must be power of two");

    using position_type = std::pair<int_type,int_type>; //first is layer index, second is index in the layer

    kary_layered_complete_tree(std::vector<Real> leaves){
      //std::cout<<"kary layered complete tree constructor begun"<<std::endl;
        layers.push_back(leaves);
        node_count+=layers[0].size();
        std::vector<Real> nextRow;
        while(layers[layers.size()-1].size()>1){
          nextRow.clear();
          for(int i=0;i<layers[layers.size()-1].size()/fanout;i++){
              Real sum=0;
              for(int j=0;j<fanout;j++){
                  sum+=layers[layers.size()-1][i*fanout+j];
              }
              nextRow.push_back(sum);
          }
          if (layers[layers.size()-1].size()%fanout>0){
              Real sum=0;
              for(int i=0;i<layers[layers.size()-1].size()%fanout;i++){
                  sum+=layers[layers.size()-1][layers[layers.size()-1].size()-1-i];
              }
              nextRow.push_back(sum);

          }
          layers.push_back(nextRow);
          node_count+=nextRow.size();
        }
        //std::cout<<"kary layered complete tree constructor complete"<<std::endl;
    }

    // // Resize to accommodate at least n leaves
    // void resize(size_t n) {
    //     if (n == 0) {
    //         data_.assign(1, Real(0));
    //         leaf_start_ = 0;
    //         leaf_count_ = 0;
    //         return;
    //     }

    //     auto [total_nodes, leaf_start] = minimal_tree_shape(n);
    //     data_.assign(total_nodes, Real(0));
    //     leaf_count_ = n;
    //     leaf_start_ = leaf_start;
    // }

    size_t size() const {
        return node_count;
    }

    size_t num_children(const position_type& p)const{
      size_t layerSize = layers[p.first].size();
      if (p.second<layerSize-1){
        return fanout;
      }
      else{
        return fanout-((layerSize<<log2_fanout)-layers[p.first-1].size());
      }
    }

    size_t num_layers()const{
      return layers.size();
    }
    
        void printTree() const{
      std::cout<< "printing tree"<<std::endl;
      for(int i=num_layers()-1;i>=0;i--){
        std::cout<<"layer "<<i<<": ";
        for(int j=0;j<layers[i].size();j++){
          std::cout<<layers[i][j]<<", ";
        }
        std::cout<<std::endl;
      }
      std::cout<<std::endl;
    }

    size_t leaf_count() const{
      return layers[0].size();
    }

    Real& value_of(const position_type& p) {
        //assert(p < data_.size());
        return layers[p.first][p.second];
    }

    const Real& value_of(const position_type& p) const {
        // assert(p < data_.size());
        // return data_[p];
        return layers[p.first][p.second];
    }

    // Tree navigation (0-based indexing)
    position_type root() const {
        return std::pair<int_type,int_type>(layers.size()-1,0);
    }

    position_type parent_of(const position_type& i) const {
        //assert(i > 0);
        return std::pair<int_type,int_type>((i.first)+1,(i.second)>>log2_fanout);
    }

    void parent_of_inplace (position_type &i) const{
      i.first+=1;
      i.second=i.second>>log2_fanout;
    }

    position_type first_child_of(const position_type& i) const {
        return std::pair<int_type,int_type>((i.first)-1,(i.second)<<log2_fanout);
    }

    void first_child_of_inplace (position_type &i) const{
      i.first-=1;
      i.second=i.second>>log2_fanout;
      return;
    }

    position_type child_index(const position_type& parent, size_t child_num) const {
      position_type fc=first_child_of(parent);
      fc.second+=child_num;
      return fc; 
    }

    void child_index_inplace (position_type& parent, size_t child_num) const{
      first_child_of_inplace(parent);
      parent.second+=child_num;
      return;
    }

    bool is_leaf(const position_type& i) const {
        return (i.first==0);
    }

    // size_t leaf_start() const {
    //     return leaf_start_;
    // }

private:
    std::vector<std::vector<Real>> layers;
    size_t node_count=0;

    static constexpr size_t log2_fanout = [] {
        size_t v = fanout;
        size_t r = 0;
        while (v > 1) {
            v >>= 1;
            ++r;
        }
        return r;
    }();
};


//Class to randomly select an index where each index's probability of being
//selected is weighted by a given vector. Supports O(log(n)) selection and
//update while using about O(n) memory space.
// Uses exponential tree with fixed fanout.
template <
  class int_type = size_t,
  class Real = double,
  size_t fanout = 16,
  size_t precision = std::numeric_limits<Real>::digits
>
class incremental_exponential : protected kary_layered_complete_tree<int_type, Real, fanout> {
  using This = incremental_exponential<int_type, Real, fanout, precision>;
  using BaseTree = kary_layered_complete_tree<int_type, Real, fanout>;
  using PosType = typename BaseTree::position_type;

public:
  using result_type = int_type;

  class Param {
  public:
    Param(const std::vector<Real>& weights) : weights_(weights) {}
    Param(std::vector<Real>&& weights) : weights_(std::move(weights)) {}

    std::vector<Real> weights() const { return weights_; }

    bool operator==(const Param& other) const { return weights_ == other.weights_; }
    bool operator!=(const Param& other) const { return !(*this == other); }

  private:
    std::vector<Real> weights_;
    friend class incremental_exponential<int_type, Real, fanout, precision>;
  };

  // Default constructor
  incremental_exponential() : BaseTree(1){}

  // Construct from vector
  incremental_exponential(const std::vector<Real>& weights)
    : BaseTree(weights) {}

  // Construct from initializer list
  incremental_exponential(const std::initializer_list<Real>& il)
    : incremental_exponential(il.begin(), il.end()) {}

  // Construct from iterator pair
  template<class InputIt>
  incremental_exponential(InputIt first, InputIt last)
    : BaseTree(weightItToVector(first,last)){}

  template<class InputIt>
  std::vector<Real> weightItToVector(InputIt first, InputIt last){
    std::vector<Real> weights;
    for (auto it=first;it!=last;it++){
      weights.push_back(*it);
    }
    return weights;
  }

  // No-op reset
  void reset() {}

  bool operator==(const This& other) const {    // for(int i=0; i<5;i++){
    //     selector.update_weight(numWeights-1-i,5);
    // }
    return this->param() == other.param();
  }

  bool operator!=(const This& other) const {
    return !(*this == other);
  }

  // Select weighted random leaf index
  template<class URNG>
  result_type operator()(URNG& g) const {
    Real total = total_weight();
    if (total <= Real(0)) return 0;

    Real target = std::generate_canonical<Real, precision, URNG>(g) * total;
    if (target == Real(0)) return 0;

    //std::cout<<"total weight is "<<total<< " target is "<<target<<std::endl;

    PosType node = BaseTree::root(); // root index
    while (node.first > 0) {
      PosType child = BaseTree::first_child_of(node);
      Real cumulative = Real(0);

      size_t chosen_child = fanout; // invalid by default
      //std::cout<<"num children is "<<BaseTree::num_children(node)<<std::endl;;

      //for (size_t c = 0; c <fanout; c++) {
      for (size_t c = 0; c < BaseTree::num_children(node); c++) {
        Real w = weightsum_of(child);
        //std::cout<<"target is "<<target<<" cummulative is "<<cumulative<<" child index is "<<child.second<<std::endl;
        if (target < cumulative + w) {
          target -= cumulative;
          node = child;
          chosen_child = c;
          //std::cout<<"next child selected! node layer: "<<node.first<<" node index "<<node.second<<" node weight "<<weightsum_of(node)<<" chosen child index "<<chosen_child<<"target is "<<target<<std::endl;
          break;
        }
        child.second++;
        cumulative += w;
      }
      if (chosen_child == fanout) {
        // All children zero or no suitable child found
        break;
      }
    }

    // node is leaf, return index relative to leaf_start_
    return static_cast<result_type>(node.second);
  }

  template<class URNG>
  result_type operator()(URNG& g, const Param& param) const {
    incremental_exponential temp(param.weights_);
    return temp(g);
  }

  // Return probabilities vector
  std::vector<Real> probabilities() const {
    std::vector<Real> probs(BaseTree::leaf_count(), Real(0));
    Real total = total_weight();
    if (total <= Real(0)) return probs;

    for (std::pair<int_type,int_type> i(0,0); i.second < BaseTree::leaf_count(); ++i.second) {
      probs[i] = weightsum_of(i) / total;
    }
    return probs;
  }

  Param param() const {
    std::vector<Real> weights(BaseTree::leaf_count());
    for (size_t i = 0; i < BaseTree::leaf_count(); ++i) {
      weights[i] = get_weight(i);
    }
    return Param(weights);
  }

  void param(const Param& p) {
    *this = incremental_exponential(p.weights_);
  }

  static constexpr result_type min() { return 0; }

  result_type max() const { return BaseTree::leaf_count() == 0 ? 0 : BaseTree::leaf_count() - 1; }

  // Update a leaf weight and propagate change upward
  void update_weight(int_type leafIndex, Real new_weight) {
    std::pair<int_type,int_type> i(0,leafIndex);
    assert(new_weight >= Real(0));
    Real diff = new_weight - weightsum_of(i);
    weightsum_of(i) = new_weight;
    while (i.first < BaseTree::num_layers()-1) {
      i = BaseTree::parent_of(i);
      weightsum_of(i) += diff;
    }
  }

  void printAPair(std::string message,std::pair<int_type,int_type> pair){
    std::cout<<message<<":       layer"<<pair.first<<"   index"<<pair.second<<std::endl;
  }

  Real get_weight(PosType i) const {
    assert(i < BaseTree::leaf_count());
    return weightsum_of(i);
  }

  size_t size() const { return BaseTree::leaf_count(); }

  Real total_weight() const noexcept {
    if (BaseTree::size() == 0) return Real(0);
    return weightsum_of(BaseTree::root());
  }

  void printTree(){
    BaseTree::printTree();
  }


private:
  Real& weightsum_of(PosType p) { return BaseTree::value_of(p); }
  const Real& weightsum_of(PosType p) const { return BaseTree::value_of(p); }
};
}
}
#endif