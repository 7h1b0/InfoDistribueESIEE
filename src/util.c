#include <stdio.h>
#include <string.h>

#include "requete.h"

/* Converti un String en Requete */
void StringToRequete(char * pString, struct Requete * pRequete){
	char *token= strtok(pString, ":");
	pRequete->site = atoi(token);
	token = strtok(NULL, " ");
	pRequete->horloge = atoi(token);	
}

/* Converti une Requete en String */
void RequeteToString(struct Requete pRequete, char * pString){
	sprintf(pString, "%d:%d",pRequete.site,pRequete.horloge);
}

/* Methode pour decider si Site entre en SC */
int SC(){
	return ( (rand()%(10) ) == 9) ? 1 : 0;
}

/* Place une requete dans une Pile de requete */
void push(struct Requete pRequete,struct Requete * PILE, int* pMax){
	PILE[*pMax] = pRequete;
	*pMax+=1;
	tri_bulle(PILE, *pMax);
	//quicksort(PILE, *pMax);
}

/* Suppression de la première Requete de la liste */
void pop(struct Requete * PILE, int *pMax){
	PILE[0] = PILE[*pMax-1]; // On place la derniere requete à la première place
	*pMax -= 1; // On reduit la taille de la PILE
	tri_bulle(PILE, *pMax);//Et on trie, Oh Oui !
	//quicksort(PILE, *pMax);
}

/* Retourne vrai si Requete du site est en premiere position  */
int isFirst(struct Requete * PILE, int pSite){
	return (PILE[0].site == pSite) ? 1 : 0;
}



/* Converti une Pile en String affichable */
void PILEtoString(struct Requete * PILE,int pMax){
	int i;
	char vString[500] = "";
	for(i=0;i<pMax;i++){
		char Buffer[20];

		RequeteToString(PILE[i], Buffer);
		sprintf(vString, "%s - %s", vString, Buffer);
	}
	sprintf(vString, "%s - %d", vString, pMax);
	printf("%s \n",vString);
}

void Horloge(int * pHorloge, int pHorlogeMessage){
	if(pHorlogeMessage > *pHorloge){
		*pHorloge = pHorlogeMessage+1;
	} else{
		*pHorloge += 1;
	}
}

void wait(int pTps){
	int i,c;
	for(i=0; i<pTps*100; i++){
		c=c*50;
		c=c/50;
		c=c*33;
		c=c/33;
	}
}
