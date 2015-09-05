#ifndef HASHTABLE_CPP
#define HASHTABLE_CPP

#include <cstdlib>
#include "hashtable.h"



template <class T>
hashtable<T>::hashtable() {
  size = 11; 
  ct = 0; 
  table = new T* [size];
  memset(table, 0, size*sizeof(T*)); 
}

template <class T>
hashtable<T>::~hashtable() {
  while (!death.empty()) {
    delete death.front(); 
    death.pop();
  }
  delete [] table; 
}

template <class T>
void hashtable<T>::clean() {
  while (!death.empty()) {
    delete death.front(); 
    death.pop();
  }
  memset(table, 0, size*sizeof(T*)); 
  ct = 0;
}


template <class T>
T* hashtable<T>::find(T *obj) const {
  T *curr;
  int i, h = hash(obj); 
  for (i=0; i<size; i++) {
    curr = table[(h+i) % size]; 
    if (curr == NULL) 
      return curr;
    else if (curr != DELETED && cmp(obj, curr))
      return curr;
  }
  return NULL;
}


template <class T>
T* hashtable<T>::find(T obj) const {
  return find(&obj); 
}


template <class T>
void hashtable<T>::insert(T *obj) {
  T *curr;
  int i, h = hash(obj); 
  for (i=0; i<size; i++) {
    curr = table[(h+i) % size]; 
    if (curr == NULL || curr == DELETED) {
      table[(h+i) % size] = obj;
      if (++ct > (2* size)/3) 
        resize(nextPrime(size * 2)); 
      return;
    } 
    else if (cmp(obj, curr))
      return;
  }
}

template <class T>
void hashtable<T>::insert(T obj) {
  T *p = new T;  
  *p = obj; 
  death.push(p); 
  insert(p); 
}

template <class T>
void hashtable<T>::remove(T *obj) {
  T *curr;
  int i, h = hash(obj); 
  for (i=0; i<size; i++) {
    curr = table[(h+i) % size]; 
    if (curr == NULL)
      return;
    else if (cmp(obj, curr)) {
      table[(h+i) % size] = DELETED;
      ct--;
      return;
    }
  }
}

template <class T>  
void hashtable<T>::resize(int s) { 
  int oldSize = size;
  T **oldTable = table;

  ct = 0;
  size = s; 
  table = new T* [size];
  memset(table, 0, size*sizeof(T*));

  for (int i=0; i<oldSize; i++) 
    if (oldTable[i] && oldTable[i] != DELETED)
      insert(oldTable[i]); 

  delete [] oldTable;
}



template <class T>
bool hashtable<T>::isPrime(int n) const {
  for(int i=2; i<(n/2)+1; i++)
    if (n % i == 0)
      return false;
  return true;
}

template <class T>
int hashtable<T>::nextPrime(int n) const {
  while (!isPrime(n)) 
    n++; 
  return n;
}

#endif
