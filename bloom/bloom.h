/* bloom.h - Prototypes for Bloom filter. 
 *
 * Chris Patton 3/20/14
 */ 

#define BYTES(s) (s / sizeof(char) + (s % sizeof(char) == 0 ? 0 : 1))

/* 64 uniform-random bits. */ 
unsigned long longrand(); 

/**
 * The bloom filter data structure. 
 */

typedef struct { 
  unsigned long m, n, k, *hashv; 
  char *bits; 
} bloom_t; 

void init( bloom_t *filter, unsigned long n, double p );
void destroy( bloom_t *filter ); 
int read( bloom_t *filter, const char *fn );
void write( bloom_t *filter, const char *fn );

void set( bloom_t *filter, unsigned long i ); 
void unset( bloom_t *filter, unsigned long i ); 
unsigned long get( bloom_t *filter, unsigned long i );

void display( bloom_t *filter );

/**
 * Hash functions. These accept an input string and an initial
 * hash value. 
 */

unsigned long djb2( const char *item, unsigned long hash ); 
