/* bloom.c - Implementation of a Bloom filter, following the description
 * available on Wikipedia. 
 *
 * Chris Patton 3/20/14
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "bloom.h"

unsigned long longrand() 
/* Generate a long random number.
 *  FIXME this is biased!!! */ 
{
  int i = 0, val = rand(); 
  unsigned long res = 0; 
  while (i < sizeof(unsigned long))
  {
    while (++i < sizeof(unsigned long) && val != 0)
    {
      res += val % 256; 
      res <<= 8; 
      val >>= 8; 
    }
    res += val % 256; 
    val = rand(); 
  }
  return res; 
}


/**
 * The bloom filter data structure. 
 */

void init( bloom_t *filter, unsigned long n, double p ) 
/* Initialize bloom filter. */ 
{
  int i; 

  /* n - Number of items in the set. */ 
  filter->n = n; 

  /* m - Optimal size of the bit array, assuming optimal 
   * numbeer of hash functions for a fixed false-positive
   * probability. */ 
  filter->m = (n * log(p)) / pow(log(2), 2) * (-1);
  filter->bits = (char *) calloc(BYTES(filter->m), sizeof(char)); 
  
  /* k - Optinmal number of hash functions to apply to 
   * each item. This only helps avoid false positives if
   * the hash functions are independent. */ 
  filter->k = ((double)filter->m/n) * log(2); 

  /* Hash functions used in this program accept an input string and 
   * an initial hash value. Our notion of independence is a set of 
   * uniform i.i.d variables over the bit array's residuals. For a 
   * particular string, the hashes are functions of independent 
   * random variables and are therefore also independent. This is 
   * obviously true for the hash function included in this file, 
   * djb2(). */ 
  srand( time(NULL) );
  filter->hashv = (unsigned long*) malloc(filter->k * sizeof(unsigned long));
  for (i = 0; i < filter->k; i++)
    filter->hashv[i] = longrand() % filter->m; 
} 

void destroy( bloom_t *filter )
/* Deallocate bit array. */ 
{ 
  free(filter->bits); 
  free(filter->hashv); 
}

int read( bloom_t *filter, const char *fn )
/* Read bit vector from file. */ 
{
  FILE *fp = fopen(fn, "rb"); 
  if (!fp) 
    return 0;
  fread(&(filter->n), sizeof(unsigned long), 1, fp); 
  fread(&(filter->m), sizeof(unsigned long), 1, fp); 
  fread(&(filter->k), sizeof(unsigned long), 1, fp); 
  filter->hashv = (unsigned long*) malloc(filter->k * sizeof(unsigned long));
  filter->bits = (char *) calloc(BYTES(filter->m), sizeof(char)); 
  fread(filter->hashv, sizeof(unsigned long), filter->k, fp);  
  fread(filter->bits, sizeof(char), BYTES(filter->m), fp);
  fclose(fp); 
  return 1; 
}

void write( bloom_t *filter, const char *fn )
/* Write bit vector from file. First unsigned long is length of the vector. */ 
{
  FILE *fp = fopen(fn, "wb"); 
  fwrite(&(filter->n), sizeof(unsigned long), 1, fp); 
  fwrite(&(filter->m), sizeof(unsigned long), 1, fp); 
  fwrite(&(filter->k), sizeof(unsigned long), 1, fp); 
  fwrite(filter->hashv, sizeof(unsigned long), filter->k, fp);
  fwrite(filter->bits, sizeof(char), BYTES(filter->m), fp);
  fclose(fp);   
}

void set( bloom_t *filter, unsigned long i ) 
/* Set the ith bit high. */ 
{
  filter->bits[i / sizeof(char)] = filter->bits[i / sizeof(char)] | (1 << (i % sizeof(char))); 
}

void unset( bloom_t *filter, unsigned long i ) 
/* Set the ith bit low. */
{
  filter->bits[i / sizeof(char)] = filter->bits[i / sizeof(char)] & ~(1 << (i % sizeof(char))); 
}

unsigned long get( bloom_t *filter, unsigned long i )
/* Get the ith bit. */ 
{ 
  return ((filter->bits[i / sizeof(char)]) & (1 << (i % sizeof(char)))) > 0;
}

void display( bloom_t *filter ) 
/* Display the bits as a bit string, 
 * starting with the first element. */ 
{
  unsigned long i; 
  for (i = 0; i < filter->m; i++) 
  {
    printf("%ld", get(filter, i)); 
  }
  printf("\n"); 
}


/**
 * Hash functions. 
 */

unsigned long djb2( const char *item, unsigned long hash ) 
/* Dan Bernstein, http://www.cse.yorku.ca/~oz/hash.html. 
 * Suggested initial value for hash is 5381. */
{
  unsigned long c; 

  while ((c = *item++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}
