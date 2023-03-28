#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"

int main(int argc, char* argv[]) {


    struct Paquet {
        int token;
        int source[3];
        int destination[3];
        char payload[3][255];
    };

    int UDP_Port;

    if (argc == 2)
        UDP_Port = atoi(argv[1]);
    else {
        printf("Sur quel port lancer la boucle ?\n");
        scanf("%d", &UDP_Port);
    }
    int sockFD = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(IP);
    sa.sin_port = htons(UDP_Port);

    socklen_t tailleSA_IN = sizeof(struct sockaddr_in);

    struct Paquet message;
    message.token = 0;

    sendto(sockFD, &message, sizeof(struct Paquet), 0, (struct sockaddr*)&sa, tailleSA_IN);
    perror("sendto");

    close(sockFD);

    exit(EXIT_SUCCESS);
}