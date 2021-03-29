#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <netdb.h>
#define MAXLINE 1024 

// Driver code 
int main(int argc, char** argv) { 
	if(argc < 2){
        printf("Enough arguments not provided.\n");
        exit (0);
    }

	int PORT = atoi(argv[1]);
	int sockfd; 
	char buff[MAXLINE]; 
	// char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	else 
        printf("Socket opened.\n");
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	else
        printf("Binded Successfully.\n");
	while(1) {

		int len, n; 

		len = sizeof(cliaddr); //len is value/resuslt 
		
		n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
      
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
                sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
            }
	}
	else if(he->h_addrtype == AF_INET6)
	{
	struct in6_addr **addr_list = (struct in6_addr **)he->h_addr_list;
            for(int i = 0; addr_list[i]!=NULL; i++){
                inet_ntop(AF_INET6, addr_list[i], buff, INET_ADDRSTRLEN);
                sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
            }
	}
	
	bzero (buff, MAXLINE);
	strcpy(buff, "done");	
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
	
	}
	
	return 0; 
}

/*
Commands:
gcc 190020020_dns_server.c -o server
./server 500000020


Output:
Socket opened.
Binded Successfully.
*/

