# Input: cặp số nguyên có giá trị [-2^127, 2^127 - 1]
# Output: kết quả phép toán +, -, *, /, &, |, ^ trên cặp số nguyên được phát sinh

import random
print('How many pair of numbers to be generated?')
n = int(input())
f1 = open('expected-xor.txt', 'a')
f2 = open('xor.txt', 'a')

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
    y = random.randint(-2 ** 127, 2 ** 127 - 1)
    # Với phép +, -, *, kết quả được tính bằng cách gán z = a (+, -, *) b
    # Với phép /, thực hiện chia trị tuyệt đối của hai số, cuối cùng đổi dấu thương nếu hai số ban đầu khác dấu.
    z = x ^ y                # abs(x) // abs(y)
    z = valueOf(z)
    # if x * y < 0:
    #     z *= -1

    f1.write(str(z) + '\n')
    f2.write(str(x) + '\n')
    f2.write(str(y) + '\n')

print('Successfully generated {} pair of numbers.'.format(n))
f1.close()
f2.close()