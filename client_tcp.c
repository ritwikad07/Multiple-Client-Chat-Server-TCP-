// Write CPP code here
#include <netdb.h>
#include "key_press.h"
#include <sys/time.h>
//#include<string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8888
#define SA struct sockaddr

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

		int valread;

	char name[20];
	char temp_buff[20];
	char temp_buff1[20];
	char var_name[20];

	puts("Enter the name of your server");
	gets(name);

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	//printf("the ntohs: %s \n", ntohs(servaddr.sin_port));
	//printf("the ntohs: %s \n", servaddr.sin_port);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");


	char buff[MAX];
	char server[MAX];
	char client_name[MAX];
	int n;
	int client_num;

	read(sockfd, server, sizeof(server));
	client_num = atoi(server);
	printf("Your client number : %d \n",client_num);


	//printf("Client %d : ", client_num);
	write(sockfd, name, sizeof(name));

  char opinion[20];

  printf("JUST PRESS ANY KEY IF YOU WANT TO TYPE:\n");

	sleep(2);
		system("clear");
		printf("YOU ARE %s ", name);
	while(1) {

    //printf("Do you want to send?(yes/no)");

		initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
		while (1) {
      //sleep(2);
        if (kbhit()) {
            //printw("Key pressed! It was: %d\n", getch());
            strcpy(opinion,"yes");

            refresh();
            endwin();
            break;
        }
        else
        {
          //printf("no key");
          strcpy(opinion,"no");
          refresh();
          endwin();
          break;
        }
    }
		//puts(opinion);

    //bzero(opinion, sizeof(opinion));
    write(sockfd, opinion, sizeof(opinion));

		//system(clear);
    if ((strcmp(opinion,"yes")) == 0)
    {
      printf("enter the name of the client you want to talk to: \n");
      gets(var_name);
      write(sockfd, var_name, sizeof(var_name));
	    //for(;;)
		  //{
			bzero(buff, sizeof(buff));
			printf("%s : ", name);
			n = 0;
			while ((buff[n++] = getchar()) != '\n')
				;
				write(sockfd, buff, sizeof(buff));
			bzero(buff, sizeof(buff));

    }
    else
    {


      	bzero(buff, sizeof(buff));
				//printf("\n First you got from server %s\n", buff);
				//memset(buff, 0, sizeof(buff));
				read(sockfd, buff, sizeof(buff));
				//puts(buff);
				//printf("val is %d",valread);
				//puts(buff);
				if(strcmp(buff,"nothing")!=0)
				{
							//puts(buff);
							//printf("inside2");
							//printf("val is %d",valread);
								printf("\n%s :", buff);
								bzero(buff, sizeof(buff));

								read(sockfd, buff, sizeof(buff));
									//memset(buff, 0, sizeof(buff));
								//read(sockfd, buff, sizeof(buff))
								//puts(buff);
								printf(" %s\n", buff);
								bzero(buff, sizeof(buff));
					}
						//strcpy(opinion,"nothing");
							strcpy(buff,"nothing");

    }
		//bzero(opinion, sizeof(opinion));


		/*bzero(buff,sizeof(buff));
		printf("Do you want to exit? ");
		gets(buff);
    */
		/*if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
      */
			//printf("end of infinite");
		}
	// close the socket
	close(sockfd);
}
