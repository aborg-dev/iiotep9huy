#include "RemoteProcessClient.h"

#include <algorithm>

using namespace model;
using namespace std;

const bool LITTLE_ENDIAN_BYTE_ORDER = true;
const int INTEGER_SIZE_BYTES = sizeof(int);
const int LONG_SIZE_BYTES = sizeof(long long);

RemoteProcessClient::RemoteProcessClient(std::string host, int port) {
	socket.Initialize();

	if (!socket.Open((uint8*) host.c_str(), port)) {
		exit(10001);
	}
}

void RemoteProcessClient::WriteToken(std::string token) {
	WriteEnum(AUTHENTICATION_TOKEN);
    WriteString(token);
}

int RemoteProcessClient::ReadTeamSize() {
    EnsureMessageType((MessageType) ReadEnum(), TEAM_SIZE);
    return ReadInt();
}

void RemoteProcessClient::WriteSelectedTanks(vector<TankType> tank_types) {
    WriteEnum(TANK_TYPES);

    int type_count = tank_types.size();
    WriteInt(type_count);

    for (int type_index = 0; type_index < type_count; ++type_index) {
        WriteEnum(tank_types[type_index]);
    }
}

PlayerContext* RemoteProcessClient::ReadPlayerContext() {
    MessageType message_type = (MessageType) ReadEnum();
    if (message_type == GAME_OVER) {
        return NULL;
    }

    EnsureMessageType(message_type, PLAYER_CONTEXT);

	if (!ReadBoolean()) {
		return NULL;
	}

	vector<Tank> tanks = ReadTanks();
	World world = ReadWorld();

	return new PlayerContext(tanks, world);
}

void RemoteProcessClient::WriteMoves(vector<Move> moves) {
    WriteEnum(MOVES);

    int move_count = moves.size();
    WriteInt(move_count);

    for (int move_index = 0; move_index < move_count; ++move_index) {
        Move move = moves[move_index];

		WriteBoolean(true);

		WriteDouble(move.left_track_power());
        WriteDouble(move.right_track_power());
        WriteDouble(move.turret_turn());
        WriteEnum(move.fire_type());
    }
}

void RemoteProcessClient::Close() {
	socket.Close();
}

World RemoteProcessClient::ReadWorld() {
    if (!ReadBoolean()) {
        exit(10002);
    }

	int tick = ReadInt();
	double width = ReadDouble();
	double height = ReadDouble();
	vector<Player> players = ReadPlayers();
	vector<Obstacle> obstacles = ReadObstacles();
	vector<Tank> tanks = ReadTanks();
	vector<Shell> shells = ReadShells();
	vector<Bonus> bonuses = ReadBonuses();

    return World(tick, width, height, players, obstacles, tanks, shells, bonuses);
}

vector<Player> RemoteProcessClient::ReadPlayers() {
    int player_count = ReadInt();
    if (player_count < 0) {
        exit(10003);
    }

    vector<Player> players;
	players.reserve(player_count);

    for (int player_index = 0; player_index < player_count; ++player_index) {
        if (ReadBoolean()) {
			string name = ReadString();
			int score = ReadInt();
			bool strategy_crashed = ReadBoolean();

            players.push_back(Player(name, score, strategy_crashed));
        } else {
			exit(10004);
		}
    }

    return players;
}

vector<Obstacle> RemoteProcessClient::ReadObstacles() {
    int obstacle_count = ReadInt();
    if (obstacle_count < 0) {
        exit(10015);
    }

    vector<Obstacle> obstacles;
	obstacles.reserve(obstacle_count);

    for (int obstacle_index = 0; obstacle_index < obstacle_count; ++obstacle_index) {
        if (ReadBoolean()) {
			long long id = ReadLong();
			double width = ReadDouble();
			double height = ReadDouble();
			double x = ReadDouble();
			double y = ReadDouble();

			obstacles.push_back(Obstacle(id, width, height, x, y));
        } else {
			exit(10016);
		}
    }

    return obstacles;
}

vector<Tank> RemoteProcessClient::ReadTanks() {
    int tank_count = ReadInt();
    if (tank_count < 0) {
        exit(10005);
    }

    vector<Tank> tanks;
	tanks.reserve(tank_count);

    for (int tank_index = 0; tank_index < tank_count; ++tank_index) {
        if (ReadBoolean()) {
			long long id = ReadLong();
			string player_name = ReadString();
			int teammate_index = ReadInt();
			double x = ReadDouble();
			double y = ReadDouble();
			double speed_x = ReadDouble();
			double speed_y = ReadDouble();
			double angle = ReadDouble();
			double angular_speed = ReadDouble();
			double turret_relative_angle = ReadDouble();
			int crew_health = ReadInt();
			int hull_durability = ReadInt();
			int reloading_time = ReadInt();
			int remaining_reloading_time = ReadInt();
			int premium_shell_count = ReadInt();
			bool teammate = ReadBoolean();
			TankType type = (TankType) ReadEnum();

            tanks.push_back(Tank(
				id, player_name, teammate_index, x, y,
				speed_x, speed_y, angle, angular_speed, turret_relative_angle,
				crew_health, hull_durability, reloading_time,
				remaining_reloading_time, premium_shell_count, teammate, type));
        } else {
			exit(10006);
		}
    }

    return tanks;
}

