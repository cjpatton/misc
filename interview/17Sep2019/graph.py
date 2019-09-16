import random

def new(v):
  G = []
  for _ in range(v):
    G.append([None] * v)
  return G

def gen(v, e, w_low, w_high):
  G = new(v)
  for _ in range(e):
    x = random.randint(0,v-1)
    y = random.randint(0,v-1)
    w = random.randint(w_low, w_high)
    G[x][y] = w
    G[y][x] = w
  return G

def print_graph(G):
  n = len(G)
  print '  ',
  for v in range(n):
    print '%-2d' % v,
  print
  for u in range(n):
    print '%-2d' % u,
    for v in range(n):
      if G[u][v] != None:
        print '%-2d' % G[u][v],
      else:
        print '  ',
    print

def cc(G):
  n = len(G)
  comp = [None] * n
  for u in range(n):
    S = [u]
    while len(S) > 0:
      v = S.pop()
      if comp[v] == None: # not visited
        comp[v] = u # visit
        for w in range(n): # add neighbors to stack
          if G[v][w] != None:
            S.append(w)
  C = new(n)
  for u in range(n):
    for v in range(n):
      if G[u][v] != None:
        C[u][v] = comp[u]
  return C


G = gen(10, 4, 1, 1)
print_graph(G)
print_graph(cc(G))
