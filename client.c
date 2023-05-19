#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


/***************** ADMIN_MENU **********/

int ADMIN_MENU(){
int choix;
printf("\n*******************MENU*******************\n");
printf("         1-Ajouter un contact \n");
printf("         2-Rechercher un contact \n");
printf("         3-Supprimer un contact \n");
printf("         4-Modifier un contact \n");
printf("         5-Afficher tous les contacts \n");
printf("         6-Quitter \n");
printf("\n\n   Entrez votre choix : ");
scanf("%d",&choix);
return choix;
}

/*********  visiteur_menu **********/

int VISITEUR_MENU(){
int choix;
printf("\n***************MENU*********************\n");
printf("         1-Rechercher un contact  \n");
printf("         2-Afficher tous les contacts \n");
printf("         3-Quitter \n");
printf("\n\n   Enrez votre choix : ");
scanf("%d",&choix);
return choix;
}

/****** les structures ***********/

typedef struct {
    char rue[50];
    char ville[30];
    char pays[30];
}Adresse;

typedef struct Contact{
    char    nom[50];
    char prenom[50];
    long        GSM;
    char  email[50];
    Adresse  adresse;
}Contact;



/******* la fonction INFORMATIONS *********/


Contact INFORMATIONS(){
   Contact c;
   printf("Donner le nom du contact : ");
   scanf("%s",c.nom);
   printf("Entrez le prenom du contact: ");
   scanf("%s",c.prenom);
   printf("Entrez le GSM : ");
   scanf("%ld",&c.GSM);
   printf("Entrez l'email' : ");
   scanf("%s",c.email);
   printf("  \n Saisie de l'adresse:\n\n");
   printf("veuillez entrer le nom de la rue:  ");
   scanf("%s",c.adresse.rue);
   printf("veuillez entrer la ville du contact:  ");
   scanf("%s",c.adresse.ville);
   printf("veuillez entrer le pays du contact:  ");
   scanf("%s",c.adresse.pays);
   printf("\n\nContact saisi %s#%s#%s#%ld#%s#%s#%s \n",c.nom,c.prenom,c.email,c.GSM,c.adresse.rue,c.adresse.ville,c.adresse.pays);
return c;
}



/****************    MAIN    ******************/

