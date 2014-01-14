#include "requete.h"

void permute(struct Requete *x,struct Requete *y) {
	struct Requete tmp;
	tmp = *x;
	*x = *y;
	*y = tmp;
}

int tri_bulle_horloge(struct Requete * t, int n) {
	int num_balayage, i;
	for (num_balayage = 1 ; num_balayage < n ; ++num_balayage) /* n-1 balayages */
		for (i = 1 ; i < n ; ++i) /* balayage n°i : n-1 comparaisons */
			if (t[i].horloge < t[i-1].horloge)  /* si deux elements ne sont pas dans l'ordre */
					permute(&t[i], &t[i-1]); /* alors on les echange */
	return 0;
}

int tri_bulle_site(struct Requete * t, int n) {
	int num_balayage, i;
	for (num_balayage = 1 ; num_balayage < n ; ++num_balayage) /* n-1 balayages */
		for (i = 1 ; i < n ; ++i) /* balayage n°i : n-1 comparaisons */
			if (t[i].site < t[i-1].site && t[i].horloge == t[i-1].horloge)  /* si deux elements ne sont pas dans l'ordre */
					permute(&t[i], &t[i-1]); /* alors on les echange */
	return 0;
}

int tri_bulle(struct Requete * t, int n) {
	tri_bulle_horloge(t,n);
	tri_bulle_site(t,n);
}
