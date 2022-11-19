from audioop import reverse


N, Q = [int(i) for i in input().split(' ')[:2]]

L = [int(i) for i in input().split(' ')[:N]]

for i in range(Q):
    text = input()
    t = int(text[0])
    if t == 1:
        k, p = [int(x) for x in text.split()[1:]]
        L.insert(p, k)
    elif t == 2:
        p = int(text.split()[1])
        del(L[p])
    elif t == 3:
        l, r = [int(x) for x in text.split()[1:]]
        L[l:r + 1] = reversed(L[l:r + 1])
    elif t == 4:
        l, r, x, y = [int (i) for i in text.split()[1:]]
        newL = L[:l] + L[x:y + 1] + L[r + 1: x] + L[l:r + 1]
        L = newL
    elif t == 5:
        l, r, k = [int(x) for x in text.split()[1:]]
        L[l:r + 1] = [max(x, k) for x in L[l:r + 1]]
    else:
        l, r = [int(x) for x in text.split()[1:]]
        print(sum(L[l:r + 1]))
