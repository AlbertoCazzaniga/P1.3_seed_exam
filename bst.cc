#include "bst.h"
#include <iostream>
#include <iterator> //iterator
#include <memory>  //unique_ptr
#include <utility> //pair
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <chrono>

#ifdef BENCH
int main(int argc, char* argv[]){
#else
int main(){
#endif

  #ifdef TEST
   
  // NOTE: begin and end of iterator, as well as its ++(), *(), and ->() operators, are automatically tested by transversing the tree.  
  
  // We re-construct the binary search tree of the exam test, and start performing some checks.
  
  
  bst<int, std::string> t1;
  std::pair<typename bst<int, std::string>::iterator,bool> in = t1.insert({8,"a"});
  in = t1.insert({3,"b"});
  in = t1.insert({1,"c"});
  in = t1.insert({10,"d"});
  in = t1.insert({14,"e"});
  in = t1.insert({13,"f"});
  in = t1.insert({6,"g"});
  in = t1.insert({4,"h"});
  in = t1.insert({7,"i"});
  
  
  // put-to-operator
  std::cout << "Exam sheet tree printed ordered with standard ordering by key value:\n" 
  << t1 << "\n" ;
  
  // insert()
  std::cout << "++++++++++++++++ insert() test: ++++++++++++++++" << std::endl; 
  std::cout << "The insertion truth for 7: " << (t1.insert({7, "error"}).second ? "true" : "false") << std::endl;
  
  std::cout << "The tree should be as above:\n"
  << t1 << "\n" ; 
  
  std::cout << "The insertion truth for 2: " << (t1.insert({2, "new element"}).second ? "true" : "false") << std::endl;
  
  std::cout << "The tree should contain pair {2,new element}:\n"
  << t1 << "\n" ; 
  
  // emplace() 
std::cout << "++++++++++++++++ emplace() test: ++++++++++++++++" << std::endl;
  std::cout << "The emplace truth for 7: " << (t1.emplace(7, "error").second ? "true" : "false") << std::endl;
  
  std::cout << "Tree should be as above:"
  << t1 << "\n"; 
  
  std::cout << "The emplace truth for 12: " << (t1.emplace(12, "emplaced element").second ? "true" : "false") << std::endl;
  std::cout << "The tree should contain pair {12,emplaced element}:\n"
  << t1 <<"\n"; 
  
  // constructors
  std::cout << "++++++++++++++++ constructors ++++++++++++++++" << std::endl;
  bst<int, std::string> t3, t5;
  bst<int, std::string> temp{t1};
  bst<int, std::string> temp2{t1};
  bst<int, std::string> t2{t1}; 
  t3 = t1;
  bst<int, std::string> t4 = std::move(temp);
  t5 = std::move(temp2);
  
  std::cout << "Copied t1 into t2 (copy), t3 (copy ass.), t4 (move), t5 (move ass.).\n";
   
  std::cout << "Tree t2: \n" << t2 << std::endl; 
  std::cout << "Tree t3: \n" << t3 << std::endl;
  std::cout << "Tree t4: \n" << t4 << std::endl;
  std::cout << "Tree t5: \n" << t5 << std::endl;
  
  // clear()
  std::cout << "++++++++++++++++ clear() test ++++++++++++++++" << std::endl;
  
  std::cout << "t1 before clear():\n" << t1 << std::endl;
  
  t1.clear();
  
  std::cout << "t1 after clear():\n" << t1 << "\n" << std::endl;
  
  // balance()
  std::cout << "++++++++++++++++ balance test ++++++++++++++++" << std::endl;
  std::cout << "Unbalanced t2 elements: \n" << t2 << std::endl;
  t2.balance();
  std::cout << "Balanced t2 elements: \n" << t2 << std::endl;
  
  
  // subscripting
  std::cout << "++++++++++++++++ subscripting operator test ++++++++++++++++" << std::endl;
  std::cout << "Value of key 2 of t2: \n" << t2[2] << "\n"<< std::endl;
  
  std::cout << "Value of key 2020 of t2: " << t2[2020] << "\nAnd tree t2 after subscription of non existing value 2020: \n" << t2 << std::endl;


  // comparison operator
  std::cout << "++++++++++++++++ reverse comparison operator ++++++++++++++++" << std::endl;
  using op_cmp = std::greater<int>;
  bst<int, std::string, op_cmp> t_rev;
  
  t_rev.insert({8,"a"});
  t_rev.insert({3,"b"});
  t_rev.insert({1,"c"});
  t_rev.insert({10,"d"});
  t_rev.insert({14,"e"});
  t_rev.insert({13,"f"});
  t_rev.insert({6,"g"});
  t_rev.insert({4,"h"});
  t_rev.insert({7,"i"});
  
  std::cout << "Exam sheet tree printed ordered with reverse ordering of key value:\n" << t_rev ;
  
  #endif
  
  #ifdef BENCH
  using namespace std::chrono;
  
  if(argc != 3){
    std::cout << "Need 2 arguments for this version of the program:\n"<<
    "N: number nodes, and N_keys: number keys for retreival." << std::endl;
    return 1;
  }
    
  const size_t N = atoi(argv[1]);
  const size_t N_keys = atoi(argv[2]);
  if( N < N_keys){
    std::cout << "First input N must be greater than second N_keys." << std::endl;
  } 
  
  double time_unb{}, time_ran{}, time_map{};
  bst<int,int>::iterator it{nullptr};
  std::map<int,int>::iterator it_map{nullptr};
    
  bst<int, int> random_tree{}, unbalanced_tree{};
  std::map<int, int> m{};
  
  std::vector<int> w{};
  
  // insert numbers from 1 to N in unbalanced tree (maximally unbalanced)
  for(size_t i = 0; i < N; i++){
    w.push_back(i);
    unbalanced_tree.emplace(i, 0);
  }

  // insert elements from 1 to N in random order in m and random_tree (randomly balanced)   
  std::random_shuffle( w.begin(), w.end() );
  for(size_t i = 0; i < N; i++) random_tree.emplace(w[i],0);
  for(size_t i = 0; i < N; i++) m.emplace(w[i], 0);
    
  std::vector<int> v{};
  for(size_t i =0; i < N_keys; i++) v.push_back(rand()%N);  
  
  auto start = high_resolution_clock::now();  
  for(size_t i =0; i < N_keys; i++){
    it = unbalanced_tree.find(v[i]);  
  }
  
  auto end = high_resolution_clock::now();
  time_unb = duration_cast<nanoseconds>(end - start).count();
  
  start = high_resolution_clock::now();
  for(size_t i =0; i < N_keys; i++){
    it = random_tree.find(v[i]);  
  }
  
  end = high_resolution_clock::now();
  time_ran = duration_cast<nanoseconds>(end - start).count();
  
  start = high_resolution_clock::now();
  for(size_t i =0; i < N_keys; i++){
    it_map = m.find(v[i]);
  }

  end = std::chrono::high_resolution_clock::now();
  time_map = duration_cast<nanoseconds>(end - start).count();
  std::cout << time_unb * 10e-6 << " " <<  time_ran * 10e-6  << " " << time_map * 10e-6 << std::endl;
  #endif
  
return 0;
}
