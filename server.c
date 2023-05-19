#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>



/****les structures : adress et contact ******/

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


/**  FUNCTION ADD ***/


void ADD(char *nom_fich,Contact c){
    FILE *f=fopen(nom_fich,"a");
    if(f!=NULL){
        fprintf(f,"%s %s %ld %s %s %s %s\n",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);
        fclose(f);
        printf("\n------- un Contact a ete ajoute !!!-------!!!\n\n");
    }
    else
        printf("impossible d'ouvrire le fichier !!!");

}

/** fonction DELETE_WITH_GSM pour supprimer en utilisons gsm *******/

int DELETE_WITH_GSM(char *nom_fichier,long gsm) {
    char nom_fichier1[]="ListesContactes.txt";
    Contact c;
    int r=0;
    FILE *F_in=fopen(nom_fichier,"r");
    FILE *F_out=fopen(nom_fichier1,"w");
    while (fscanf(F_in, "%s %s %ld %s %s %s %s", c.nom, c.prenom, &c.GSM, c.email, c.adresse.rue, c.adresse.ville, c.adresse.pays) == 7){
        if (gsm == c.GSM) {
            r=1;
        }
        else {
            fprintf(F_out,"%s %s %ld %s %s %s %s\n",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);
        }
    }
    fclose(F_out);
    fclose(F_in);
    remove(nom_fichier);
    rename(nom_fichier1,nom_fichier);
    return r;
}


/** fonction MODIFIER  *******/

int MODIFIER(char *nom_fichier,long gsm, Contact c1) {
    Contact c;
    int r=0;
    char nom_fichier1[]="ListesContactes.txt";
    FILE *F_in=fopen(nom_fichier,"r");
    FILE *F_out=fopen(nom_fichier1,"w");
    while(!feof(F_in)) {
    if(fscanf(F_in,"%s%s%ld%s%s%s%s",c.nom,c.prenom,&c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays)==7){
        if (gsm == c.GSM) {
            fprintf(F_out,"%s %s %ld %s %s %s %s\n",c1.nom,c1.prenom,c1.GSM,c1.email,c1.adresse.rue,c1.adresse.ville,c1.adresse.pays);
            r=1;
        }
        else {
            fprintf(F_out,"%s %s %ld %s %s %s %s\n",c.nom,c.prenom,c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays);

        } }
    }
    fclose(F_out);
    fclose(F_in);
    remove(nom_fichier);
    rename(nom_fichier1,nom_fichier);
    return r;
}

/*** fonction Rechercher en utilisons GSM  *****/

int RESEARCH_WITH_GSM(char *nom_fichier, long gsm) {
    FILE *f=fopen(nom_fichier,"r");
    Contact c;
    int r=0;
    while(!feof(f)) {
       if (fscanf(f,"%s%s%ld%s%s%s%s",c.nom,c.prenom,&c.GSM,c.email,c.adresse.rue,c.adresse.ville,c.adresse.pays)){
        if (gsm==c.GSM) {
            r=1;
            printf("\n---------------------- Contact existe!!!!-----------------------\n");

            break;     
        }
    }}
    fclose(f);
    if (r == 0) {
        printf("\nContact introuvable !!!!");
    }
    return r;
}




