from math import *
from model.FireType import FireType
from model.TankType import TankType

class MyStrategy:
    def move(self, me, world, move):
        move.left_track_power = -1.0
        move.right_track_power = 1.0
        move.turret_turn = pi
        move.fire_type = FireType.PREMIUM_PREFERRED

    def select_tank(self, tank_index, team_size):
        return TankType.MEDIUM