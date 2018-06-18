
#name = input()
name = "1048576_matrices_cuda.out"
f = open(name)
x = f.read()
f.close()

x = x.split()
x = [int(i) for i in x]
x.sort()

ret = [round(i/1000,2) for i in x]
print(ret)

f = open(name + ".script", "w+")
for i in ret:
    f.write(str(i) + "\n")

f.close()



