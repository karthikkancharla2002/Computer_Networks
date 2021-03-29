
#include <stdio.h> 
#include <string.h>  
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
	
#define TRUE 1 
#define FALSE 0 
#define PORT 8888 
	
int main(int argc , char *argv[]) 
{ 
	int opt = TRUE; 
	int master_socket , addrlen , new_socket , client_socket[30] , 
		max_clients = 30, activity, i , valread , sd; 
	int max_sd; 
	struct sockaddr_in address; 
		
	char buffer[1025]; //data buffer of 1K 
    char str[50];
		

	fd_set readfds; 
		
	//a message 
	char *message = "ECHO Daemon v1.0"; 
	
	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	} 
		
	//create a  socket 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
	//set  socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	
	// assign address here 
	address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( PORT );   
		
	//bind the socket to localhost port  
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
	printf("SERVER: Socket binded successfully...\n");
		
	//try to specify maximum number of pending connections for the socket and listen
	if (listen(master_socket, 3) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
	printf("SERVER: Server started listening for client...\n");
		
	//accept the incoming connection (Waiting for connections)
	addrlen = sizeof(address); 
	printf("SERVER: Server started listening for client at port number: 8888...\n");
		
	while(TRUE) 
	{ 
		//clear the socket set 
		 FD_ZERO(&readfds); 
	
		//add socket to set 
		FD_SET(master_socket, &readfds);   
        max_sd = master_socket; 

		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 
			
		//If check incoming connection using FD_ISSET
		if (FD_ISSET(master_socket, &readfds)) 
		{ 
			//accept the connection
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			

		
			//send new connection greeting message 
			// if( send(new_socket, message, strlen(message), 0) != strlen(message)  ) 
			// { 
			// 	perror("send"); 
			// } 
				
			// puts("Welcome message sent successfully"); 
				
			//add new socket to array of sockets 
			for (i = 0; i < max_clients; i++) 
			{ 
				if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                         
                    break;   
                }  
			} 
		} 
			
		//else its some IO operation on some other socket 
		for (i = 0; i < max_clients; i++) 
		{ 
			sd = client_socket[i]; 
				
			//Check for FD_ISSET 
			
			if (FD_ISSET( sd , &readfds)) 
			{ 
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((valread = read( sd , buffer, 1024)) == 0) 
				{ 
					//Somebody disconnected , get his details and print 
					/*Code here*/
					getpeername(sd , (struct sockaddr*)&address ,(socklen_t*)&addrlen); 
						
					//Close the socket and mark as 0 in list for reuse 
					close( sd ); 
					client_socket[i] = 0; 
				} 
					
				//Echo back the message that came in 
				else
				{ 
					//set the string terminating NULL byte on the end 
					//of the data read 
					/*Code here*/
                    buffer[valread] = '\0';
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
	send(sd , message , strlen(message) , 0 ); 

	sprintf(str, "%d", word_count+1);
	char message2[] = "Number of words in the given paragraph: ";
	strcat(message2, str);
	strcat(message2, "\n");
	send(sd , message2 , strlen(message2) , 0 ); 

	sprintf(str, "%d", sentence_count);
	char message3[] = "Number of sentences in the given paragraph: ";
	strcat(message3, str);
	strcat(message3, "\n");
	send(sd , message3 , strlen(message3) , 0 ); 





	printf("SERVER: Paragraph analysis sent to client.\n"); 
                     
				} 
			} 
		} 
	} 
		
	return 0; 
} 


