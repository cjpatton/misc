# Studying for my interview in two hours.
import os
import struct

class Node:

    def __init__(self, val):
        assert(val is not None)
        self.val = val
        self.children = {}

    def __getitem__(self, idx):
        return self.children.get(idx)

    def __setitem__(self, idx, node):
        assert(isinstance(node, Node))
        self.children[idx] = node

def binsert(root, val):
    if root.val == val: # Done.
        return
    elif val < root.val: # Go left.
        if root[0] is None:
            root[0] = Node(val)
        else:
            binsert(root[0], val)
    else: # Go right.
        if root[1] is None:
            root[1] = Node(val)
        else:
            binsert(root[1], val)

def bsort(root, S):
    if root[0] is not None:
        bsort(root[0], S)
    S.append(root.val)
    if root[1] is not None:
        bsort(root[1], S)

def qsort(X):
    if len(X) < 2:
        return X
    p = X[len(X)>>1] # Pick a pivot.
    L = []
    R = []
    for x in X:
        if x < p:
            L.append(x)
        elif x > p:
            R.append(x)
    return qsort(L) + [p] + qsort(R)

if __name__ == '__main__':

    length = 20 # Size of the array.
    width = 8 # Length of maximum integer bits.
    assert(0 < width and width <= 32)

    # Create an array of random integers in range [0, 2**width).
    X = []
    for i in range(length):
        X.append(struct.unpack('<L', os.urandom(4))[0] >> (32 - width))

    # Sort X with quick sort to get S.
    S = qsort(X)
    print S

    # Create a binary tree from X.
    root = Node(X[0])
    for x in X[1:]:
        binsert(root, x)
    T = []
    bsort(root, T)
    print T
