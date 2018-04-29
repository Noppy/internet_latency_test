#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
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

struct str_result {
    struct timespec start;
    struct timespec end;
    long   time;
    long   throughput;
};

int main(int argc, char* args[]){

    int    sock;
    struct addrinfo hints, *res;
    char   buffer[BUFFER_SIZE];
    int    size_mb;
    int    ret, i;
    struct str_result Data;

    /* check argumets & initialize */
    if ( argc != 3 ){
        printf("Usage: %s $SIZE(MB) $SERVER_LISTEN_IP\n", args[0]);
        exit(1);
    }
    size_mb = atoi(args[1]);
    memset(buffer, 0xFF, sizeof(buffer) );

    /* resolv address */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;     /* Only IPv4 */
    hints.ai_socktype = SOCK_STREAM; /* stream socket */
    hints.ai_flags    = 0;
    hints.ai_protocol = 0;          /* Any protoclo */
 
    ret = getaddrinfo(args[2], PORT_THROUGH, &hints, &res);
    if( ret != 0 ){
        die_with_error("error in getaddrinfo");
    }

    /* create socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if( sock < 0 ){
        die_with_error("can not create socket");
    }

    /* connect */
    ret = connect(sock, res->ai_addr, res->ai_addrlen);
    if( ret < 0 ){
	die_with_error("connect() failed\n");
    }
  
    /* throughput test */
    printf("send data\n");
    (void)clock_gettime(CLOCK_REALTIME, &Data.start);
    for(i=0; i<size_mb; i++){
        ret = send(sock, buffer, sizeof(buffer), 0);
        if( ret < 0 ){
            die_with_error("error in send");
        }     
    }
    (void)clock_gettime(CLOCK_REALTIME, &Data.end);
    printf("fin\n");

    /* close */
    close(sock);

    /* purint result */
    printf("         time(sec), size(kbit), throughput(Kbps),host\n");
    printf("result=, %d, %ld, %ld, %s\n",
                (int)(Data.end.tv_sec - Data.start.tv_sec),
                (long)size_mb << 13, 
                (long)(((long)size_mb) << 13 )/(long)(Data.end.tv_sec - Data.start.tv_sec),
                args[2]);

    return 0;
}
