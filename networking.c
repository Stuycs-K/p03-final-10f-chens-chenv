#include "networking.h"

/*Create and bind a socket.
* Place the socket in a listening state.
* returns the socket descriptor
*/
int server_setup() {
  //setup structs for getaddrinfo
  int status;
struct addrinfo hints;
struct addrinfo *results;  // will point to the results

memset(&hints, 0, sizeof hints); // make sure the struct is empty
hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

if ((status = getaddrinfo(NULL, PORT, &hints, &results)) != 0) {
    fprintf(stderr, "gai error: %s\n", gai_strerror(status));
    exit(1);
}


  //create the socket
  int clientd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if (clientd < 0) {
        err(-1, "error with creating socket\n");
    }
//connect(clientd, results->ai_addr, results->ai_addrlen);
  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");

  //bind the socket to address and port
  int b = bind(clientd, results->ai_addr, results->ai_addrlen);
  err(b,"bind error");
  //printf("bind complete\n");

  //set socket to listen state
  int l = listen(clientd, 10);
  err(l,"listen error");
  printf("Welcome to Tic-Tac-Toe: Multiplayer Ranked!\n");


  //free the structs used by getaddrinfo
  freeaddrinfo(results);

  return clientd;
}

/*Accept a connection from a client
 *return the socket descriptor for the new socket connected to the client
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
  int client_socket;

  //accept() the client connection
  client_socket = accept(listen_socket, NULL, NULL);
  err(client_socket, "server_tc-_handshake: issue with accepting");

  return client_socket;
}


/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {

  //getaddrinfo
  //setup structs for getaddrinfo
  int status;
  struct addrinfo hints;
  struct addrinfo *results;  // will point to the results

  memset(&hints, 0, sizeof hints); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

  if ((status = getaddrinfo(server_address, PORT, &hints, &results)) != 0) {
    fprintf(stderr, "gai error: %s\n", gai_strerror(status));
    exit(1);
  }


  //create the socket


  int serverd;//store the socket descriptor here
  //create the socket
  serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if (serverd < 0) {
    err(-1, "error with creating socket\n");
  }

  //connect() to the server
  if (connect(serverd, results->ai_addr, results->ai_addrlen) < 0) {
    err(-1, "error connecting");
  }

  // free(hints);
  freeaddrinfo(results);

  return serverd;
}




void err(int i, char*message){
  if(i < 0){
	  printf("Error: %s - %s\n",message, strerror(errno));
  	exit(1);
  }
}
