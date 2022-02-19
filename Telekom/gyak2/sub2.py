from subprocess import PIPE, Popen
from time import sleep

# subprocess párhuzamosítás
## ezt a szerkezetet fel lehet használni első beadandóhoz

#process amit meghívunk majd
cmd = ["ping","-n","2", "localhost"]	#windows
    #-n : kettő darab csomagot küldünk...?
    #saját gépem pingeljük

process = []
for i in range(2):     #max active process (0 és 1 értékekre fig lefutni)
    #nyissunk meg lét processt és tároljuk el őket a tömbben
    p = Popen(cmd, stdout=PIPE, stderr=PIPE)
                  #kimenetet adja vissza nekünk
    process.append(p)

ures = False ##üres-e már a process lista?
while (not ures):
    futok = 0
    i = 0
    while(i < len(process)): ##végiglépdelünk a listán
        if process[i]:
            if process[i].poll() == None:
                futok +=1 #fut = fut + 1
            else:
                print(p.communicate()[0].decode()) #egyből kiírjuik az eredményt ha lefutott
                process[i] = None
        i += 1
    if (futok > 0):
        print("Meg futnak")
        sleep(1)
    else:
        ures = True
