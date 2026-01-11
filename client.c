#include "networking.h"
  char buffer[256];

static void sighandler(int signo) {
    if ( signo == SIGINT ) {
      //send smth to server so it knows this clinet has disconnecrted.
      //close(listen_socket);
      exit(0);
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
  /*
  Send the user input to the client. ??? not server??
  */
int len, bytes_sent;
len = strlen(buffer);
bytes_sent = send(server_socket, buffer, len, 0);
if(bytes_sent < 0) {
  err(-1, "error sending"); //check err
}
else {
  bytes_sent =  recv(server_socket, buffer, sizeof(buffer)-1, 0);
}
if(bytes_sent < 0) {
  err(-1, "error receiving"); //check err
}
if(bytes_sent == 0) {
  exit(0); //scok
}
buffer[bytes_sent] = '\0';
  /*
Read the modified string from the server
*/


/*prints the modified string
*/
printf("received: %s", buffer);

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

  for(int i = 0; i < 0; i++) {//fix printing grid, get correct Xs and Os
    printf("  %s  |  %s  |  %s  \n", "-1", "-1", "-1"); //complete
    printf("=====*=====*=====\n");
      printf("  %s  |  %s  |  %s  \n","-1", "-1", "-1"); //complete
printf("=====*=====*=====\n");
        printf("  %s  |  %s  |  %s  \n","-1", "-1", "-1"); //complete
        printf("Where do you want to place your %s, %s?\n [enter number from 1-9, numbered clockwise starting from top left]\n", pieceType, buffer);
        fgets(whichSpot,255,stdin);
        sscanf(whichSpot,"%d",&spot);
        if(spot < 0 || spot > 9) {
          printf("Invalid! ");
          printf("Where do you want to place your %s, %s?\n [enter number from 1-9, numbered clockwise starting from top left]\n", pieceType, buffer);
        }//also check other cases, eg taken, and adjust for wrong int second time
  }
  int server_socket = client_tcp_handshake(IP);
  printf("Welcome to TTT!\n");

  clientLogic(server_socket);
  close(server_socket);
  return 0;
}
