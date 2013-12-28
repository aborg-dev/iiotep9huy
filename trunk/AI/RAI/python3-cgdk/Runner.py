import sys
from MyStrategy import MyStrategy
from RemoteProcessClient import RemoteProcessClient
from model.Move import Move

class Runner:
    def __init__(self):
        if sys.argv.__len__() == 4:
            self.remote_process_client = RemoteProcessClient(sys.argv[1], int(sys.argv[2]))
            self.token = sys.argv[3]
        else:
            self.remote_process_client = RemoteProcessClient("localhost", 31000)
            self.token = "0000000000000000"

    def run(self):
        try:
            self.remote_process_client.write_token(self.token)
            team_size = self.remote_process_client.read_team_size()

            strategies = []
            tank_types = []

            for strategy_index in range(team_size):
                strategy = MyStrategy()
                strategies.append(strategy)
                tank_types.append(strategy.select_tank(strategy_index, team_size))

            self.remote_process_client.write_selected_tanks(tank_types)

            while True:
                player_context = self.remote_process_client.read_player_context()
                if player_context is None:
                    break

                player_tanks = player_context.tanks
                if player_tanks.__len__() != team_size:
                    break

                moves = []

                for strategy_index in range(team_size):
                    move = Move()
                    moves.append(move)
                    strategies[strategy_index].move(player_tanks[strategy_index], player_context.world, move)

                self.remote_process_client.write_moves(moves)
        finally:
            self.remote_process_client.close()

Runner().run()