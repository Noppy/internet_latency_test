#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "echo_common.h"
#define  BUFFER_SIZE 256

void die_with_error(char *errorMessage) {
    perror(errorMessage);
    exit(1);
}

int main(int argc, char* args[]){

    int    sock;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    unsigned int clientAddrLength;
    char echoBuffer[BUFFER_SIZE];
    int receivedMessageSize;

    /* create socket */
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if( sock < 0 ){
	die_with_error("socket() failed\n");
    }

    /* set server address */
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port        = htons( PORT );

    /* bind */
    int bindResult = bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if( bindResult < 0 ){
	die_with_error("bind() failed\n");
    }

    printf("Waiting client connected...\n");
    
    for( ;; ) {
	memset(echoBuffer, 0, sizeof(echoBuffer));
	clientAddrLength = sizeof(clientAddr);
  
        /* read datagram from the socket */
	receivedMessageSize = recvfrom(sock, echoBuffer, sizeof(echoBuffer), \
                                       0, (struct sockaddr *)&clientAddr, &clientAddrLength);
	if( receivedMessageSize < 0 ){
	    die_with_error("recvfrom() error\n");
	}else{
	    printf("Client connected, %s\n", inet_ntoa(clientAddr.sin_addr));
	    printf("%s\n", echoBuffer);

            /* send it backf */
            sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr *)&clientAddr, clientAddrLength);
        }
    }

    close(sock);
    
    return 0;
}
