from model.Unit import Unit

class Bonus(Unit):
    def __init__(self, id, width, height, x, y, type):
        Unit.__init__(self, id, width, height, x, y, 0.0, 0.0, 0.0, 0.0)

        self.type = type