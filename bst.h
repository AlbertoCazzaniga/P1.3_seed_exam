#ifndef __bst_h
#define __bst_h

#include <iostream>
#include <iterator> //iterator
#include <memory>  //unique_ptr
#include <utility> //pair
#include <vector> //vector


template <typename KT, typename VT>
struct node{
  std::pair<const KT, VT> _pair;
  std::unique_ptr<node> _left;
  std::unique_ptr<node> _right;
  node * _upper;
    
  //constructors (all custom)
  //custom "copy pair and upper" constructor
  node(const std::pair<const KT, VT> & pair, node * upper) : 
  _pair{pair}, _left{}, _right{}, _upper{upper} {} 
  
  //custom "move pair and copy _upper" constructor
  node(std::pair<const KT, VT> && pair, node * upper) :
  _pair{std::move(pair)}, _left{}, _right{}, _upper(upper) {}
};


template <typename KT, typename VT, typename cmp = std::less<KT>>
class bst{
  std::unique_ptr<node<KT,VT>> root;
  cmp op;
  
  //function first() returning pointer to node with element with the lowest key, support for begin() and const_begin functions
  node<KT,VT> * first() const noexcept;  
  
  //function for recursive insertion of nodes from pointer to node, support for the copy constructor
  void rec_copy(node<KT,VT> * n);
  
  //function inserting recursively in the tree the mid point between first and last of a vector of pairs, support for the balance() function   
  void insert_mid_from_vect(std::vector<std::pair<const KT, VT>> v, int first, int last);
    
  public:
  
  //CONSTRUCTORS
  //Copy constructor: calls recursively the rec_copy(node *) function starting from the head 
  bst(const bst & b): op{b.op} {
    rec_copy(b.root.get());  
  }
  
  //copy assigmenmt: standard from copy constructor
  bst& operator=(const bst& b) {
    root.reset();
    auto tmp{b}; 
    *this = std::move(tmp);
    return *this;
  }
  
  // default constructor
  bst() noexcept = default;
  
  //constructor from only comparison operator
  bst(cmp x) noexcept : root{}, cmp{x} {}
  
  // default move constructor and move assignments
  bst(bst && b) noexcept = default;
  bst& operator=(bst&& b) noexcept = default;
  
  
  
  //iterator class, begin and end functions
  template <typename O>
  class __iterator;
  
  using iterator = __iterator<std::pair<const KT, VT>>;
  using const_iterator = __iterator<const std::pair<const KT, VT>>;
  
  //IMPLEMENT BEGIN, CONST BEGIN, CONST CBEGIN, delegating to first()
  iterator begin() noexcept {return iterator{first()}; }
  iterator end() noexcept { return iterator{nullptr}; }
  
  const_iterator begin() const noexcept { return const_iterator{first()}; }
  const_iterator end() const noexcept { return const_iterator{nullptr}; }
  
  const_iterator cbegin() const noexcept { return const_iterator{first()}; }
  const_iterator cend() const noexcept { return const_iterator{nullptr}; }
   
    
  
  //MEMBER FUNCTIONS
  //insert new node from const l-value and r-value ref to key
  std::pair<iterator, bool> insert(const std::pair<const KT, VT>& pair);
  std::pair<iterator, bool> insert(std::pair<const KT, VT>&& pair);
  
  //emplace. Automatic conversion to pair, check.
  template < class...Types >
  std::pair<iterator, bool> emplace(Types&&...args) { return insert({std::forward<Types>(args)...}); }
  
  //clear function: resets root thus recursevely resetting all the nodes.
  void clear() noexcept { root.reset(); }
  
  //find
  iterator find(const KT & k);
  //const iterator find. NOTE: need const function as constant iterator has no non-constant access to data
  const_iterator find(const KT & k) const { return const_iterator{find(k)}; }  
  
  //balance(): after copying ordered pairs in a support vector, clear the tree and call the recursive function insert_mid_from_vect
  void balance();
  
  //subscripting operators: if key not find insert with default value 
  VT& operator[](const KT& k);
  VT& operator[](KT&& k);
  
  //put to operator (print message if tree is empty)
  friend  
  std::ostream& operator<<(std::ostream& os, const bst& b){
    if(!b.root){
      os << "Binary tree is empty.";
      return os;
    }  
    for( auto x : b){
      os << x.first << " " << x.second <<  "\n";
    }
    return os;
  }
};

//CLASS ITERATOR
template <typename KT, typename VT, typename cmp>
template <typename O>
class bst<KT, VT, cmp>::__iterator{
  node<KT,VT> * current;
  
