# Protocole Réseau

## Fonctionnement
Des messages sont envoyés dans un anneau.
Chaque message est envoyé à la machine suivante, et la machine suivante l'envoie à la suivante, etc.

Voici comment chaque machine traite un paquet arrivant :

    Si le paquet contient des messages, il vérifie si des messages lui sont adressés ou envoyé par lui même.
    Si c'est le cas, il supprime les messages concernés et on lui retire le droit d'envoyer un nouveau message.

    Si la machine possède toujours le droit d'envoyer un message à la fin du traitement,
        elle choisit un message aléatoire parmis les 10 proposés
        et l'envoit au hasard à une machine du réseau (entre 8000 et 8003) qui n'est pas elle même.

## Architecture
```
|- build  # Fichiers généré par le make, contient les fichiers de sortie.  
|- object # Fichiers généré par le make, contient les fichiers objets.  
|- src  
    |- client.c   # Code chargé d'envoyer le premier paquet et de lancer la boucle.  
    |- socket.c   # Code représentant les machines de la boucle  
```

## Pour compiler
```bash 
$ make 
```

## Pour nettoyer les fichiers pas nécessaire
```bash
$ make clean
```

## Pour lancer automatiquement l'anneau avec 4 utilisateurs

⚠️**Attention**, cette commande nécessite tmux et avoir fait un make au préalable.
```bash
$ make run
```