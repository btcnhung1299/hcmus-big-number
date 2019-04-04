f1 = open('input.txt', 'r')
f2 = open('answer.txt', 'a')

line_1 = f1.readline()

while line_1:
    x = float(line_1)
    line_1 = f1.readline()
    y = float(line_1)
    z = x / y
    f2.write(str(z) + '\n')
    
    line_1 = f1.readline()

f1.close()
f2.close()