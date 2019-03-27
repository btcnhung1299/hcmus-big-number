def valueOf(x):
    x &= 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    is_negative = (x >> 127) & 1
    
    if is_negative:
        x = ~(x + 1)
    
    value = 0
    for i in range(128):
        bit = (x >> i) & 1
        value += bit * (2 ** i)

    if is_negative:
        value *= -1
    
    return value

x = 628021444369604579313555637492626046586
print(valueOf(x))