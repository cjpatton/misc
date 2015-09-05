/* Chris ~30 Jan 2013
 * ADTs
 */

#include <iostream>
using namespace std; 


/**
 * Queue of pointers! Don't Deallocate. 
 */

template <class T> 
class PointerQueue {
public: 

  PointerQueue();
  ~PointerQueue(); 

  void enqueue( T *object ); 

  T   *dequeue();

  T   *front() const; 

  bool empty() const; 

private: 

  struct entry_t {
    entry_t *next; 
    T       *object;  
    entry_t (T *obj, entry_t *n) {
      object = obj; 
      next = n; 
    }
  }; 

  entry_t *head, *tail;

}; 

template <class T>
PointerQueue<T>::PointerQueue() {
  head = tail = NULL; 
}

template <class T>
PointerQueue<T>::~PointerQueue() {
  while (!empty())
    dequeue(); 
}

template <class T>
void PointerQueue<T>::enqueue( T *obj ) {
  if (!head) 
    head = tail = new entry_t(obj, NULL); 
  else 
    tail = tail->next = new entry_t(obj, NULL); 
}

template <class T>
T *PointerQueue<T>::dequeue() {
  T       *obj = head->object; 
  entry_t *tmp = head; 
  head = head->next; 
  delete tmp; 
  return obj; 
} 

template <class T>
T *PointerQueue<T>::front() const {
  if (head) 
    return head->object; 
  else 
    return NULL; 
}

template <class T>
bool PointerQueue<T>::empty() const {
  return (head == NULL); 
}


/**
 * Stack of pointers! Don't deallocate. 
 */

template <class T>
class PointerStack {
public:

  PointerStack(); 
  ~PointerStack(); 

  void push( T *obj ); 

  T   *pop(); 

  T   *top() const; 

  bool empty() const; 

private:

  struct entry_t {
    entry_t *next; 
    T       *object; 
    entry_t(T *obj, entry_t *n) {
      object = obj; 
      next = n; 
    }
  };

  entry_t *head; 

}; 

template <class T>
PointerStack<T>::PointerStack() {
  head = NULL;
}

template <class T>
PointerStack<T>::~PointerStack() {
  while (!empty())
    pop(); 
}

template <class T>
T *PointerStack<T>::top() const {
  if (head) 
    return head->object; 
  else 
    return NULL; 
}

template <class T>
void PointerStack<T>::push( T *obj ) {
  head = new entry_t(obj, head); 
}

template <class T>
T *PointerStack<T>::pop() {
  T       *obj = head->object; 
  entry_t *tmp = head;
  head = head->next; 
  delete tmp; 
  return obj; 
}

template <class T>
bool PointerStack<T>::empty() const {
  return (head == NULL); 
}


/**
 * A dumb type
 */ 

class Int {
friend ostream& operator<<( ostream &out, const Int &integer );
int val; 
public: 

  Int(int v) {
    val = v; 
  }

};

ostream& operator<<( ostream &out, const Int &integer ) {
  out << integer.val; 
  return out; 
}


/**
 * Main - do something boring. 
 */ 

int main() {

  PointerQueue<Int> Q; 
  PointerStack<Int> S; 
  for (int i = 0; i < 10; i++) {
    Q.enqueue( new Int(i+1) );
  }

  while (!Q.empty()) {
    S.push( Q.front() ); 
    cout << *Q.dequeue() << endl;
  }

  while (!S.empty()) {
    cout << *S.top() << endl;
    delete S.pop(); 
  }

  return 0; 
}
