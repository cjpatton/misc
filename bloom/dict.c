/* dict.c - Create and query a compact dictionary quickly. This would be more 
 * useful as a daemon. It might be good practice to set this up as a UNIX file 
 * socket that accepts as input a set of queries and returns the ones that that 
 * belong to the set. 
 *
 * Implenented with a bloom filter, a probabilistic data structure with low 
 * false positive probability and zero false negative probability. 
 *
 * Chris Patton ~3/20/14
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "bloom.h"

#define BUFF_LENGTH 2048

void die(const char *msg) 
{ 
  fprintf(stderr, "bloom: %s\n", msg); 
  exit(1); 
}

char *readline( char *buffer, unsigned long len, FILE *fp ) 
/* Preferable to fgets(), since it chops the rest of the 
 * line and doesn't read the new line character into the 
 * buffer. */
{
  unsigned long i, c = fgetc(fp); 
  for (i = 0; i < len - 1 && c != EOF && (char)c != '\n'; i++) {
    buffer[i] = (char)c;
    c = fgetc(fp); 
  }
  buffer[i] = '\0';

  while (c != EOF && c != '\n') 
    c = fgetc(fp); 
  
  return (c == EOF ? NULL : buffer); 
}


int main(int argc, const char **argv) 
{

  bloom_t filter; 
  char item[BUFF_LENGTH]; 
  unsigned long i; 

  if (argc == 4 && strcmp(argv[1], "-c") == 0) // -c dict n
  {
    unsigned long n = atoi(argv[3]); 
    if (n < 0) 
      die("error: n must be positive integer."); 

    init(&filter, n, 0.01); 
    write(&filter, argv[2]); 
    destroy(&filter); 
  }

  else if (argc == 3 && strcmp(argv[1], "-a") == 0) // -a dict < words 
  {
    if (!read(&filter, argv[2]))
      die("error: file not found.");

    while (readline(item, BUFF_LENGTH, stdin) != NULL) 
    {
      for (i = 0; i < filter.k; i++)
        set(&filter, djb2(item, filter.hashv[i]) % filter.m); 
    }
    write(&filter, argv[2]); 
    destroy(&filter); 
  }

  else if (argc == 3 && strcmp(argv[1], "-t") == 0) // -t dict < queries > words
  {
    die("uh-oh: not implemented."); 
  }
  
  else if (argc == 3 && strcmp(argv[1], "-s") == 0) // -s dict
  {
    if (!read(&filter, argv[2]))
      die("error: file not found.");
    
    printf("%s n=%ld, k=%ld, %.2f KB\n", argv[2], filter.n, filter.k, (float)BYTES(filter.m)/1024);
    destroy(&filter);   
  }

  else if (argc == 3) // dict query 
  {
    if (!read(&filter, argv[1]))
      die("error: file not found.");

    unsigned long hits = 0; 
    for (i = 0; i < filter.k; i++)
      hits += get(&filter, djb2(argv[2], filter.hashv[i]) % filter.m);

    if (hits == filter.k) 
      printf("Oh, you betch'ya!\n"); 
    else
      printf("Auf keinen Fall.\n"); 

    destroy(&filter); 
  }

  else die("usage: bloom -cat <dict> [n] / <dict> <query>"); 

  return 0; 
}
