

def kset0(S, k, x, current, solution): 

  if len(current) == k: 
    solution.append(list(current))
    return

  if (x == len(S)):
    return

  current.add(S[x])
  kset0(S, k, x+1, current, solution)
  current.remove(S[x])
  kset0(S, k, x+1, current, solution)

def kset(S, k):
  res = []
  kset0(S, k, 0, set(), res)
  return res

def powerset(S):
  res = [S]
  for k in reversed(range(2,len(S))):
    res += kset(S, k)
  res += [[s] for s in S]
  res.append([])
  return res

print len(powerset([1,2,3,4]))
