/* T. Grandpierre - Application distribue'e pour TP IF4-DIST 2004-2005

But : 

fournir un squelette d'application capable de recevoir des messages en 
mode non bloquant provenant de sites connus. L'objectif est de fournir
une base pour implementer les horloges logique/vectorielle/scalaire, ou
bien pour implementer l'algorithme d'exclusion mutuelle distribue'

Syntaxe :
         arg 1 : Numero du 1er port
	 arg 2 et suivant : nom de chaque machine

--------------------------------
Exemple pour 3 site :

Dans 3 shells lances sur 3 machines executer la meme application:

pc5201a>./dist 5000 pc5201a.esiee.fr pc5201b.esiee.fr pc5201c.esiee.fr
pc5201b>./dist 5000 pc5201a.esiee.fr pc5201b.esiee.fr pc5201c.esiee.fr
pc5201c>./dist 5000 pc5201a.esiee.fr pc5201b.esiee.fr pc5201c.esiee.fr

pc5201a commence par attendre que les autres applications (sur autres
sites) soient lance's

Chaque autre site (pc5201b, pc5201c) attend que le 1er site de la
liste (pc5201a) envoi un message indiquant que tous les sites sont lance's


Chaque Site passe ensuite en attente de connexion non bloquante (connect)
sur son port d'ecoute (respectivement 5000, 5001, 5002).
On fournit ensuite un exemple permettant 
1) d'accepter la connexion 
2) lire le message envoye' sur cette socket
3) il est alors possible de renvoyer un message a l'envoyeur ou autre si
necessaire 

*/


#include <stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<fcntl.h>
#include<netdb.h>
#include<string.h>

#include "requete.h"


typedef struct Requete Requete;

/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/

int main (int argc, char* argv[]) {
	struct sockaddr_in sock_add, sock_add_dist;
	int size_sock, s_ecoute, s_service;
	char texte[40];
	int i,l,v,vMax=0, vHorloge = 0, vCompteur=1, vBoolean=1;
	Requete* PILE;
	PILE = malloc(5000 * sizeof(Requete) );

	int PortBase=-1; /*Numero du port de la socket a` creer*/
	int NSites=-1; /*Nb total de sites*/


	if (argc<3) {
		printf("Erreur: il faut donner au moins 2 sites pour faire fonctionner l'application: NumeroPortBase et liste_des_sites");
	exit(-1);
	}

	/*----Nombre de sites (adresses de machines)---- */
	NSites=argc-2;


	/*CREATION&BINDING DE LA SOCKET DE CE SITE*/
	PortBase=atoi(argv[1])+GetSitePos(NSites, argv);
	printf("Numero de port de ce site %d\n",PortBase);

	sock_add.sin_family = AF_INET;
	sock_add.sin_addr.s_addr= htons(INADDR_ANY);  
	sock_add.sin_port = htons(PortBase);

	if ( (s_ecoute=socket(AF_INET, SOCK_STREAM,0))==-1) {
		perror("Creation socket");
		exit(-1);
	}

	if ( bind(s_ecoute,(struct sockaddr*) &sock_add,sizeof(struct sockaddr_in))==-1) {
		perror("Bind socket");
		exit(-1);
	}
  
	listen(s_ecoute,30);
	/*----La socket est maintenant cre'e'e, binde'e et listen----*/

	if (GetSitePos(NSites, argv) ==0) { 
		// Le site 0 attend une connexion de chaque site :
		for(i=0;i<NSites-1;i++){
			WaitSync(s_ecoute);
		}
		puts("Toutes les synchros recues");

		// et envoie un msg a chaque autre site pour les synchroniser
		for(i=0;i<NSites-1;i++){ 
			SendSync(argv[3+i], atoi(argv[1])+i+1);
		}

	}else{
		// Chaque autre site envoie un message au site0  (1er  dans la liste) pour dire qu'il est lance'
		SendSync(argv[2], atoi(argv[1]));
		
		// et attend un message du Site 0 envoye' quand tous seront lance's
		puts("Wait Synchro du Site 0");
		WaitSync(s_ecoute);
		puts("Synchro recue de  Site 0");
	}

	printf("Je suis le site numero : %d \n",GetSitePos(NSites, argv) );

  
	/* Passage en mode non bloquant du accept pour tous*/
	/*---------------------------------------*/
	fcntl(s_ecoute,F_SETFL,O_NONBLOCK);
	size_sock=sizeof(struct sockaddr_in);

	/* Boucle infini*/
	//for(v=0;v<50;v++) {
	while(1){
		// On commence par tester l'arrivée d'un message
		s_service=accept(s_ecoute,(struct sockaddr*) &sock_add_dist,&size_sock);
		if (s_service>0) {
			
			//Extraction et affichage du message
			l=read(s_service,texte,39);
			texte[l] ='\0';

			//Conversion du message recu en requete
			Requete Requete1;
			StringToRequete(texte, &Requete1);

			//Mise a jour de l'horloge
			Horloge(&vHorloge, Requete1.horloge); 

			//Distinction des messages en fonction de la valeur du site
			if(Requete1.site == NSites +1){
				printf("Reponse recu : %d \n", vHorloge);
				vCompteur ++;
			} else if(Requete1.site == NSites +2){
				printf("Liberation recu : %d \n",vHorloge);
				pop(PILE, &vMax);
			} else {
				push(Requete1, PILE,&vMax);

				//On envoi la reponse de la bonne reception de la requete
				char vTexte[20];
				sprintf(vTexte,"%d:%d",NSites+1,vHorloge);
				SendMsg(argv[2+Requete1.site], atoi(argv[1])+Requete1.site, vTexte);
				printf("Requete recu : %d \n",vHorloge); 
			}
			
			close (s_service);

		}else if(SC() == 1 && vBoolean){ //On veux entrer en SC
			vHorloge++; //On augmente l'horloge car evenement -- envoi
			printf("Requete envoye : %d \n", vHorloge);

			vBoolean = 0; //On ne peut pas demander 2 fois de suite la SC
			
			//On créé la requete
			Requete Requete1;
			Requete1.site = GetSitePos(NSites, argv);
			Requete1.horloge = vHorloge;
			char vBuffer[20];
			RequeteToString(Requete1,vBuffer);

			//On envoi la requete a tout le monde
			for(i=0;i<NSites;i++){
				if(i != Requete1.site ){ //Tout le monde sauf nous-même
					SendMsg(argv[2+i], atoi(argv[1])+i, vBuffer);
				} else{ //Si nous même, on ajoute dans notre pile
					push(Requete1, PILE,&vMax);
				}
			}
		} 

  		if(vCompteur == NSites ){
			if(isFirst(PILE, GetSitePos(NSites, argv) )) {
				puts("Je passe en SC");			
			
				vCompteur = 1;
				vBoolean = 1; //On reactive la possibilite de rentrer en SC

				wait(10000); //attente
				puts("Je sors de la SC");

				for(i=0;i<NSites;i++){
					if(i != GetSitePos(NSites, argv) ){ //Tout le monde sauf nous-même
						char vTexte[20];
						sprintf(vTexte,"%d:%d",NSites+2,vHorloge);
						SendMsg(argv[2+i], atoi(argv[1])+i, vTexte);
					} else { // Si c'est moi, on delete le premier de la liste
						pop(PILE, &vMax);
					}
				}
				puts("Envoi des liberations");
			}
				
		}

		wait(10000); //attente
    
   		//printf(".");fflush(0); /* pour montrer que le serveur est actif*/
	}

	puts("merdouille");
	PILEtoString(PILE, vMax);

	free(PILE);
	close (s_ecoute);  
	return 0;
}
