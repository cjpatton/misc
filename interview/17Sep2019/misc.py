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

  def _use(self, key):
    del self.queue[self.order[key]]
    self.queue.append(key)
    self.order[key] = len(self.queue) - 1

  def get(self, key):
    val = self.cache.get(key)
    if val != None:
      self._use(key)
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
