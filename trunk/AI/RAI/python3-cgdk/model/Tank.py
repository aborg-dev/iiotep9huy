from math import *

from model.TankType import TankType
from model.Unit import Unit

def get_width(type):
    return {
               TankType.MEDIUM: 90.0,
               TankType.HEAVY: 105.0,
               TankType.TANK_DESTROYER: 112.5
           }[type]


def get_height(type):
    return {
               TankType.MEDIUM: 60.0,
               TankType.HEAVY: 75.0,
               TankType.TANK_DESTROYER: 67.5
           }[type]


class Tank(Unit):
    def __init__(self, id, player_name, teammate_index, x, y, speed_x, speed_y, angle, angular_speed,
                 turret_relative_angle, crew_health, hull_durability,
                 reloading_time, remaining_reloading_time, premium_shell_count, teammate, type):
        Unit.__init__(self, id, get_width(type), get_height(type), x, y, speed_x, speed_y, angle, angular_speed)

        self.player_name = player_name
        self.teammate_index = teammate_index
        self.turret_relative_angle = turret_relative_angle
        self.crew_health = crew_health
        self.hull_durability = hull_durability
        self.reloading_time = reloading_time
        self.remaining_reloading_time = remaining_reloading_time
        self.premium_shell_count = premium_shell_count
        self.teammate = teammate
        self.type = type

        self.virtual_gun_length = {
                                      TankType.MEDIUM: 67.5,
                                      TankType.HEAVY: 82.5,
                                      TankType.TANK_DESTROYER: 97.5
                                  }[type]

        self.mass = {
                        TankType.MEDIUM: 10.0,
                        TankType.HEAVY: 20.0,
                        TankType.TANK_DESTROYER: 15.0
                    }[type]

        self.engine_power = {
                                TankType.MEDIUM: 7500.0,
                                TankType.HEAVY: 7500.0,
                                TankType.TANK_DESTROYER: 5000.0
                            }[type]

        self.engine_rear_power_factor = {
                                            TankType.MEDIUM: 0.75,
                                            TankType.HEAVY: 0.5,
                                            TankType.TANK_DESTROYER: 0.35
                                        }[type]

        self.turret_turn_speed = {
                                     TankType.MEDIUM: 1.0 * pi / 180.0,
                                     TankType.HEAVY: 0.5 * pi / 180.0,
                                     TankType.TANK_DESTROYER: 1.5 * pi / 180.0
                                 }[type]

        self.turret_max_relative_angle = {
                                             TankType.MEDIUM: 1.0 * pi / 180.0,
                                             TankType.HEAVY: 0.5 * pi / 180.0,
                                             TankType.TANK_DESTROYER: 1.5 * pi / 180.0
                                         }[type]

        self.crew_max_health = {
                                   TankType.MEDIUM: 100,
                                   TankType.HEAVY: 100,
                                   TankType.TANK_DESTROYER: 100
                               }[type]

        self.hull_max_durability = {
                                       TankType.MEDIUM: 200,
                                       TankType.HEAVY: 250,
                                       TankType.TANK_DESTROYER: 250
                                   }[type]

        self.frontal_armor = {
                                 TankType.MEDIUM: 175,
                                 TankType.HEAVY: 200,
                                 TankType.TANK_DESTROYER: 250
                             }[type]

        self.side_armor = {
                                   TankType.MEDIUM: 150,
                                   TankType.HEAVY: 175,
                                   TankType.TANK_DESTROYER: 125
                               }[type]

        self.read_armor = {
                                   TankType.MEDIUM: 100,
                                   TankType.HEAVY: 100,
                                   TankType.TANK_DESTROYER: 100
                               }[type]

    def get_turret_angle_to(self, x, y):
        absolute_angle_to = atan2(y - self.y, x - self.x)
        relative_angle_to = absolute_angle_to - self.angle - self.turret_relative_angle

        while relative_angle_to > pi:
            relative_angle_to -= 2.0 * pi

        while relative_angle_to < -pi:
            relative_angle_to += 2.0 * pi

        return relative_angle_to

    def get_turret_angle_to_unit(self, unit):
        return self.get_turret_angle_to(unit.x, unit.y)