import random
# How do you find duplicate numbers in an array if it contains multiple duplicates?
# How are duplicates removed from a given array in Java?
# How is an integer array sorted in place using the quicksort algorithm?
# How do you remove duplicates from an array in place?
# How do you reverse an array in place in Java?
# How are duplicates removed from an array without using any library?

def gen(total, low, high):
  return [ random.randint(low, high) for _ in range(total) ]

def shuffled(low, high):
  out = list(range(low, high+1))
  random.shuffle(out) # TODO How might I implement this?
  return out

# P1: How do you find the missing number in a given integer array of 1 to 100?
#
# Don't forget to ask about assumptions, e.g., are the numbers contiguous? Can I
# mutate the data? Is it ordered?
def p1(low, high, s):
  array = shuffled(low, high)
  return array[0] == s(array[1:], low, high)

def p1s1(leftover, low, high):
  total = (high - low + 1) * (high - low + 2) / 2
  leftover_total = 0
  for x in leftover:
    leftover_total += x
  return total - leftover_total

def p1s0(leftover, low, high):
  rep = [ False ] * (len(leftover)+1)
  for x in leftover:
    rep[x-low] = True
  for i in range(len(rep)):
    if not rep[i]: return low+i

# P2: How do you find the duplicate number on a given integer array?
#
# Assume we're not guaranteed a contiguous set of integers.
#
# TODO Can I do better
def p2(low, high, s):
  array = shuffled(low, high)
  i = random.randint(0, len(array)-1)
  array[random.randint(0, len(array)-1)] = array[i]
  return array[i] == s(array, low, high)

def p2s0(array, low, high):
  rep = {}
  for (i, x) in enumerate(array):
    if rep.get(x) != None:
      return x
    rep[x] = True

# P3: How do you find the largest and smallest number in an unsorted integer
# array?
#
# What's the lowest and highest possible numbers? Say 0 and 2**32.
def p3(s):
  low = random.randint(1,100)
  high = random.randint(1,100)
  low, high = min(low, high), max(low, high)
  array = shuffled(low, high)
  return (low, high) == s(array)

def p3s0(array):
  low = 2**32
  high = 0
  for x in array:
    if x < low:
      low = x
    if x > high:
      high = x
  return (low, high)

# P4: How do you find all pairs of an integer array whose sum is equal to a
# given number?
#
# What are the range of numbers? Is the target above or below the highest
# possible value?
#
# TODO Can I solve this in less than O(n^2) time?
def p4(total, s):
  target = 150
  array = gen(total, 0, 100)
  num_solutions = 0
  for i in range(len(array)):
    for j in range(i+1,len(array)):
      if array[i] + array[j] == target:
        num_solutions += 1
  return num_solutions == s(array, 0, 100, target)

def p4s0(array, low, high, target):
  return 0

if __name__ == '__main__':
  print 'p1', 'pass' if p1(1, 1000, p1s1) else 'fail'
  print 'p2', 'pass' if p2(1, 1000, p2s0) else 'fail'
  print 'p3', 'pass' if p3(p3s0) else 'fail'
  print 'p4', 'pass' if p4(100, p4s0) else 'fail'
