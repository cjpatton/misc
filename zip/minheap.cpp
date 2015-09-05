#ifndef MINHEAP_CPP
#define MINHEAP_CPP

#include <cstdlib>
#include "minheap.h"

/**
 * PRIORITY QUEUE
 */

template <class T> 
minheap<T>::minheap(int s) {
  index = -1; 
  heap = new T* [s]; 
}

template <class T> 
minheap<T>::~minheap() {
  delete [] heap; 
}

template <class T> 
void minheap<T>::swap(int me, int you) {
  T *swp = heap[me];
  heap[me] = heap[you];
  heap[you] = swp;
}

template <class T> 
void minheap<T>::push(T *inter) {
  heap[++ index] = inter;
  int child = index;
  if (child==0) {
    return; // first value on heap
  }
  int parent = (child-1)/2;
  while (*heap[parent] > *heap[child])
  {	
    heap[child] = heap[parent];		
    heap[parent] = inter; 
    child = parent;
    parent = (child-1)/2;
  }
}

template <class T> 
T* minheap<T>::pop () {
  if (index==-1)
    return NULL;

  T *swp, *ret = heap[0];
  heap[0] = heap[index --];
  int min, parent = 0,
    cl = (parent*2)+1,
    cr = (parent*2)+2;
  while (1)
  {
    if (cl == index) min = cl; 
    else if (cr <= index) 
      min = (*heap[cl] < *heap[cr] ? cl : cr);
    else 
      break;
    if (*heap[parent] > *heap[min]) {
      swp = heap[parent];
      heap[parent] = heap[min];
      heap[min] = swp;
    }
    else 
      break;
    parent = min;
    cl = (parent*2)+1;
    cr = (parent*2)+2; 
  }
  return ret; 
}

template <class T> 
bool minheap<T>::empty() const {
  return (index == -1);
}

#endif
