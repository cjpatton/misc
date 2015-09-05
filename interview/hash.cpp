/* Chris 
 * ~21 Jan 2013 
 * A table for hashing. Linear probing. */

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std; 


class Person {
friend ostream &operator<< (ostream &out, const Person &p); 
public:
  char job [128]; 
  int age; 

  Person(const char *str, int ag) {
    strncpy(job, str, 128); 
    age = ag; 
  }
};

ostream &operator<< (ostream &out, const Person &p) {
  out << p.job << ' ' << p.age; 
  return out;
}

int hashPerson(const string &str) {
  return str.size(); 
}


template <class T, class K, int (*H)(const K&)> 
class Hash {

  struct entry_t {
    entry_t(const K &k, const T &d) : key(k), data(d) {}
    K key; 
    T data; 
  };
  
  entry_t** table; 
  const entry_t *DELETED; 

  int size, ct; 

public: 

  Hash( int s = 19 ) {
    size = s; 
    ct = 0; 
    table = new entry_t* [s]; 
    memset(table, 0, s * sizeof(entry_t*));
  }

  ~Hash() {
    for (int i = 0; i < size; i++)
      if (table[i] && table[i] != DELETED) 
        delete table[i]; 
    delete [] table; 
  }

  T *insert( const K &key, const T &data ) { 
    if (ct > size) 
      return NULL;  
    int i = H(key) % size; 
    while (table[i] && table[i] != DELETED && table[i]->key != key ) 
      i = (i + 1) % size;
    
    if (!table[i] || table[i] == DELETED) 
      table[i] = new entry_t (key, data); 
    else 
      table[i]->data = data;

    ct++; 
    return &table[i]->data; 
  }

  void remove( const K &key ) {
    int i = H(key) % size, checked = 0; 
    while (checked != size && table[i] && (table[i] == DELETED || table[i]->key != key)) {
      i = (i + 1) % size;
      checked ++; 
    }
    if (checked == size || !table[i] || table[i] == DELETED) {
    } else {
      delete table[i]; 
      table[i] = DELETED; 
    }
    
  }

  T *find( const K &key ) {
    int i = H(key) % size, checked = 0; 
    while (checked != size && table[i] && (table[i] == DELETED || table[i]->key != key)) {
      i = (i + 1) % size;
      checked ++; 
    }
    if (checked == size || !table[i] || table[i] == DELETED) 
      return NULL; 
    return &table[i]->data; 
  }
  
  void disp() const {
    cout << "--------------------------------------\n"; 
    for (int i = 0; i < size; i++) {
      printf("- %-8d", i); 
      if (table[i] == NULL) 
        cout << "NULL" << endl; 
      else if (table[i] == DELETED) 
        cout << "DELETED" << endl;
      else 
        cout << table[i]->key << " -> " << table[i]->data << endl;
    }
    cout << "--------------------------------------\n"; 
 }
}; 






int main() {

  Hash<Person, string, hashPerson> htable; 
  htable.insert(string("Joey"),    Person("eskimo pimp", 23));
  htable.insert(string("Carlos"),  Person("shitter", 97));
  htable.insert(string("Greg"),    Person("Lord and savior", 97));
  htable.insert(string("Julie"),   Person("magnet penis sculpture and sniffler", 24));

  htable.disp(); 
  
  htable.remove(string("Joey")); 
  
  htable.disp(); 
  


  return 0; 
}
