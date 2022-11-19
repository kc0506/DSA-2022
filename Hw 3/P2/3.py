from itertools import starmap
from re import A, L
from turtle import circle

def f():
    Map = [
        ['q', 'i', 's', 'q', 'p'],
        ['h', 'r', 'r', 'q', 'q'],
        ['a', 'b', 'c', 'd', 'e'],
    ]

    Pattern = [
        ['i', 's', 'q', 'p'],
        ['r', 'r', 'q', 'q'],
    ]

    n = len(Map)
    m = len(Map[0])

    g = len(Pattern)
    h = len(Pattern[0])

    d = 26
    q = int(1e9 + 7)

    key = [0 for i in range(g)]
    for i in range(g):
        for j in range(h):
            key[i] = (key[i] * d + ord(Pattern[i][j])) % q

    Prefix = [0 for i in range(g)]
    k = 0
    for i in range(1, g):
        while k > 0 and key[k] != key[i]:
            k = Prefix[k - 1]
        if key[k] == key[i]:
            k += 1
        Prefix[i] = k


    base = 1
    for i in range(h - 1):
        base *= d


    MapHash = [0 for i in range(n)]
    for i in range(n):
        for j in range(h):
            MapHash[i] = (MapHash[i] * d + ord(Map[i][j])) % q

    for start in range(m):
        # First check, then roll.

        # Use KMP to match hash value.
        l = 0
        for i in range(n):
            while l > 0 and MapHash[i] != key[l]:
                l = Prefix[l - 1]
            if MapHash[i] == key[l]:
                l += 1
            if l == g:
                print(f'Match at start = {start}, [{i - g + 1}, {i}].')
                return True

        # Unmatch, then roll.
        for i in range(n):
            MapHash[i] = (MapHash[i] - ord(Map[i][start]) * base) % q
            MapHash[i] = (MapHash[i] * d + ord(Map[i][(start + h) % m])) % q
            MapHash[i] = (MapHash[i] + q) % q
    return False

f()