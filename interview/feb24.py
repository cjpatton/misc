# Studying for interview next week. 

import random
N = 100


def findmax(A, i=None, j=None):
  if i == None: i = 0
  if j == None: j = len(A) - 1
  k = (i + j) / 2
  if A[k-1] < A[k] and A[k] > A[k+1]: # Gotcha!
    return k
  elif A[k-1] < A[k] and A[k] < A[k+1]: # Increasing
    return findmax(A, k, j)
  else: # Decreasing
    return findmax(A, i, k)

def qsort(A):
  if len(A) < 2: 
    return A
  else:
    p = A[len(A) / 2]
    P = []; L = []; R = []
    for x in A:
      if x < p: 
        L.append(x)
      elif x > p:  
        R.append(x) 
      else: 
        P.append(x)
    return qsort(L) + P + qsort(R)


def swap(A, i, j):
  tmp = A[i]
  A[i] = A[j]
  A[j] = tmp

def _qsort(A, i, j): # Qsort in-place
  if j > i + 1: 
    swap(A, i, (j + i) / 2)
    l = i + 1
    r = j 
    while l < r:
      if A[l] < A[i]:
        l += 1
      else:
        r -= 1
        swap(A, l, r)
    l -= 1
    swap(A, l, i)
    _qsort(A, i, l)
    _qsort(A, r, j)

def qsort(A):
  _qsort(A, 0, len(A))
  return A


def kth(A, k): # Assume elements of A are distinct. 
  if len(A) == 1:
    return A[0]
  p = A[random.randint(0, len(A)-1)]
  L = []; R = []
  for x in A:
    if x < p: 
      L.append(x)
    elif x > p: 
      R.append(x)
  if k == len(L) + 1: 
    return p
  elif k < len(L) + 1:
    return kth(L, k)
  else: 
    return kth(R, k - len(L) - 1)

# One maximum
k = random.randint(0, N-1)
A = list(range(N-k, N)) + list(reversed(range(k, N+1)))
print k, findmax(A)

# Kth larget element
A = list(range(N))
random.shuffle(A)
print kth(A, 27)

#print qsort(A) 

