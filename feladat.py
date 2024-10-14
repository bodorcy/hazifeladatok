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
        jelenlegi_urtartalom = self.jelenlegi_urtartalom + other.jelenlegi_urtartalom

        if self.ital != other.ital:
            self.ital = "keverek" if jelenlegi_urtartalom != 0 else other.ital


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
p.jelenlegi_urtartalom = 5
p += p2
assert p == Palack("viz", 10, 7)
