# Input: số nguyên có giá trị [-2^127, 2^127 - 1]
# Output: giá trị số nguyên được biểu diễn bằng 128 bit và giá trị của phép ~ trên số nguyên đó

import random
print('How many numbers to be generated?')
n = int(input())
f1 = open('expected-not.txt', 'a')
f2 = open('not.txt', 'a')

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
    f1.write(str(valueOf(~x)) + '\n')
    f2.write(str(x) + '\n')

print('Successfully generated {} numbers.'.format(n))
f1.close()
f2.close()