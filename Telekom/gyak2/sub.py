from subprocess import PIPE, Popen
import time

#PIPE: speciális érték -> nyitni kell egy csövet a standard streamre
p1 = Popen(["ping", '-n', '5', 'berkeley.edu'], stdout=PIPE)


while p1.poll()==None:
       print(" még fut " )
       time.sleep(1)

print("Miután véget ért:", p1.communicate()[0].decode())