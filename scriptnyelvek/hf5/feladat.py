# Nev: Vincze Nándor
# Neptun: MOYLJW
# h: h379409


def legnagyobb_stadion(path):
    max_capacity_stadium = ('Nincs','Nincs',0) #city, stadium, capacity

    with open(path, 'r') as file:
        file.readline()
        for line in file:
            if line == "\n":
                continue
            stadium = tuple(data.strip() for data in line.split(',')[2:5])
            if int(stadium[2]) > int(max_capacity_stadium[2]):
                max_capacity_stadium = stadium

    with open("legnagyobb.txt", "w") as file:
        if max_capacity_stadium[0] == "Nincs":
            file.write("Nincs (Nincs)\n")
        else:
            file.write(f"{max_capacity_stadium[1]} ({max_capacity_stadium[0]})\n")

def osszes_arena(path, nagy=50000, header=True, park=False):
    with open("arena_park.csv", 'w' if not park else 'a') as out_file, open(path, 'r') as in_file:
        if header:
            out_file.write("Stadium,City,Country,Big\n")
        in_file.readline()
        for line in in_file:
            if line == "\n":
                continue
            stadium = [data.strip() for data in line.split(',')]
            if stadium[3].endswith("Arena" if not park else "Park"):
                out_file.write(",".join((stadium[3], stadium[2], stadium[7], str(int(stadium[4]) > nagy))) + "\n")

def osszes_park(path):
    osszes_arena(path, nagy=20000, header=False, park=True)

def varosok_szama(path, *args):
    if len(args) == 0:
        raise Exception("Nincs megadva egy orszag sem!")

    orszagok = dict()
    for orszag in args:
        orszagok[orszag] = set()

    with open("varosok.txt", "w") as out_file, open(path, "r") as in_file:
        in_file.readline()
        for line in in_file:
            stadium = [data.strip() for data in line.split(',')]
            if stadium[-1] in orszagok.keys():
                orszagok[stadium[-1]].add(stadium[2])

        for orszag in orszagok:
            out_file.write(f"{orszag} varosai:\n")
            for varos in orszagok[orszag]:
                out_file.write(f"\t{varos}\n")
            out_file.write("-"*10 + "\n")
