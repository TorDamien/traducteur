				/******** COMPILATION *******/

Il suffit simplement d'utiliser la commande "make". Le Makefile s'occupe de tout.
Possibilité d'enlever les fichiers avec l'extention .o à l'aide de la commande "make clean"

				/******* Execution ********/

Les fichiers contenant le langage haut niveau à traduire sont ou sont à mettre dans le répertoire "Tests".

1) Pour le traducteur python, deux choix possibles:

	- Lecture depuis un fichier -> ./traducteur_python ./Tests/nomFichier nomFichierSortie.py 

	- Lecture depuis l'entrée standard -> ./traducteur_python nomFichierSortie.py

2) Pour tester seulement les lexemes:

	- Lecture depuis un fichier -> ./test_lexeme ./Tests/nomFichier

	- Lecture depuis l'entrée standard -> ./test_lexeme

3) Il existe aussi un fichier test_table.c que l'on appel simplement en faisant ./test_table qui permet de vérifier le bon fonctionnement de la table des symboles.

					/******* Résultats ********/

Les résultats sont consultables dans le répertoire "Resultats".