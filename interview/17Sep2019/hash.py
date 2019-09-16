import hashlib
import hmac
import os
import pickle
import struct

def H(K, obj):
  return hmac.new(K, pickle.dumps(obj), hashlib.sha256).digest()

def I(h, size):
  # Figure how many bits are needed to encode the size.
  N = len(h) << 3
  B = 0
  for i in range(N):
    B += (size >> i) > 0

  # Try to find a B-bit chunk of h that is less than n.
  for i in range(0,N,B):
    if i + B > N:
      break # This is very unlikely!
    n = 0
    for j in range(B):
      n |= struct.unpack('<B', h[(i+j) >> 3])[0] & (1 << j)
    if n < size:
      return n

  # In the unlikely event that we did not succeed to find a suitable return
  # value, fail hard.
  return None

class Table:
  load_max = 0.7
  def __init__(self, size):
    self.table = [None] * size
    self.K = os.urandom(16)
    self.ct = 0

  def set(self, key, val):
    h = H(self.K, key)
    n = len(self.table)
    idx = I(h, n)
    while self.table[idx] != None and self.table[idx][0] != h:
      idx = (idx + 1) % n
    if self.table[idx] != None: # overwrite existing value
      self.table[idx] = (h, val)
    else: # new key/value pair, so increment the counter and (possibly) resize.
      if float(self.ct) / n > self.load_max:
        self.resize(int(n * 1.3))
        self.set(key, val) # Increments the counter.
      else:
        self.table[idx] = (h, val)
        self.ct += 1

  def resize(self, new_size):
    new_table = [None] * new_size
    n = new_size
    for row in self.table:
      if row != None:
        (h, val) = row
        idx = I(h, n)
        while new_table[idx] != None:
          idx = (idx + 1) % n
        new_table[idx] = (h, val)
    self.table = new_table

  def get(self, key):
    h = H(self.K, key)
    n = len(self.table)
    idx = I(h, n)
    while self.table[idx] != None and self.table[idx][0] != h:
      idx = (idx + 1) % n
    if self.table[idx] != None:
      return self.table[idx][1]
    return None

  def remove(self, key):
    h = H(self.K, key)
    n = len(self.table)
    idx = I(h, n)
    while self.table[idx] != None and self.table[idx][0] != h:
      idx = (idx + 1) % n
    if self.table[idx] != None:
      self.table[idx] = (h, None)
      self.ct -= 1


T = Table(100)
T.set('hella', 7)
print 'pass' if T.get('hella') == 7 else 'fail'
T.set('hella', 4)
print 'pass' if T.get('hella') == 4 else 'fail'
T.remove('hella')
print 'pass' if T.get('hella') == None else 'fail'
T.set('hella', 5)
print 'pass' if T.get('hella') == 5 else 'fail'

for i in range(1000):
  T.set(i, i)
ok = True
for i in range(1000):
  ok = ok and T.get(i) != None
print 'pass' if ok else 'fail'
