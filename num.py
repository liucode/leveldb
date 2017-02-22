def totalfilesize():   
    fp = open("testdb/LOG","r")
    filesize = 0;
    while(True):
        line = fp.readline()
        if line =='':
            break;
        lines = line.split()
        if(lines[2].startswith('Level-0')):
            if(lines[5].startswith('started')):
               continue
            else:
               continue
               #filesize = filesize + float(lines[5])/1024/1024

        if(lines[2].startswith('Generated')):
            filesize = filesize +float(lines[7])/1024/1024
    print filesize
   
totalfilesize()
