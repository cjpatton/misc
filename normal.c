/* Compile with gcc -Wall normal.c -lm h
 * Do ./a.out > normal && R, then at the
 * prompt, type hist(scan("normal")) */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

const double pi = (double)22/7;

/* Box-Muller method for approximating a
 * normal distribution. */

double unif() {
  /* uniform random number in (0,1] */
  return ((double)(rand() % 100000) / 100000);
}

void normal(double *X, double *Y, double mean, double sd) {
  /* X and Y are independent random variables drawn from a normal
   * distribution. Default is X,Y~N(1,0), or standard normal Z. */
  double U = unif(), V = unif();
  U = sqrt(-2 * log(U));
  *X = U * cos(2 * pi * V);
  *Y = U * sin(2 * pi * V);
  *X = (*X * sd) + mean;
  *Y = (*Y * sd) + mean;
}


int main() {
  srand(time(NULL));
  double X, Y;

  int i;
  for (i = 0; i < 100; i ++) {
    normal(&X, &Y, 500, 200);
    printf("%d\n%d\n", (int)X, (int)Y);
  }

  return 0;
}
