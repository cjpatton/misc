#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <queue>

/** 
 * HASHTABLE 
 * yup. I'm gonna do it again
 */


template <class T>
class hashtable {
public: 
  hashtable(); 
  ~hashtable();
  void clean();

  T *find(T *obj) const; 
  T *find(T obj) const; 
  void insert(T *obj); 
  void insert(T obj); 
  void remove(T *obj); 
  
protected:
  T **table;
  T *DELETED; 
  int size, ct;
  std::queue<T*> death;
  
  void resize(int s);   
  virtual int hash(T *obj) const=0; 
  virtual bool cmp(T *a, T *b) const=0;

  bool isPrime(int n) const;
  int nextPrime(int n) const; 
};


#include "hashtable.cpp"
#endif
