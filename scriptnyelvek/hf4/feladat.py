# Nev: Vincze Nándor
# Neptun: MOYLJW
# h: h379409

class Palack:
    def __init__(self, ital:str, max_urtartalom, _jelenlegi_urtartalom=1):
        self.ital = ital
        self.max_urtartalom = max_urtartalom
        self._jelenlegi_urtartalom = _jelenlegi_urtartalom

    @property
    def jelenlegi_urtartalom(self):
        return self._jelenlegi_urtartalom

    @jelenlegi_urtartalom.setter
    def jelenlegi_urtartalom(self, urtartalom):
        self._jelenlegi_urtartalom = min(urtartalom, self.max_urtartalom)
        if self._jelenlegi_urtartalom == 0:
            self.ital = None

    def suly(self):
        return self.max_urtartalom / 35 + self.jelenlegi_urtartalom

    def __str__(self):
        return f"Palack, benne levo ital: {self.ital}, jelenleg {self.jelenlegi_urtartalom} ml van benne, maximum {self.max_urtartalom} ml fer bele."

    def __eq__(self, other):
        if isinstance(other, Palack):
            return self.ital == other.ital and self.jelenlegi_urtartalom == other.jelenlegi_urtartalom and self.max_urtartalom == other.max_urtartalom

        return False

    def __iadd__(self, other):
        if self.ital != other.ital:
            self.ital = "keverek" if self.jelenlegi_urtartalom != 0 else other.ital

        self.jelenlegi_urtartalom = self.jelenlegi_urtartalom + other.jelenlegi_urtartalom

        return self

class VisszavalthatoPalack(Palack):
    def __init__(self, ital:str, max_urtartalom, _jelenlegi_urtartalom=1, palackdij=25):
        super().__init__(ital, max_urtartalom, _jelenlegi_urtartalom)
        self.palackdij = palackdij

    def __str__(self):
        return "Visszavalthato" + super().__str__()

class Rekesz:
    def __init__(self, max_teherbiras):
        self.max_teherbiras = max_teherbiras
        self.palackok = list()

    def suly(self):
        return sum(self.palackok)

    def uj_palack(self, palack:Palack):
        if(palack.suly() + self.suly() <= self.max_teherbiras):
            self.palackok.append(palack)

    def osszes_penz(self):
        return sum([x.palackdij for x in self.palackok if isinstance(x, VisszavalthatoPalack)])
#PALACK TESZT_________________________________________________________________
p = Palack("viz", 10)
assert p.jelenlegi_urtartalom == 1
#setter test
p.jelenlegi_urtartalom = p.max_urtartalom + 1
assert p.jelenlegi_urtartalom == p.max_urtartalom
p.jelenlegi_urtartalom = 0
assert p.ital is None and p.jelenlegi_urtartalom == 0
#__str__
assert str(p) == f"Palack, benne levo ital: {p.ital}, jelenleg {p.jelenlegi_urtartalom} ml van benne, maximum {p.max_urtartalom} ml fer bele."
#__eq__
p2 = Palack("narancsle", 100, 10)
p3 = Palack("viz", 10, 2)

assert p != p2
assert p != p3
assert p != 4
#__iadd__
p.ital = "viz"
p.jelenlegi_urtartalom = 5
p += p2
assert p == Palack("keverek", 10, 10)
p.jelenlegi_urtartalom = 0
p += p2
assert p.ital == "narancsle" and p.jelenlegi_urtartalom == 10 and p.max_urtartalom == 10
#_______________________________________________________________________________

#VISSZAVALTHATO PALACK TESZT____________________________________________________
vp = VisszavalthatoPalack("citromossor", 500)
assert str(vp) == f"VisszavalthatoPalack, benne levo ital: citromossor, jelenleg 1 ml van benne, maximum 500 ml fer bele."
