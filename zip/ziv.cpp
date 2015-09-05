#include <cstring>
#include <cstdio>
#include "huffman.h"
#include "lempel.h"

using namespace std;

/* TODO
 * see test.cpp for some stuff
 * This gets messed up when piped. I think it's cause I'm lseeking the stdin
 * Also, convert huffman.cpp to use buffers instead of characters at a time.
 * 
 * Finally, pack it into actual bits
 * 
 *
 */



const char help [] = "\
usage: ziv -c,-d <file>\n\n\
  -h,--help  show this\n\n\
  -c         compress\n\n\
  -d         decompress\n\n\
  <file>     input file. outputs to <file>.ziv\n\
             If <file> is not specified, zip acts\n\
             as a stream.\n"; 


int main(int argc, char **argv) {

  int i;
  char *filename = NULL;
  bool compress = true;
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
        fprintf(stderr, "%s", help); 
        return 0; 
      } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--compress") == 0) {
      } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decompress") == 0) {
        compress = false; 
      } else {
        fprintf(stderr, "unkown option '%s'\n", argv[i]);  
        return 1; 
      }
    }
    else {
      filename = argv[i];
    }
  }

  //huffman hc; 
  if (compress && !filename) {
    zip(stdin,stdout);
    //hc.encode(stdin, stdout);
    //hc.print_codes();
  } else {
    unzip(stdin,stdout);
    //hc.decode(stdin, stdout); 
  }

  return  0; 
}
