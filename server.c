#include "networking.h"
int listen_socket;

struct Player players[MAX_PLAYERS];
struct Match matches[MAX_MATCHES];
int num_players = 0;
int num_matches = 0;

static void sighandler(int signo) {
  if ( signo == SIGINT ) {
    close(listen_socket);
    exit(0);
  }
}

void print_leaderboard() {
  printf("Leaderboard:\n");
  printf("Rank\tUsername\tWins\tLosses\n");
  //printing random players for now but need to sort (prob using merge sort or smth)
  //can make this prettier
  for(int i = 0; i < 10; i++) {
    printf("%d\t%s\t%d\t%d\n", i + 1, players[i].username, players[i].wins, players[i].losses);
  }
  printf("_______________________________\n");
}

void update_stats(char* username, int win) {

  for(int i = 0; i < num_players; i++) {
    if(strcmp(players[i].username, username) == 0) {
      if (win) {
        players[i].wins += 1;
      } else {
        players[i].losses += 1;
      }
      print_leaderboard();
      break;
    }
  }
}

int find_player(int fd) {
  for(int i = 0; i < num_players; i++) {
    if(players[i].fd == fd) {
      return i;
    }
  }
  return -1;
}

void remove_player(int fd) {
  
  int i = find_player(fd);
  if(i == -1) {
    return;
  }

  printf("%s has left.\n", players[i].username);

  //add checking if player is in a match, add handling to client

  for(int j = i; j < num_players - 1; j++) {
    players[j] = players[j + 1];
  }
  num_players--;
  print_leaderboard();
}

void add_player(char* username, int fd) {

  //check if username alr exists

  if (num_players >= MAX_PLAYERS) {
    printf("Max players reached, cannot add more players.\n");
    return;
  }
  else {
    struct Player new_player;
    strncpy(new_player.username, username, BUFFER_SIZE);
    new_player.wins = 0;
    new_player.losses = 0;
    new_player.ranking = num_players + 1;
    new_player.searching = 1;
    new_player.fd = fd;

    num_players++;
    players[num_players - 1] = new_player;
    printf("%s has joined.\n", username);
    print_leaderboard();
  }
}

  



void subserver_logic(int client_socket){
  char username[BUFFER_SIZE];
  char buffer[BUFFER_SIZE];

  int k = recv(client_socket, username, sizeof(username) - 1, 0);
  if(k <= 0) {
    close(client_socket);
    return;
  }

  username[k - 1] = '\0';
  //printf("%s has joined.\n", username);
  
  add_player(username, client_socket);
  send(client_socket, "POOL_WAIT\n", 10, 0);

  //check for matches

  while(1) {  
    int k = recv(client_socket, buffer, sizeof(buffer)-1, 0); //recv or read??
    if(k < 0) {
        close(client_socket);
        err(k, "issue receiving");
    }
    if(k == 0) {
      //printf("%s has left.\n", username);
      remove_player(client_socket);
      close(client_socket);
      exit(0);
      break;
    }
    buffer[k] = '\0';

    int n = send(client_socket, buffer, strlen(buffer), 0); //send or write??
    err(n, "issue sending in subserver");
  }
  close(client_socket);
}

int main(int argc, char *argv[] ) {
  signal(SIGINT, sighandler);
  listen_socket = server_setup();

  while(1) {
    int client_socket = server_tcp_handshake(listen_socket);
    if (client_socket < 0) continue;
    int f = fork();

    if(f == 0) {
      close(listen_socket);
      subserver_logic(client_socket);
      exit(0);
    }
    //always listen for disconnecting clients?
    else {
      // parent
      close(client_socket);
    }
  }
  return 0;
}
