ListAll = open('BreaKHisALL.txt','r')
BH40X = open('BreaKHis40X.txt','w')
BH100X = open('BreaKHis100X.txt','w')
BH200X = open('BreaKHis200X.txt','w')
BH400X = open('BreaKHis400X.txt','w')

for line in ListAll:
    line = line.strip()
    if line.find('40X') > 0:
        if line.find('benign') > 0:
            BH40X.write(line+' 1')
        else:
            BH40X.write(line+' 0')
        BH40X.write('\n')
    elif line.find('100X') > 0:
        if line.find('benign') > 0:
            BH100X.write(line+' 1')
        else:
            BH100X.write(line+' 0')
        BH100X.write('\n')
    elif line.find('200X') > 0:
        if line.find('benign') > 0:
            BH200X.write(line+' 1')
        else:
            BH200X.write(line+' 0')
        BH200X.write('\n')
    elif line.find('400X') > 0:
        if line.find('benign') > 0:
            BH400X.write(line+' 1')
        else:
            BH400X.write(line+' 0')
        BH400X.write('\n')
ListAll.close()
BH40X.close()
BH100X.close()
BH200X.close()
BH400X.close()
