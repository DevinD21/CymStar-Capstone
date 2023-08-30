/*
 * TCP_Server.c
 *
 *Contains TCP protocal functions and serveral fucntions for parsing a transaction
 *
 *  Created on: Mar 6, 2023
 *      Author: Devin Dickerson
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

#define MAX_MES_SIZE 6 //MAX Size of clientMes due to having to add '\0' at the end of the word when added to transaction array
#define MAX_WORD_COUNT 68  //MAX possible number of words in a transaction
#define MAX_WORD_SIZE 5 //Max size for command and status word arrays
#define PORT 8080
FILE *BM;
char transaction[MAX_WORD_COUNT][MAX_MES_SIZE];//2D char array to hold the entire 1553 Transaction that is received
int wordCount = 0; //Number of words in received transaction
int syncCounter = 0; //Counter to differentiate between Command words and Status words
int dataWordCount = 0; // Counts the number of data words in the recieved transaction
int transactionCount = 0; //Counts number of transactions proccesed

//Creates the CSV and sets the header
void createCSV()
{
	BM = fopen("BusMonitor.csv","w+"); //Create CSV file
	fprintf(BM,"Transaction#, Direction, Command Word, Data, Status, Message Time (us)\n"); //Write header of csv
}

//Extracts TR from Command word and returns it
//qw: D11-D8
int extractTR(int qw)
{	
	return ((qw >> 1) & 1); //The TR bit is D9 which is in position 1 in halfWord
}

//Extracts the terminal address from the command words 3rd and 4th elements
//terminal Add: D4-D0
// qw = quarter of a word
//qw2: D7-D4
//qw3: D3-D0
int extractTerminalAdd(int qw2,int qw3)
{
	int terminalAdd = 0;

	//bit 0 of qw3 = D0
	if(((qw3 >> 0) & 1) == 1)
	{	
		//if D0 = 1 set the 0th bit of terminalAdd to 1
		terminalAdd |= (1 << 0);
	}
	
	//bit 1 of qw3 = D1
	if(((qw3 >> 1) & 1) == 1)
	{	
		// if D1 = 1 set the 1st bit of terminalAdd to 1
		terminalAdd |= (1 << 1);
	}
	
	//bit 2 of qw3 = D2
	if(((qw3 >> 2) & 1) == 1)
	{	
		//if D2 = 1 set the 2nd bit of terminalAdd to 1
		terminalAdd |= (1 << 2);
	}

	//bit 3 of qw3 = D3
	if(((qw3 >> 3) & 1) == 1)
	{	
		//if D3 = 1 set the 3rd bit of terminalAdd to 1
		terminalAdd |= (1 << 3);
	}	
	
	//bit 0 of qw2 = D4
	if(((qw2 >> 0) & 1) == 1)
	{	
		//if D4 = 1 set the 4th bit of terminalAdd to 1
		terminalAdd |= (1 << 4);
	}
	return terminalAdd;
}

//Extracts the sub address from the command words first and second elements
// sub Address: D10-D6
// qw = quarter of a word
//qw1: D11-D8
//qw2: D7-D4
int extractSubAdd(int qw1, int qw2)
{
	int subAdd = 0;
	
	//bit 2 of qw2 = D6
	if(((qw2 >> 2) & 1) == 1)
	{
		//if D6 = 1 set the 0th bit in subAdd to 1
		subAdd |=  (1 << 0);
	}
	
	//bit 3 of qw2 = D7
	if(((qw2 >> 3) & 1) == 1)
	{	
		//if D7 = 1 set the 1st bit in subAdd to 1
		subAdd |=  (1 << 1);
	}
	
	//bit 0 of qw1 = D8
	if(((qw1 >> 0) & 1) == 1)
	{
		//if D8 = 1 set the 2nd bit in subAdd to 1
		subAdd |=  (1 << 2);
	}
	
	//bit 1 of qw1 = D9
	if(((qw1 >> 1) & 1) == 1)
	{
		//if D9 = 1 set the 3rd bit in subAdd to 1
		subAdd |=  (1 << 3);
	}
	
	//bit 2 of qw1 = D10
	if(((qw1 >> 2) & 1) == 1)
	{
		//if D10 = 1 set the 4th bit in subAdd to 1
		subAdd |=  (1 << 4);
	}

	return subAdd;
}

//Extracts the data word count from a command word
//NOTE: will only be used when parsing a RT-> RT transaction
//Data word count: D15-D11
//qw0: D15-D12
//qw1: D11-D8
int extractDataCount(int qw0, int qw1)
{
	int dataCount = 0;	
	
	//bit 3 of qw1 = D11
	if(((qw1 >> 3) & 1) == 1)
	{	
		//if D11 = 1 set the 0th bit in dataCount to 1
		dataCount |=  (1 << 0);
	}
	
	//bit 0 of qw0 = D12
	if(((qw0 >> 0) & 1) == 1)
	{
		//if D12 = 1 set the 1th bit in dataCount to 1
		dataCount |=  (1 << 1);
	}
	
	//bit 1 of qw0 = D13
	if(((qw0 >> 1) & 1) == 1)
	{
		
		//if D13 = 1 set the 2th bit in dataCount to 1
		dataCount |=  (1 << 2);
	}
	
	//bit 2 of qw0 = D14
	if(((qw0 >> 2) & 1) == 1)
	{
		//if D14 = 1 set the 3th bit in dataCount to 1
		dataCount |=  (1 << 3);
	}
	
	//bit 2 of qw0 = D15
	if(((qw0 >> 3) & 1) == 1)
	{
		//if D15 = 1 set the 4th bit in dataCount to 1
		dataCount |=  (1 << 4);
	}
	
	return dataCount;
}

//Takes in received buffer and parses it to a file
//transaction[word position][0]: SYNC bit
//transaction[word position][1]: D15-D12
//transaction[word position][2]: D11-D8
//transaction[word position][3]: D7-D4
//transaction[word position][4]: D3-D0
void parseTransaction()
{
	//if the syncCount is 2 the transaction can be of two types: 
	// 1. RT to Controller
	// 2. Controller to RT
	if(syncCounter == 2)
	{
		//Arrays set to size 4 because a word without sync bit attached will be 4 chars instead of the 6 like in the transaction array
		//Command word for these transaction types will always be the first word recieved;
		char command[MAX_WORD_SIZE] = {transaction[0][1],transaction[0][2],transaction[0][3],transaction[0][4]}; 
		int terminalAdd = extractTerminalAdd(command[2],command[3]);
		int subAdd = extractSubAdd(command[1],command[2]);
		
		// IF the TR bit is 1 then the transaction type is RT->BC
		if(extractTR(command[1]) == 1)
		{
			char dir[7] = "RT->BC";
				
			//Status word is the last word in RT->BC transaction
			char status[MAX_WORD_SIZE] = {transaction[wordCount-1][1],transaction[wordCount-1][2],transaction[wordCount-1][3],transaction[wordCount-1][4]};
			
			//Fill the data array
			//Data words start at transaction[1] and end at transaction[wordCount -2]
			char data[dataWordCount*4];
			int dataIndex = 0;	
			
			for(int w = 1; w < wordCount; w++)
			{
				for(int i = 1; i <= 4; i++)
				{
					if(dataIndex <= dataWordCount*4)
					{
						data[dataIndex] = transaction[w][i];
						dataIndex++;
					}
				}
			}
			int one = 1;
			//Print transaction to the next row in the csv
			fprintf(BM,"%d,%s, %d-TX-%d-%d <%s>,%s,<%s>,%d\n",transactionCount,dir,terminalAdd,subAdd,dataWordCount,command,data,status,one);
		}
		else if(extractTR(command[1]) == 0) // IF the TR bit is 0 then the transaction type is BC->RT
		{
			char dir[7] = "BC->RT";

			//The status word is the second word in the transaction array
			char status[MAX_WORD_SIZE] = {transaction[1][1],transaction[1][2],transaction[1][3],transaction[1][4]};

			//Data words start at transaction[1] and end at transaction[wordCount -2]
			char data[dataWordCount*4];
			int dataIndex = 0;	
			for(int w = 2; w < wordCount; w++)
			{
				for(int i = 1; i <= 4; i++)
				{
					if(dataIndex <= dataWordCount*4)
					{
						data[dataIndex] = transaction[w][i];
						dataIndex++;
					}
				}
			}
			
			//Print transaction to the next row in the csv
			int one = 1;
			fprintf(BM,"%d,%s, %d-RX-%d-%d <%s>,%s,<%s>,%d\n",transactionCount,dir,terminalAdd,subAdd,dataWordCount,command,data,status,one);
		}	
	}
	else if(syncCounter == 4) //If the transaction has 4 sync bits then the transaction type is RT->RT
	{
		char dir[7] = "RT->RT";

		//a recieve command will be in transaction[0]
		char rCommand[MAX_WORD_SIZE] = {transaction[0][1],transaction[0][2],transaction[0][3],transaction[0][4]};
		int rTerminalAdd = extractTerminalAdd(rCommand[3],rCommand[2]);
		int rSubAdd = extractSubAdd(rCommand[1],rCommand[2]);
		int rDataCount = extractDataCount(rCommand[0],rCommand[1]);
		
		//a transmit command will be in transaction[1]
		char tCommand[MAX_WORD_SIZE] = {transaction[1][1],transaction[1][2],transaction[1][3],transaction[1][4]};
		int tTerminalAdd = extractTerminalAdd(tCommand[3],tCommand[2]);
		int tSubAdd = extractSubAdd(tCommand[1],tCommand[2]);
		int tDataCount = extractDataCount(tCommand[0],tCommand[1]);
		

		// a status word will be in transaction[2]
		char rStatus[MAX_WORD_SIZE] = {transaction[2][1],transaction[2][2],transaction[2][3],transaction[2][4]};

		// a status word will be in transaction[wordCount - 1]
		char tStatus[MAX_WORD_SIZE] = {transaction[wordCount-1][1],transaction[wordCount-1][2],transaction[wordCount-1][3],transaction[wordCount-1][4]};

		// the data words will be in transaction[3] - transaction[wordCount - 2];
		char data[dataWordCount*4];
		int dataIndex = 0;	
		for(int w = 2; w < wordCount; w++)
		{
			for(int i = 1; i <= 4; i++)
			{
				if(dataIndex <= dataWordCount*4)
				{
					data[dataIndex] = transaction[w][i];
					dataIndex++;
				}
			}
		}
		int one = 1;
		//Print transaction to the next row in the csv
		fprintf(BM,"%d,%s,%d-TX-%d-%d <%s> %d-RX-%d-%d <%s>,%s,<%s> <%s>,%d\n",transactionCount,dir,rTerminalAdd,rSubAdd,rDataCount,rCommand,tTerminalAdd,tSubAdd,tDataCount,tCommand,data,rStatus,tStatus,one);
	}

	//Clear transaction and reset counters
	memset(transaction, '\0',wordCount);
	wordCount = 0;
	syncCounter = 0;
	dataWordCount = 0;
}

//Create IPv4 TCP stream socket for server
int createSocket()
{
	int socketfd;
	printf("Creating Socket for server...\n");
	if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Socket creation failed...\n");
		exit(1);
	}
	printf("Socket creation successful...\n");
	return socketfd;
}

//Bind server socket to a port
void bindSocket(int socketfd)
{
	struct sockaddr_in serverAddr;

	//FIXME: maybe need to cast to (char*) (&serverAddr, sizeof(serverAddr))
	bzero(&serverAddr, sizeof(serverAddr));

	// assign IP, PORT
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORT);

	printf("Binding Socket...\n");
	while((bind(socketfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr))) != 0)
	{
		printf("socket bind failed...\n");
		exit(1);
	}
	printf("Socket successfully binded..\n");
}

//Listen for connection from client
void listenConn(int socketfd)
{
	if(listen(socketfd,10) == -1) //Backlog queue of 10
	{
		printf("Listen failed...\n");
		exit(1);
	}
	printf("Server listening...\n");
}

//Accept connection from client
int acceptConnection(int socketfd)
{
	int newSocket;
	struct sockaddr_in clientAddr; //client address
	unsigned int clientLen;

	clientLen = sizeof(clientAddr);

	printf("Accepting connection to client...\n");
	if((newSocket = accept(socketfd, (struct sockaddr *) &clientAddr, &clientLen)) == -1)
	{
		printf("Accept failed...\n");
		exit(1);
	}
	printf("Connection to client accepted...\n");
	return newSocket;
}

//Read incoming 1553 word from Ethernet adapter
void recvWord(int clientSocket)
{
	char clientMes[MAX_MES_SIZE];
	if(recv(clientSocket, clientMes, MAX_MES_SIZE,0) < 0)
	{
		printf("Unable to receive the client's message...\n");
		exit(1);
	}
	
	//The '~' indicates the end of a 1553 transaction
	if(clientMes[0] != '~')
	{	
		//If the Sync bit is 1 add 1 to the sync counter
		
		if(clientMes[0] == '1')
		{	
			++syncCounter;
		}
		else //if sync bit is 0 add 1 to the data word counter
			dataWordCount++;
	
		strcpy(transaction[wordCount],clientMes);
		wordCount++;
	}	
	
	if(clientMes[0] == '~')
	{	
		printf("%c\n",clientMes[0]);
		++transactionCount;
		parseTransaction();
	}
}

//FIXME: Did not use for bus monitor
void sendWord(int clientSocket)
{
	//Message buffer for words sent to client
	char server_mes[1024] = {'M','e','s',' ','R','e','c','e','i','v','e','d'};

	if(send(clientSocket,server_mes, strlen(server_mes),0) < 0)
	{
		printf("Unable to send message to client...\n");
		exit(1);
	}
}

//Close the passed socket
void closeSocket(int socketfd)
{
	close(socketfd);
}

