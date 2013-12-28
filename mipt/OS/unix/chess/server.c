/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  Chess server implementation 
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */

#define _POSIX_SOURCE
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <stdarg.h> 
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#include "parallel_entity.h"
#include "./libs/string.h"
#include "./libs/list.h"

#define REPORT(message) fprintf(stderr, "%d: %s\n", getpid(), message);
#define REPORT2(s1, s2) fprintf(stderr, "%d: %s %s\n", getpid(), s1, s2);
#define DOMAIN AF_INET
#define SOCKET_TYPE SOCK_STREAM
#define SIN_FAMILY AF_INET
#define SIN_PORT 5555
#define SIN_ADDR INADDR_ANY

#define MAX_GAMELEVEL 11

void server_log(const char* fmt, ...)
{
  char* tmp_str;
  va_list ap;

  va_start(ap, fmt);
  vasprintf(&tmp_str, fmt, ap);
  va_end(ap);

  FILE* log_file = fopen("chesslog.txt", "a");
  fprintf(log_file, "%s\n", tmp_str);
  fclose(log_file);
  free(tmp_str);
}

struct player_info
{
	string name;
	int game_level;
};

struct game_info
{
  size_t moves_number;
  list* first_player_moves;
  list* second_player_moves;
};

struct game
{
  string game_name;
  struct client* first_client;
  struct client* second_client;
  struct game_info game_info;
};

void add_game(list* games_list, struct game* new_game)
{
  list_push_back(games_list, (void*) new_game);
}

struct game* get_game(list_node* node)
{
  return (struct game*) node->data;
}

struct client
{
	int socket;
  time_t connection_time;
	struct player_info player;
};

void add_client(list* client_list, struct client* new_client, pthread_mutex_t* client_list_is_used)
{
  pthread_mutex_lock(client_list_is_used);
  list_push_back(client_list, (void*) new_client);
  pthread_mutex_unlock(client_list_is_used);
}

struct client* get_client(list_node* node)
{
  return (struct client*) node->data;
}

struct connection_handler_params
{
  int socket;
  list* client_list;
  pthread_mutex_t* client_list_is_used;
};

struct server_info
{
  list client_list;
  list games_list;
  pthread_mutex_t client_list_is_used;
  int listener;
  time_t start_time;
};

void* handle_connection_request(void* params)
{
  server_log("Client is trying to connect");
  struct connection_handler_params* connection_params = (struct connection_handler_params*) params;
  struct client new_client;
  new_client.socket = connection_params->socket;
  new_client.connection_time = clock();

  init_string(&new_client.player.name);
  recv_to_string(new_client.socket, &new_client.player.name);
  server_log("Client name: %s", new_client.player.name.content);

  string game_level;
  init_string(&game_level);
  recv_to_string(new_client.socket, &game_level);
  new_client.player.game_level = string_to_int(&game_level);
  server_log("Client game_level: %d", new_client.player.game_level);

  server_log("Client %s with game level %d connected\n", new_client.player.name.content, 
                                                         new_client.player.game_level);

  add_client(connection_params->client_list, &new_client, connection_params->client_list_is_used);
  return 0;
}

void* handle_game_request(void* params)
{
  struct game* game_params = (struct game*) params;
  struct client* first_client = game_params->first_client;
  struct client* second_client = game_params->second_client;

  server_log("Game %s between %s and %s started\n", game_params->game_name, 
                                                    first_client->player.name.content,
                                                    second_client->player.name.content);
  return 0;
}

int prepare_server_listener()
{
  int listener;
  struct sockaddr_in addr;

  listener = socket(DOMAIN, SOCKET_TYPE, 0);
  if (listener < 0)
  {
    perror("Can't create listener");
    exit(1);
  }
  
  addr.sin_family = SIN_FAMILY;
  addr.sin_port = htons(SIN_PORT);
  addr.sin_addr.s_addr = htonl(SIN_ADDR);
  if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    perror("Can't bind listener");
    exit(2);
  }

  listen(listener, 1);
  return listener;
}

struct server_info* create_server_info()
{
  struct server_info* info = (struct server_info*) malloc(sizeof(struct server_info));
  list_init(&info->client_list);
  list_init(&info->games_list);
  info->listener= prepare_server_listener();
  info->start_time = clock();
  return info;
}

