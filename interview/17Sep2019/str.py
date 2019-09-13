import random
# How do you check if two strings are a rotation of each other?
# How do you check if a given string is a palindrome?

alphabet = 'abcdefghijklmnopqrstuvwxyz'

def gen(alphabet, length):
  out = ''
  for _ in range(length):
    out += alphabet[random.randint(0,len(alphabet)-1)]
  return out

# P1 How do you print duplicate characters from a string?
def print_dups(x):
  rep = {}
  for c in x:
    printed = rep.get(c)
    if printed == None:
      rep[c] = False
    elif printed == False:
      print c,
      rep[c] = True
  print

print_dups(gen(alphabet, 100))

# P2 How do you check if two strings are anagrams of each other?
def are_anagrams(x, y):
  if len(x) != len(y):
    return False
  for i in range(len(x)):
    if x[i] != y[-(i+1)]:
      return False
  return True

print are_anagrams('he', 'llo')
print are_anagrams('x', 'y')
print are_anagrams('racxcar', 'racecar')
print are_anagrams('x', 'x')
print are_anagrams('racecar', 'racecar')

# P3 How do you find all permutations of a string?
#
# Here's Heap's algorithm.
#
# TODO Wrap my head around this.
def generate(k, A):
  if k == 1:
    yield A
  else:
    for Y in generate(k-1, A):
      yield Y
    for i in range(k-1):
      if k % 2 == 0: # even
        j = i
      else:
        j = 0
      tmp = A[j]
      A[j] = A[k-1]
      A[k-1] = tmp
      for Y in generate(k-1, A):
        yield Y

x = '12'
for Y in generate(len(x), list(x)):
  print ''.join(Y)


# P4 How do you print the first non-repeated character from a string?
def first_unique(x):
  # Index of first occurrence of each character (first[c] == len(x) if c is a
  # repeat).
  first = {}

  for (i, c) in enumerate(x):
    f = first.get(c)
    if f == None: # Haven't seen c
      first[c] = i
    else: # Have seen c
      first[c] = len(x)

  m = len(x)
  for i in first.values():
    if i < m:
      m = i
  return x[m]

print first_unique('xxxxyaxxaby')

# P5 How can a given string be reversed using recursion?
def rev(X, i, j):
  if i < j:
    tmp = X[i]
    X[i] = X[j]
    X[j] = tmp
    rev(X, i+1, j-1)

X = list('1234567')
rev(X, 0, len(X)-1)
print ''.join(X)

# P6 How do you reverse words in a given sentence without using any library
# method?
#
# Punctuation?
def rev_words(x):
  if len(x) == 0:
    return x

  tokens = []
  tok_start = 0
  is_word = x[0] in alphabet # True if the first token is a word.
  for (i, c) in enumerate(x):
    if (c in alphabet) != is_word: # new token
      tokens.append(x[tok_start:i])
      tok_start = i
      is_word = c in alphabet
  tokens.append(x[tok_start:])
  y = ''
  for tok in reversed(tokens):
    y += tok
  return y

print rev_words(' this is applebees and it is the best, please?')
