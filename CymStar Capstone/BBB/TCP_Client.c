

/*
 * TCP_Client.c
 *
 *  Created on: Mar 6, 2023
 *      Author: devindickerson
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_MES_SIZE 5 //MAX Size of buffer array + 1
#define MAX_WORD_COUNT 35 //MAX possible number of words in a transaction
#define PORT 8080
FILE * BM;

char Test_Transaction1[7][MAX_MES_SIZE] = 
{
	"1DaVI",
	"1TFHY",
	"0O#GT",
	"0HGYW",
	"0CGF#",
	"0B0MN",
	"~"
};
char Test_Transaction2[7][MAX_MES_SIZE] = 
{
	"1GHRN",
	"0#*@^",
	"0($^H",
	"0&#H@",
	"0!*BC",
	"1)<$*",
	"~"
};

char Test_Transaction3[7][MAX_MES_SIZE] = 
{
	"1L@%$",
	"1J^?/",
	"0{#G|",
	"0#*@^",
	"1CGF#",
	"1_+F#",
	"~"
};

int createSocket()
{
	int socketfd;
	printf("Creating Socket for server...");
	if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Socket creation failed...\n");
		exit(1);
	}

	printf("Socket creation successful...\n");
	return socketfd;
}

void connectSocket(int socketfd)
{
	struct sockaddr_in serverAddr;//server address
	// assign IP, PORT
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("192.168.0.30");
	serverAddr.sin_port = htons(PORT);

	while((connect(socketfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr))) != 0)
	{
		printf("Socket connection to server failed...\n");
		exit(1);
	}
	printf("Socket successfully connected to server...\n");
}

void socketSend(int socketfd, char clientMes[5])
{
	//First copy message to temp array to clear any extra data added to array due to how arrays are passed to functions in c
	char mesTemp[MAX_MES_SIZE];
	for(int i = 0;i < MAX_MES_SIZE;i++)
	{
		mesTemp[i] = clientMes[i];
	}

	if(send(socketfd, mesTemp, strlen(mesTemp),0) < 0)
	{
		printf("Unable to send message");
		exit(1);
	}
}

void socketRecv(int socketfd)
{
	char server_mes[100];
	if(recv(socketfd,server_mes, sizeof(server_mes),0) < 0)
	{
		printf("Unable to receive message");
		exit(1);
	}

	printf("Server's response: %s\n",server_mes);
}

//FIXME: main in TCP_Client for demo other wise main would be in the BM file
int main()
{
	int socketfd; // Client Socket Descriptor
	
	//Variables for send test words during demo
	int k = 0;
	int i = 0;
	int tyTrans = 0;
	while(1)
	{
		char client_mes[5];
		//FIXME: Set up like this to show demeo of server and client communication
		for(int j = 0; j < 5; j++)
		{
			if(tyTrans == 0)
			{
				client_mes[j] = Test_Transaction1[i][j];
			}
			else if(tyTrans == 1)
			{
				client_mes[j] = Test_Transaction2[i][j];
			}	
			else if(tyTrans == 2)
			{
				client_mes[j] = Test_Transaction3[i][j];
			}
		}
		
		socketfd = createSocket();

		connectSocket(socketfd);

		socketSend(socketfd,client_mes);
	
		close(socketfd);
		
		//For demo to send test words
		++i;
		++k;
		if(i == 7 && k == 7)
		{
			tyTrans = 1;
			i = 0;
		}
		else if(k == 14)
		{
			tyTrans = 2;
			i= 0;
		}
	}

	close(socketfd);
}