void free_server_info(struct server_info* info)
{
  list_free(&info->client_list);
  list_free(&info->games_list);
  free(info);
}

void start_new_connection(int socket, struct server_info* server_info)
{
  struct connection_handler_params* params
    = (struct connection_handler_params*) malloc(sizeof(struct connection_handler_params));
  params->socket = socket;
  params->client_list = &server_info->client_list;
  pthread_t thread;
  pthread_create(&thread, NULL, &handle_connection_request, (void*) params);
}

struct game* create_new_game(string game_name,
                            struct client* first_client, 
                            struct client* second_client)
{
  struct game* new_game
    = (struct game*) malloc(sizeof(struct game*));
  new_game->game_name = game_name;
  new_game->first_client = first_client;
  new_game->second_client = second_client;
  return new_game;
}

void start_new_game(string game_name,
                    struct client* first_client, 
                    struct client* second_client,
                    list* games_list)
{
  struct game* game_params = create_new_game(game_name, first_client, second_client);
  add_game(games_list, game_params);
  pthread_t thread;
  pthread_create(&thread, NULL, &handle_game_request, (void*) game_params);
}

struct game_manager_params
{
  list* client_list;
  list* games_list;
  pthread_mutex_t* client_list_is_used;
};

string get_game_name(int game_number)
{
  string game_name;
  init_string(&game_name);
  print_to_string(&game_name, "Game %d", game_number);
  return game_name;
}

void* game_manager(void* params)
{
  server_log("Game manager started");
  struct game_manager_params* manager_params = (struct game_manager_params*) params;
  list* client_list = manager_params->client_list;
  list* games_list = manager_params->games_list;
  pthread_mutex_t* client_list_is_used = manager_params->client_list_is_used;

  while (true)
  {
    server_log("Trying to find matching pairs");
    list players[MAX_GAMELEVEL];
    for(int game_level = 1; game_level < MAX_GAMELEVEL; game_level++)
    {
      list_init(&players[game_level]);
    }
    for(list_node* it = list_begin(client_list); it != list_end(client_list); it = it->next_node)
    {
      struct client* current_client = get_client(it);
      int player_game_level = current_client->player.game_level;
      list_push_back(&players[player_game_level], (void*) it);
    }
    for(int game_level = 1; game_level < MAX_GAMELEVEL; game_level++)
    {
      if (players[game_level].size > 1)
      {
        pthread_mutex_lock(client_list_is_used);
        list_node* current_node = list_begin(&players[game_level]);
        while (current_node->next_node != list_end(&players[game_level]))
        {
          struct client* first_client = get_client(current_node);
          struct client* second_client = get_client(current_node->next_node);
          start_new_game(get_game_name(games_list->size), first_client, second_client, games_list);
          list_erase(client_list, current_node->next_node);
          list_erase(client_list, current_node);
          current_node = list_begin(&players[game_level]);
        }
        pthread_mutex_unlock(client_list_is_used);
      }
    }
    sleep(1);
  }
}

void start_game_manager(struct server_info* server_info)
{ 
  struct game_manager_params* manager_params 
    = (struct game_manager_params*) malloc(sizeof(struct game_manager_params));
  manager_params->client_list = &server_info->client_list;
  manager_params->games_list = &server_info->games_list;
  manager_params->client_list_is_used = &server_info->client_list_is_used;
  pthread_t thread;
  pthread_create(&thread, NULL, &game_manager, manager_params);
}

void run_server()
{
  unlink("chesslog.txt");
  server_log("Chess server started");
  struct server_info* server_info = create_server_info();
  start_game_manager(server_info);
  
  while(true)
  {
    int socket = accept(server_info->listener, NULL, NULL);
    server_log("Socket accepted %d\n", socket);
    if(socket < 0)
    {
      perror("Can't accept connection");
      exit(3);
    }

    start_new_connection(socket, server_info);
    //close(socket);
  }

  free_server_info(server_info);
  server_log("Chess server finished");
}

int main(int argc, char** argv)
{
	run_server();
	return 0;
}
