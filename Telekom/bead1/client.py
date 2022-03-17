from subprocess import PIPE, Popen
from time import sleep
import json
import datetime
import platform
import os
import sys


today = datetime.date.today()
platform = platform.system()
targets = []

with open(sys.argv[1], "rb") as file: #opening in binary format
    #reading the first 10 websites
    for i in range(10): #read the first 10 lines
        targets.append( file.readline().decode().split(',')[1].rstrip() ) # ditching the number
    
    #reading the last 10 lines:

    # Go to the end of the file before the last break-line
    file.seek(-2, os.SEEK_END)

    # seek back 10 lines
    for i in range(10):
        # Keep reading backward until the next break-line
        while file.read(1) != b'\n':
            file.seek(-2, os.SEEK_CUR) 
        file.seek(-2, os.SEEK_CUR) #have to seek back the '\n' we just read
    file.seek(2, os.SEEK_CUR) #counteracting the last line for the last iteration

    # now that our seek is at the right place, we just read the last 10 lines
    for i in range(10):  
        targets.append( file.readline().decode().split(',')[1].rstrip() )


#opening the ping processes
processes = []
for target in targets:
    if(target):
        param = "-n" if (target == "Windows") else "-c" 
        p = Popen(["ping", param, "10", target], stdout=PIPE, stderr=PIPE, encoding='utf-8')
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
                pings[i] = {
                    "target" : targets[i],
                    "output" : processes[i].communicate()[0]
                }
                processes[i] = None
        i += 1
    if (futok > 0):
        print("Meg futnak")
        sleep(5)
    else:
        ures = True
print("pings succesful")


#creating the data for ping.json
ping_data = {
    "date" : today.strftime("%Y%m%d"),
    "system" : platform,
    "pings": pings
}


#writing ping.json
with open('ping.json', 'w') as f:
    json.dump(ping_data,f, indent=4)

print("written ping.json succesfully")


#opening the traceroute processes
processes = []
for target in targets:
    if(target):
        param = "traceroute" if (target == "Windows") else "tracert" 
        p = Popen([param, target], stdout=PIPE, stderr=PIPE, encoding='utf-8')
        processes.append(p)


#tracerouting
traces = [None] * 20 #initializing the list in advance
ures = False
while (not ures):
    futok = 0
    i = 0
    while(i < len(processes)): ##végiglépdelünk a listán
        if processes[i]: #ha még nem None ez a process
            if processes[i].poll() == None:
                futok +=1 #fut = fut + 1
            else:
                traces[i] = {
                    "target" : targets[i],
                    "output" : processes[i].communicate()[0]
                }
                processes[i] = None
        i += 1
    if (futok > 0):
        print("Meg futnak")
        sleep(5)
    else:
        ures = True
print("tracing succesful")

#creating the data for traceroute.json
ping_data = {
    "date" : today.strftime("%Y%m%d"),
    "system" : platform,
    "traces": traces
}


#writing ping.json
with open('traceroute.json', 'w') as f:
    json.dump(ping_data,f, indent=4)

print("written traceroute.json succesfully")