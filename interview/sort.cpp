/* Chris 
 * ~21 Jan 2013
 * Some sorting algorithms
 */ 

#include <iostream>
#include <cstdlib>
#include <assert.h>

#define LEFT(i) 2*i
#define RIGHT(i) (2*i) + 1
using namespace std; 

template <class T> 
void isort(T arr[], int size) {
  T tmp; 
  for (int i = 0; i < size; i++) {
    for (int j = i; j > 0 && arr[j] < arr[j-1]; j--) {
      tmp = arr[j-1]; 
      arr[j-1] = arr[j]; 
      arr[j] = tmp; 
    }
  }
}


template <class T> 
void swap(T arr[], int i, int j) {
  T tmp = arr[i]; 
  arr[i] = arr[j]; 
  arr[j] = tmp; 
}

template <class T> 
void qsort(T arr[], int left, int right) {
  if (right > left + 1) {
    int i = left + 1, j = right, pindex = (rand() % (right-left+1)) + left;
    T piv = arr[pindex]; 
    swap(arr, left, pindex); 
    while (i < j) {
      if (arr[i] < piv) 
        i++; 
      else   
        swap(arr, i, --j); 
    }
    swap(arr, --i, left);
    qsort( arr, left, i );
    qsort( arr, j, right );
  }
}


template <class T> 
class PriorityQueue {

  T *heap;
  int size, ct;

  void resize(int newSize) {
    T  *newHeap = new T [newSize]; 
    for (int i = 0; i < size; i++) 
      newHeap[i] = heap[i];
    delete [] heap;
    heap = newHeap; 
    size = newSize; 
  }

public:

  PriorityQueue( int s = 256 ) {
    size = s; 
    ct = 0; 
    heap = new T [size]; 
  }

  ~PriorityQueue() {
    delete [] heap; 
  }

  void enqueue(const T &object) {
    int index = ct; 
    if (++ct == size) 
      resize( size * 2 );
    heap[index] = object; 
    while (heap[index] < heap[index/2]) {
      swap(heap, index, index/2); 
      index /= 2; 
    }
  }

  T dequeue() {
    int min, i = 0; 
    T ret = heap[i];
    heap[i] = heap[--ct];
    
    if (RIGHT(i) < ct && heap[RIGHT(i)] < heap[LEFT(i)])
      min = RIGHT(i); 
    else min = LEFT(i); 
    while (2*i < ct && heap[min] < heap[i]) {
      swap(heap, min, i); 
      i = min; 
      if (RIGHT(i) < ct && heap[RIGHT(i)] < heap[LEFT(i)])
        min = RIGHT(i); 
      else min = LEFT(i); 
    }
    return ret; 
  }

  T &top() {
    return heap[0]; 
  }
  
  bool empty() const {
    return ct == 0;
  }

}; 

int main() {
  
  int nums [] = {12, 43, 2, 92, -34, 42, 2, 2, 7, 65, 0, 7, 9, 12, 43, 162, 2};
  PriorityQueue<int> q; 
  
  for (int i = 0; i < 17; i++) {
    cout << nums[i] << ' '; 
    q.enqueue( nums[i] ); 
  }
  cout << endl;

  while (!q.empty()) 
    cout << q.dequeue() << ' '; 
  cout << endl;

  //for (int i = 0; i < 17; i++)
  //  cout << nums[i] << ' '; 
  //cout << endl;


  
  return 0; 

}
