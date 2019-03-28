# Input: một số nguyên x có giá trị [-2^127, 2^127 - 1] và một số nguyên k có giá trị [0, 127]
# Output: kết quả phép toán >>, <<

import random
print('How many pair of numbers to be generated?')
n = int(input())
f1 = open('expected-lshift.txt', 'a')
f2 = open('lshift.txt', 'a')

def valueOf(x):
    is_negative = (x >> 127) & 1
    if is_negative:
        x = ~(x - 1)
    
    value = 0
    for i in range(128):
        bit = (x >> i) & 1
        value += bit * (2 ** i)

    if is_negative:
        value *= -1
    
    return value

for _ in range(n):
    x = random.randint(-2 ** 127, 2 ** 127 - 1)
    k = random.randint(0, 127)
    z = x << k
    z = valueOf(z)

    f1.write(str(z) + '\n')
    f2.write(str(x) + '\n')
    f2.write(str(k) + '\n')

print('Successfully generated {} pair of numbers.'.format(n))
f1.close()
f2.close()