vector<Shell> RemoteProcessClient::ReadShells() {
    int shell_count = ReadInt();
    if (shell_count < 0) {
        exit(10007);
    }

    vector<Shell> shells;
	shells.reserve(shell_count);

    for (int shell_index = 0; shell_index < shell_count; ++shell_index) {
        if (ReadBoolean()) {
			long long id = ReadLong();
			string player_name = ReadString();
			double width = ReadDouble();
			double height = ReadDouble();
			double x = ReadDouble();
			double y = ReadDouble();
			double speed_x = ReadDouble();
			double speed_y = ReadDouble();
			double angle = ReadDouble();
			double angular_speed = ReadDouble();
			ShellType type = (ShellType) ReadEnum();

            shells.push_back(Shell(id, player_name, width, height, x, y, speed_x, speed_y, angle, angular_speed, type));
        } else {
			exit(10008);
		}
    }

    return shells;
}

vector<Bonus> RemoteProcessClient::ReadBonuses() {
    int bonus_count = ReadInt();
    if (bonus_count < 0) {
        exit(10009);
    }

    vector<Bonus> bonuses;
	bonuses.reserve(bonus_count);

    for (int bonus_index = 0; bonus_index < bonus_count; ++bonus_index) {
        if (ReadBoolean()) {
			long long id = ReadLong();
			double width = ReadDouble();
			double height = ReadDouble();
			double x = ReadDouble();
			double y = ReadDouble();
			BonusType type = (BonusType) ReadEnum();

			bonuses.push_back(Bonus(id, width, height, x, y, type));
        } else {
			exit(10010);
		}
    }

    return bonuses;
}

void RemoteProcessClient::EnsureMessageType(MessageType actual_type, MessageType expected_type) {
    if (actual_type != expected_type) {
        exit(10011);
    }
}

signed char RemoteProcessClient::ReadEnum() {
	return this->ReadBytes(1)[0];
}

void RemoteProcessClient::WriteEnum(signed char value) {
	vector<signed char> bytes;
	bytes.push_back(value);
	this->WriteBytes(bytes);
}

string RemoteProcessClient::ReadString() {
	int length = this->ReadInt();
	if (length == -1) {
		exit(10014);
	}

	vector<signed char> bytes = this->ReadBytes(length);
	return string((char*) (&bytes[0]), length);
}

void RemoteProcessClient::WriteString(string value) {
    vector<signed char> bytes(value.size());
	
	memcpy(&bytes[0], value.c_str(), value.size());

	this->WriteInt(bytes.size());
	this->WriteBytes(bytes);
}

bool RemoteProcessClient::ReadBoolean() {
	return this->ReadBytes(1)[0] == 1;
}

void RemoteProcessClient::WriteBoolean(bool value) {
	vector<signed char> bytes;
	bytes.push_back((signed char) (value ? 1 : 0));
	this->WriteBytes(bytes);
}

int RemoteProcessClient::ReadInt() {
	vector<signed char> bytes = this->ReadBytes(INTEGER_SIZE_BYTES);

	if (this->IsLittleEndianMachine() != LITTLE_ENDIAN_BYTE_ORDER) {
		reverse(&bytes[0], &bytes[INTEGER_SIZE_BYTES - 1]);
	}

	int value;

	memcpy(&value, &bytes[0], INTEGER_SIZE_BYTES);

	return value;
}

void RemoteProcessClient::WriteInt(int value) {
    vector<signed char> bytes(INTEGER_SIZE_BYTES);

	memcpy(&bytes[0], &value, INTEGER_SIZE_BYTES);

	if (this->IsLittleEndianMachine() != LITTLE_ENDIAN_BYTE_ORDER) {
		reverse(&bytes[0], &bytes[INTEGER_SIZE_BYTES - 1]);
	}

	this->WriteBytes(bytes);
}

long long RemoteProcessClient::ReadLong() {
	vector<signed char> bytes = this->ReadBytes(LONG_SIZE_BYTES);

	if (this->IsLittleEndianMachine() != LITTLE_ENDIAN_BYTE_ORDER) {
		reverse(&bytes[0], &bytes[LONG_SIZE_BYTES - 1]);
	}

	long long value;

	memcpy(&value, &bytes[0], LONG_SIZE_BYTES);

	return value;
}

void RemoteProcessClient::WriteLong(long long value) {
	vector<signed char> bytes(LONG_SIZE_BYTES);

	memcpy(&bytes[0], &value, LONG_SIZE_BYTES);

	if (this->IsLittleEndianMachine() != LITTLE_ENDIAN_BYTE_ORDER) {
		reverse(&bytes[0], &bytes[LONG_SIZE_BYTES - 1]);
	}

	this->WriteBytes(bytes);
}

double RemoteProcessClient::ReadDouble() {
	long long value = this->ReadLong();
	return *((double*) &value);
}

void RemoteProcessClient::WriteDouble(double value) {
	this->WriteLong(*((long long*) &value));
}

vector<signed char> RemoteProcessClient::ReadBytes(unsigned int byte_count) {
	vector<signed char> bytes(byte_count);
	unsigned int offset = 0;
    int received_byte_count;

	while (offset < byte_count && (received_byte_count = socket.Receive(byte_count - offset)) > 0) {
		memcpy(&bytes[offset], socket.GetData(), received_byte_count);
        offset += received_byte_count;
    }

    if (offset != byte_count) {
        exit(10012);
    }

    return bytes;
}

void RemoteProcessClient::WriteBytes(vector<signed char> bytes) {
	vector<signed char>::size_type byte_count = bytes.size();
	unsigned int offset = 0;
    int sent_byte_count;

	while (offset < byte_count && (sent_byte_count = socket.Send((uint8*) &bytes[offset], byte_count - offset)) > 0) {
        offset += sent_byte_count;
    }

    if (offset != byte_count) {
        exit(10013);
    }
}

bool RemoteProcessClient::IsLittleEndianMachine() {
    union {
        uint16 value;
        unsigned char bytes[2];
    } test = {0x0201};

    return test.bytes[0] == 1; 
}
