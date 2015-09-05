/* Chris ~31 Jan 2013
 * A splay tree
 */

#include <iostream>
#include <cstdlib> /* rand() */ 
#include <cstring>
using namespace std; 

class Error {
friend ostream& operator<< (ostream& out, const Error &err);
private:
  char message [256]; 

public:
  Error(const char *msg) {
    strncpy(message, msg, 256 - 1); 
  }
}; 

ostream& operator<< (ostream& out, const Error &err) {
  out << "BST Error: " << err.message; 
  return out; 
}

namespace BalancingStrategy 
{

  /* Plain binary search tree ... don't do any balancing. */
  template <class Comparable>
  class None {
  protected:
    None *left, *right, *parent; 
    Comparable object; 

  public: 

    None(const Comparable &obj, None *l, None *r, None *p) {
      left = l; 
      right = r; 
      parent = p; 
      object = obj; 
    }

    ~None() {
      if (left) 
        delete left; 
      if (right) 
        delete right; 
    }

    void insert (const Comparable &obj) {
      if (obj < object) {
        if (left) 
          left->insert(obj); 
        else 
          left = new None (obj, NULL, NULL, this); 
      } else if (obj > object) {
        if (right) 
          right->insert(obj); 
        else 
          right = new None (obj, NULL, NULL, this); 
      } else {
        object = obj; 
      }
    }

    void remove (const Comparable &obj) {
      if (obj < object) {
        if (left) 
          left->remove(obj); 
        else throw Error("not found");
      } else if (obj > object) {
        if (right) 
          right->remove(obj); 
        else throw Error("not found"); 
      } else {
                 
      }
    }

    const Comparable& find (const Comparable &ovj) {
    }

    void traverse() const {
      if (left) 
        left->traverse(); 
      cout << object << endl; 
      if (right) 
        right->traverse(); 
    }

  };

  /* Balance by splaying. O(log n) in amortized time */
  template <class Comparable> 
  class Splaying : public None<Comparable> {

  }; 

  /* Red black tree. Balance guarentee with little overhead. */ 
  template <class Comparable> 
  class RedBlack : public None<Comparable> {

  };

} // namespace BalancingStrategy


template <class T> 
class BST {
public: 

  BST() {
    root = NULL; 
  }

  ~BST() {
    if (root) 
      delete root; 
  }

  void insert(const T &obj) {
    if (root) 
      root->insert(obj); 
    else 
      root = new BalancingStrategy::None<T> (obj, NULL, NULL, NULL); 
  }

  void remove(const T &obj) {
    if (root) 
      root->remove(obj); 
  }

  const T& find(const T &obj) {
    if (root) 
      return root->find(obj); 
  }
 
  void traverse() const {
    root->traverse();
  }

private: 
  BalancingStrategy::None<T> *root; 
}; 




int main()
{
  int nums [] = {12, 43, 2, 92, -34, 42, 7, 65, 0, 9, 162}; 

  BST<int> tree; 
  for (int i = 0; i < 11; i++) 
    tree.insert(nums[i]); 

  tree.traverse(); 

  return 0; 
}
