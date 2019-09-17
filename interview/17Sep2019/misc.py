import random

def genip(S, I):
  if len(I) == 4:
    yield '.'.join(map(str, I))
  elif len(I) == 3 and len(S) > 0:
    x = int(S)
    if x < 255:
      yield '.'.join(map(str, I + [x]))
  else:
    for i in range(1,4):
      if len(S) >= i:
        x = int(S[:i])
        if x < 255:
          for ip in genip(S[i:], I + [x]):
            yield ip

for ip in genip('11211', []):
  print ip

class LRU:
  def __init__(self, cap):
    assert cap > 0
    self.cap = cap
    self.cache = {}
    self.order = {}
    self.queue = []

  def get(self, key):
    val = self.cache.get(key)
    if val != None:
      del self.queue[self.order[key]]
      self.queue.append(key)
      self.order[key] = len(self.queue) - 1
    return val

  def set(self, key, val):
    if self.cache.get(key) != None: # Update key
      self.cache[key] = val
    else: # Insert new key
      if len(self.queue) == self.cap: # Evict LRU element
        evicted = self.queue[0]
        del self.cache[evicted]
        del self.order[evicted]
        self.queue = self.queue[1:]
      self.queue.append(key)
      self.order[key] = len(self.queue) - 1
      self.cache[key] = val

lru = LRU(3)
lru.set(1, 'hello')
lru.set(2, 'go')
lru.set(3, 'gators')
print 'pass' if lru.get(1) == 'hello' else 'fail'
lru.set(4, 'chomp')
print 'pass' if lru.get(2) == None else 'fail'


# Given some number of tasks with possibly overlapping time frames, figure out
# how many activities can be performed by a single person.
def schedule(events):
  sched = []
  last = 0
  for (start, end) in sorted(events, key=lambda pair: pair[1]):
    if start >= last:
      sched.append((start, end))
      last = end
  return len(sched)

starts = [1, 3, 2, 5, 8, 5]
ends =   [2, 4, 6, 7, 9, 9]
print schedule(zip(starts, ends))

starts = [1, 3, 2, 5]
ends =   [2, 4, 3, 6]
print schedule(zip(starts, ends))


# Find the kth largest element across two sorted arrays.
X = sorted([ random.randint(0,100) for _ in range(100) ])
Y = sorted([ random.randint(0,100) for _ in range(100) ])
k = 37

def kth(X, Y, k):
  assert len(X) + len(Y) >= k
  i = 0
  j = 0
  last = 0
  while i + j < k - 1:
    if X[i] < Y[j]:
      last = X[i]
      if i < len(X) - 1:
        i += 1
    else:
      last = Y[j]
      if j < len(Y) - 1:
        j += 1
  return last

print 'pass' if kth(X, Y, k) == sorted(X + Y)[k-1] else 'fail'
