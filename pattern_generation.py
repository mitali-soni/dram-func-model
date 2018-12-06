import random
maxBank = 8
maxRow = 8192
# maxRow = 512
maxCol = 1024
# maxCol = 64
busWidth = 16

threshold = 6

file = open('pattern.txt','w')
# output format: 3bit-bank index, 13bit-row index, 1024 2B data in each column in hex format
# f ffff ffff ffff ffff ffff ffff ffff
for bankIdx in range(0,maxBank):
    for rowIdx in range(0,maxRow):
        # rowInFile = str(hex(bankIdx).split('x')[-1])+" "+str(hex(rowIdx).split('x')[-1])+" "
        rowInFile = str(hex(bankIdx))+" "+str(hex(rowIdx))+" "
        # print(rowInFile)
        # 2-byte level distribution, within two bytes the randomness is not so good.
        if rowIdx % 512 == 0:
            high = random.randint(0,1)
        # if sparse is 0, 16 bit will be dense.
        # if sparse is 1, 16 bit will be sparse.
        sparse = ((rowIdx >> 9) % 2) ^ high
        for readIdx in range(0,maxCol):
            if sparse ^ (readIdx % 2) == 1:
                oneNum = random.randint(0,threshold)
            else:
                oneNum = random.randint(16-threshold,16)
            randList=[]
            for shiftIdx in range(0,busWidth):
                if shiftIdx < oneNum:
                    randList.append(1)
                else:
                    randList.append(0)
            # print(randList)
            random.shuffle(randList)
            # print(randList)
            randStr = str(randList[0])
            for x in range(1,busWidth):
                randStr += str(randList[x])
            rowInFile += str(hex(int(randStr,2)))
            if readIdx != maxCol-1:
                rowInFile += " "
        rowInFile += "\n"
        # print(rowInFile)
        file.write(rowInFile)


file.close()
