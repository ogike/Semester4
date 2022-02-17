import subprocess


#                                             pipe-on keresztül menjen...?
p = subprocess.Popen(["echo", "hello world"], stdout=subprocess.PIPE, shell = True)
    # shell = true: vannak utasítások, amik önmagukban nem értelmezhetőek (pl dir)
    #               ha ez igaz, először egy shellt(cmdt) indít, majd azon belül hívja

##megvárja amíg véget ér, és utána adja vissza nekünk hogy mi lett az eredménye
#print(p.communicate()) # eredménye egy tuple (stdout, stderr)

# output: (b'"hello world"\r\n', None)
#          b: bytestring (byteok vannak összefűzve)
#                   itt épp szöveget reprezentál, de lehet számok, stb

result = p.communicate()
print(result[0].decode()) #ez már bytestringből embernek olvasható szöveget csinál