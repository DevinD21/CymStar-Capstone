/*Run_TCP_Server.c
 *
 *Contains main function that calls TCP functions to start the server
 *
 *  Created on: Mar 6, 2023
 *      Author: Devin Dickerson
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//Function declarations from TCP_Server.c

int extractTR(int upperHalf);

int createSocket();

void bindSocket(int socketfd);

void listenConn(int socketfd);

int acceptConnection(int socketfd);

void recvWord(int clientSocket);

void sendWord(int clientSocket);

void closeSocket(int socketfd);

void createCSV();

int main()
{
	createCSV();
	int socketfd; // Socket Descriptor for Server
	int clientSocketfd; // Client Socket descriptor
	char c = 
	socketfd = createSocket(); //Create socket for server

	bindSocket(socketfd); //Bind server socket to the PORT
	int wordCount = 0;
	int T = 1;
	//listenConn(socketfd); //Listen for incoming connections from client
	while(T)
	{	
		listenConn(socketfd); //Listen for incoming connections from client

		clientSocketfd = acceptConnection(socketfd); //Acception the connection from the client

		recvWord(clientSocketfd);//Receive word from BBB
		wordCount++;

		closeSocket(clientSocketfd);
		if(wordCount == 21)
		{
			T = 0;
		}
	}
	closeSocket(socketfd);
}

