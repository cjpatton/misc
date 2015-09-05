#ifndef HUFFMAN_H
#define HUFFMAN_H

/**
 * TREE NODE
 */

struct node_t {
node_t(int w=0, node_t *p=NULL, node_t *l=NULL, node_t *r=NULL) 
  {
    weight = w; 
    parent = p;
    left = l;
    right = r;
    leaf = '\0';
  }
  int weight; 
  node_t *parent, *left, *right;
  bool edge;
  char leaf; 
}; 

/**
 * HUFFMAN CODER 
 */

class huffman {
public: 

  huffman();
  ~huffman();

  void encode(FILE *in_fd, FILE *out_fd);
  void decode(FILE *in_fd, FILE *out_fd);

  void print_codes() const;
  
private: 
  node_t *hc, *root; 
  int ct; /* number of leaves in tree */
  
  node_t *huff();
  void free_node(node_t *n);

  void build_tree(FILE *fd); 
  void encode_tree(FILE *out_fd); 
  void encode_text(FILE *in_fd, FILE *out_fd);
  
  void decode_tree(FILE *in_fd);
  void decode_text(FILE *in_fd, FILE *out_fd);
 
};

#endif
