#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>


#ifndef NETWORKING_H
#define NETWORKING_H
#define PORT "19230"
#define BUFFER_SIZE 1024
#define MAX_PLAYERS 100
#define MAX_MATCHES 50
void err(int i, char*message);
int server_setup();
int client_tcp_handshake(char*server_address);
int server_tcp_handshake(int listen_socket);

struct Player {
  char username[BUFFER_SIZE];
  int wins;
  int losses;
  int ranking;
  int searching; //0 if in match
  int fd;
};

struct Match {
  int id;
  struct Player player1;
  struct Player player2;
  char board[10];
  int turn;
  int end;
};

#endif
