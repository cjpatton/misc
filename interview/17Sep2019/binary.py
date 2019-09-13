import random

class Node:
  def __init__(self, val):
    self.val = val
    self.left = None
    self.right = None
    self.x = False

  def insert(self, val):
    if val < self.val:
      if self.left != None:
        self.left.insert(val)
      else:
        self.left = Node(val)
    else:
      if self.right != None:
        self.right.insert(val)
      else:
        self.right = Node(val)

  def find(self, val):
    if val == self.val:
      return True
    elif val < self.val and self.left != None:
      return self.left.find(val)
    elif self.right != None:
      return self.right.find(val)
    return False

  def visited(self, p):
    return self.x is not p

  def visit(self, p):
    self.x = not p

  def count(self):
    ct = 0
    stack = [self]
    p = self.x
    while len(stack) > 0:
      top = stack.pop()
      if top.visited(p) or (top.left == None and top.right == None):
        top.visit(p)
        ct += 1
      else:
        top.visit(p)
        if top.right != None:
          stack.append(top.right)
        stack.append(top)
        if top.left != None:
          stack.append(top.left)
    return ct

  def preordered(self, array):
    array.append(self.val)
    if self.left != None:
      self.left.preordered(array)
    if self.right != None:
      self.right.preordered(array)

  def preordered_it(self, array):
    stack = [self]
    while len(stack) > 0:
      node = stack.pop()
      array.append(node.val)
      if node.right != None:
        stack.append(node.right)
      if node.left != None:
        stack.append(node.left)

  def inordered(self, array):
    if self.left != None:
      self.left.inordered(array)
    array.append(self.val)
    if self.right != None:
      self.right.inordered(array)

  def inordered_it(self, array):
    stack = [self]
    p = self.x
    while len(stack) > 0:
      top = stack.pop()
      if top.visited(p):
        array.append(top.val)
      else:
        top.visit(p)
        if top.left == None and top.right == None:
          stack.append(top)
        else:
          if top.right != None:
            stack.append(top.right)
          stack.append(top)
          if top.left != None:
            stack.append(top.left)

if __name__ == '__main__':
  ct = random.randint(50,150)
  tree = Node(random.randint(0,100))
  for _ in range(ct-1):
    tree.insert(random.randint(0,100))

  # P1 How do you traverse a given binary tree in preorder without recursion?
  expected = []
  tree.preordered(expected)
  got = []
  tree.preordered_it(got)
  print 'p1', 'pass' if got == expected else 'fail'

  # P2 How do you print all nodes of a given binary tree using inorder traversal
  # without recursion?
  expected = []
  tree.inordered(expected)
  got = []
  tree.inordered_it(got)
  print 'p2', 'pass' if got == expected else 'fail'

  # P3 How do you count a number of leaf nodes in a given binary tree?
  print 'p2', 'pass' if ct == tree.count() else 'fail'
