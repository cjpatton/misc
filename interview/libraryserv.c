/* Chris 
 * ~13 Jan 2013
 *
 * To prepare for my google interview, I want to write a small TCP/IP application
 * in C that makes use of stream sockets and UNIX system calls. 
 *
 * The client interacts with the server with some basic commands: 
 * ls          - list available files to download
 * more <file> - download text file and pipe it through less to read.  
 * 
 * The server should be able to handle multiple connections. Much of the point
 * of this program is to practice with receiving many packets. Let's go. 
 *
 *
 */ 

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> /* socket(), connect(), send(), recv() */ 
#include <arpa/inet.h>  /* struct sockaddr_in, inet_addr() */
#include <unistd.h>     /* close() */
#include <errno.h>      
#include <dirent.h>     /* for listing directories */
#include <fcntl.h>      /* file open(), read(), close() */

const char *command_help = 
"ibraryserv help.\n\
    -p N          port number (default 5000)\n\
    -a address    IP address (default localhost)\n\
    -d directory  directory with library\n";

const char *library_help = 
"Welcome to your virtual library! Here you can list the texts we have\n\
available and read them. Very fancy. Available commands are\n\
\thelp\tls\tread <file>\tquit\n";


char *ls(const char *directory) {
/* from 'http://www.gnu.org/software/libc/manual' 
 * List a directory and return a pointer to a string 
 * representation of its contents. */
  static char buffer [4096]; 

  if (directory == NULL)
    return buffer; 

  buffer[0] = '\0';

  DIR *dp;
  struct dirent *ep;

  dp = opendir (directory);
  if (dp != NULL)
  {
    while ((ep = readdir (dp))) {
      if (ep->d_name[0] != '.') {
        strcpy(&buffer[strlen(buffer)], ep->d_name); 
        strcpy(&buffer[strlen(buffer)], "\n"); 
      }
    }
    (void) closedir (dp);
  }
  else
    perror ("Couldn't open the directory");

  return buffer; 
} /* ls() */

int readfile(char *buffer, const char *name, const char *directory) {
/* Read text file 'directory/name' into buffer. Return -1
 * if the filte doesn't exist or open() returns an error. */
  static char name_buffer [1024];
  sprintf(name_buffer, "%s/%s", directory, name); 

  int bytes_read, fd = -1;
  char *pch = strtok(ls(directory), "\n"); 
  while (pch != NULL) {
    if (strcmp(pch, name) == 0) 
      fd = open(name_buffer, O_RDONLY, 0444);
    pch = strtok(NULL, "\n"); 
  }

  if (fd != -1) {
    strcpy(buffer, "GOOD;"); 
    bytes_read = read(fd, &buffer[strlen(buffer)], BUFFLENGTH - strlen(buffer) - 1);
    buffer[bytes_read] = '\0'; 
    close(fd); 
  }
  return fd; 
} /* readfile() */

void handle_client( int sockfd, struct sockaddr_in *client_addr, param_t *options ) {
/* Communicate with a single client. The socket associated with 
 * sockfd should be of the TCP variety. */ 
  int bytes_received; 
  char *pch, *send_data=NULL, *receive_data=NULL; 
     
  printf("connection from (%s, %d)\n", 
              inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

  receive_data = malloc(BUFFLENGTH*sizeof(char)); 
  send_data = malloc(BUFFLENGTH*sizeof(char)); 

  do {
    if ((bytes_received = recv(sockfd, receive_data, BUFFLENGTH - 1, 0)) 
                                                                          == -1)
      die("connection severed"); 

    receive_data[bytes_received] = '\0'; 
    if (strlen(receive_data) == 0) 
      continue; 
    printf("(%s, %d) says '%s'\n", inet_ntoa(client_addr->sin_addr), 
                                  ntohs(client_addr->sin_port), receive_data);

    pch = strtok(receive_data, ", \t"); 

    if (strcmp(pch, "help") == 0) 
      send(sockfd, library_help, strlen(library_help), 0);
    
    else if (strcmp(pch, "ls") == 0) {
      strcpy(send_data, ls(options->dir)); 
      send(sockfd, send_data, strlen(send_data), 0);
    }

    else if (strcmp(pch, "read") == 0) {
      /* Send response packet */ 
      if ((pch = strtok(NULL, ", \t")) == NULL)
        send(sockfd, "BAD;Please specify an entry.", 28, 0); 
      else if (readfile(send_data, pch, options->dir) == -1)
        send(sockfd, "BAD;File not found.", 19, 0);
      else 
        send(sockfd, send_data, strlen(send_data), 0);

    }

    else if (strcmp(pch, "quit") == 0) break; 
    
    else {
      strcpy(send_data, "Command '");
      strncpy(&send_data[9], pch, 256); 
      strncpy(&send_data[strlen(send_data)], "' not found.\n", 
                                      BUFFLENGTH - strlen(send_data) - 13);
      send(sockfd, send_data, strlen(send_data), 0); 
    }

  } while (1); 

  free(receive_data);
  free(send_data); 
  close(sockfd); 
  printf("connection with (%s, %d) closed\n", 
              inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

} /* handle_client() */




int main(int argc, const char **argv) {

  /* Program command line parameters */
  param_t options; 
  if (!parse_options(&options, argc, argv)) { 
    puts(command_help);
    return 1; 
  }

  int pid, sockfd, clientsockfd, yes=1; 
  struct sockaddr_in serv_addr, client_addr; 
  size_t sin_size = sizeof(struct sockaddr_in); 

  /* setup socket */
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    die("socket"); 

  /* reset socket so kernel knows it's time to reuse it */
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
    die("setsockopt"); 

  /* Construct local addres structure */ 
  memset(&serv_addr, 0, sizeof(serv_addr));  /* zero out structure */ 
  serv_addr.sin_family = AF_INET;            /* internet address family */ 
  serv_addr.sin_port = htons(options.port);  /* local port number */ 
  serv_addr.sin_addr.s_addr = INADDR_ANY;    /* any incoming interface */ 

  /* bind address to port */ 
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) 
                                                                     == -1)
    die("bind"); 

  /* set pending queue size */
  if (listen(sockfd, MAXPENDING) == -1)
    die("listen"); 

  printf("Library server running on port %d\n", options.port); 
  fflush(stdout); 

  while (1) {
     
    /* accept new connection and fork a process to handle it. This seems
     * safe to me and I don't see any problem with doing it other than it
     * requires a process per client. It's probably better to spawn a 
     * thread instead. */
    clientsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
    if ((pid=fork()) == -1) 
      die("fork"); 

    else if (pid == 0) {
      close(sockfd); 
      handle_client(clientsockfd, &client_addr, &options); 
      exit(0); 
    }

    else {
      close(clientsockfd); 
    }
  }

  close(sockfd);

  return 0; 
}
