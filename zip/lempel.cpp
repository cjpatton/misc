#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include "lempel.h"

#define BUFF_LENGTH 1024
#define ESC '\\'

void lempel::show() const {
  for (int i=0; i<size; i++) {
    if (table[i] && table[i] != DELETED)
      fprintf(stderr, "%-6d %.3s %d\n", i, table[i]->str, table[i]->index);
    else 
      fprintf(stderr, "%-6d\n", i);
  }
}

int lempel::hash(match_t *obj) const {
  return (int)obj->str[0]*7 + (int)obj->str[1]*11 + (int)obj->str[2]*13; 
}

bool lempel::cmp(match_t *a, match_t *b) const {
  return (a->str[0] == b->str[0] &&
          a->str[1] == b->str[1] &&
          a->str[2] == b->str[2]);
}

char in_buff [BUFF_LENGTH *2];
char out_buff [BUFF_LENGTH *2];


void zip(FILE *in_fd, FILE *out_fd) {
  lempel table;
  
  size_t read;
  match_t *match; 
  int i,j, match_len;

  while ((read=fread(in_buff, sizeof(char), BUFF_LENGTH, in_fd)) != 0) {
    table.clean();
    j = 0; 
    
    // insert characters in window
    for (i = 0; i < WINDOW; i++)
      out_buff[j++] = in_buff[i]; 

    // shift the window and look for matches
    for (;i < (int)read; i++) {
      
      // match, insert reference to it: (offset, length)
      if ((match=table.find( match_t(&in_buff[i-WINDOW]) )) != NULL) {
        // calculate match length  
        for ( match_len=0; match_len < MAX_MATCH
              && (match_len + i-WINDOW) < (int)read 
              && in_buff[i-WINDOW + match_len] == match->str[match_len];
              match_len ++ )
          ;
        // insert (offset, length)
        j += sprintf(&out_buff[j-WINDOW], "(%d,%d)", j - match->index, match_len) - WINDOW;
        i += match_len - WINDOW;
        out_buff[j++] = in_buff[i];
      } 
      
      // no match. insert new match, add char to window
      else {
        table.insert( match_t(&in_buff[i-WINDOW], j) );
        if (in_buff[i] == '(' || in_buff[i] == ESC) 
          out_buff[j++] = ESC;
        out_buff[j++] = in_buff[i];
      }

    }

    fwrite(out_buff, sizeof(char), j, out_fd); 
  }
}

void unzip(FILE *in_fd, FILE *out_fd) {
  size_t read;
  int i,j, offset, match_len;
  bool escape; 
  while ((read=fread(in_buff, sizeof(char), BUFF_LENGTH, in_fd)) != 0) {
    j = 0; 
    
    for (i=0; i<(int)read; i++) {

      if (escape && (in_buff[i] == '(' || in_buff[i] == ESC)) { 
        out_buff[j++] = in_buff[i];
        escape = false;
      }
      
      else if (in_buff[i] == '(') {
        sscanf(&in_buff[i], "(%d,%d)", &offset, &match_len);
        memcpy(&out_buff[j], &in_buff[i-offset], match_len);
        j += match_len; 
        for (;i<(int)read && in_buff[i]!=')';i++)
          ;
      }
      
      else if (in_buff[i] == ESC)
        escape = true;

      else 
        out_buff[j++] = in_buff[i];
      
    }

    fwrite(out_buff, sizeof(char), j, out_fd); 
  }




}
