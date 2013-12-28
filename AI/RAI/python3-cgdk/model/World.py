class World:
    def __init__(self, tick, width, height, players, obstacles, tanks, shells, bonuses):
        self.tick = tick
        self.width = width
        self.height = height
        self.players = players
        self.obstacles = obstacles
        self.tanks = tanks
        self.shells = shells
        self.bonuses = bonuses