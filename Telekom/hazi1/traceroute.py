from subprocess import PIPE, Popen
from time import sleep
import json
import datetime
import platform


today = datetime.date.today()
platform = platform.system()

targets = []

#reading the first 10 websites
with open("top-1m.csv") as file:
    #TODO: set to 10
    for i in range(5): #read the first 10 lines
        targets.append( file.readline().split(',')[1].rstrip() )


#opening the ping processes
processes = []
for target in targets:
    p = Popen(["ping", "-n", "10", target], stdout=PIPE, stderr=PIPE, encoding='utf-8')
                  #kimenetet adja vissza nekünk
    processes.append(p)


#pinging
pings = [None] * 20 #initializing the list in advance
ures = False
while (not ures):
    futok = 0
    i = 0
    while(i < len(processes)): ##végiglépdelünk a listán
        if processes[i]: #ha még nem None ez a process
            if processes[i].poll() == None:
                futok +=1 #fut = fut + 1
            else:
                pings[i] = processes[i].communicate()[0]
                processes[i] = None
        i += 1
    if (futok > 0):
        print("Meg futnak")
        sleep(1)
    else:
        ures = True

#creating the data for ping.json


data = {
    "date" : today.strftime("%Y%m%d"),
    "system" : platform,
    "pings": pings
}



#writing ping.json
with open('ping.json', 'w') as f:
    json.dump(data,f, indent=4)



# to try for last 10 lines:
# import os

# with open("file.txt", "rb") as file:
#     # Go to the end of the file before the last break-line
#     file.seek(-2, os.SEEK_END) 
#     # Keep reading backward until you find the next break-line
#     while file.read(1) != b'\n':
#         file.seek(-2, os.SEEK_CUR) 
#     print(file.readline().decode())