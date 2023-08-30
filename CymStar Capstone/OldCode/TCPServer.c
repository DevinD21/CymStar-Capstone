//===============================================================================
// Classification: UNCLASSIFIED
// Description:    Main processing for the CymIO simulation model.
//                  Handles data acquisition for the RACKtangle I/O system.
// Author:         CymSTAR (Garrett Meidl)
// Date:           12/5/2022
//===============================================================================
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
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr
FILE * BM;
// Switch case for length of message
int len(char c) {
    switch(c) {
        case 'a' : return 1;
        case 'b' : return 2;
        case 'c' : return 3;
        case 'd' : return 4;
        case 'e' : return 5;
        case 'f' : return 6;
        case 'g' : return 7;
        case 'h' : return 8;
        case 'i' : return 9;
        case 'j' : return 10;
        case 'k' : return 11;
        case 'l' : return 12;
        case 'm' : return 13;
        case 'n' : return 14;
        case 'o' : return 15;
        case 'p' : return 16;
        case 'q' : return 17;
        case 'r' : return 18;
        case 's' : return 19;
        case 't' : return 20;
        case 'u' : return 21;
        case 'v' : return 22;
        case 'w' : return 23;
        case 'x' : return 24;
        case 'y' : return 25;
        case 'z' : return 26;
        case '!' : return 27;
        case '.' : return 28;
        case '[' : return 29;
        case ']' : return 30;
        case '(' : return 31;
        case ')' : return 32;
        case '{' : return 33;
    }
}
// Parses the incoming words
int parse(char* data,int x) {
    int n;
    if(!BM) {
        return 1;
    }
    for(int i = 1; i <= x; i++) {
        if(i == 1) {
            fprintf(BM, "command:%c", data[i-1]);
        }
        else if(i == 4) {
            fprintf(BM, "%c data:", data[i-1]);
        }
        else if(i == x) {
            fprintf(BM, "%c\n", data[i-1]);
        }
        else if(i%4 == 0) {
            fprintf(BM, "%c ", data[i-1]);
        }
        else {
            fprintf(BM, "%c", data[i-1]);
        }
    }
    return 0;
}
// Reads Bus Monitor messages
int BMpull(int connfd) {
    char buff[256];
    int x = 0;
    int y = 0;
    int z = 1;
    x = read(connfd, buff, 1);
    if(buff[0] == '~') {
       return 0;
    }
    if(x <=0) {
        return 1;
    }
    while(x==1) {
        char readsize = buff[0];
        y = len(readsize) * 4;
        read(connfd, buff, y);
        if(parse(buff,y)){
            printf("No file found");
        }
        x = read(connfd, buff, 1);
        if(buff[0] == '~') {
            return 0;
        }
    }
    return 1;
}
// Initialization function
int* createSocket(void)
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    int* x;
    int out[2];
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    // Binding newly created socket to given IP and verification
    while((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
    }
    printf("Socket successfully binded..\n");
   
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    while (connfd < 0) {
        printf("server accept failed...\n");
        connfd = accept(sockfd, (SA*)&cli, &len);
    }
    printf("server accept the client...\n");
    out[0] = connfd;
    out[1] = sockfd;
    x = out;
    BM = fopen("BMdata.txt", "w");
   
   return x;
}
// Close the Socket
int terminateSocket(int sockfd, int connfd) {
    char buff[4] = {'e','x','i','t'};
    write(connfd, buff, strlen(buff));
    close(sockfd);
}
