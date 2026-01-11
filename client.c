#include "networking.h"
  char buffer[256];

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
  printf("  %s  |  %s  |  %s  \n", board[0], board[1], board[2]); //complete
    printf("=====*=====*=====\n");
      printf("  %s  |  %s  |  %s  \n",board[3],board[4],board[5]); //complete
printf("=====*=====*=====\n");
        printf("  %s  |  %s  |  %s  \n",board[6],board[7],board[8]); //complete

}

void prompt() {
  
  for(int i = 0; i < 0; i++) {//fix printing grid, get correct Xs and Os
            printf("Where do you want to place your %s, %s?\n [enter number from 1-9, numbered clockwise starting from top left]\n", pieceType, buffer);
        fgets(whichSpot,255,stdin);
        sscanf(whichSpot,"%d",&spot);
        if(spot < 0 || spot > 9) {
          printf("Invalid! ");
          printf("Where do you want to place your %s, %s?\n [enter number from 1-9, numbered clockwise starting from top left]\n", pieceType, buffer);
        }//also check other cases, eg taken, and adjust for wrong int second time
  }
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
    printf("You lost, sending back to pool...")
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
  char pieceType[] = "Y"; //figure out how to randomize
  char whichSpot[256] = "-1";
  int spot = 0;

  int server_socket = client_tcp_handshake(IP);
  printf("Welcome to TTT!\n");

  clientLogic(server_socket);
  close(server_socket);
  return 0;
}
