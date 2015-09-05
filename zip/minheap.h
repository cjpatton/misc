#ifndef MINHEAP_H
#define MINHEAP_H

/**
 * PRIORITY QUEUE
 */

template <class T>
class minheap {
  int index;
  T **heap; 

  void swap(int me, int you); 

public:

 minheap(int s);
  ~minheap(); 

  void push(T *inter); 
  T* pop ();
  bool empty() const;

};

#include "minheap.cpp"
#endif
