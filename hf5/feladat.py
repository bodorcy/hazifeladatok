# Nev: Vincze Nándor
# Neptun: MOYLJW
# h: h379409


def legnagyobb_stadion(path):
    max_capacity_stadium = ('Nincs','Nincs',0) #city, stadium, capacity

    with open(path, 'r') as file:
        file.readline()
        for line in file:
            stadium = tuple(data.strip() for data in line.split(',')[2:5])
            if int(stadium[2]) > int(max_capacity_stadium[2]):
                max_capacity_stadium = stadium

    with open("legnagyobb.txt", "w") as file:
        file.write(f"{{{max_capacity_stadium[1]}}} ({{{max_capacity_stadium[0]}}})\n")

def osszes_arena(path):
    nagyok = ""
    pass