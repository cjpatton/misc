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
 */ 

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> /* socket(), connect(), send(), recv() */ 
#include <arpa/inet.h>  /* struct sockaddr_in, inet_addr() */
#include <netdb.h>      /* gethostbyname() */
#include <unistd.h>     /* close() */
#include <sys/wait.h>   /* waitpid() */
#include <errno.h>      
#include <signal.h>

int sockfd; 
char *send_data, *receive_data;  

const char *command_help = 
"ibraryclient help.\n\
    -p N          port number (default 5000)\n\
    -a address    IP address (default localhost)\n";

void handle_sigint(int signo) {
  
  if (send_data)
    free(send_data);

  if (receive_data)
    free(receive_data); 

  if (sockfd != -1) {
    send(sockfd, "quit", 4, 0); 
    close(sockfd); 
  }

  exit(0); 
}


int main(int argc, const char **argv) {

  sockfd = -1; 
  send_data = receive_data = NULL; 
  (void) signal(SIGINT, handle_sigint); 

  /* Program command line parameters */
  param_t options; 
  if (!parse_options(&options, argc, argv)) {
    puts(command_help);
    return 1; 
  }

  int bytes_received, pid, status, fd[2];
  char *pch; 
  
  struct hostent *host; 
  struct sockaddr_in server_addr;

  /* setup server address struct */
  if ((host = gethostbyname(options.addr)) == NULL) {
    herror("gethostbyname");
    exit(1); 
  }

  /* create socket */
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    die("socket"); 

  /* setup server structure */
  memset(&server_addr, 0, sizeof(struct sockaddr_in));  /* zero out structure */
  server_addr.sin_family = AF_INET;                   /* internet addr family */ 
  server_addr.sin_port = htons(options.port);                  /* server port */
  server_addr.sin_addr = *((struct in_addr *)host->h_addr); /* server IP addr */

  /* establish TCP connection with server */
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                                                                    == -1)
    die("connect"); 
    
  send_data = malloc(BUFFLENGTH*sizeof(char));
  receive_data = malloc(BUFFLENGTH*sizeof(char)); 

  do {

    /* issue server a command */
    printf("library> "); readline(send_data, 256); 
    if (strlen(send_data) == 0) 
      continue; 
    send(sockfd, send_data, strlen(send_data), 0); 

    if (strcmp(send_data, "quit") == 0) break;
    
    else if (strncmp(send_data, "read", 4) == 0) {
      
      /* Get response from server about request */
      if ((bytes_received = recv(sockfd, receive_data, BUFFLENGTH - 1, 0)) == -1)
        die("no response or connection severed permanently"); 
      
      receive_data[bytes_received] = '\0';
      pch = strtok(receive_data, ";"); 
      if (strcmp(pch, "BAD") == 0) { /* something went wrong */
        pch = strtok(NULL, ";"); 
        printf("%s\n", pch); 
      }

      else if (strcmp(pch, "GOOD") == 0) {
        if (pipe(fd) == -1) die("pipe"); 
        if ((pid = fork()) == -1) die("fork"); 
        
        else if (pid == 0) { /* pipe downloaded file through less */ 
          close(sockfd); 
          close(fd[1]); 
          dup2(fd[0], 0);
          execl("/usr/bin/less", "less", "-M" , NULL);
          close(fd[0]); 
          exit(0); 
        }

        else { 
          close(fd[0]);
          pch = strtok(NULL, ""); 
          write(fd[1], pch, strlen(pch)); 
          waitpid(pid, &status, 0); 
          close(fd[1]); 
        }
      }
    } /* read */

    else 
    {
      if ((bytes_received = recv(sockfd, receive_data, BUFFLENGTH - 1, 0)) == -1)
        die("no response or connection severed permanently"); 
      receive_data[bytes_received] = '\0'; 
      printf("%s", receive_data); 
    } /* ls, help, otherwise */
    

  } while (strcmp(send_data, "quit") != 0);

  free(receive_data);
  free(send_data); 
  close(sockfd); 

  return 0; 
}
