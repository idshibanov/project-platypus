#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "defines.h"
#include "socket.h"

int main() {
   struct addrinfo *ailist;
   struct sockaddr_in *sinp;
   socklen_t len = sizeof(struct sockaddr_in);
   int sockfd, port = SERV_PORT, result, fd;
   fd_set readfds, testfds;
   char message[BUFSIZ + 1], username[11];
   int row=0;
   printf("\n*** Client program starting (enter \"quit\" to stop): \n");
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   getaddrinfo(SERV_IP, NULL, NULL, &ailist);
   sinp = (struct sockaddr_in *)ailist->ai_addr;
   sinp->sin_port = htons(port);
   if (connect(sockfd, (struct sockaddr *)sinp, len)) exit(0);
   /* Initialize a file descriptor set */
   FD_ZERO(&readfds);
   FD_SET(sockfd, &readfds);
   FD_SET(0, &readfds);  /* Add keyboard to file descriptor set */
   /* Now wait for messages from the server or keyboard */
   while (1) {
      testfds = readfds;
      select(FD_SETSIZE, &testfds, NULL, NULL, NULL);
      /* If there is activity, find which descriptor it's on using FD_ISSET */
      for (fd = 0; fd < 4; fd++) {
         if (FD_ISSET(fd, &testfds)) {
            if (fd == 0)  {  /* Process keyboard activity */
               fgets(message, BUFSIZ + 1, stdin);
               if (strcmp(message, "quit\n") == 0) {
                  close(sockfd);
                  exit(EXIT_SUCCESS);
               } else {
                  //write(sockfd, message, strlen(message));
                  printf("writes\n");
                  
                  ClientSocketHandler serv(sockfd);
                  if (serv.SendChatMsg(message))
                  {
                     printf("successfully\n");
                  } else
                  {
                     printf("false from SendChatMsg\n");
                  }
               }
            } else {
               result = read(sockfd, message, BUFSIZ);
               message[result] = '\0';
               printf("%s", message + 1);
               if (message[0] == 'X') {
                  close(sockfd);
                  exit(EXIT_SUCCESS);
               }
            }
         }
      }
   }
}


