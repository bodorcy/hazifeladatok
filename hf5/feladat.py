# Nev: Vincze Nándor
# Neptun: MOYLJW
# h: h379409
import distutils.util


def legnagyobb_stadion(path):
    with open(path, 'r') as file:
        stadium = dict()
        for key in [k.strip() for k in file.readline().split(',')]:
            stadium[key] = None

        record = ""
        for line in file:
            record += line
            if record.count(',') < 7:
                continue

            data = [d.strip() for d in record.split(',')]

            for key, data in stadium.keys(), data:
                stadium[key] = data



            record = ""
