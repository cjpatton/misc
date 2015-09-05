# Chris
# ~17 Feb 2013
# Bellman-Ford  

class Node: 

  def __init__(self, label): 
    self.label = label
    self.neighbors = [] # (weight integer, verticy Node) pairs
    self.prev = None     
    self.dist = float("+inf")
    self.visited = False
    self.finished = False
  
  def __str__(self):
    return self.label

  def drawEdgeTo(self, node, weight): 
    if type(weight) == int and isinstance(node, Node): 
      self.neighbors.append((weight, node))
    else: raise TypeError("expected Node and integer weight") 

  def visit(self): 
    self.visited = True

  def unvisit(self): 
    self.visited = False

  def reset(self): 
    self.prev = None
    self.dist = None
    self.visited = False
    self.finished = False


## Breadth First Search ## 
def BFS(u, f=(lambda u: None)):
  queue = [u]
  while len(queue) > 0:
    u = queue.pop()
    u.visited = True
    f(u)
    for (_, v) in u.neighbors: 
      if not v.visited: 
        queue.insert(0,v)


## Depth First Search ##
def DFS(u, f=(lambda u: None)):
  u.visited = True
  f(u)
  for (_, v) in u.neighbors:
    if not v.visited:
      DFS(v, f)


## detect cycles ##
def cycles(u, clist=None):
  u.visited = True
  res = False
  for (_, v) in u.neighbors: 
    if not v.visited:
      v.prev = u
      res = cycles(v, clist) or res
    elif not v.finished:
      if clist != None:
        c = [u]
        while c[-1] != v:
          c.append(c[-1].prev)
        clist.append(c)
      res = True
  u.finished = True
  return res



## Bellman-Ford single-sourced shortest path ##
def BellmanFord(alist, s): 
  map(lambda u: u.reset(), alist)

  print "round 0 ", 
  opt = []
  for i in range(len(alist)):
    if alist[i] == s: 
      opt.append((0, s))
    else:
      opt.append((float("+inf"), None))
    (alist[i].dist, alist[i].prev) = opt[-1]
    print "%s=(%s, %s) " % ((alist[i],) + opt[i]),
  print

  for i in range(1,len(alist)): # number of hops
    for j in range(len(alist)): 
      if alist == s:
        opt[j] = (0, s)
      else:
        (minw, minv) = (alist[j].dist, alist[j].prev)
        for (w, v) in alist[j].neighbors: 
          if v.dist + w < minw:
            (minw, minv) = (v.dist + w, v)
        opt[j] = (minw, minv) 

    print "round %-2d" % i,
    for j in range(len(alist)):
      (alist[j].dist, alist[j].prev) = opt[j]
      print "%s=(%s, %s) " % ((alist[j],) + opt[j]),
    print



if __name__ == '__main__':

  adjacencyList = [Node("t"), Node("a"), Node("b"), Node("c"), Node("d"), Node("e")]
  graph = { u.label : u for u in adjacencyList }

  # Topology of Figure 6.23 in Tardos/Kleinberg (pp.294) 
  graph['a'].drawEdgeTo(graph['t'], -3)
  graph['a'].drawEdgeTo(graph['b'], -4)
  graph['b'].drawEdgeTo(graph['d'], -1)
  graph['b'].drawEdgeTo(graph['e'], -2)
  graph['c'].drawEdgeTo(graph['b'], 8)
  graph['c'].drawEdgeTo(graph['t'], 3)
  graph['d'].drawEdgeTo(graph['t'], 4)
  graph['d'].drawEdgeTo(graph['a'], 6)
  graph['e'].drawEdgeTo(graph['t'], 2)
  graph['e'].drawEdgeTo(graph['c'], -3)

  BellmanFord(adjacencyList, graph['t'])
  map(lambda u: u.reset(), adjacencyList)

  cycleList = []
  c = cycles(graph['a'], cycleList)
  for cycle in cycleList:
    print "cycle!", map(lambda u: u.__str__(), cycle)
  print c
  
  a = Node("a")
  b = Node("b")
  c = Node("c")
  d = Node("d")
  a.drawEdgeTo(b, 1)
  b.drawEdgeTo(c, 1)
  c.drawEdgeTo(d, 1)
  b.drawEdgeTo(d, 1)
  #c.drawEdgeTo(a, 1)
  
  cycleList = []
  res = cycles(a, cycleList)
  for cycle in cycleList:
    print "cycle!", map(lambda u: u.__str__(), cycle)
  print res

  


  

