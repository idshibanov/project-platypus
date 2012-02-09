#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include "defines.h"

#include "datatypes.h"
#include "packet.h"
#include "socket.h"

int main()
{  
   fd_set readfds, testfds;
   sockaddr_in serv_addr, client_addr;
   int serv_addr_len, client_addr_len;
   int serv_fd, client_fd;
   int fd_max, fd_cur, nread, bytes_got;
   
   ClientSocketArray client_sock; // holds client connections
   
   int fd_cur2; // temp variable
   int fd_min_client; // replace later   
   
   char kb_msg[MAX_KB_INPUT+1];
   char serv_msg[MAX_PACKET_SIZE+1]; // must be replaced with packets
   char client_msg[MAX_PACKET_SIZE+1]; // must be replaced with packets
    
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(SERV_PORT);
   serv_addr_len = sizeof(serv_addr);
   
   if((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      perror("socket call failed");
      exit(1);
   }
   if(bind(serv_fd, (struct sockaddr *)&serv_addr, serv_addr_len) == -1)
   {
      perror("bind call failed");
      exit(1);
   }
   if(listen(serv_fd, 5) == -1)
   {
      perror("listen call failed");
      exit(1);
   }
   
   // fd sets init
   
   FD_ZERO(&readfds);
   FD_ZERO(&testfds);
   FD_SET(serv_fd, &readfds);
   FD_SET(STDIN_FILENO, &readfds);
   fd_max = serv_fd + 1;
   fd_min_client = serv_fd + 1;
   
   while(1)
   {
      testfds = readfds;
      // remember to add 1 to fd_max
      // select ( highest_fd_number+1, &read_fd_set, &write_fd_set, &except_fd_set, &timeval)
      select( fd_max, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *) 0);
      
      for(fd_cur = 0; fd_cur < fd_max; fd_cur++)
      {
         if (FD_ISSET(fd_cur, &testfds))
         {
            if (fd_cur == serv_fd)
            {
               // something on server socket 
               
               if((client_fd = accept(serv_fd, (struct sockaddr *) 0,
                              (socklen_t *)0)) == -1)
               {
                  // can not accept connection
	               perror("accept call failed");
	               continue;
	            } else if (client_sock.AddClient(client_fd) == false)
	            {
	               printf("no more space left\n");
	               close(client_fd);
	               continue;
	            } else
	            {
	               // new client init, already added to client_sock list
	               	               
	               FD_SET(client_fd, &readfds);
	               printf("adding client on fd %d\n", client_fd);
	               if ( client_fd + 1 > fd_max ) fd_max = client_fd + 1;
	            }
            } else if (fd_cur == STDIN_FILENO)
            {
               // server keyboard input               
               //bytes_got = read(STDIN_FILENO, &kb_msg, MAX_KB_INPUT);
               //kb_msg[bytes_got] = '\0';
               
               // flush the rest
               // fflush(stdin);
               fgets(kb_msg, MAX_KB_INPUT, stdin);
               if (strcmp(kb_msg, "quit\n") == 0)\
               {
                  // shutdown command
                  sprintf(serv_msg, "XServer is shutting down.\n");
                  for (fd_cur2 = fd_min_client; fd_cur2 < fd_max; fd_cur2++)
                  {
                     if (FD_ISSET(fd_cur2, &readfds))
                     {
                        write(fd_cur2, serv_msg, strlen(serv_msg));
                        close(fd_cur2);
                     }
                  }
                  close(serv_fd);
                  exit(EXIT_SUCCESS);
               } else
               {
                  // other
                  sprintf(serv_msg, "MModerator: %s", kb_msg);
                  for (fd_cur2 = fd_min_client; fd_cur2 < fd_max; fd_cur2++)
                     if (FD_ISSET(fd_cur2, &readfds))
                        write(fd_cur2, serv_msg, strlen(serv_msg));
               }
            
            } else  
            {
               // something on client socket
               printf("on client socket\n");
               
               // check if client is alive
               ioctl(fd_cur, FIONREAD, &nread);
               
               if (nread == 0)
               {
                  // got junk, kill client
                  client_sock.RemoveClient(fd_cur);
                  close(fd_cur);
                  FD_CLR(fd_cur, &readfds);
                  
               } else
               {
                  // get client message and process it
                  printf("reads\n");
                  if (client_sock[fd_cur]->RecvPacket())
                  {
                     printf("successfully\n");
                  } else
                  {
                     printf("RecvPacket returned false\n");
                  }
               }
            }
                 
         } // end of if(fd_isset) condition
         
      } // end of for(fd_set iteration) loop
      
   } // end of while(1) loop
   
   return 0;
}


