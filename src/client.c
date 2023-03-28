#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define IP_addr "127.0.0.1"
//82.66.143.165
int main() {
    int sock_C;
    struct sockaddr_in sa_S;
    unsigned int taille_sa_S;
    char buffer[120];

    printf("port client : \n");
    int udpPort = 0;
    scanf("%d", &udpPort);




    sock_C = socket(PF_INET, SOCK_DGRAM, 0);
    perror("socket");
    bzero((char*)&sa_S, sizeof(struct sockaddr));
    sa_S.sin_family = AF_INET;
    sa_S.sin_addr.s_addr = inet_addr(IP_addr);
    sa_S.sin_port = htons(udpPort);

    taille_sa_S = sizeof(struct sockaddr_in);

    struct Message {
        int token;
        int source;
        int destination;
        char payload[255];
    };

    struct Message message;
    message.token = 1;
    sendto(sock_C, &message, sizeof(struct Message), 0, (struct sockaddr*)&sa_S, taille_sa_S);
    perror("sendto");

    recvfrom(sock_C, buffer, 120 * sizeof(char), 0, (struct sockaddr*)&sa_S, &taille_sa_S);
    perror("recvfrom");

    printf("%s", buffer);

    close(sock_C);
    perror("close");

    exit(EXIT_SUCCESS);
}