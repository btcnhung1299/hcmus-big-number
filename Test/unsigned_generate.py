import random
print('How many unsigned numbers you want to generate?')
n = int(input())

f = open("unsigned.txt", "a")
f.write(str(n) + "\n")

for _ in range(n):
    x = random.randint(1, 2 ** 127 - 1)
    f.write(str(x) + "\n")
    f.write('{0:0128b}\n'.format(x))

print('Successfully generate {} unsigned number'.format(n))
f.close()