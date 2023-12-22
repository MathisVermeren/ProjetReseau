#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bank.h"


void initialize_client(Client *client, int client_id, const char *password) {
    if (client == NULL || password == NULL) {
        printf("Erreur d'argument\n ");
        return; // Erreur d'argument
    }

    // Initialiser les champs du client
    client->client_id = client_id;
    strncpy(client->password, password, MAX_PASSWORD_LENGTH - 1);
    client->password[MAX_PASSWORD_LENGTH - 1] = '\0';
    client->num_accounts = 0;
    printf("Client créé avec succès\n");
    return; // Initialisation réussie
}

void add_account(Client *client, int account_id, const char *password) {
    if (client == NULL || password == NULL) {
        printf("Erreur d'argument\n ");
        return; // Erreur d'argument
    }

    // Vérifier si le client a atteint le nombre maximum de comptes
    if (client->num_accounts >= MAX_ACCOUNTS) {
        printf("Nombre maximum de comptes atteint\n ");
        return; // Nombre maximum de comptes atteint
    }

    // Vérifier si le compte existe déjà
    for (int i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            printf("Le compte existe déjà\n ");
            return; // Le compte existe déjà
        }
    }

    // Ajouter le compte
    client->accounts[client->num_accounts].account_id = account_id;
    client->accounts[client->num_accounts].balance = 0.0;
    client->num_accounts++;
    printf("Compte créé avec succès\n");
    return; // Ajout de compte réussi
}

void ajout(Client *client, int client_id, int account_id,  const char *password, char *res, double amount) {
    // Vérifier l'authentification du client (remplacez cela par votre logique d'authentification)
    printf("Translated : AJOUT %d %d %s %f\n",client->client_id, account_id, password, amount);
    if (strcmp(client->password, password) != 0) {
        strncpy(res,"KO",255); // Authentification échouée
    }

    // Rechercher le compte
    int i;  
    for (i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            // Ajouter la somme au solde du compte
            client->accounts[i].balance += amount;
            strncpy(res,"OK : Opération réussie",255);
        }
    }

    strncpy(res,"KO : Compte non trouvé",255); 
    return;
}

void retrait(Client *client, int client_id, int account_id, const char *password, char *res, double amount) {
    // Vérifier l'authentification du client (remplacez cela par votre logique d'authentification)
    if (strcmp(client->password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255);
    }

    // Rechercher le compte
    int i;
    for (i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            // Vérifier si le solde est suffisant
            if (client->accounts[i].balance >= amount) {
                // Retirer la somme du solde du compte
                client->accounts[i].balance -= amount;
                strncpy(res,"OK : Opération réussie",255);
            } else {
                strncpy(res,"KO : Solde insuffisant",255);
            }
        }
    }

    strncpy(res,"KO : Compte non trouvé",255); 
    return;
}

void solde(Client *client, int client_id, int account_id, const char *password, char *res) {
    // Vérifier l'authentification du client (remplacez cela par votre logique d'authentification)
    if (strcmp(client->password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255); 
    }

    // Rechercher le compte
    int i;
    for (i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            // Formater la réponse
            sprintf(res,"RES_SOLDE %.2f date_dernière_opération\n", client->accounts[i].balance);
        }
    }

    strncpy(res,"KO : Compte non trouvé",255);
    return;
}

void operations(Client *client, int client_id, int account_id, const char *password, char *res) {
    // Vérifier l'authentification du client (remplacez cela par votre logique d'authentification)
    if (strcmp(client->password, password) != 0) {
        strncpy(res,"KO : Authentification échouée",255); // Authentification échouée
    }

    // Rechercher le compte
    int i;
    for (i = 0; i < client->num_accounts; i++) {
        if (client->accounts[i].account_id == account_id) {
            // Formater la réponse (vous devez remplacer cela par votre logique pour récupérer les 10 dernières opérations)
            printf("RES_OPERATIONS type_opération date_opération montant_operation\n");
            strncpy(res,"OK : Opération réussie",255); 
        }
    }

    strncpy(res,"KO : Compte non trouvé",255); 
    return;
}

void process_command(Client *client, char *buffer, char *res) {
    printf("Received from %s: %s\n", client->name, buffer);
    fflush(stdout);
    char command[20];
    int id_client, id_compte;
    char password[MAX_PASSWORD_LENGTH];
    double somme = 0.0; // Initialiser à zéro
    // Utiliser sscanf pour extraire les arguments du buffer
    int result = sscanf(buffer, "%s %d %d %s %lf", command, &id_client, &id_compte, password, &somme);
    printf("Command : %s\n",command);
    if (result < 3) {
        strncpy(res,"KO : Le format de la commande est invalide",255);
    }
    // Traiter la commande en fonction du type
    if (strcmp(command, "AJOUT") == 0 || strcmp(command, "RETRAIT") == 0) {
        if (result < 5) {
            strncpy(res,"KO : Pas suffisamment d'arguments pour AJOUT ou RETRAIT",255);
        }
    }
    // Traiter la commande en fonction du type
    if (strcmp(command, "AJOUT") == 0) {
        ajout(client, id_compte, password, res, somme);
    } else if (strcmp(command, "RETRAIT") == 0) {
        retrait(client, id_compte, password, res, somme);
    } else if (strcmp(command, "SOLDE") == 0) {
        solde(client, id_compte, password, res);
    } else if (strcmp(command, "OPERATIONS") == 0) {
        operations(client, id_compte, password, res);
    } else {
        strncpy(res,"KO : Commande inconnue",255);
    }
    printf("Response : %s\n",res);
    
    return;
}
