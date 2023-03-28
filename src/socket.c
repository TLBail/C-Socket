#include <stdio.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


char* duck_messages[] = {
    "Coucou ! Je suis un canard, et toi ?",
    "Moi aussi, je suis un canard ! Quel étang préfères-tu ?",
    "J'adore l'étang près du vieux moulin. Et toi ?",
    "Ah, je préfère celui près de la forêt, il y a toujours de la nourriture là-bas.",
    "As-tu déjà rencontré ce drôle de canard blanc avec le chapeau ?",
    "Oui, je l'ai vu l'autre jour ! Il est vraiment amusant.",
    "Oh, il commence à pleuvoir. Allons nous abriter sous les arbres.",
    "Bonne idée ! Je déteste être mouillé, même si je suis un canard.",
    "Après la pluie, on pourrait chercher des escargots à manger, qu'en dis-tu ?",
    "Excellente idée, j'adore les escargots ! À tout à l'heure, alors."
};


struct Paquet {
    int token;
    int source[3];
    int destination[3];
    char payload[3][255];
};

int udpPortReception;
int udpPortEnvoie;

char* getRandomDuckMessage() {
    int index = rand() % 10;
    return duck_messages[index];
}

int lePaquetMestDestiner(struct Paquet* paquet, int index) {
    return paquet->destination[index] == udpPortReception;
}

int lePaquetPossedeUnMessage(struct Paquet* paquet) {
    return paquet->token != 0;
}

int lePaquetEstDeMoi(struct Paquet* paquet, int index) {
    return paquet->source[index] == udpPortEnvoie;
}

void traiterPaquet(struct Paquet* paquet) {

    int canSend = 1;

    if (lePaquetPossedeUnMessage(paquet)) {
        for (int indexPaquet = 0; indexPaquet < 3; indexPaquet++)
        {
            if (lePaquetMestDestiner(paquet, indexPaquet)) {
                canSend = 0;

                printf("paquet reçu : \n %s", paquet->payload[indexPaquet]);
                printf("set paquet vide\n");
                printf("index paquet %d\n", indexPaquet);
                paquet->token = paquet->token & ~(1 << indexPaquet);
            }
            if (lePaquetEstDeMoi(paquet, indexPaquet)) {
                canSend = 1;

                printf("Destinataire pas trouvé : %d\n", paquet->source[indexPaquet]);
                printf("set paquet vide\n");
                printf("index paquet %d\n", indexPaquet);
                paquet->token = paquet->token & ~(1 << indexPaquet);
            }
        }
    }
    if (canSend)
    {
        // pas de paquet je trouve un slot libre et je gen un paquet
        for (int indexPaquet = 0; indexPaquet < 3; indexPaquet++)
        {
            if ((paquet->token & (1 << indexPaquet)) == 0) { // slot libre
                printf("Pas de paquet gen d'un message\n j'utilise index %d\n", indexPaquet);
                paquet->token = paquet->token | (1 << indexPaquet); //On modifie la valeur token pour occuper le slot.
                paquet->source[indexPaquet] = udpPortEnvoie;

                int destinataire = 0;
                do {
                    destinataire = (rand() % (8003 - 8000 + 1)) + 8000;
                } while (destinataire == udpPortReception);
                paquet->destination[indexPaquet] = destinataire;
                printf("Envoi du message a %d\n", destinataire);

                strcpy(paquet->payload[indexPaquet], getRandomDuckMessage());
                break;
            }
        }

    }
}

int main(int argc, char* argv[]) {

    srand(time(NULL));

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
        struct Paquet paquet;
        recvfrom(sock_S, &paquet, sizeof(struct Paquet), 0, NULL, &taille_sa);
        perror("reception du paquet");
        sleep(2);
        traiterPaquet(&paquet);
        sendto(sock_S, &paquet, sizeof(struct Paquet), 0, (struct sockaddr*)&sockAddrEnvoie, taille_sa);
        perror("renvoie du paquet");
    }

    close(sock_S);
    perror("close : ");
    exit(EXIT_SUCCESS);
}