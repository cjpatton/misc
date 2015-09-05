#ifndef common_H
#define common_H

#define BUFFLENGTH 16777216 /* data buffer size ~ 1.6 mb */ 
#define MAXPENDING 5        /* number of pending connections to handle */ 

/* Command line options */ 
typedef struct { 

  char dir  [64]; 
  char addr [64]; 
  int port; 

} param_t; 

void die(const char *msg); 

int readline(char *buffer, int len);

  const char *check_addr( const char *addr ); 

int parse_options( param_t *options, int argc, const char **argv ); 

#endif /* common_H */