  public:
    explicit __iterator(node<KT,VT> * x) noexcept : current{x} {}
    //define difference between iterator elements
    using difference_type = std::ptrdiff_t;
    //define category of iterator (can read data from the point-to element on)
    using iterator_category = std::forward_iterator_tag; 
    using value_type = O;
    using reference = value_type&;
    using pointer = value_type*;
    
    
    //"dereference" operator, standard
    reference operator*() const noexcept { return current-> _pair; }
    //operator ->() (uses "dereference operator")
    pointer operator->() const noexcept { return &(*(*this)); } 
    //pre-increment operator
    __iterator& operator++() noexcept {
      if(current->_right){
        current = current->_right.get();
        while(current->_left){
          current = current->_left.get();
        }
      }
      else{
        current = current->_upper;
      }
      return *this;
    }
    //post-increment operator (uses pre-increment operator)
    __iterator operator++(int) noexcept { 
      __iterator tmp{current};
      ++(*this);
      return tmp;
    }
    // equality operator
    friend bool operator==(const __iterator& a, const __iterator& b) {
      return a.current == b.current;
    }
    // inequality operator (uses equality operator)
    friend bool operator!=(const __iterator& a, const __iterator& b) {
      return !(a == b);
    } 
};





template <typename KT, typename VT, typename cmp>
node<KT,VT> * bst<KT,VT,cmp>::first() const noexcept{
    auto tmp = root.get();
    while( tmp->_left )
      tmp = tmp->_left.get();
    return tmp;
  }



template <typename KT, typename VT, typename cmp>  
void bst<KT,VT,cmp>::rec_copy(node<KT,VT> * n){
    if(!n)
      return; 
    insert(n->_pair);
    rec_copy(n->_left.get());
    rec_copy(n->_right.get());
  }



template <typename KT, typename VT, typename cmp>
std::pair<typename bst<KT,VT,cmp>::iterator, bool> 
bst<KT,VT,cmp>::insert(const std::pair<const KT, VT>& pair){
    if(!root){
      root.reset(new node<KT,VT>(pair, nullptr));
      return {iterator(root.get()), true};
    }
    auto tmp = root.get();
    while(true){
      if( op(pair.first, tmp->_pair.first) ){
        if(!tmp->_left){
          tmp->_left.reset(new node<KT,VT>(pair,tmp));
          return {iterator(tmp->_left.get()), true};
        }
        tmp = tmp->_left.get();
      }
      else if( op(tmp->_pair.first, pair.first) ){
        if(!tmp->_right){
          tmp->_right.reset(new node<KT,VT>(pair, tmp->_upper));
          return {iterator(tmp->_right.get()), true};
        }
        tmp = tmp->_right.get();
      }
      else
        return {iterator(tmp),false};
    }
  }

template <typename KT, typename VT, typename cmp>  
std::pair<typename bst<KT,VT,cmp>::iterator, bool> 
bst<KT,VT,cmp>::insert(std::pair<const KT, VT>&& pair){
    if(!root){
      root.reset(new node<KT,VT>(std::move(pair), nullptr));
      return {iterator(root.get()), true};
    }
    auto tmp = root.get();
    while(true){
      if( op(pair.first, tmp->_pair.first) ){
        if(!tmp->_left){
          tmp->_left.reset(new node<KT,VT>(std::move(pair),tmp));
          return {iterator(tmp->_left.get()), true};
        }
        tmp = tmp->_left.get();
      }
      else if( op(tmp->_pair.first, pair.first) ){
        if(!tmp->_right){
          tmp->_right.reset(new node<KT,VT>(std::move(pair), tmp->_upper));
          return {iterator(tmp->_right.get()), true};
        }
        tmp = tmp->_right.get();
      }
      else
        return {iterator(tmp),false};
    }
  }



template <typename KT, typename VT, typename cmp>
typename bst<KT,VT,cmp>::iterator bst<KT,VT,cmp>::find(const KT & k){
    auto tmp = root.get();
    while(tmp){
      if( op(k, tmp->_pair.first) ){
        tmp = tmp->_left.get();
      }
      else if ( op(tmp->_pair.first, k) ){
        tmp = tmp->_right.get();  
      }
      else return iterator(tmp);
    }
    return end();
  }



template <typename KT, typename VT, typename cmp> 
void bst<KT,VT,cmp>::insert_mid_from_vect(std::vector<std::pair<const KT, VT>> v, int first, int last) 
{
    int mid = (first+last)/2;
    insert(v[mid]);
    if(first >= last) return;
    insert_mid_from_vect(v, 0, mid-1);
    insert_mid_from_vect(v, mid+1, last);  
  }

template <typename KT, typename VT, typename cmp>  
void bst<KT,VT,cmp>::balance(){
    if(!root) return;
    std::vector<std::pair<const KT, VT>> v;
    for( auto pair : *this)
      v.push_back(pair);
    clear();
    insert_mid_from_vect(v, 0, v.size()-1);
  }



template <typename KT, typename VT, typename cmp>  
VT& bst<KT,VT,cmp>::operator[](const KT& k){
    iterator i = find(k);
    if( i==end() ) i = emplace(k,VT{}).first;
    return (*i).second; 
  }

template <typename KT, typename VT, typename cmp>  
VT& bst<KT,VT,cmp>::operator[](KT&& k){
    iterator i = find(k);
    if( i==end() ) i = emplace(std::move(k),VT{}).first;
    return (*i).second; 
  }
    
  
#endif
