# Deprecated. 

from galois import *

_gf32 = \
'''10000
01000
00100
00010
00001
10100
01010
00101
10110
01011
10001
11100
01110
00111
10111
11111
11011
11001
11000
01100
00110
00011
10101
11110
01111
10011
11101
11010
01101
10010
01001'''

#GF32 = []
#for elem in _gf32.split('\n'):
#  elem = map(lambda(b) : int(b), elem)
#  GF32.append(Poly(elem, GF))
#
#print len(GF32)
#C = lambda(i) : i % 31
#Z = Poly.zero(5, GF)
#for (i, elem) in enumerate(GF32):
#  sig = GF32[0] + GF32[C(i+19)] + GF32[C((2*i) + 6)] # r1
#  #sig = GF32[0] + GF32[C(i+2)] + GF32[C((2*i) + 28)] # r2:Q
#  print "%-2d sig(%s) = %s %s" % (i, elem, sig, '*' if sig == Z else '') 




phi1 = Poly([1,0,1,0,0,1], GF)
phi2 = Poly([1,0,1,1,1,1], GF)
r1 = Poly.zero(32, GF); r1[30] = GF(1); r1[7] = GF(1)
r2 = Poly.zero(32, GF); r2[0] = r2[17] = r2[28] = GF(1)
(q, r) = r1 / phi1
print r
(q, r) = r1 / phi2
print r

#
#
#import numpy as np
#from poly import *
#
#class GF4(object): #GF(2^2)
#
#  add = [[0,1,2,3],
#         [1,0,3,2],
#         [2,3,0,1],
#         [3,2,1,0]]
#
#  mult = [[0,0,0,0],
#          [0,1,2,3],
#          [0,2,3,1],
#          [0,3,1,2]]
#
#  def __init__(self, val):
#    self.val = val & 3
#  
#  def __add__(self, elem):
#    return GF4(self.add[self.val & 3][elem.val & 3])
#  
#  def __mul__(self, elem):
#    return GF4(self.mult[self.val & 3][elem.val & 3])
#  
#  def __eq__(self, elem):
#    return (self.val & 3) == (elem.val & 3)
#
#  def __ne__(self, elem):
#    return (self.val & 3) != (elem.val & 3)
#
#  def __repr__(self):
#    return str(self.val & 3)
#  
#  @classmethod
#  def order(cls):
#    return 4
#
#
#_gf16 = \
#'''10
#01
#21
#23
#11
#20
#02
#32
#31
#22
#30
#03
#13
#12
#33'''
#
#
#GF16 = Extend(GF4, 2, _gf16)
#print GF16.order()
#
## Syndrome
#S = []
#for i in range(1,5):
#  r = GF16.scalarmult(GF4(3), i * 9) 
#  y = GF16.scalarmult(GF4(2), i * 8)
#  r = GF16.add(r, y)
#  r = GF16.add(r, i * 3)
#  r = GF16.add(r, i)
#  S.append(r)
#print S
#
## Error location polynomial
#one = Poly([1], GF4)
#sig = { -1 : one,       0 : one }
#dis = {   -1 : GF16[0], 0 : S[0] }  
#for mu in range(4):
#  d = None
#  j = 0
#  for i in reversed(range(mu + 1)):
#    d = GF16.add(d, S[i])
#    d = GF16.add(d, sig[mu].v[j])
#    j += 1
#  dis[mu] = d
#  if d == None:
#    sig[mu + 1] = sig[mu]
#  else: # Correction term
#    sig[mu + 1] = sig[mu]
#     
#     
#
#    
#
#
#
#
##phi1 = Poly([1,0,1,0,0,1], GF)
##phi2 = Poly([1,0,1,1,1,1], GF)
##r1 = Poly.zero(32, GF); r1[30] = GF(1); r1[7] = GF(1)
##r2 = Poly.zero(32, GF); r2[0] = r2[17] = r2[28] = GF(1)
##(q, r) = r2 / phi1
##print r
##(q, r) = r2 / phi2
##print r
#
#
#
