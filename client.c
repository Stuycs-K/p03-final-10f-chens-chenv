#include "networking.h"
  char buffer[256]; //username
  char board[10]; //space, X, or O.
  char pieceType; //X, or O,
  int server_socket;

/*

$ Enter username:
Xxxxxx
Thank you. Entering pool…
Entered pool. Waiting for a match… 
Entering match with abcde…

*/

/*
Opponent has placed their ejffjbge. Where do you want to place your ewhbwbgb? [enter number from 1 to 9]
2
Invalid! Where do you want to place your ewhbwbgb?
3
*/
static void sighandler(int signo) {
    if ( signo == SIGINT ) {
      //send smth to server so it knows this clinet has disconnecrted.
      //close(listen_socket);
      exit(0);
    }
  }

  void printtheboard() {
  printf("  %c  |  %c  |  %c  \n", board[1], board[2], board[3]); //complete
    printf("=====*=====*=====\n");
      printf("  %c  |  %c  |  %c  \n",board[4],board[5],board[6]); //complete
printf("=====*=====*=====\n");
        printf("  %c  |  %c  |  %c  \n",board[7],board[8], board[9]); //complete

}

void prompt() {
  char whichSpot[256];
  int spot;
  while(1) {//keeps prompting until right
            printf("Where do you want to place your %c, %s?\n", pieceType, buffer);
            printf("[enter number from 1-9, numbered clockwise starting from top left]\n");
        if(!fgets(whichSpot,255,stdin)) {
          continue;
        }
        int k = sscanf(whichSpot,"%d",&spot);
        if(k != 1) {
          printf("Invalid! Please enter a number.\n");
          continue;
        }
        if(spot < 1 || spot > 9) {
          printf("Invalid! number should be from 1-9");
          continue;
        //   printf("Where do you want to place your %s, %s?\n", pieceType, buffer);
        //   printf("[enter number from 1-9, numbered clockwise starting from top left]\n");
        // 
        if (board[spot] != ' ') {
      printf("Invalid! Spot already taken.\n");
      continue;
    }

        break; //worked
        }//also check other cases, eg taken, and adjust for wrong int second time
  }
  board[spot] = pieceType;
  char msg[64];
  sprintf(msg, "MOVE %d\n", spot);
  send(server_socket, msg, strlen(msg), 0);
}

  void readinput(char *msg) {
  if(!strncmp(msg, "THANKS", 6)) {
    printf("Thank you. Entering pool...\n");
  }
  else if(!strncmp(msg, "POOL_WAIT", 9)) {
    printf("Entered pool. Waiting for a match...\n");
  }
  else if(!strncmp(msg, "MATCH", 5)) {
    char opponent[64], piece;
    sscanf(msg, "MATCH %s %c", opponent, &piece);
    printf("Entering match with %s...\n", opponent);
    for (int i = 1; i <= 9; i++) {
      board[i] = ' ';
    }
  }
  else if(!strncmp(msg, "YOUR_TURN", 9)) {
    printtheboard();
    prompt();
  }
  else if(!strncmp(msg, "INVALID", 7)) {
    printf("Invalid! Try again.\n");
  }
  else if(!strncmp(msg, "WIN", 3)) {
    printf("You win!\nSending back to pool...\n");
  }
  else {
    printf("You lost, sending back to pool...");
  }
}

void clientLogic(int server_socket){
  printf("Please enter a username: ");
  fflush(stdout);
    if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
      printf("no input given, ending program\n");
      exit(0);
      return;
  }
  send(server_socket, buffer, strlen(buffer), 0);
  while(1) {
  int k = recv(server_socket, buffer, sizeof(buffer)-1, 0);
  if(k <= 0) {
    exit(0);
  }
  buffer[k] = '\0';
  readinput(buffer);

}
}

int main(int argc, char *argv[] ) {
  signal(SIGINT, sighandler);
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }

  server_socket = client_tcp_handshake(IP);
  printf("Welcome to TTT!\n");

  clientLogic(server_socket);
  close(server_socket);
  return 0;
}
