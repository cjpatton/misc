/* RNA secondary structure
 * dynamic programming algorithm
 * 222a - Martel/Gusfield
 *
 * Chris Patton
 * ~13 Oct 2012
 *
 * Dynamic programming algorithm for predicting the secondary structure of 
 * RNA. Compute the OPT matrix. The recurrence is as follows: 
 *  OPT(i,j) = max { 
 *    OPT(i,j-1), 
 *    max { 1 + OPT(i,t-1) + OPT(t+1,j-1) }
 *  }
 * for i < j and valid pairing (t,j). The algorithm takes cubic time. Return 
 * the number of basepairs in the optimal solution. This solution is from 
 * Kleinberg and Tardos, Ch 6.5.
 */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define LOOP 5 
#define MAX  256
#define NUCLEOTIDE(x) (x == 'A' || x == 'U' || x == 'G' || x == 'C')
#define ALPHABETIC(x) (('A' <= x && x <= 'Z') || x == '?')

/* cell_t - a cell of the OPT matrix */
typedef struct {
  int score;
  int a, b;
} cell_t; 
  
const cell_t EMPTY_CELL_T = {0, -1, -1};  

/* rna_t - the OPT matrix */
typedef struct {
  cell_t opt [MAX][MAX];
  int n; 
} rna_t; 

void init_rna_t ( rna_t *mol ) 
/* Initialize a struct rna_t */ 
{
  int i,j; 
  for (i = 0; i < MAX; i++)
    for (j = 0; j < MAX; j++)
      mol->opt[i][j] = EMPTY_CELL_T;
  mol->n = 0; 
}


int match( const char *rna, const rna_t *mol, int t, int j ) 
/* Determine whether (t,j) is a valid base pair. */
{
  char a = rna[t], b = rna[j]; 
  const cell_t *right = &mol->opt[t+1][j-1]; 

  /* no sharp-turn condition */
  int r = right->score == 0 || 
    (right->a - t == 1 && j - right->b == 1) ||
    (right->a - t > LOOP && j - right->b > LOOP);

  /* pair condition */
  if (NUCLEOTIDE(a) && NUCLEOTIDE(b))
    return r && (
     (a == 'A' && b == 'U') ||
     (a == 'U' && b == 'A') ||
     (a == 'G' && b == 'C') ||
     (a == 'C' && b == 'G')); 

  /* If not nucleotide, but alphabetic, match. Otherwise, don't match. */
  else
    return r && ALPHABETIC(a) && ALPHABETIC(b);
}

int predict( const char *rna, rna_t *mol )
/* Compute the OPT matrix. 
 * Treat alphabetic characters that are not A, U, G, or C as wildcards, 
 * but not non-alphabetic characters. */  
{
  int i, j, k, t; 
  cell_t max, *left, *right; 

  init_rna_t (mol); 
  mol->n = strlen(rna); 
  for (k = LOOP; k < mol->n; k++)   
  {
    for (i = 0; i <= mol->n - k; i++) 
    /* for all k-length intervals (0, k), (1, k+1), ... (n-k, n) */
    {
      j = i + k;
      max = mol->opt[i][j-1]; 
      
      for (t =  i; j - t > LOOP; t++) 
      /* for all valid base pairs (t,j), pick the base pair that maximizes 
       * the scores for the subproblems OPT(i,t-1) and OPT(t+1,j-1) */
      {
        left  = &mol->opt[i][t-1]; 
        right = &mol->opt[t+1][j-1]; 
  
        if (match (rna, mol, t, j) && left->score + right->score + 1 > max.score)
        {  
          max.score = left->score + right->score + 1;
          max.a = t;    // save the pair so we can recover the structure 
          max.b = j;    // later. 
        }
      }

      mol->opt[i][j] = max; 

    }
  }

  return mol->opt[0][mol->n-1].score;
}

void print_opt (const rna_t *mol) 
/* Print the computed OPT matrix */  
{
  int i,j;
  for (j = LOOP; j < mol->n; j++) printf("%3d", j); 
  for (i = 0; i < mol->n - LOOP; i++)
  {
    printf("\n%-3d", i); 
    for (j = LOOP; j < mol->n; j++)
    {
      printf("%3d", mol->opt[i][j].score); 
    }
  }   
  printf("\n");
}

void print_pairs (const rna_t *mol, int i, int j)
/* Recover the the optimal secondary structure from the computed OPT matrix. 
 * Print the base pairs of the molecule. */
{
  int a = mol->opt[i][j].a,
      b = mol->opt[i][j].b; 

  if (a >= 0 && j >= 0) 
  {
    printf("(%d, %d)\n", a,b);
    print_pairs (mol, i, a-1); 
    print_pairs (mol, a+1, b-1);
  }
}

void make_structure (const rna_t *mol, char *structure, int i, int j)
/* Called by print_structure. Reconstruct the optimal structure 
 * and create a friendly string. */
{
  int a = mol->opt[i][j].a,
      b = mol->opt[i][j].b; 

  if (a >= 0 && j >= 0) 
  {
    structure[a] = '(';
    structure[b] = ')'; 
    make_structure (mol, structure, i, a-1); 
    make_structure (mol, structure, a+1, b-1);
  }

}

void print_structure (const rna_t *mol)
/* Print a string representation of the structure. */
{
  char structure [MAX]; 
  memset(structure, '.', sizeof(char) * MAX); 
  make_structure (mol, structure, 0, mol->n-1);
  if (mol->n == MAX)
    structure[mol->n-1] = '\0';
  else 
    structure[mol->n] = '\0'; 
  printf("%s", structure); 
} 


int main(int argc, const char **argv)
{
  
  char c, rna [MAX]; 
  int i, j=0, score;
  rna_t *molecule = (rna_t *)malloc(sizeof(rna_t)); 

  /* Get RNA strand from stdin. Finish input on EOF */ 
  for (i = 0; (c=getchar()) != EOF && i < MAX; i++) 
  {
    switch (c) 
    {
      case ' ': 
      case '\t':
      case '\n': break;
      case 'A':
      case 'G':
      case 'C':
      case 'U':
      default: rna[j++] = c; 
    }
  }
  rna[j] = '\0';

  printf("%s\n", rna); 

  /* Compute OPT */ 
  score = predict( rna, molecule ); 
 
  //printf("score: %d\n   ", score); 
  //print_opt (molecule);
  print_structure (molecule); 
  printf("  score: %d\n", score); 
  print_pairs (molecule, 0, strlen(rna)-1); 

  free (molecule); 
  return 0;
}
