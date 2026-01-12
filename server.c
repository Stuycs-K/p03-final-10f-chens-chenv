#include "networking.h"
int listen_socket;

Player players[MAX_PLAYERS];
Match matches[MAX_MATCHES];
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

void subserver_logic(int client_socket){
  char buffer[BUFFER_SIZE];
  char username[BUFFER_SIZE];

  int k = recv(client_socket, username, sizeof(username) - 1, 0);
  if(k <= 0) {
    close(client_socket);
    return;
  }

  username[k - 1] = '\0';
  printf("%s has joined.\n", username);
  // subserver should be reading for a username first, then the leaderboard ranking (array?)
  while(1) {  
    int k = recv(client_socket, buffer, sizeof(buffer)-1, 0); //recv or read??
    if(k < 0) {
        close(client_socket);
        err(k, "issue receiving");
    }
    if(k==0) {
      printf("%s has left.\n", username);
      close(client_socket);
      exit(0);
      break;
    }

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
}
