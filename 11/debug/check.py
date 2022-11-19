with open(r"C:\Users\kchon\桌面\DSA\b10401006\11\debug\diff.txt", 'w') as fw:
    with open(r"C:\Users\kchon\桌面\DSA\b10401006\11\debug\correct.txt") as f:
        with open(r"C:\Users\kchon\桌面\DSA\b10401006\11\debug\test.txt") as f1:
            for i, (line, line1) in enumerate(zip(f, f1)):
                if(line != line1):
                    fw.write(f"line {i}:\n")
                    fw.write(f"correct = {line}\n")
                    fw.write(f"test = {line1}\n")
                    # print(f"line {i}:")
                    # print(f"line = {line}")
                    # print(f"line1 = {line1}")