/**
 * memcache.c, 2/12/14
 *
 * Figured I should see what all the fuss is about. Installing memcache on 
 * Ubuntu is as easy as `sudo apt-get install memcached`. The C API must be
 * downloaded and built from source. Go to: 
 *  <https://launchpad.net/libmemcached/+download> and get it. 
 * Build with `./configure && make && sudo make install`. Compile this 
 * program with `gcc -Wall -o memcache memcache.c -lmemcached`. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libmemcached/memcached.h>

int main(int argc, char **argv) 
{
  memcached_server_st *servers = NULL; 
  memcached_st        *memc; 
  memcached_return     rc; 

  char  *key, *value;
  size_t value_length; 

  memc = memcached_create(NULL); 

  servers = memcached_server_list_append(servers, "localhost", 11211, &rc); 
  rc = memcached_server_push(memc, servers); 
  if (rc != MEMCACHED_SUCCESS) 
  {
    fprintf(stderr, "memcache: error: couldn't add server: %s\n.",
                                        memcached_strerror(memc, rc)); 
    memcached_free(memc); 
    return 1; 
  }

  if (argc == 3) // Insert (key, value) pair 
  {
    key = argv[1];
    value = argv[2]; 
    rc = memcached_set(memc, key, strlen(key), value, strlen(value), 
             (time_t)0, (uint32_t)0);

    if (rc != MEMCACHED_SUCCESS)
    {
      fprintf(stderr, "memcache: error: couldn't store key: %s\n.", 
           memcached_strerror(memc, rc)); 
      memcached_free(memc);
      return 2; 
    }
  }

  else if (argc == 2) // Get value given key
  {
    key = argv[1]; 
    value = memcached_get(memc, key, strlen(key), &value_length, 
                                                  (uint32_t)0, &rc); 
    if (rc != MEMCACHED_SUCCESS)
    {
      fprintf(stderr, "memcache: error: couldn't get key: %s.\n", 
                                        memcached_strerror(memc, rc)); 
      memcached_free(memc);
      return 3; 
    }
    printf("%s\n", value); 
  }

  else 
  {
    fprintf(stderr, "usage: memcache <key> [<value>]\n");
  }

  memcached_free(memc);
  return 0; 
}
