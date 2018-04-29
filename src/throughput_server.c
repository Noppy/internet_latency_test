#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "throughput_common.h"
#define  BUFFER_SIZE 1024 * 1024
#define  TIMEOUT            3000
#define  KEEPALIVE_DELAY    10
#define  KEEPALIVE_INTERVAL 5
#define  KEEPALIVE_COUNT    2

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
    pid_t  pid, cpid, status;
    int    ret;

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
    int option;
    option=1;                 setsockopt( sock, SOL_SOCKET,  SO_KEEPALIVE, (void *)&option, sizeof(option) );
    option=KEEPALIVE_DELAY;   setsockopt( sock, IPPROTO_TCP, TCP_KEEPIDLE, (void *)&option, sizeof(option) );
    option=KEEPALIVE_INTERVAL;setsockopt( sock, IPPROTO_TCP, TCP_KEEPINTVL,(void *)&option, sizeof(option) );
    option=KEEPALIVE_COUNT;   setsockopt( sock, IPPROTO_TCP, TCP_KEEPCNT,  (void *)&option, sizeof(option) );
  
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
        ssize_t size, ret_recv;

        /* accept */
        socktmp = accept(sock, (struct sockaddr *)&clientAddr, &clientAddrLength); 

        pid = fork();
        if( pid < 0 ){
            die_with_error("can not fork");
        }else if( pid != 0 ){
            /* parent process */
            while ((cpid = waitpid(-1, &status, WNOHANG)) > 0);
        }else{        
            /* recive data */
            printf("fork process\naccept a session\n");
            size = 0;
            for( ;; ){ 
                ret_recv = recv(socktmp, buffer, sizeof(buffer), 0);
                if( ret_recv <= 0 ){
                    break;
                }
                if( strncmp(buffer, "EOF", 3) == 0){
                    printf("done\n");
                    break;
                }
                size += ret_recv; 
            }
            printf("recieve %zu byte\n", size);

            /* close */
            close(socktmp);
            printf("close a session\n");
            return(EXIT_SUCCESS);
        }
    }
    close(sock);
    
    return 0;
}
