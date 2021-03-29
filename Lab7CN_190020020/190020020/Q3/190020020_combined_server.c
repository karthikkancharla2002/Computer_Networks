#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <time.h>
#include <errno.h> 
#include <netdb.h>
#include <signal.h> 
#define MAXLINE 1024 
int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 


int main(int argc, char** argv) 
{ 

    if(argc < 2){
        printf("Enough arguments not provided.\n");
        exit (0);
    }

    int PORT = atoi(argv[1]);

    int listenfd, connfd, udpfd, nready, maxfdp1; 
    char buff[MAXLINE]; 
    pid_t childpid; 
    fd_set rset; 
    ssize_t n; 
    socklen_t len; 
    const int on = 1; 
    struct sockaddr_in cliaddr, servaddr; 
    // char* message = "Hello Client"; 
    void sig_chld(int); 
  
    /* create listening TCP socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0); 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // binding server addr structure to listenfd 
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    listen(listenfd, 10); 
    
  
    /* create UDP socket */
    udpfd = socket(AF_INET, SOCK_DGRAM, 0); 
    // binding server addr structure to udp sockfd 
    bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
  
  	
    // clear the descriptor set 
    FD_ZERO(&rset); 
  
    // get maxfd 
    maxfdp1 = max(listenfd, udpfd) + 1; 
    for (;;) { 
  
        // set listenfd and udpfd in readset 
        FD_SET(listenfd, &rset); 
        FD_SET(udpfd, &rset); 
  
        // select the ready descriptor 
        nready = select(maxfdp1, &rset, NULL, NULL, NULL); 
  
        // if tcp socket is readable then handle 
        // it by accepting the connection 
        if (FD_ISSET(listenfd, &rset)) { 
            //write your code here
            len = sizeof(cliaddr); 
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len); 
            if ((childpid = fork()) == 0) { 
                close(listenfd); 
                bzero(buff, sizeof(buff)); 
                time_t rawtime;
  	struct tm * timeinfo;
  	time ( &rawtime );
  	timeinfo = localtime ( &rawtime );
        //printf("Date and time sent.\n");
        write(connfd,asctime(timeinfo),100);
           }
        } 
        // if udp socket is readable receive the message. 
        if (FD_ISSET(udpfd, &rset)) { 
            //write your code here
            len = sizeof(cliaddr); 
            bzero(buff, sizeof(buff)); 
            n = recvfrom(udpfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
      
		buff[n-1] = '\0'; 
		/* gethostbyname() to find ip address for dns
                   put your code here */
               struct hostent *he = gethostbyname(buff);
               
               
               if (he == NULL)
               {
                herror("gethostbyname");
                exit(1);
               }
              

	if (he->h_addrtype == AF_INET)
	{
	struct in_addr **addr_list = (struct in_addr **)he->h_addr_list;
            for(int i = 0; addr_list[i]!=NULL; i++){
                inet_ntop(AF_INET, addr_list[i], buff, INET_ADDRSTRLEN);
                sendto(udpfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
            }
	}
	else if(he->h_addrtype == AF_INET6)
	{
	struct in6_addr **addr_list = (struct in6_addr **)he->h_addr_list;
            for(int i = 0; addr_list[i]!=NULL; i++){
                inet_ntop(AF_INET6, addr_list[i], buff, INET_ADDRSTRLEN);
                sendto(udpfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
            }
	}
	
	bzero (buff, MAXLINE);
	strcpy(buff, "done");	
	sendto(udpfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
	
	}
         }   
    
    return 0;

}

/* Commands : gcc 190020020_combined_server.c -o server
./server 500000020 
Output: */



