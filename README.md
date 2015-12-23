# IT-4201B - TP2
Exclusion mutuelle dans les systèmes répartis.

## Objectifs
* Satisfaire les demandes de différents sites dans l’ordre des demandes d’entrée en section critique après accord de tous les autres sites
* Distribution d’une file d’attente associé à un mécanisme d’estampillage des
messages. Basé sur un jeton circulant dans un anneau logique de processus.

## Lancer le programme
1. Copier le programme sur plusieurs PC
2. Lancer dans chaque PC un terminal dans le dossier src et faire ``` make ```
3. Puis lancer la commande ```./serveur 5500 monPC1 monPC2 monPC3 ... ```
