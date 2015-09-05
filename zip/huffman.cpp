#include <cstring>
#include <cstdio>
#include <queue>
#include <stack>
#include "huffman.h"
#include "minheap.h"

/**
 * Huffman encoding
 *
 * TODO add file i/o 
 * TODO do on bytes instead of text!
 * TODO Lempel-Ziv
 * TODO test minheap (?)
 *
 * Then we can do Lempel-Ziv and encode its output
 * with huffman codes! --> gzip
 */

bool operator< (const node_t &a, const node_t &b) {
  return a.weight < b.weight;
}

bool operator> (const node_t &a, const node_t &b) {
  return a.weight > b.weight;
}


/**
 * HUFFMAN CODER 
 * public methods
 */


huffman::huffman() {
  hc = root = NULL;
}

huffman::~huffman() {
  if (root)   
    free_node(root);
  if (hc) 
    delete [] hc; 
}


void huffman::encode(FILE *in_fd, FILE *out_fd) {
  build_tree(in_fd);
  fseek(in_fd, 0, SEEK_SET); // screwy for pipline? 
  encode_tree(out_fd);
  encode_text(in_fd, out_fd);
}

void huffman::decode(FILE *in_fd, FILE *out_fd) {
  decode_tree(in_fd); 
  decode_text(in_fd, out_fd);
}

void huffman::print_codes() const {
  /* print tree breadth first */
  std::queue<node_t*> q; 
  q.push(root); 
  node_t *n;
  while (!q.empty()) {
    n = q.front();
    q.pop();
    if (!n->leaf) {
      fprintf(stderr, "%d ", n->weight); 
      q.push(n->left);
      q.push(n->right);
    } else 
      fprintf(stderr, "%d/%c ", n->weight, n->leaf);
  }
  fprintf(stderr, "\n"); 
  
  /* print codes */
  char code[100];
  int j;
  for (int i=0; i<256; i++) {
    if (hc[i].weight > 0) {
      fprintf(stderr, "%c ", hc[i].leaf);
      n = &hc[i];
      j = 0; 
      while(n != root) {
        code[j++] = (n->edge ? '1': '0'); 
        n = n->parent;
      }
      while(--j >= 0)
        fputc(code[j], stderr); 
      fprintf(stderr, "\n");
    }
  }
}

/**
 * private methods
 */

node_t *huffman::huff() {  
  
  /* cool part of this code. construct huffman code tree 
   * This method expects 'hc' to be populated with leaves
   * and their weights. */

  minheap<node_t> q(256); 
  node_t *p, *l, *r; 
  
  ct = 0; 
  for (int i=0; i<256; i++) {
    hc[i].leaf = (char)i;
    if (hc[i].weight > 0) {
      //cout << (char)i << " -> " << hc[i].weight << endl;
      q.push(&hc[i]);
      ct ++;
    }
  }

  while (!q.empty()) {
    l = q.pop(); 
    r = q.pop();
    if (r == NULL) { // one node_t on heap
      //root = l; 
      break;
    }
  
    p = new node_t (l->weight + r->weight, NULL, l, r);
  
    l->parent = r->parent = p; 
    l->edge = 0; 
    r->edge = 1; 
    q.push(p);
  }

  return l; 
}


void huffman::free_node(node_t *n) {

  /* delete tree nodes recursively (except leaves) */

  if (n->left)
    free_node(n->left); 
  if (n->right)
    free_node(n->right); 
  if (!n->leaf)
    delete n;
}


void huffman::build_tree(FILE *fd) {

  /* initialize 'hc' and build tree */

  if (root)   
    free_node(root);
  if (hc) 
    delete [] hc; 

  hc = new node_t [256];
  char c; 

  /* calculate weights of leaves */
  while ((c=fgetc(fd)) != EOF) 
    hc[(int)c].weight++;

  root = huff();
}


void huffman::encode_tree(FILE *out_fd) {

  /* encode the leaves and their weights, plus the 
   * number of leaves. This is enough ro reconstruct 
   * the tree when unziping */ 
 
  fprintf(out_fd, "%d;", ct); 
  for (int i=0; i<256; i++) {
    if (hc[i].weight > 0) {
      fprintf(out_fd, "%c:%d:", hc[i].leaf, hc[i].weight);
    }
  }

}


void huffman::encode_text(FILE *in_fd, FILE *out_fd) {

  /* TODO */
  char c,code[100];
  int j;
  node_t *n;
  while ((c=fgetc(in_fd)) != EOF) {
    if (hc[(int)c].weight > 0) {
      n = &hc[(int)c];
      j = 0;
      while(n != root) {
        code[j++] = (n->edge ? '1': '0'); 
        n = n->parent;
      }
      while(--j >= 0)
        fputc(code[j], out_fd); 
    }
  }

}



void huffman::decode_tree(FILE *in_fd) {
 
  /* initialize 'hc' and build tree */

  if (root)   
    free_node(root);
  if (hc) 
    delete [] hc; 

  hc = new node_t [256];

  fscanf(in_fd, "%d;", &ct);
  char leaf;
  int weight; 
  while(ct--) {
    fscanf(in_fd, "%c:%d:", &leaf, &weight);
    hc[(int)leaf].leaf = leaf; 
    hc[(int)leaf].weight = weight;
  }
 
  root = huff(); 
}


void huffman::decode_text(FILE *in_fd, FILE *out_fd) {
  /* TODO */
  
  char c;
  node_t *n;
  while(c != EOF) {
    n = root;
    while (c != EOF && !n->leaf) {
      c = fgetc(in_fd);
      if (c == '1') 
        n = n->right;
      else 
        n = n->left;
    }
    fputc(n->leaf, out_fd);
  }

}

