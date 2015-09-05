#include <cstdio>
#include <cstring>
#include "lempel.h"
using namespace std;


 /* TODO 
  * hashtable is "good enough". Seems to work without extensive testing
  * test it with input file 'dij' just to make sure.
  *
  * test it y'all. in'n'out.  
  *
  */


int main() {
  
 zip(stdin, stdout);
  

  /*
  lempel table;

  for (int i=0; i<strlen(text)-2; i++)
    table.insert( match_t(&text[i], i) );

  for (int i=0; i<strlen(text)-2; i++)
    if (!table.find( match_t(&text[i]) ))
      printf("shit\n");
  */

  return 0;
}
