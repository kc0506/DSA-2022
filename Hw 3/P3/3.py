Arr = [3, 2, 1, 5, 4]

Min = []
Res = []
for x in Arr:
    print(Res)
    print(Min)
    if Min:
        if x <= Min[-1]:
            Min.append(x)
        else:
            while Min:
                if not x >= Min[-1]:
                    break
                Res.append(Min[-1])
                Min = Min[:-1]
            Min.append(x)
    else:
        Min.append(x)
while Min:
    Res.append(Min[-1])
    Min = Min[:-1]
print(Res)