int main()
{
    char server_message[256] = "vous avez connectez au serveur";

    // creation du server socket
    int server_socket;
    server_socket=socket(AF_INET,SOCK_STREAM,0);

    // definir une adresse pour socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;
    //inet_addr("192.168.64.255"); 

    // bind socket to our specified IP address
    bind(server_socket,(struct sockaddr *) &server_address, sizeof(server_address));
    listen(server_socket,5); //5 combien des sockets seront connectes

     int client_socket;
     client_socket = accept(server_socket, NULL, NULL);
    

    // envoi des donnees
    send(client_socket,server_message,sizeof(server_message),0);


    
    //////////////////////////// tester LOGIN et password //////////////////////////////////////////

   
    char MOT_DE_PASSE[30];    // mot de passe
    char LOGIN[50],nom[50];   //login et USERNAME=nom
    char motpasse[10];       
    char PROFIL[50];      // profil : admin ou invite
    int testvalue=0;
    int TESTING=0;       // testing : 1 pour admin et 2 pour invite
     

    FILE *f=fopen("users.txt","r");
    if(f==NULL){
       printf("impossible d'ouvrire le fichier users.txt \n");
       exit(1);
    }
    else{
    printf("Authentification \n ");
    bzero(nom,50);
    recv( client_socket ,nom, sizeof(nom),0);
    printf("%s \n ",nom);
    while(!feof(f)){
    fscanf(f,"%s%s%s",LOGIN,motpasse,PROFIL);
    if(strcmp(nom,LOGIN)==0){
         while( testvalue <=3){
            bzero(MOT_DE_PASSE,30);
            recv( client_socket , MOT_DE_PASSE, sizeof(MOT_DE_PASSE),0);
            printf("%s \n ",MOT_DE_PASSE);
               if(strcmp(MOT_DE_PASSE,motpasse)==0) {
                      if(strcmp(PROFIL,"admin")==0){
                          TESTING=1;
                          send(client_socket , &TESTING , sizeof(TESTING) , 0 );
                          break;

                      }
                     else if(strcmp(PROFIL,"visit")==0){
                           TESTING=2;
                           send(client_socket , &TESTING , sizeof(TESTING) , 0 );
                           break;
                     }
               }
         TESTING=0;
         send(client_socket , &TESTING , sizeof(TESTING) , 0 );
         testvalue++;
       }

    }
    else{
    TESTING=-1;
    }
       }
       send(client_socket , &TESTING , sizeof(TESTING) , 0 );
    }

//////////////////////////////    tester les fonction selon le choix       //////////////////////////////////////////////

int choix=0;
Contact c;
Contact c1;
int REPONCE=0;
long  gsm;

  do {
        recv(client_socket , &choix , sizeof(choix),0);     //recevoire choix du client 

                      switch (choix){


                        case 1 : {

                                 recv( client_socket , &c , sizeof(c),0);      //recevoire les informations pour les ajouter
                                 ADD("contacts.txt",c);                    //ajouter les nouveaux infos sur fichier cntacts.txt
                                 REPONCE=1;
                                 send(client_socket , &REPONCE , sizeof(REPONCE), 0);     //validation de reponce
                                  break;}


                        case 2 :{

                              recv(client_socket , &gsm, sizeof(gsm), 0);
                              REPONCE=RESEARCH_WITH_GSM("contacts.txt",gsm);          //rechercher sur les contacts par gsm
                              send(client_socket , &REPONCE , sizeof(REPONCE), 0);
                              break;}


                        case 3 :{
                              recv(client_socket , &gsm, sizeof(gsm), 0);
                              REPONCE=DELETE_WITH_GSM("contacts.txt",gsm);            //supprimer le contact par gsm 
                              send(client_socket , &REPONCE , sizeof(REPONCE), 0);
                              break;}


                        case 4 :{
                              recv(client_socket , &gsm, sizeof(gsm), 0);           //recevoire le gsm du  contact a modifier
                              recv(client_socket , &c , sizeof(c), 0);             //recevoire les nouveaux informations
                              REPONCE=MODIFIER("contacts.txt",gsm,c);
                              send(client_socket , &REPONCE , sizeof(REPONCE), 0);
                              break;}


                        case 5 :{
                              printf("le choix de l'utilisateur : %d \n",choix);
                              FILE *f2=fopen("contacts.txt", "r");
                              if(f2!=NULL)
                              {
                              while(!feof(f2))
                              {
                         fscanf(f2,"%s %s %ld %s %s %s %s\n",c1.nom,c1.prenom,&c1.GSM,c1.email,c1.adresse.rue,c1.adresse.ville,c1.adresse.pays);
                         send(client_socket, &c1, sizeof(Contact), 0);

                               }
                             fclose(f2);
                              }
                              int traitement =1;
                              send(client_socket, &traitement, sizeof(int), 0);
                              break;}


                        case 6 :     close(server_socket); break;

                        default :
                                 printf("votre choix est %d \n\n ",choix);
                                 break;
                      }
        } while(choix !=6);


    close(server_socket);
    return 0;
}