from model.Unit import Unit

class Obstacle(Unit):
    def __init__(self, id, width, height, x, y):
        Unit.__init__(self, id, width, height, x, y, 0.0, 0.0, 0.0, 0.0)