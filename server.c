#include "networking.h"
int listen_socket;
char * rot13(char*s) {
  int i = 0;
  while(s[i]) {
    unsigned char c = s[i]; //overflowed previously bc z too close to 127
      if (c>=65 && c<=90) {
          c = ((c-65+13)%26) + 'A';
      }
      else if (c>=97 && c<=122) {
          c = ((c-97+13)%26) + 'a';
      }
      s[i] = c;
      i++;
  }
  return s;
}
static void sighandler(int signo) {
    if ( signo == SIGINT ) {
      close(listen_socket);
      exit(0);
    }
  }
void subserver_logic(int client_socket){
  char buffer[BUFFER_SIZE];

while(1) {
    int k = recv(client_socket, buffer, sizeof(buffer)-1, 0); //recv or read??
    if(k < 0) {
        close(client_socket);
        err(k, "issue receiving");
    }
    if(k==0) {
      close(client_socket);
      exit(0);
      break;
    }
    else { //
      buffer[k] = '\0';
      rot13(buffer);
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

  else {
    // parent
    close(client_socket);
  }

}}
