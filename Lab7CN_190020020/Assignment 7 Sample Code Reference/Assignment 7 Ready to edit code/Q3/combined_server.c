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
        } 
        // if udp socket is readable receive the message. 
        if (FD_ISSET(udpfd, &rset)) { 
            //write your code here
    } 
    return 0;
}
} 


