# Studying for my interview in twi hours.
import os
import struct

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

    # create an array of random integers in range [0, 2**width).
    X = []
    for i in range(length):
        X.append(struct.unpack('<L', os.urandom(4))[0] >> (32 - width))
    print X
    S = qsort(X)
    print S
