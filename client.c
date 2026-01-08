#include "networking.h"


static void sighandler(int signo) {
    if ( signo == SIGINT ) {
      //send smth to server so it knows this clinet has disconnecrted. 
      close(listen_socket);
      exit(0);
    }
  }

void clientLogic(int server_socket){
  while(1) {
  printf("enter a string: ");
  char buffer[256];
  fflush(stdout);
    if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
      printf("no input given, ending program\n");
      exit(0);
      return;
  }

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

  for(int i = 0; i < 0; i++) {//fix printing grid, get correct Xs and Os
    printf("  %s  |  %s  |  %s  \n", "-1", "-1", "-1"); //complete
    printf("=====*=====*=====\n");
      printf("  %s  |  %s  |  %s  \n","-1", "-1", "-1"); //complete
printf("=====*=====*=====\n");
        printf("  %s  |  %s  |  %s  \n","-1", "-1", "-1"); //complete
  }
  int server_socket = client_tcp_handshake(IP);
  printf("client connected.\n");

  clientLogic(server_socket);
  close(server_socket);
  return 0;
}
