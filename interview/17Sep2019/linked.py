import random
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

def linked_to_array(linked):
  array = []
  curr = linked
  while curr != None:
    array.append(curr.val)
    curr = curr.child
  return array

# P1 How do you find the middle element of a singly linked list in one pass?
#
# This is easy if I can store O(n) data or do recursion. What data are stored?
# Is it arbitrary?
#
# TODO Can we do this without O(n) space?
def p1(s):
  length = random.randint(0,100)
  data = [ random.randint(0,100) for _ in range(length) ]
  return data[length/2] == s(make_linked(data))

def p1s1(linked):
  links = []
  curr = linked
  while curr != None:
    links.append(curr)
    curr = curr.child
  return links[len(links)/2].val


# P2 How do you check if a given linked list contains a cycle? How do you find
# the starting node of the cycle?
#
# I take this to mean that some value repeats.
def p2(s):
  data = list(range(100))
  i = random.randint(0,99)
  j = random.randint(0,99)
  random.shuffle(data)
  data[i] = data[j]
  return data[j] == s(make_linked(data))

def p2s1(linked):
  rep = {}
  curr = linked
  while curr != None:
    if rep.get(curr.val) != None:
      return curr.val
    rep[curr.val] = True
    curr = curr.child
  return None

# P3 How do you reverse a singly linked list without recursion?
def p3(s):
  data = list(range(100))
  return list(reversed(data)) == linked_to_array(s(make_linked(data)))

def p3s1(linked):
  head = linked
  prv = None
  while head != None:
    nxt = head.child
    head.child = prv
    prv = head
    head = nxt
  return prv

# P4 How are duplicate nodes removed in an unsorted linked list?
#
# Is it doubly or singly linked?
def p4(s):
  data = [ random.randint(0,10) for _ in range(100) ]
  return list(set(data)) == list(sorted(linked_to_array(s(make_linked(data)))))


def p4s1(linked):
  unique_vals = set()
  prv = None
  head = linked
  while head != None:
    val = head.val
    if val in unique_vals:
      prv.child = head.child
    else:
      prv = head
    unique_vals.add(val)
    head = head.child
  return linked

if __name__ == '__main__':
  print 'p1', 'pass' if p1(p1s1) else 'fail'
  print 'p2', 'pass' if p2(p2s1) else 'fail'
  print 'p3', 'pass' if p3(p3s1) else 'fail'
  print 'p4', 'pass' if p4(p4s1) else 'fail'
