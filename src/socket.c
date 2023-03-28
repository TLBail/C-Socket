#include <stdio.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct Packet {
    int token;
    int source;
    int destination;
    char payload[255];
};


int udpPortReception;
int udpPortEnvoie;


int lePacketMestDestiner(struct Packet* packet) {
    return packet->destination == udpPortReception;
}

int lePacketPossedeUnMessage(struct Packet* packet) {
    return packet->token;
}


void traiterPacket(struct Packet* packet) {
    if (lePacketPossedeUnMessage(packet)) { // pas de massage
        printf("pas de packet gen d'un packet à destination de 8000\n");
        packet->token = 0;
        packet->source = udpPortEnvoie;
        packet->destination = 8000;
        strcpy(packet->payload, "salut");
    }
    else {
        //packet 
        if (lePacketMestDestiner(packet)) {
            printf("packet reçu : \n %s", packet->payload);
            printf("set packet vide");
            packet->token = 1;
        }
        else {
            printf("transfert du packet ");
        }
    }
}


int main(int argc, char* argv[]) {
    int sock_S;
    struct sockaddr_in sockAddrReception;
    struct sockaddr_in sockAddrEnvoie;
    unsigned int taille_sa;

    if (argc == 3) {
        udpPortReception = atoi(argv[1]);
        udpPortEnvoie = atoi(argv[2]);
    }
    else {
        printf("port socket : \n");
        scanf("%d", &udpPortReception);

        printf("port envoie : \n");
        scanf("%d", &udpPortEnvoie);
    }

    printf("socket setup sur \n - le port :  %d reception\n - le  port %d envoie \n", udpPortReception, udpPortEnvoie);

    // création du socket 
    sock_S = socket(PF_INET, SOCK_DGRAM, 0);
    perror("socket S :");

    //setup socket S reception
    bzero((char*)&sockAddrReception, sizeof(struct sockaddr));
    sockAddrReception.sin_family = PF_INET;
    sockAddrReception.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddrReception.sin_port = htons(udpPortReception);

    //bind socket S reception 
    bind(sock_S, (struct sockaddr*)&sockAddrReception, sizeof(struct sockaddr));
    perror("bind  S: ");

    //setup socket address envoie 
    bzero((char*)&sockAddrEnvoie, sizeof(struct sockaddr));
    sockAddrEnvoie.sin_family = AF_INET;
    sockAddrEnvoie.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddrEnvoie.sin_port = htons(udpPortEnvoie);


    taille_sa = sizeof(struct sockaddr);

    while (1) {
        struct Packet packet;
        struct sockaddr_in sockAddrRecvFrom;
        recvfrom(sock_S, &packet, sizeof(struct Packet), 0, (struct sockaddr*)&sockAddrRecvFrom, &taille_sa);
        perror("reception du paquet");
        printf("address reception %d\n", sockAddrRecvFrom.sin_port);
        sleep(2);
        traiterPacket(&packet);
        sendto(sock_S, &packet, sizeof(struct Packet), 0, (struct sockaddr*)&sockAddrEnvoie, taille_sa);
        perror("renvoie du paquet");
    }

    close(sock_S);
    perror("close : ");
    exit(EXIT_SUCCESS);
}