int main()
{
    // creation du socket
    int network_socket;
    network_socket=socket(AF_INET,SOCK_STREAM,0);

    // definir une adresse pour socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = inet_addr("192.168.64.1");

    // conexion au socket
    int connection_status = connect(network_socket,(struct sockaddr *) &server_address, sizeof(server_address));

    // tester la connexion
    if (connection_status == -1){
        printf("Erreur de connexion \n\n");
    }

    // reception des donnees
    char server_response[256]; // pour stocker les donnees
    recv(network_socket,&server_response,sizeof(server_response),0);

    // print les donnees
    printf("le serveur a envoyer : %s \n",server_response);


    ////////////////////////// TESTER LE LOGIN ET PASSWORD /////////////////////////////////////////////

    char USERNAME[100];
    char MOT_DE_PASSE[30];
    int n=0;
    int TESTING=0;


         printf("RQ : vous avez le droit d'enter comme -Admin ou -Invite seulement \n");
         printf("votre login : ");
         scanf("%s",USERNAME);
         send(network_socket ,USERNAME , strlen(USERNAME), 0 );   // envoyer username au serveur

    do {
        printf("votre mot de passe (vouz avez %d essai!) : ",3-n);
        scanf("%s",MOT_DE_PASSE);
        send(network_socket , MOT_DE_PASSE , strlen(MOT_DE_PASSE), 0 );   // envoyer mot de passe au serveur

        recv( network_socket , &TESTING, sizeof(TESTING),0);  //recv la valeur du testing 1 pour admin 2 pour invite et zero sinon

        printf("message envoye \n");
        fflush(stdin);
        n++;

    } while(TESTING==0 && n<3 );   //ressayer 3 fois au maximum

    int choix;
    int REPONCE=0;
    Contact c;
    Contact c1;
    long gsm;
    long gsm1;
     if (TESTING==1) {

               printf("--------------Vous etes indministrateur ----------------------\n\n");
               do {
                      choix=ADMIN_MENU();             //afficher menu de l'admin
                      printf("choix est %d \n",choix);
                      send(network_socket , &choix ,sizeof(choix), 0 );

                      switch (choix){

                        case 1 :  c=INFORMATIONS();
                                 send(network_socket , &c , sizeof(c) , 0 );    // envoyer les nouveaux donnees au serveur pour les ajouter
                                 break;


                        case 2 : { printf("entrez le gsm du contact recherer: ");
                                   scanf("%ld",&gsm);
                                   send(network_socket , &gsm, sizeof(gsm) , 0 );            //envoyer gsm au serveur
                                   recv(network_socket , &REPONCE , sizeof(REPONCE) , 0 );    // recc reponce

                                    if(REPONCE){
                                      printf("le contact existe \n ");
                                    }
                                    else printf("le contact n'existe pas \n ");
                                    break;
                        }


                        case 3 : { printf("entrez le gsm du contact a supprimer: ");
                                   scanf("%ld",&gsm);
                                   send(network_socket , &gsm, sizeof(gsm) , 0 );      //envoyer gsm au serveur
                                   recv(network_socket , &REPONCE , sizeof(REPONCE) , 0 );              // recc reponce

                                    if(REPONCE){
                                      printf("suppression avec succes \n ");
                                    }
                                    else printf("le contact n'existe pas \n ");
                                    break;
                        }

                        case 4 : { printf("entrez le gsm  de contact a modifier: ");
                                   scanf("%ld",&gsm);
                                   send(network_socket , &gsm, sizeof(gsm) , 0 );

                                   printf(" Saise les nouvelles donnees du contatcs  \n");
                                   c = INFORMATIONS();

                                   send(network_socket , &c, sizeof(c) , 0 );     //envoyer les nouveaux donnees au serveur
                                   getchar();

                                   recv(network_socket , &REPONCE , sizeof(REPONCE) , 0 );
                                    if(REPONCE){
                                      printf(" la modificatin est fait avec  succes \n ");
                                    }
                                    else printf("le contact n'existe pas \n ");
                                    break;
                        }
                        case 5: {

                                fflush(stdin);
                                recv(network_socket, &c1, sizeof(Contact), 0);
                                 gsm1=c1.GSM;
                                 while(recv(network_socket, &c1, sizeof(Contact), 0)!=-1 && gsm1!=c1.GSM )
                                {
                               gsm1=c1.GSM;
                               printf("%s %s %ld %s %s %s %s \n",c1.nom,c1.prenom,c1.GSM,c1.email,c1.adresse.rue,c1.adresse.ville,c1.adresse.pays);
                                }
                                      printf("l'affichage a  realiser avec succes\n");
                                break; }

                        case 6 : {
                                       close(network_socket); break;
                                 }

                         default :
                                         printf("votre choix %d  est invalide\n\n ",choix);
                                 break;
                      }

               } while(choix !=6);
     }
     else if (TESTING==2){
               printf("--------------Vous etes ivite ----------------------\n");
              do {
                      choix=VISITEUR_MENU();

                        if (choix==1) choix=2;         // 1 pour choix de rechercher  qui est 2eme choix
                        else if (choix==2) choix=5;    // 2 pour choix d'afficher qui est 5eme choix
                        else if (choix==3) choix=6;    // 3 pour choix d'arreter qui est 6eme choix

                        send(network_socket , &choix ,sizeof(choix), 0 );

                     switch (choix){

                    case 5: {       //fonction d'affichage

                           fflush(stdin);
                           recv(network_socket, &c1, sizeof(Contact), 0);
                           gsm1=c1.GSM;
                            while(recv(network_socket, &c1, sizeof(Contact), 0)!=-1 && gsm1!=c1.GSM )
                           {
                            gsm1=c1.GSM;
                          printf("%s %s %ld %s %s %s %s \n",c1.nom,c1.prenom,c1.GSM,c1.email,c1.adresse.rue,c1.adresse.ville,c1.adresse.pays);
                            }
                                printf("\n\nl'affichage a realiser avec succes\n");
                            break;
                            }


                                  //fonction de recherche

                    case 2 : { printf("entrez le gsm du contact recherer: ");
                                   scanf("%ld",&gsm);
                                   send(network_socket , &gsm, sizeof(gsm) , 0 );
                                   recv(network_socket , &REPONCE , sizeof(REPONCE) , 0 );
                                    if(REPONCE){
                                      printf("le contact existe \n ");
                                    }
                                    else printf("le contact n'existe pas \n ");
                                    break;}

                    case 6 : {
                                  close(network_socket); break;
                              }

                    default :
                                 printf("votre choix est invalide \n\n ");
                                 break;
                      }

               } while(choix !=6);
     }
     else{
        printf(" mode non specifier \n ");
        exit(1);
     }

    //fermer socket
    close(network_socket);
    return 0;
}