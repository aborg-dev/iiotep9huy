import _socket
import struct
from model.Bonus import Bonus
from model.BonusType import BonusType
from model.Obstacle import Obstacle
from model.Player import Player
from model.PlayerContext import PlayerContext
from model.Shell import Shell
from model.ShellType import ShellType
from model.Tank import Tank
from model.TankType import TankType
from model.World import World

class RemoteProcessClient:
    LITTLE_ENDIAN_BYTE_ORDER = True

    SIGNED_BYTE_FORMAT_STRING = '<b' if LITTLE_ENDIAN_BYTE_ORDER else '>b'
    INTEGER_FORMAT_STRING = '<i' if LITTLE_ENDIAN_BYTE_ORDER else '>i'
    LONG_FORMAT_STRING = '<q' if LITTLE_ENDIAN_BYTE_ORDER else '>q'
    DOUBLE_FORMAT_STRING = '<d' if LITTLE_ENDIAN_BYTE_ORDER else '>d'

    SIGNED_BYTE_SIZE_BYTES = 1
    INTEGER_SIZE_BYTES = 4
    LONG_SIZE_BYTES = 8
    DOUBLE_SIZE_BYTES = 8

    def __init__(self, host, port):
        self.socket = _socket.socket()
        self.socket.connect((host, port))

    def write_token(self, token):
        self.write_enum(RemoteProcessClient.MessageType.AUTHENTICATION_TOKEN)
        self.write_string(token)

    def read_team_size(self):
        message_type = self.read_enum(RemoteProcessClient.MessageType)
        self.ensure_message_type(message_type, RemoteProcessClient.MessageType.TEAM_SIZE)
        return self.read_int()

    def write_selected_tanks(self, tank_types):
        self.write_enum(RemoteProcessClient.MessageType.TANK_TYPES)

        if tank_types is None:
            self.write_int(-1)
        else:
            self.write_int(tank_types.__len__())

            for type in tank_types:
                self.write_enum(type)

    def read_player_context(self):
        message_type = self.read_enum(RemoteProcessClient.MessageType)
        if message_type == RemoteProcessClient.MessageType.GAME_OVER:
            return None

        self.ensure_message_type(message_type, RemoteProcessClient.MessageType.PLAYER_CONTEXT)
        return PlayerContext(self.read_tanks(), self.read_world()) if self.read_boolean() else None

    def write_moves(self, moves):
        self.write_enum(RemoteProcessClient.MessageType.MOVES)

        if moves is None:
            self.write_int(-1)
        else:
            self.write_int(moves.__len__())

            for move in moves:
                if move is None:
                    self.write_boolean(False)
                else:
                    self.write_boolean(True)

                    self.write_double(move.left_track_power)
                    self.write_double(move.right_track_power)
                    self.write_double(move.turret_turn)
                    self.write_enum(move.fire_type)

    def close(self):
        self.socket.close()

    def read_world(self):
        if not self.read_boolean():
            return None

        return World(
            self.read_int(), self.read_double(), self.read_double(), self.read_players(),
            self.read_obstacles(), self.read_tanks(), self.read_shells(), self.read_bonuses()
        )

    def read_players(self):
        player_count = self.read_int()
        if player_count < 0:
            return None

        players = []

        for player_index in range(player_count):
            if self.read_boolean():
                player = Player(self.read_string(), self.read_int(), self.read_boolean())
                players.append(player)
            else:
                players.append(None)

        return players

    def read_obstacles(self):
        obstacle_count = self.read_int()
        if obstacle_count < 0:
            return None

        obstacles = []

        for obstacle_index in range(obstacle_count):
            if self.read_boolean():
                obstacle = Obstacle(
                    self.read_long(), self.read_double(), self.read_double(),
                    self.read_double(), self.read_double()
                )
                obstacles.append(obstacle)
            else:
                obstacles.append(None)

        return obstacles

    def read_tanks(self):
        tank_count = self.read_int()
        if tank_count < 0:
            return None

        tanks = []

        for tank_index in range(tank_count):
            if self.read_boolean():
                tank = Tank(
                    self.read_long(), self.read_string(), self.read_int(),
                    self.read_double(), self.read_double(), self.read_double(), self.read_double(),
                    self.read_double(), self.read_double(), self.read_double(),
                    self.read_int(), self.read_int(), self.read_int(), self.read_int(),
                    self.read_int(), self.read_boolean(), self.read_enum(TankType)
                )
                tanks.append(tank)
            else:
                tanks.append(None)

        return tanks

    def read_shells(self):
        shell_count = self.read_int()
        if shell_count < 0:
            return None

        shells = []

        for shell_index in range(shell_count):
            if self.read_boolean():
                shell = Shell(
                    self.read_long(), self.read_string(), self.read_double(), self.read_double(),
                    self.read_double(), self.read_double(), self.read_double(), self.read_double(),
                    self.read_double(), self.read_double(), self.read_enum(ShellType)
                )
                shells.append(shell)
            else:
                shells.append(None)

        return shells

    def read_bonuses(self):
        bonus_count = self.read_int()
        if bonus_count < 0:
            return None

        bonuses = []

        for bonus_index in range(bonus_count):
            if self.read_boolean():
                bonus = Bonus(
                    self.read_long(), self.read_double(), self.read_double(),
                    self.read_double(), self.read_double(), self.read_enum(BonusType)
                )
                bonuses.append(bonus)
            else:
                bonuses.append(None)

        return bonuses

    def ensure_message_type(self, actual_type, expected_type):
        if actual_type != expected_type:
            raise ValueError("Received wrong message [actual=%s, expected=%s]" % (actual_type, expected_type))

    def read_enum(self, enum_class):
        bytes = self.read_bytes(RemoteProcessClient.SIGNED_BYTE_SIZE_BYTES)
        value = struct.unpack(RemoteProcessClient.SIGNED_BYTE_FORMAT_STRING, bytes)[0]

        for enum_key, enum_value in enum_class.__dict__.items():
            if not str(enum_key).startswith("__") and value == enum_value:
                return enum_value

        return None

    def write_enum(self, value):
        self.write_bytes(struct.pack(RemoteProcessClient.SIGNED_BYTE_FORMAT_STRING, -1 if value is None else value))

    def read_string(self):
        length = self.read_int()
        if length == -1:
            return None

        bytes = self.read_bytes(length)
        return bytes.decode()

    def write_string(self, value):
        if value is None:
            self.write_int(-1)
            return

        bytes = value.encode()

        self.write_int(len(bytes))
        self.write_bytes(bytes)

    def read_boolean(self):
        bytes = self.read_bytes(RemoteProcessClient.SIGNED_BYTE_SIZE_BYTES)
        return struct.unpack(RemoteProcessClient.SIGNED_BYTE_FORMAT_STRING, bytes)[0] == 1

    def write_boolean(self, value):
        self.write_bytes(struct.pack(RemoteProcessClient.SIGNED_BYTE_FORMAT_STRING, 1 if value else 0))

    def read_int(self):
        bytes = self.read_bytes(RemoteProcessClient.INTEGER_SIZE_BYTES)
        return struct.unpack(RemoteProcessClient.INTEGER_FORMAT_STRING, bytes)[0]

    def write_int(self, value):
        self.write_bytes(struct.pack(RemoteProcessClient.INTEGER_FORMAT_STRING, value))

    def read_long(self):
        bytes = self.read_bytes(RemoteProcessClient.LONG_SIZE_BYTES)
        return struct.unpack(RemoteProcessClient.LONG_FORMAT_STRING, bytes)[0]

    def write_long(self, value):
        self.write_bytes(struct.pack(RemoteProcessClient.LONG_FORMAT_STRING, value))

    def read_double(self):
        bytes = self.read_bytes(RemoteProcessClient.DOUBLE_SIZE_BYTES)
        return struct.unpack(RemoteProcessClient.DOUBLE_FORMAT_STRING, bytes)[0]

    def write_double(self, value):
        self.write_bytes(struct.pack(RemoteProcessClient.DOUBLE_FORMAT_STRING, value))

    def read_bytes(self, byte_count):
        data = bytes()

        while len(data) < byte_count:
            chunk = self.socket.recv(byte_count - len(data))

            if not len(chunk):
                raise IOError("Can't read %s bytes from input stream." % str(byte_count))

            data += chunk

        return data

    def write_bytes(self, bytes):
        self.socket.sendall(bytes)

    class MessageType:
        UNKNOWN = 0
        GAME_OVER = 1
        AUTHENTICATION_TOKEN = 2
        TEAM_SIZE = 3
        TANK_TYPES = 4
        PLAYER_CONTEXT = 5
        MOVES = 6