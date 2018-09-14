import os

filesList = open('BreaKHisList.txt','w')
for root, dirs, files in os.walk('D:/Downloads/BreaKHis_v1/histology_slides/breast/'):
    for fn in files:
        filePath = root+'/'+fn
        filePath = filePath.replace('\\','/')
        filesList.write(filePath)
        filesList.write('\n')
filesList.close()
#D:/Downloads/BreaKHis_v1/histology_slides/breast/
#BreaKHisList.txt

