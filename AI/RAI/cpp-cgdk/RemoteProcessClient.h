#pragma once

#ifndef _REMOTE_PROCESS_CLIENT_H_
#define _REMOTE_PROCESS_CLIENT_H_

#include <string>
#include <vector>

#include "csimplesocket/ActiveSocket.h"
#include "model/Bonus.h"
#include "model/Move.h"
#include "model/Obstacle.h"
#include "model/Player.h"
#include "model/PlayerContext.h"
#include "model/Shell.h"
#include "model/Tank.h"
#include "model/World.h"

enum MessageType {
    UNKNOWN_MESSAGE,
    GAME_OVER,
    AUTHENTICATION_TOKEN,
    TEAM_SIZE,
    TANK_TYPES,
    PLAYER_CONTEXT,
    MOVES
};

class RemoteProcessClient {
private:
	CActiveSocket socket;

	model::World ReadWorld();
	std::vector<model::Player> ReadPlayers();
	std::vector<model::Obstacle> ReadObstacles();
	std::vector<model::Tank> ReadTanks();
	std::vector<model::Shell> ReadShells();
	std::vector<model::Bonus> ReadBonuses();

    static void EnsureMessageType(MessageType actual_type, MessageType expected_type);

    signed char ReadEnum();
    void WriteEnum(signed char value);
	std::string ReadString();
	void WriteString(std::string value);
    bool ReadBoolean();
    void WriteBoolean(bool value);
    int ReadInt();
    void WriteInt(int value);
    long long ReadLong();
    void WriteLong(long long value);
    double ReadDouble();
    void WriteDouble(double value);
	std::vector<signed char> ReadBytes(unsigned int byte_count);
    void WriteBytes(std::vector<signed char> bytes);

	static bool IsLittleEndianMachine();
public:
	RemoteProcessClient(std::string host, int port);

	void WriteToken(std::string token);
	int ReadTeamSize();
	void WriteSelectedTanks(std::vector<model::TankType> tank_types);
    model::PlayerContext* ReadPlayerContext();
	void WriteMoves(std::vector<model::Move> moves);

	void Close();
};

#endif
