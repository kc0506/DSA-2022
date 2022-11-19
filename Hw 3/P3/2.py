from email.policy import default


Arr = [None, 73, 4, 184, 504, 76, 47, 299, 9]

Hash = [0 for i in range(int(1e7))]
newArr = [0 for i in range(len(Arr))]
n = len(Arr)
for i in range(1, n):
    Hash[Arr[i]] += 1
for i in range(1, len(Hash)):
    Hash[i] += Hash[i - 1]
for i in range(1, n):
    newArr[Hash[Arr[i]]] = Arr[i]
    Arr[i] -= 1
print(newArr)
