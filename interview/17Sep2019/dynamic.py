# Longest common substring
def longest_common_substr(X, Y):
  L = [ [ 0 for _ in range(len(Y)) ] for _ in range(len(X)) ]
  z = (0, 0, 0)
  for i in range(len(X)):
    for j in range(len(Y)):
      if X[i] == Y[j]:
        if i == 1 or j == 1:
          L[i][j] = 1
        else:
          L[i][j] = L[i-1][j-1] + 1
        if L[i][j] > L[z[0]][z[1]]:
          z = (i, j, L[i][j])

  for row in L:
    for val in row:
      if val > 0:
        print '%-2d' % val,
      else:
        print '  ',
    print

  (_, j, l) = z
  return Y[j-l+1:j+1]

# Fibonacci
def fib(n):
  M = [ None for _ in range(n+1) ]
  def fib0(M, n):
    if n == 0 or n == 1: return n
    if M[n] != None:
      return M[n]
    M[n] = fib0(M, n-1) + fib0(M, n-2)
    return M[n]
  return fib0(M, n)

X = 'ABCDGHskjdfshakespeare'
Y = 'ACDGHRlkdjf kj  shakespxx'
print longest_common_substr(X, Y)

def moves(n):
  M = [None] * (n+1)
  S = [(1, 0)]
  while len(S) > 0:
    (x, d) = S.pop()
    if x <= n:
      if M[x] == None:
        M[x] = d
        S.append((x+1, d+1))
        S.append((x*2, d+1))
      elif d < M[x]:
        M[x] = d
  return M[n]

for x in [7, 8, 113, 123, 234]:
  print moves(x)
