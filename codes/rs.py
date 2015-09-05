# rs.py -- Reed-Solomon codes 
# 
# Reed-Solomon is a cyclic, q-ary BCH code defined over the field GF(q)
# parameterized by number of error symbols (t) it can correct. Its generator 
# polynomial is simply g(x) = (x - a)(x - a^2) ... (x - a^2t) where a is a 
# primitive element of the field. The block length (number of symbols, i.e. 
# length of the codeword) is q - 1, the dimension (lengh of input word) is 
# q - 1 - 2t, and the minimum Hamming distance is 2t + 1. 
#

from galois import *


### Reed-Solomon code encoding and decoding. ##################################

def Generator(t, F):
  ''' Compute generator polynomial for t-error-correcting RS code. ''' 
  g = Poly([1,0], F)
  for i in range(2, 2*t+1):
    p = Poly.zero(2, F)
    p[0] = F(i).reciprocal()
    p[1] = F(0).reciprocal()
    g = g * p
  return g


def Syndrome(r, t, F):
  ''' Compute syndrome of `r` over F for t-error-correcting RS code. '''
  S = Poly.zero(2*t, F)
  for i in range(1, 2*t+1):
    S[i-1] = r(F(i))
  return S


def Decode(r, t, S, F, ErrorPoly, verbose=False): 
  ''' Decode received polynomial r(x).
    
    Return the error polynomial e(x). If the number of errors is at most t, 
    then v(x) = r(x) - e(x) is the transmitted codeword. 
  ''' 

  (sig, Z0, roots) = ErrorPoly(t, S, F, verbose)

  # Error locations. 
  locations = map(lambda(elem) : elem.inverse().i, roots)

  if verbose:
    print "Roots of sig:", roots
    print "Error locations:", locations
  
  sig1 = sig.deriv()

  if verbose: 
    print "Z0   =", Z0
    print "sig' =", sig1
  
  # Error values, error polynomial.
  e = Poly.zero(len(r), F)
  for (i, elem) in zip(locations, roots):
    e[i] = Z0(elem).reciprocal() / sig1(elem)
  
  return e



### Find error location/value polynomials. ####################################

def Euclid(t, S, F, verbose=False): 
  ''' Find sig(x), Z0(x), and the roots of sig(x) in F. 
  
    An algorithm for decoding an RS or BCH code is equivalent to a solution to 
    the congruence `sig(x)S(x) \equiv Z0(x) mod X^2t`. This is easily solved by 
    Euclid's algorithm. Let a(x) = x^2t and b(x) = S(x). Suppose the numer of 
    errors is at most t. If at step i, deg Ri < deg Gi <= t, then 
      
      Ri(X) = Z0(x) and Gi(x) = sig(x). 
  ''' 

  # Initialization
  zero = F(None)
  R0 = Poly.zero(2*t+1, F); R0[2*t] = F(0)
  R1 = S

  F0 = Poly([0], F); F1 = Poly([None], F)
  G0 = Poly([None], F); G1 = Poly([0], F)

  _fmt = "%-40s %-30s %-30s %-30s"
  fmt = "%2d " + _fmt
  if verbose: # Show initialization
    print " i " + _fmt % ("Z0", "Q", "f", "sig")
    print fmt % (-1, R0, '--', F0, G0)
    print fmt % (0,  R1, '--', F1, G1)
 
  i = 1
  while not (R1.deg() < G1.deg() and G1.deg() <= t):
    (Q, tmp) = R0 / R1;  R0 = R1; R1 = tmp
    tmp = F0 - (Q * F1); F0 = F1; F1 = tmp
    tmp = G0 - (Q * G1); G0 = G1; G1 = tmp
    
    if verbose: 
      print fmt % (i,  R1, Q, F1, G1)
    i += 1
  
  # Finalization
  roots = []
  for i in range(F.order() - 1):
    elem = F(i)
    if G1(elem) == zero: 
      roots.append(elem)
  
  return (G1, R1, roots) # sig, Z0, roots of sig


def Berklekamp(t, S, F, verbose=False): 
  ''' Find sig(x), Z0(x), and the roots of sig(x) in F. 

    The Berklekamp-Massey algorithm was discvored before the application of the
    Euclidean algorithm. sig(x) is found by solving Newton's Identity equations 
    relating the syndrome to the error location polynomial coefficients. Z0(x) 
    is then easily computed from the syndrom and sig(x). 

        Berklekamp runs in a constant (2t) number of rounds, where Euclid's 
    runs in at most 2t rounds. (However, reduced-round versions of Berklekamp
    have been discovered.) Its main advantage is that it avoids polynomial 
    division.
  '''
 
  # Initialization
  sig = { -1 : Poly([0], F),  0 : Poly([0], F) }
  d =   { -1 : F(0) }
  zero = F(None)

  # Iterate
  for mu in range(2 * t):
    d[mu] = S[mu]
    for i in range(1,sig[mu].deg()+1):
      d[mu] = d[mu] + (sig[mu][i] * S[mu - i])
    if d[mu] == zero:
      sig[mu+1] = sig[mu]
    else: 
      for rho in reversed(range(-1,mu)):
        if sig[rho].deg() < sig[rho+1].deg(): break
      p = Poly.zero(mu - rho + 1, F)
      p[mu - rho] = d[mu] * d[rho].inverse()
      sig[mu+1] = sig[mu] - (p * sig[rho])

  if verbose:
    d[mu + 1] = '--'
    print "%5s  %-30s  %-10s" % ("round", "sigma", "discrepency")
    for mu in range(-1,2*t+1):
      print "%5d  %-30s  %-10s" % (mu, sig[mu], d[mu])
 
  # Error locator polynomial
  sig = sig[2*t]

  # Finalization
  roots = []
  for i in range(F.order()):
    elem = F(i)
    if sig(elem) == zero: 
      roots.append(elem)

  # Error evaluator polynomial
  v = len(roots)
  Z0 = Poly.zero(v, F)
  for i in range(v): # S[i]
    Z0[i] = S[i]
    for j in range(1,i+1): # sig[j]
      Z0[i] += S[i - j] * sig[j]

  return (sig, Z0, roots) # roots of sig





### Some fields. ##############################################################

# GF(2^5)
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

GF32 = Extend(GF, 5, _gf32.split('\n'))

_gf16 = \
'''1000
0100
0010
0001
1100
0110
0011
1101
1010
0101
1110
0111
1111
1011
1001'''

GF16 = Extend(GF, 4, _gf16.split('\n'))




### Testing, testing ... ######################################################

print GF16

#print "t=3,", Generator(3,GF16)

# Example 7.2 in Lin-Costello
#r = Poly.zero(15, GF16)
#r[3] = GF16(7)
#r[6] = GF16(3)
#r[12] = GF16(4)

# Example 7.5 in Lin-Costello
#r[3] = GF16(7)
#r[10] = GF16(11)

print "Problem 7.5"
r = Poly.zero(15, GF16)
r[3] = GF16(4)
r[8] = GF16(9)
r[13] = GF16(3)

S = Syndrome(r, 3, GF16)
e = Decode(r, 3, S, GF16, Euclid, verbose=True)

print 
print "r(x)  =", r
print "e(x)  =", e
print "v*(x) =", r - e


print "Problem 7.6"
r = Poly.zero(31, GF32)
r[0] = GF32(2)
r[12] = GF32(21)
r[20] = GF32(7)

S = Syndrome(r, 3, GF32)
e = Decode(r, 3, S, GF32, Euclid, verbose=True)

print 
print "r(x)  =", r
print "e(x)  =", e
print "v*(x) =", r - e
