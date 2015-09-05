
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMERIC(x) (x >= '0') && (x <= '9') 
#define ALPHABETIC(x) ((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z'))

void die(const char *msg) {
  perror(msg); 
  exit(1);
}

int readline(char *buffer, int len) {
  int i;
  for (i=0; i<len; i++) {
    scanf("%c", &buffer[i]);
    if (buffer[i] == '\n')
      break;
  }
  buffer[i] = '\0';
  return i;
}

const char *check_addr(const char *addr) 
{
  /* verify address format: www.xxx.yyy.zzz */
  int i, start=0, ct=0;  
  for (i=0; i<strlen(addr); i++) {
    if (addr[i] == '.') {
      if (i == start)                           /* '..' not valid */
        return (const char*)0; 
      start = i+1;
      ct ++; 
    } else if (addr[i] <= '9' && addr[i] >= '0') {
      if (i - start > 3) 
        return (const char*)0;                  /* subnet strings are 'xxx'. */
    } else {                              
      return (const char*)0;                    /* no other characters are valid */
    }
  }
  if (ct > 3 || addr[strlen(addr)-1] == '.' ) 
    return (const char*)0;                      /* 'www.xxx.yyy.zzz' */

  return addr; 
}

int parse_options(param_t *options, int argc, const char **argv) 
{
  /* Parse command line options */
  options->dir[0] = options->addr[0] = '\0';
  options->port = -1; 

  int i; 
  for (i = 1; i < argc; i++) 
  {
    if (strcmp(argv[i], "-p") == 0 && (argc - i) > 1) {
      if (!NUMERIC(argv[i+1][0])) 
        return 0; 
      options->port = atoi(argv[++i]); 
      if (options->port < 0)
        return 0; 
    } 
    else if (strcmp(argv[i], "-a") == 0 && (argc - i) > 1) {
      i++; 
      if (check_addr(argv[i]) || strcmp(argv[i], "localhost") == 0)
        strncpy(options->addr, argv[i], 64); 
      else return 0; 
    }

    else if (strcmp(argv[i], "-d") == 0 && (argc - i) > 1) {
      i++; 
      strncpy(options->dir, argv[i], 64); 
    }
    
    else 
      return 0; 
  }
  
  if (options->port < 0) 
    options->port = 5000; 

  if (options->addr[0] == '\0')
    strcpy(options->addr, "127.0.0.1"); 

  if (options->dir[0] == '\0') 
    strcpy(options->dir, "./library");

  return 1; 

}
