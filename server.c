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
  //maybe change to only rpitn exisiting players if less htan 10 plaers
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
      }
      else {
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
  if (num_players >= 0) {
    players[num_players] = (struct Player) {0};
  }
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

    players[num_players] = new_player;
    num_players++;
    printf("%s has joined.\n", username);
    print_leaderboard();
  }
}

void matchmake() {

  for(int i = 0; i < num_players; i++) {
    if(players[i].searching == 1) {
      for(int j = i + 1; j < num_players; j++) {
        if(players[j].searching == 1) {

          //maybe add pritn statemnet for match found, who vs who
          //handle matchcount
          players[i].searching = 0;
          players[j].searching = 0;

          struct Match new_match;
          new_match.id = num_matches + 1;
          new_match.player1 = players[i];
          new_match.player2 = players[j];
          memset(new_match.board, ' ', sizeof(new_match.board));
          new_match.turn = 1;
          new_match.end = 0;

          matches[num_matches] = new_match;
          num_matches++;

          //notify players
          char msg1[BUFFER_SIZE+9];
          char msg2[BUFFER_SIZE+9];
          snprintf(msg1, sizeof(msg1), "MATCH %s X\n", players[j].username);
          snprintf(msg2, sizeof(msg2), "MATCH %s O\n", players[i].username);

          send(players[i].fd, msg1, strlen(msg1), 0);
          send(players[j].fd, msg2, strlen(msg2), 0);


          send(players[i].fd, "YOUR_TURN\n", 10, 0);
          send(players[j].fd, "NOTTURN\n", 10, 0);



          return;
        }
      }
    }
  }
}

//game_move
// check if valid move done
// make move
// send board update to both players
// check for win/draw
// switch turn
// tell next player
/*
***
DONE, need to check though **
***
*/


int winnerdinner(char board[10], char piece) {
    int wins[8][3] = {
        {1,2,3},{4,5,6},{7,8,9},{1,4,7},{2,5,8},{3,6,9},{1,5,9},{3,5,7}
    }; //rows, then  cols diags.
    for(int i=0;i<8;i++){
        if(board[wins[i][0]]==piece && board[wins[i][1]]==piece && board[wins[i][2]]==piece)
            return 1;
    }
    return 0;
}

void game_move(int i, int spot) {

// find match player is in
/*
***
DONE
***
*/
  int matidx = -1;
  int playidx = find_player(i);
  if(playidx==-1) {
    return;
  }
  for(int i=0;i<num_matches;i++){
        if((matches[i].player1.fd == i || matches[i].player2.fd == i) && !matches[i].end){
            matidx = i;
            break;
        }
    }

    struct Match *m = &matches[matidx];
    char player_piece;
    if (m->player1.fd == i) {
        player_piece = 'X';
    }
    else {
        player_piece = 'O';
    }

    //check if its players turn
    /*
    ***
    DONE
    ***
    */

    if((m->turn == 1 && player_piece != 'X') || (m->turn == 2 && player_piece != 'O')) {
        send(i, "NOTTURN\n", 8, 0);
        return;
    }
    while(1) {
      char buff[256] = "-1";
      char buff2[256]="-1";
      int idx;
      if(m->turn == 1) {
        idx = 0;
      }
      else{
        idx = 1;
      }
      if(idx%2==0) {
        send(m->player1.fd, "YOUR_TURN\n", 10, 0);
        recv(m->player1.fd, buff, 7, 0);
        strncpy(buff2,buff,4);
        buff2[4]='\0';
        if(strcmp(buff, "MOVE")==0) {
            m->turn = 2;
        }
        for(int ir = 0; ir < 10; ir++) {
            m->player2.board[ir] = m->player1.board[ir];
        }
        if(winnerdinner(m->player2.board, player_piece)==1) {
          send(m->player2.fd, "LOSE\n", 4,0);
          send(m->player1.fd, "WIN\n", 4,0);
          break;
        }
        if(player_piece=='X') {
          player_piece='O';
        }
        else {
          player_piece = 'X';
        }
      }
      else {
        send(m->player2.fd, "YOUR_TURN\n", 10, 0);
        recv(m->player2.fd, buff, 7, 0);
        strncpy(buff2,buff,4);
        buff2[4]='\0';
        if(strcmp(buff, "MOVE")==0) {
            m->turn = 1;
        }
        for(int ir = 0; ir < 10; ir++) {
            m->player1.board[ir] = m->player2.board[ir];
        }
        if(winnerdinner(m->player1.board, player_piece)==1) {
          send(m->player2.fd, "WIN\n", 4,0);
          send(m->player1.fd, "LOSE\n", 4,0);
          break;
        }
        if(player_piece=='X') {
          player_piece='O';
        }
        else {
          player_piece = 'X';
        }
    }
  }
}


int main(int argc, char *argv[] ) {
  signal(SIGINT, sighandler);
  listen_socket = server_setup();

  fd_set read_fds1, read_fds2;
  FD_ZERO(&read_fds1);
  FD_SET(listen_socket, &read_fds1);
  int max = listen_socket;

  while(1) {

    read_fds2 = read_fds1;
    select(max + 1, &read_fds2, NULL, NULL, NULL);

    for (int i = 0; i <= max; i++) {
      if (FD_ISSET(i, &read_fds2)) {

        if (i == listen_socket) {
          int client_socket = server_tcp_handshake(listen_socket);
          if (client_socket < 0) continue;

          FD_SET(client_socket, &read_fds1);
          if (client_socket > max) {
            max = client_socket;
          }

          //printf("new client");
        }
        else {
          char buffer[BUFFER_SIZE];
          int k = recv(i, buffer, sizeof(buffer)-1, 0); //recv or read??

          if (k <= 0) {

            remove_player(i);
            close(i);
            FD_CLR(i, &read_fds1);
            matchmake();

          }
          else {

            buffer[k] = '\0';
            int index = find_player(i);

            if(index < 0) {
              buffer[strlen(buffer) - 1] = '\0';
              add_player(buffer, i);
              send(i, "POOL_WAIT\n", 10, 0);
              matchmake();
            }
            else {

              if (strncmp(buffer, "MOVE", 4) == 0) {
                int spot;
                if (sscanf(buffer + 5, "%d", &spot) == 1) {
                  game_move(i, spot);
                  matchmake();
                }
              }
            }
          }

        }
      }
    }
  }
  return 0;
}
