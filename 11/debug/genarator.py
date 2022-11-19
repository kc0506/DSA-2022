
with open(r"C:\Users\kchon\桌面\DSA\b10401006\11\debug\test.txt", 'w') as f:

    import random

    N = 10000
    Q = 100000

    f.write(f"{N} {Q}\n")
    # print(N, Q)

    L = [str(random.randint(1, 10**5)) for i in range(N)]
    f.write(" ".join(L) + "\n")
    # print(" ".join(L))

    size = N - 1
    for i in range(Q):
        t = random.randint(5, 7)
        if(t == 1):
            p = random.randint(0, size + 1)
            k = random.randint(1, 10**5)
            size += 1
            f.write(f"1 {p} {k}\n")
        elif(t == 2):
            p = random.randint(1, size + 1)
            size -= 1
            f.write(f"2 {p}\n")
        elif(t == 3):
            l = random.randint(1, size + 1)
            r = random.randint(l, size + 1)
            f.write(f"3 {l} {r}\n")
        elif t == 4:
            l = random.randint(1, size + 1)
            r = random.randint(l, size + 1)
            x = random.randint(1, size + 1)
            y = random.randint(x, size + 1)
            f.write(f"4 {l} {r} {x} {y}\n")
        elif t == 5:
            l = random.randint(1, size + 1)
            r = random.randint(l, size + 1)
            k = random.randint(1, 1000)
            f.write(f"5 {l} {r} {k}\n")
        else:
            l = random.randint(1, size + 1)
            r = random.randint(l, size + 1)
            f.write(f"6 {l} {r}\n")
