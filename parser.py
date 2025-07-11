with open('test.vm', 'r') as file:
    out = open('test.asm','w')
    print("open successfully")
    line = file.readline()
    print(line)
    line = file.readline()
    print(line)
    line = file.read()
    print(line)