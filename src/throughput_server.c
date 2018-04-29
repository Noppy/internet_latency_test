#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "throughput_common.h"
#define  BUFFER_SIZE 1024 * 1024

void die_with_error(char *errorMessage) {
    perror(errorMessage);
    exit(1);
}

int main(int argc, char* args[]){

    int    sock, socktmp;
    struct sockaddr_in clientAddr;
    int    clientAddrLength;
    struct addrinfo hints, *res;
    char   buffer[BUFFER_SIZE];
    char   delimiter[256];
    int    delimiter_len;
    int    ret;

    /* set delimiter code */
    strncpy(delimiter, DELIMITER_CODE, sizeof(delimiter));
    delimiter_len = strlen(delimiter);

    /* resolv address */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;     /* Only IPv4 */
    hints.ai_socktype = SOCK_STREAM; /* stream socket */
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_protocol = 0;          /* Any protoclo */
 
    ret = getaddrinfo(NULL, PORT_THROUGH, &hints, &res);
    if( ret != 0 ){
        die_with_error("error in getaddrinfo");
    }

    /* create socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if( sock < 0 ){
        die_with_error("can not create socket");
    }

    /* bind & listen */
    ret = bind(sock, res->ai_addr, res->ai_addrlen);
    if( ret < 0 ){
	die_with_error("bind() failed\n");
    }

    ret = listen(sock, 10);
    if( ret != 0 ){
        die_with_error("can not listen");
    }
    printf("Waiting client connected...\n");
  
    /* main loop */
    for( ;; ){
	clientAddrLength = sizeof(clientAddr);

        /* accept */
        socktmp = accept(sock, (struct sockaddr *)&clientAddr, &clientAddrLength); 

        /* recive data */
        printf("accept a session\n");
        ssize_t size, ret_recv;
        size = 0;
        for( ;; ){ 
            ret_recv = recv(socktmp, buffer, sizeof(buffer), 0);
            if( ret_recv < 0 ){
                perror("recive data");
                break;
            }
            if( strncmp( buffer, delimiter, delimiter_len ) == 0 ){
                break;
            }
            size += ret_recv; 
        }
        printf("recieve %zu byte\n", size);

        /* send data */
        //memset(buffer, 0xFF, sizeof(buffer) );
        
        /* close */
        close(socktmp);
        printf("close a session\n");
    }
    close(sock);
    
    return 0;
}
