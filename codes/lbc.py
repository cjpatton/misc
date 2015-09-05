# lbc.py -- Exploring linear block codes. 
# EEC269A, Fall 2014 - UC Davis
# Written by Chris Patton 

import numpy as np

### Class for Galois Field GF(2) elements. ####################################

class GF: 
  
  def __init__(self, val): 
    self.val = val

  def __add__(self, elem):
    return GF(self.val ^ elem.val)
  
  def __mul__(self, elem):
    return GF(self.val & elem.val)
  
  def __rmul__(self, elem):
    return GF(self.val & elem.val)

  def __eq__(self, elem):
    if isinstance(elem, GF): 
      return elem.val == self.val
    else: raise TypeError("can't compare GF(2) element with type `%s`" % type(elem))

  def __ne__(self, elem):
    if isinstance(elem, GF): 
      return elem.val != self.val
    else: raise TypeError("can't compare GF(2) element with type `%s`" % type(elem))

  def __repr__(self):
    return '1' if self.val == 1 else '0'


### Convert from between GF() and int() representation. #######################

def to_bin(X): 
  return map(lambda (row) : map(
           lambda (x) : GF(1) if x == 1 else GF(0), row), X)

def to_int(X): 
  return map(lambda (row) : map(
           lambda (x) : 1 if x == GF(1) else 0, row), X)


### Convert vector over GF(2) to string. ######################################

def as_string(v): 
  s = ''
  for i in range(len(v)): 
    s += '1' if v[i] == GF(1) else '0'
  return s


### Generate message texts in int() representation. ###########################

def u_gen(k): 
  for i in range(2 ** k):
    _u = []; x = 1
    for j in range(k): 
      _u.append((i & x) >> j)
      x *= 2
    yield _u




### G -- the k-by-n generator matrix. #########################################

_g = [[0,1,1,1, 1,0,0,0], 
      [1,1,1,0, 0,1,0,0],
      [1,1,0,1, 0,0,1,0], 
      [1,0,1,1, 0,0,0,1]]

G = np.array(to_bin(_g))

### H -- the (n-k)-by-n parity-check matrix. ##################################

_h = [[1,0,0,0, 0,1,1,1], 
      [0,1,0,0, 1,1,1,0],
      [0,0,1,0, 1,1,0,1], 
      [0,0,0,1, 1,0,1,1]]

H = np.array(to_bin(_h))


### Error patterns (coset leaders). ###########################################

_e = [[0,0,0,0,0,0,0,0], 
      [1,0,0,0,0,0,0,0], 
      [0,1,0,0,0,0,0,0], 
      [0,0,1,0,0,0,0,0], 
      [0,0,0,1,0,0,0,0], 
      [0,0,0,0,1,0,0,0], 
      [0,0,0,0,0,1,0,0], 
      [0,0,0,0,0,0,1,0], 
      [0,0,0,0,0,0,0,1], 
      [1,1,0,0,0,0,0,0],
      [1,0,1,0,0,0,0,0],
      [1,0,0,1,0,0,0,0],
      [1,0,0,0,1,0,0,0],
      [1,0,0,0,0,1,0,0],
      [1,0,0,0,0,0,1,0],
      [1,0,0,0,0,0,0,1]]



print "msg  codeword"
for u in to_bin(u_gen(4)):
  u = np.array(u)
  print as_string(u), as_string(np.dot(u, G))
print

print "syn. error"
for e in to_bin(_e): 
  e = np.array(e)
  s = np.dot(e, H.transpose())
  print as_string(s), as_string(e)
