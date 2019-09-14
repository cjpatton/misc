def genip(S, I):
  if len(I) == 4:
    yield '.'.join(map(str, I))
  elif len(I) == 3 and len(S) > 0:
    x = int(S)
    if x < 255:
      yield '.'.join(map(str, I + [x]))
  else:
    for i in range(1,4):
      if len(S) >= i:
        x = int(S[:i])
        if x < 255:
          for ip in genip(S[i:], I + [x]):
            yield ip

for ip in genip('11211', []):
  print ip
