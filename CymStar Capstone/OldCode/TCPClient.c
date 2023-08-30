#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr
//#include "SubAdr.cpp"
//#include <iostream>
char toChar(int i) {
    switch(i) {
        case 0 : return '0';
        case 1 : return '1';
        case 2 : return '2';
        case 3 : return '3';
        case 4 : return '4';
        case 5 : return '5';
        case 6 : return '6';
        case 7 : return '7';
        case 8 : return '8';
        case 9 : return '9';
        case 10 : return 'A';
        case 11 : return 'B';
        case 12 : return 'C';
        case 13 : return 'D';
        case 14 : return 'E';
        case 15 : return 'F';
    }
}
int toDec(char c) {
    switch(c) {
        case '0' : return 0;
        case '1' : return 1;
        case '2' : return 2;
        case '3' : return 3;
        case '4' : return 4;
        case '5' : return 5;
        case '6' : return 6;
        case '7' : return 7;
        case '8' : return 8;
        case '9' : return 9;
        case 'A' : return 10;
        case 'B' : return 11;
        case 'C' : return 12;
        case 'D' : return 13;
        case 'E' : return 14;
        case 'F' : return 15;
    }
}
char len(int i) {
    switch(i) {
        case 1 : return 'a';
        case 2 : return 'b';
        case 3 : return 'c';
        case 4 : return 'd';
        case 5 : return 'e';
        case 6 : return 'f';
        case 7 : return 'g';
        case 8 : return 'h';
        case 9 : return 'i';
        case 10 : return 'j';
        case 11 : return 'k';
        case 12 : return 'l';
        case 13 : return 'm';
        case 14 : return 'n';
        case 15 : return 'o';
        case 16 : return 'p';
        case 17 : return 'q';
        case 18 : return 'r';
        case 19 : return 's';
        case 20 : return 't';
        case 21 : return 'u';
        case 22 : return 'v';
        case 23 : return 'w';
        case 24 : return 'x';
        case 25 : return 'y';
        case 26 : return 'z';
        case 27 : return '!';
        case 28 : return '.';
        case 29 : return '[';
        case 30 : return ']';
        case 31 : return '(';
        case 32 : return ')';
        case 33 : return '{';
    }
}
char tst1[13] = {'c','A','6','F','C','B','C','E','F','4','9','3','2'};
char tst2[37] = {'i','B','C','E','F','4','9','3','2','A','4','6','1','D','7','E','4','D','7','E','4','1','2','3','4','1','2','3','4','1','2','3','4','1','2','3','4'}; 
char tst3[33] = {'h','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','1','2'};
char tst4[69] = {'q','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','1','2','B','C','E','F','4','9','3','2','A','4','6','1','D','7','E','4','D','7','E','4','1','2','3','4','1','2','3','4','1','2','3','4','1','2','3','4'};
char end[1] = {'~'};
// Converts 16 bits of data into 4 equivalent hex chars
char* convToChar(int idx, char* c, int data) {
    int k = 12;
    
	for(int j = idx; j < idx+4; ++j) {
	    c[j] = toChar((data >> k) & 0xf);
	    k-=4;
	}
	return c;
}
// Writes the message to the socket
void pushBM(int sockfd, char* data, int length) {
    // First character lenght of the message
    write(sockfd, data, length);
    
}
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    
    int s = 1;
 
    // socket create and verification
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
    servaddr.sin_addr.s_addr = inet_addr("192.168.0.30");
    servaddr.sin_port = htons(PORT);
 
    // connect the client socket to server socket
    while(connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        // wait for connection
    }
    printf("connected to the server..\n");
    
    
    pushBM(sockfd,tst2,37);
    
    pushBM(sockfd,tst1,13);
    
    pushBM(sockfd,tst2,37);
    
    pushBM(sockfd,tst1,13);
    
    pushBM(sockfd,tst2,37);
    
    pushBM(sockfd,tst4,69);
    
    pushBM(sockfd,tst2,37);
    
    pushBM(sockfd,tst3,33);
    
    pushBM(sockfd,end,1);
    
    printf("end\n");
    
    close(sockfd);
}
