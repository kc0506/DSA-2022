
k = 3

Arr = [1, 4, 2, 5, 3]

S = [[] for i in range(3)]

Res = []

for x in Arr:
    pushed = False
    for s in S:
        if not s:
            continue
        if x <= s[-1]:
            s.append(x)
            pushed = True
            break
    if not pushed:
        for s in S:
            if len(s) == 0:
                s.append(x)
                break
for i in range(len(Arr)):
    m = 10000
    i = -1
    for j, s in enumerate(S):
        if not s:
            continue
        if s[-1] <= m:
            i, m = j, s[-1]
    S[i] = S[i][:-1]
    Res.append(m)
print(Res)
