// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	char *hello = "Hello from server";
	char str[50];
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("SERVER: Socket created...\n");
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	printf("SERVER: Socket binded successfully...\n");
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	printf("SERVER: Server started listening for client at port number: 8080 ...\n");
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	printf("SERVER: Accepted the connection from client...\n");
	valread = read( new_socket , buffer, 1024); 
	
	
	int char_count=0;
	int word_count=0;
	int sentence_count=0;
	
	
	for(int i=0; buffer[i] != '\0'; i++)
	{
		if(buffer[i] == '.')
		{
			sentence_count++;
		}
		if(buffer[i] == ' ' && buffer[i+1] != ' ')
		{
			word_count++;
		}
	}
	
	char_count=strlen(buffer);
	
	
	printf("SERVER: Paragraph received from client: \n%s\n\n",buffer ); 
	printf("SERVER: Sending the below information to the client:\n\n");
	printf("Number of characters in the given paragraph: %d\n",char_count);
	printf("Number of words in the given paragraph: %d\n",word_count+1);
	printf("Number of sentences in the given paragraph: %d\n\n",sentence_count);
	
	sprintf(str, "%d", char_count);
	char message[] = "Number of characters in the given paragraph: ";
	strcat(message, str);
	strcat(message, "\n");
	send(new_socket , message , strlen(message) , 0 ); 

	sprintf(str, "%d", word_count+1);
	char message2[] = "Number of words in the given paragraph: ";
	strcat(message2, str);
	strcat(message2, "\n");
	send(new_socket , message2 , strlen(message2) , 0 ); 

	sprintf(str, "%d", sentence_count);
	char message3[] = "Number of sentences in the given paragraph: ";
	strcat(message3, str);
	strcat(message3, "\n");
	send(new_socket , message3 , strlen(message3) , 0 ); 





	printf("SERVER: Paragraph analysis sent to client.\n"); 
	close(new_socket);
	printf("SERVER: Connection closed\n");
 
	return 0; 
} 


