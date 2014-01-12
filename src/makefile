serveur : TP-DIST.o reseau.o tri.o util.o
	gcc -o serveur TP-DIST.o reseau.o tri.o util.o
TP-DIST.o : TP-DIST.c reseau.h 
	gcc -c TP-DIST.c
reseau.o: reseau.c reseau.h
	gcc -c reseau.c
tri.o: tri.c tri.h requete.h
	gcc -c tri.c
util.0: util.c util.h requete.h
	gcc -c util.c
clean:
	rm *.o
