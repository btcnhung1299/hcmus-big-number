import random
print('How many floating point numbers you want to generate?')
n = int(input())
f1 = open('input.txt', 'a')

for _ in range(n):
    integer = random.randint(-2 ** 50, 2 ** 50)
    fractional = random.random()
    s = str(integer) + str(fractional)[1:]
    f1.write(s + '\n')

print('Successfully generate {} floating point numbers'.format(n))
f1.close()