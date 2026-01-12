#include "networking.h"
  char buffer[256]; 
  char username[256]; 
  char board[10]; //space, X, or O.
  char pieceType; //X, or O,
  int server_socket;

static void sighandler(int signo) {
  if ( signo == SIGINT ) {
    exit(0);
  }
}

void printtheboard() {
  printf("  %c  |  %c  |  %c  \n", board[1], board[2], board[3]); 
  printf("=====*=====*=====\n");
  printf("  %c  |  %c  |  %c  \n",board[4],board[5],board[6]); 
  printf("=====*=====*=====\n");
  printf("  %c  |  %c  |  %c  \n",board[7],board[8], board[9]);
}

void prompt() {
  char whichSpot[256];
  int spot;
  while(1) {
    printf("Where do you want to place your %c, %s?\n", pieceType, username);
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
    if (board[spot] != ' ') {
      printf("Invalid! Spot already taken.\n");
      continue;
    }

    break;
    }
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
    pieceType = piece;
    printf("Entering match with %s...\nYou are %c\n", opponent, pieceType);
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
    printtheboard();
    prompt();
  }
  else if(!strncmp(msg, "WIN", 3)) {
    printf("You win!\nSending back to pool...\n");
    printtheboard();
  }
  else {
    printf("You lost, sending back to pool...");
    printtheboard();
  }
  // add case for draw and opponent leaving
}

void clientLogic(int server_socket){
  printf("Please enter a username: ");
  fflush(stdout);
  if(fgets(username, sizeof(username), stdin) == NULL) {
    printf("no input given, ending program\n");
    exit(0);
    return;
  }
  username[strlen(username)] = '\0';
  send(server_socket, username, strlen(username), 0);
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
