#ifndef LEMPEL_H
#define LEMPEL_H
#include "hashtable.h"
#include <cstdio>

#define WINDOW 3
#define MAX_MATCH 32

/**
 * Lempel-Ziv
 */

struct match_t {
  match_t (char *s, int i) : index(i), str(s) 
  {
  }
  match_t (char *s) : index(-1), str(s) 
  {
  }
  match_t (): index(-1), str(NULL)
  {
  }
  int index; 
  char *str; 
};

class lempel: public hashtable<match_t> {
public:
  void show() const; 

private:
  int hash(match_t *obj) const;
  bool cmp(match_t *a, match_t *b) const;
};


void zip(FILE *in_fd, FILE *out_fd); 

void unzip(FILE *in_fd, FILE *out_fd); 

#endif
