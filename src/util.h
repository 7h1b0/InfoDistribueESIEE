void StringToRequete(char * pString, struct Requete * pRequete);
void RequeteToString(struct Requete pRequete, char * pString);
int SC();
void push(struct Requete pRequete,struct Requete * PILE, int* pMax);
int isFirst(struct Requete * PILE, int pSite);
void delFirst(struct Requete * PILE, int *pMax);
void PILEtoString(struct Requete * PILE,int pMax);
void wait(int pTps);

