//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE 1
#define FALSE 0
#define PORT 8888

int main(int argc , char *argv[])
{
	int number_of_clients=0;
	int opt = TRUE;
	int master_socket , addrlen , new_socket , client_socket[30] ,
		max_clients = 30 , activity, i , valread , sd , z,j;
	int max_sd;
	int m,n;
	int client_sd;

	int temp;

	int client_pointer[20][2];
	int x,y;
	for(x = 0; x < 20; x ++) {
    for(y = 0; y < 2; y ++) client_pointer[x][y] = 1000;
}

	int client_p_count=0;
	char message_buffer1[20][20];
	char message_buffer2[20];
	strcpy(message_buffer2,"nothing");
	char client_name[20];
	char opinion[20];

	char sendto[20];

	struct sockaddr_in address;
	struct timeval timeout;

	char name_list[20][20];
	int name_list_x=0, name_list_y=0;
	char buffer[1025]; //data buffer of 1K

	//set of socket descriptors
	fd_set readfds;

	int num = 0;
	char snum[20];
	snprintf (snum, sizeof(snum), "%d",num);

	timeout.tv_sec = 10;
  timeout.tv_usec = 0;

	//initialise all client_socket[] to 0 so not checked
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}

	//create a master socket
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	//bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);

	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//accept the incoming connection
	addrlen = sizeof(address);
	puts("Waiting for connections ...");

	//int counter=0;
	//int count[20];

	while(TRUE)
	{
		//clear the socket set
		FD_ZERO(&readfds);

		//add master socket to set
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
		activity = select( max_sd + 1 , &readfds , NULL , NULL , &timeout);

		if ((activity < 0) && (errno!=EINTR))
		{
			printf("select error");
		}

		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}

			num++;
			snprintf (snum, sizeof(snum), "%d",num);
			printf("\nnumber of clients %s \n",snum);
			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));


			//send new connection greeting message
			if( send(new_socket, snum, strlen(snum), 0) != strlen(snum) )
			{
				perror("send");
			}

			read(new_socket, client_name, sizeof(client_name));
			printf("Your new client's name is : %s \n", client_name);

			puts("Welcome message sent successfully");

			strcpy(name_list[name_list_x], client_name);
			strcpy(message_buffer1[name_list_x], "nothing");

			name_list_x++;

			for (m=0;m<name_list_x;m++)
			puts(name_list[m]);

			//add new socket to array of sockets
			for (i = 0; i < max_clients; i++)
			{
				//if position is empty
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

			if (FD_ISSET( sd , &readfds))
			{
				read(sd, opinion, sizeof(opinion));
				//puts(opinion);
				if(strcmp(opinion,"yes")==0)
				{
					read(sd, buffer, sizeof(buffer));
					puts(buffer);
					for (m=0;m<name_list_x;m++)
					{
							if ((strcmp(buffer,name_list[m]))==0)
							{
							 printf("the client numner %d ",m);
							 break;
						 }
					}

				//Check if it was for closing , and also read the
				//incoming message
				if ((valread = read( sd , buffer, 1024)) == 0)
				{
					printf("From client: %s\n To client : ", buffer);
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n" ,
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_socket[i] = 0;
				}

				//Echo back the message that came in
				else
				{

					client_sd=client_socket[m];

					//printf("Sending to client %d\n", m);
					client_pointer[client_p_count][0]=i;
					client_pointer[client_p_count][1]=m;
					client_p_count++;
					//sendto=name_list[i]
					//send(client_sd , name_list[i], strlen(name_list[i]) , 0 );

					printf("\nFrom1 client: %d\n sending to client %d: ", client_pointer[0][0],client_pointer[0][1]);
					//set the string terminating NULL byte on the end
					//of the data read
					strcpy(message_buffer1[m], buffer);
					puts(message_buffer1[m]);

					buffer[valread] = '\0';
					//strcpy(message_buffer1[m],"pending");message_buffer1

					//send(client_sd , buffer , strlen(buffer) , 0 );

				}
			}
			else
			{

					//client_sd=client_socket[m];

					//sendto=name_list[i]
					//send(sd , name_list[i], strlen(name_list[i]) , 0 );

					//printf("\nFrom1 client: %s\n To client : ", buffer);
					//set the string terminating NULL byte on the end
					//of the data read

					if(strcmp(message_buffer1[i],"nothing")!=0)
					{

						printf("your message still not null %s ", message_buffer1[i]);
						for(j=0;j<client_p_count;j++)
						{
							if(client_pointer[j][1]==i)
							{


								temp=client_pointer[j][0];
								printf("\nGot message from %s going to %s \n",name_list[temp],name_list[i]);

								bzero(buffer, sizeof(buffer));
								//buffer[valread] = '\0';
								strcpy(buffer,name_list[temp]);
								printf("first sending message %s \n",buffer);
								send(sd , buffer, strlen(buffer) , 0 );
								client_pointer[j][0]=1000;
								client_pointer[j][1]=1000;

							}
						}

						bzero(buffer, sizeof(buffer));
						//memset(buffer, 0, sizeof(buffer));
						strcpy(buffer,message_buffer1[i]);
							printf("your second message %s\n",buffer);
						send(sd , buffer , strlen(buffer), 0);
						strcpy(message_buffer1[i],"nothing");

					}
					//if(strcmp(message_buffer1[i],"pending")==0)
					//send(sd , message_buffer1[i] , strlen(message_buffer1[i]), 0);
					else//buffer[valread] = '\0';
						send(sd , message_buffer2 , strlen(message_buffer2), 0);

			}
			}
		}
	}

	return 0;
}
