#include <stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<fcntl.h>
#include<netdb.h>
#include<string.h>


/*Identification de ma position dans la liste */
int GetSitePos(int NbSites, char *argv[]) {
	char MySiteName[20]; 
	int MySitePos=-1;
	int i;
	gethostname(MySiteName, 20); // Recupere le nom de la machine et le place dans MySiteName 
	for (i=0;i<NbSites;i++){
		if (strcmp(MySiteName,argv[i+2])==0) {
			MySitePos=i;
			return MySitePos;
		}
	}
	puts("Indice du Site courant non trouve' dans la liste");
	exit(-1);
	return (-1);
}

/*Attente bloquante d'un msg de synchro sur la socket donne'e*/
void WaitSync(int s_ecoute) {
	char texte[40];
	int l, s_service, size_sock;
	struct sockaddr_in sock_add_dist;

	size_sock=sizeof(struct sockaddr_in);
	printf("WaitSync : ");fflush(0);

	s_service=accept(s_ecoute,(struct sockaddr*) &sock_add_dist,&size_sock);
	l=read(s_service,texte,39);
	texte[l] ='\0';
	printf("%s\n",texte); fflush(0);

	close (s_service);
} 

/*Envoie d'un msg de synchro a la machine Site/Port*/
void SendSync(char *Site, int Port) {
	int s_emis, size_sock;
	char chaine[15];
	struct sockaddr_in sock_add_emis;
	struct hostent* hp;

	if( (s_emis = socket(AF_INET, SOCK_STREAM,0) )==-1) {
		perror("SendSync : Creation socket");
		exit(-1);
	}

	if( (hp = gethostbyname(Site) ) == NULL) {
		perror("SendSync: Gethostbyname");
		exit(-1);
	}

	size_sock=sizeof(struct sockaddr_in);
	sock_add_emis.sin_family = AF_INET;
	sock_add_emis.sin_port = htons(Port);
	memcpy(&sock_add_emis.sin_addr.s_addr, hp->h_addr, hp->h_length);

	if( connect(s_emis, (struct sockaddr*) &sock_add_emis,size_sock)==-1) {
		perror("SendSync : Connect");
		exit(-1);
	}
	 
	sprintf(chaine,"**SYNCHRO**");

	/*Emission d'un message de synchro*/
	write(s_emis,chaine,strlen(chaine));
	close (s_emis); 
}


/*Envoie d'un msg de synchro a la machine Site/Port*/
void SendMsg(char *Site, int Port, char* vString) {
	struct hostent* hp;
	int s_emis, size_sock;
	struct sockaddr_in sock_add_emis;
  
	if( (s_emis=socket(AF_INET, SOCK_STREAM,0) )==-1) {
		perror("SendSync : Creation socket");
		exit(-1);
	}
    
	if( (hp=gethostbyname(Site)) == NULL) {
		perror("SendSync: Gethostbyname");
		exit(-1);
	}

	size_sock=sizeof(struct sockaddr_in);
	sock_add_emis.sin_family = AF_INET;
	sock_add_emis.sin_port = htons(Port);
	memcpy(&sock_add_emis.sin_addr.s_addr, hp->h_addr, hp->h_length);
  
	if (connect(s_emis, (struct sockaddr*) &sock_add_emis,size_sock)==-1) {
		perror("SendSync : Connect");
		exit(-1);
	}
     
	/*Emission d'un message de synchro*/
	write(s_emis,vString,strlen(vString));
	close (s_emis); 
}
