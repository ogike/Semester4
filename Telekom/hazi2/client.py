from array import array
import sys
import json


# opening and storing the json
with open(sys.argv[1], "r") as file:
    data = json.load(file)

#setting the current traffic field
for link in data["links"]:
    link["current"] = 0

# FINDING A GOOD CIRCUIT ##################################################

def is_link_good(point1: str, point2: str, demand: int) -> bool:
    for link in data["links"]:
        if( link["points"][0] == point1 and link["points"][1] == point2 ):
            if(link["capacity"] - link["current"] >= demand):
                return True
            else:
                return False
    return False #if we didnt find the specified link

def is_circuit_good(circuit_index: int, demand: float) -> bool:
    circuit = data["possible-circuits"][circuit_index]
    for i in range ( len(circuit) - 1 ):
        if( not is_link_good( circuit[i], circuit[i+1], demand ) ):
            return False
    return True #if every link was good

# gives back a good circuits index, or -1 if theres none
def find_good_circuit(start: str, end: str, demand: int) -> int:
    curCircuitIndex = 0 #TODO: not the prettiest
    for circuit in data["possible-circuits"]:
        if( circuit[0] == start and circuit[-1] == end): #if its the route we want
            if( is_circuit_good(curCircuitIndex, demand) ):
                return curCircuitIndex
        curCircuitIndex += 1
    return -1 #if we didnt find a good circuit

# BOOKING/FREEING CIRCUIT #################################################


#TODO: python javadoc style comments?
#Demand: positive if booking, negative if freeing
def change_link_current(point1: str, point2: str, demand: float):
    for link in data["links"]:
        # if we found the correct link
        if( link["points"][0] == point1 and link["points"][1] == point2 ):
            link["current"] += demand
            return

#Note: only works on good circuits!!!
#Demand: positive if booking, negative if freeing
def change_circuit_current(circuit_index: int, demand: float):
    circuit = data["possible-circuits"][circuit_index]
    for i in range ( len(circuit) - 1 ):
        change_link_current(circuit[i], circuit[i+1], demand)

# MAIN SIMULATION #########################################################

#TODO: write to file
def printEventBooking(eventInd : int, start: str, end : str, time: int, success: bool):
    print( "%(eventIndex)i. igény foglalás: %(start)s<->%(end)s st:%(time)i - %(success)s"
            %  {'eventIndex' : eventInd, 
                'start': start, 'end': end,
                'time': t, 
                'success': "sikeres" if success else "sikertelen"})

def printEventFreeing(eventInd : int, start: str, end : str, time: int):
    print( "%(eventIndex)i. igény felszabadítás: %(start)s<->%(end)s st:%(time)i"
            %  {'eventIndex' : eventInd, 
                'start': start, 'end': end,
                'time': t})

startTime : int = 1
endTime   : int = data["simulation"]["duration"]
demands   : object = data["simulation"]["demands"]

eventIndex : int = 1

#set booked fields (if booked: circuit index, if not: -1)
for demand in demands:
    demand["booked"] = -1

#TODO: test if endTime is inclusive or exclusive
for t in range(startTime, endTime):
    for demand in demands:
        #TODO: put into book function
        if( demand["start-time"] == t ): #booking
            endPoints : array = demand["end-points"]
            demandValue : float = demand["demand"]
            index : int = find_good_circuit( endPoints[0], endPoints[1], demandValue )
            if(index != -1):
                change_circuit_current(index, demandValue)
                demand["booked"] = index
                success : bool = True
            else:
                #demand["booked"] stays -1
                success :bool = False
            printEventBooking(eventIndex, endPoints[0], endPoints[1], t, success)
            eventIndex += 1
        if( demand["end-time"] == t and demand["booked"] != -1 ): #freeing
            change_circuit_current( demand["booked"], -1 * demand["demand"] )
            demand["booked"] = -1
            printEventFreeing(eventIndex, demand["end-points"][0], demand["end-points"][1], t)
            eventIndex += 1





