#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyse_syntaxique.h"
#include "analyse_lexicale.h"

char nom_fichier_sortie[100];
char nom_fichier_entree[100];

int main (int argc, char *argv[]) {
    char buf[1000];
    FILE *f;
    strcpy(nom_fichier_sortie,"./Resultats/");
    switch (argc) {
        case 3:
            // demarre l'analyse syntaxique sur le fichier transmis en 
            strcpy(nom_fichier_entree,argv[1]);
            strcat(nom_fichier_sortie,argv[2]);
            analyser(); 
            break ;
        case 2:
            // demarre l'analyse syntaxique sur le fichier sur la lecture clavier
            f = fopen(".tmp","w");
            while(*(fgets(buf, 1000, stdin)) != '\n') {
                fprintf(f,"%s",buf);}
            fclose(f);
            strcpy(nom_fichier_entree,".tmp");
            strcat(nom_fichier_sortie,argv[1]);
            analyser(); 
            remove(".tmp");
            break;
        default:
            printf("Nombre d'arguments incorrects !\n");
            printf("Utilisation : \n Version clavier : ./traducteur_python <nom_fichier_sortie>\n Version fichier : ./traducteur_python <nom_fichier_entree> <nom_fichier_sortie>\n");
            exit(1) ;
    } ;
    arreter() ; 

    switch (argc) {
        case 3:
            for (int i = 0; i < strlen(argv[2]) + strlen("à été généré dans le répertoire Resultats") + 2;i++)
            {
                printf("*");
            }
            printf("\n*** %s à été généré dans le répertoire Resultats ***\n",argv[2]);
            for (int i = 0; i < strlen(argv[2]) + strlen("à été généré dans le répertoire Resultats") + 2;i++)
            {
                printf("*");
            }
            printf("\n");
            break;
        case 2:
            for (int i = 0; i < strlen(argv[1]) + strlen("à été généré dans le répertoire Resultats") + 2;i++)
            {
                printf("*");
            }
            printf("\n*** %s à été généré dans le répertoire Resultats ***\n",argv[1]);
            for (int i = 0; i < strlen(argv[1]) + strlen("à été généré dans le répertoire Resultats") + 2;i++)
            {
                printf("*");
            }
            printf("\n");
        default:
            break;

    }

    return 0 ;
}
