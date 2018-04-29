#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <limits.h>
#include "echo_common.h"
#define  BUFFER_SIZE 256
#define  TIMEOUT     3000
#define  TRUE  1
#define  FALSE 0

struct str_result {
    struct timespec start;
    struct timespec end;
    long   latency;
    int    timeout;
};

void die_with_error(char *errorMessage) {
    perror(errorMessage);
    exit(1);
}

int main(int argc, char* args[]) {
    int    sock;
    struct addrinfo hints, *res;
    char   buffer[BUFFER_SIZE];
    char   recv_mes[BUFFER_SIZE];
    int    count, i;
    struct str_result *result;

    /* check argumets & initialize */
    if ( argc != 4 ){
	printf("Usage: %s $CONT $SERVER_LISTEN_IP $MESSAGE\n", args[0]);
	exit(1);
    }
    count = atoi(args[1]);
    strncpy(buffer, args[3], sizeof(buffer));

    result = (struct str_result *)malloc(sizeof(struct str_result)*count);
    if( result == NULL ){
        die_with_error("can not malloc");
    }

    /* resolv target */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;  /* IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */     
    hints.ai_flags    = 0;
    hints.ai_protocol = 0;          /* Any protoclo */

    i = getaddrinfo(args[2], PORT_STRING, &hints, &res);
    if( i != 0 ){
        die_with_error("error in getaddrinfo");
    }
      
    /* create socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if( sock < 0 ){
        die_with_error("can not create socket");
    }

    for(i=0; i< count; i++){
        struct pollfd fds[0];
        int ret;

        /* initialize recv_mes */
        memset(recv_mes, 0, sizeof(recv_mes));

        /* set fds for poll */
        fds[0].fd     = sock;
        fds[0].events = POLLIN | POLLERR; 
 
        /* send message */
        (void)clock_gettime(CLOCK_REALTIME, &(result[i].start));
        (void)sendto(sock, args[3], strlen(args[3]), 0, res->ai_addr, res->ai_addrlen);

        /* receive message */
        ret = poll(fds,1,TIMEOUT);
        if( ret == -1 ){
            die_with_error("udp receive error");
        }else if( ret == 0 ){
            /* No data within interval time */
            (void)clock_gettime(CLOCK_REALTIME, &(result[i].end));
            result[i].timeout = TRUE;

            printf("time out!!\n");
        }else if( fds[0].revents & POLLIN ){
            /* receive data is available */
            (void)recv(sock, recv_mes, sizeof(recv_mes), 0);
            (void)clock_gettime(CLOCK_REALTIME, &(result[i].end));
            result[i].timeout = FALSE;

            printf("receive=%s ",recv_mes);
        }else{
            die_with_error("why?");
        }

        /* calculate latency */
        result[i].latency = ( (long)(result[i].end.tv_sec - result[i].start.tv_sec)*1000L
                             +(long)(result[i].end.tv_nsec - result[i].start.tv_nsec)/1000000L )/2;
        printf("latency=%ld ms\n", result[i].latency);

        /* interval */
        (void)usleep( 300000 );

    }

    /* close socket */
    close(sock);

    /* calculate average */
    long long average, total, min, max, loss;
    int valid, invalid;
    average = total = valid = invalid = 0;
    min = LLONG_MAX;
    max = -1;
    for(i=0; i<count; i++){
        struct str_result *pt = result + i;
        if( pt->timeout ){
            invalid++;
        }else{
            valid++;
            total += pt->latency;
            if( pt->latency < min ){
                min = pt->latency;
            }
            if( pt->latency > max ){
                max = pt->latency;
            }
        }
    }
    average = total / valid;
    loss    = (long long)invalid*10000 / (long long)(invalid+valid);

    printf("average(ms), min(ms), max(ms), loss(%%x100), valid, invalid, host\n");
    printf("result= %lld, %lld, %lld, %lld, %d, %d, %s\n",
           average, min, max, loss, valid, invalid, args[2]);

    for(i=0; i<count; i++){
        struct str_result *pt = result + i;
        printf("%3d %d %ld.%ld %ld.%ld latency=%ld ms\n",
               count,
               pt->timeout,
               pt->start.tv_sec,
               pt->start.tv_nsec,
               pt->end.tv_sec,
               pt->end.tv_nsec,
               pt->latency);
    }
    return 0;
}
