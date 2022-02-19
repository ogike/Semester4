from subprocess import PIPE, Popen
import time
#PIPE: speciális érték -> nyitni kell egy csövet a standard streamre

#meghívjuk a processt
p1 = Popen(["ping", '-n', '5', 'berkeley.edu'], stdout=PIPE)

#negnézzük hogy befejeződött-e a process
while p1.poll()==None:
       print(" még fut " )
       time.sleep(1)

#kiírjuk a process eredményét

result = p1.communicate()[0].decode()

print("Miután véget ért:", result)