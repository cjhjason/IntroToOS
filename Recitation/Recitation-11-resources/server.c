/*Server.c --- Demonstrate the core workings of a simple server
 *              - compile/run on linux
 *              - note that server does not exit on its own, you 
 *                must kill it yourself.
 *              - Do NOT leave this running on itlabs machines
 *              - Note: some code borrowed from web, source unknown
 **********************************************************************/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>


#define DATA "Server says: Hello Client, Server here!"
#define TRUE 1
#define SERVER_PORT 6000
#define BUFFER_SIZE 1024


/* prototypes */
void die(const char *);
void pdie(const char *);


/**********************************************************************
 * main
 **********************************************************************/

int main(void) {

  int sock;                    /* fd for main socket */
  int msgsock;                 /* fd from accept return */
  struct sockaddr_in server;   /* socket struct for server connection */
  struct sockaddr_in client;   /* socket struct for client connection */
  int clientLen;               /* returned length of client from accept() */
  int rval;                    /* return value from read() */
  char buf[BUFFER_SIZE];       /* receive buffer */

  /* Open a socket */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    pdie("Opening stream socket");
  }

  /* Fill out inetaddr struct */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(SERVER_PORT);
  
  /* Bind */
  if (bind(sock, (struct sockaddr *) &server, sizeof(server))){
    pdie("Binding stream socket");
  }

  printf("Server: Socket has port %hu\n", ntohs(server.sin_port));

  /* Listen w/ max queue of 5 */
  listen(sock, 5);

  /* Loop, waiting for client connections. */
  /* This is an interactive server. */
  while (TRUE) {

    clientLen = sizeof(client);
    if ((msgsock = accept(sock, (struct sockaddr *) &client, &clientLen)) == -1) {
      pdie("Accept");
    } else {
     
      do {   
	/* Read from client until it's closed the connection. */

	if ((rval = recv(msgsock, buf, BUFFER_SIZE,0)) < 0){
	  pdie("Reading stream message");
	}

	if (rval == 0) {
	  /* Client has closed the connection */
	  fprintf(stderr, "Server: Ending connection\n");
	} else {
	  printf("Server: Rec'd msg: %s\n", buf);
	
	  /* Write back to client. */
	  if (send(msgsock, DATA, sizeof(DATA),0) < 0){
	    pdie("Writing on stream socket");
	  }
	}
      } while (rval != 0);
    }   /* else */

    close(msgsock);
  }

  exit(0);

}


/**********************************************************************
 * pdie --- Call perror() to figure out what's going on and die.
 **********************************************************************/

void pdie(const char *mesg) {

  perror(mesg);
  exit(1);
}


/**********************************************************************
 * die --- Print a message and die.
 **********************************************************************/

void die(const char *mesg) {

  fputs(mesg, stderr);
  fputc('\n', stderr);
  exit(1);
}
