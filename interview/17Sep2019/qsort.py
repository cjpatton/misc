import random
import sys

def qsort0(A):
  if len(A) < 3:
    return A
  L = []
  P = []
  R = []
  pivot = A[len(A)>>1]
  for x in A:
    if x < pivot:
      L.append(x)
    elif x == pivot:
      P.append(x)
    else:
      R.append(x)
  return qsort0(L) + P + qsort0(R)

def swap(A, i, j):
  if i != j:
    A[i] ^= A[j]
    A[j] ^= A[i]
    A[i] ^= A[j]

def qsort1(A, l, r):
  if r > l + 1:
    swap(A, l, (r+l) / 2)
    i = l + 1
    j = r
    while i < j:
      if A[i] < A[l]:
        i += 1
      else:
        j -= 1
        swap(A, i, j)
    i -= 1
    swap(A, i, l)
    qsort1(A, l, i)
    qsort1(A, j, r)


data = [ random.randint(1,100) for _ in range(10) ]
qsort1(data, 0, len(data))
print data
