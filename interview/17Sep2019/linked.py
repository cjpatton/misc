import random
# How do you find the middle element of a singly linked list in one pass?
# How do you check if a given linked list contains a cycle? How do you find the starting node of the cycle?
# How do you reverse a linked list?
# How do you reverse a singly linked list without recursion?
# How are duplicate nodes removed in an unsorted linked list?
# How do you find the length of a singly linked list?
# How do you find the third node from the end in a singly linked list?
# How do you find the sum of two linked lists using Stack?

class Node:
  def __init__(self, val):
    self.val = val
    self.parent = None
    self.child = None

def make_linked(array):
  first = None
  last = None
  for x in array:
    curr = Node(x)
    curr.last = last
    if last != None:
      last.child = curr
    if first == None:
      first = curr
    last = curr
  return first

def print_linked(linked):
  curr = linked
  while curr != None:
    print curr.val
    curr = curr.child

# P1 How do you find the middle element of a singly linked list in one pass?
#
# This is easy if I can store O(n) data or do recursion. What data are stored?
# Is it arbitrary?
def p1(s):
  length = random.randint(0,100)
  return length / 2 == s(make_linked([0 for i in range(length)]))

def p1s1(linked):
  return 23

if __name__ == '__main__':
  print 'p1', 'pass' if p1(p1s1) else 'fail'
