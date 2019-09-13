import random
# How do you print duplicate characters from a string?
# How do you check if two strings are anagrams of each other?
# How do you print the first non-repeated character from a string?
# How can a given string be reversed using recursion?
# How do you check if a string contains only digits?
# How are duplicate characters found in a string?
# How do you count a number of vowels and consonants in a given string?
# How do you count the occurrence of a given character in a string?
# How do you find all permutations of a string?
# How do you reverse words in a given sentence without using any library method?
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
