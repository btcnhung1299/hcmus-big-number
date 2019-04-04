import random
print('How many floating point numbers you want to generate?')
n = int(input())
f1 = open('input.txt', 'a')
f2 = open('ans.txt', 'a')


print('Successfully generate {} floating point numbers'.format(n))
f1.close()
f2.close()