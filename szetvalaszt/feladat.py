#Nev: Vincze Nándor
#Neptun: MOYLJW
#h: h379409

def elvalasztas(szo:str):
    mgh = {'a', 'e', 'i', 'o', 'ö', 'u', 'ü', 'á', 'é', 'í', 'ó', 'ő', 'ú', 'ű'}
    mltp = {'cs', 'dz', 'gy', 'ly', 'ny', 'sz', 'ty', 'zs', 'dzs'}

    betuk = [c for c in szo]

    match betuk:
        case [*_, 'c', 's']:
            pass

        case ['c', 's', *_]:
            pass

        case _:
            pass



#print(szetvalaszt("lalabamam"))
#print(szetvalaszt("alma"))
#print(szetvalaszt("abba"))
#print(szetvalaszt("lead"))
#print(szetvalaszt("amper"))
#print(szetvalaszt("centrum"))
