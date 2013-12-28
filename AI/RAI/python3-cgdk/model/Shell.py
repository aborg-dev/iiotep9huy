from model.Unit import Unit

class Shell(Unit):
    def __init__(self, id, player_name, width, height, x, y, speed_x, speed_y, angle, angular_speed, type):
        Unit.__init__(self, id, width, height, x, y, speed_x, speed_y, angle, angular_speed)

        self.player_name = player_name
        self.type = type