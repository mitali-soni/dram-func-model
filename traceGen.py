import sys

def getAddr(bankNum, rowNum, colNum):
    if not(0 <= bankNum <= 8): raise ValueError('bank in range 0~8'+bankNum)
    if not(0 <= rowNum <= 8192): raise ValueError('row in range 0~8192')
    if not(0 <= colNum <= 1024): raise ValueError('col in range 0~1024')
    #if (colNum%8 > 0): raise ValueError('col should be multiple of 8')
    
    bank = int2bin(bankNum, 3)
    row = int2bin(rowNum,13)
    col = int2bin(colNum,10)

    str_2 = "000" + bank + row + col + "000"
    str_16 = hex(int(str_2,2))
    
    return str_2, str_16


def int2bin(n, count=10):
    """returns the binary of integer n, using count number of digits"""
    return "".join([str((n >> y) & 1) for y in range(count-1, -1, -1)])

if __name__=="__main__":
    
    # addr = getAddr(0,3413,42*8)
    # print(addr[1])
    
    fileIn = sys.argv[1]
    fileOut = sys.argv[2]
    output = open(fileOut,"a")
    for line in open(fileIn):
        parameter = line.split(' ')
        print ('bank '+ parameter[0])
        print ('row ' + parameter[1])
        print ('col ' + parameter[2])
        addr = getAddr(int(parameter[0]),int(parameter[1]),int(parameter[2]))
        trace = addr[1] + ' ' + 'P_MEM_RD' + ' ' + parameter[3]
        output.write(trace)

    
    output.close()