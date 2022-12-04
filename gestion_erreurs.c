#include "gestion_erreurs.h"
#include "analyse_syntaxique.h"
#include "analyse_lexicale.h"
#include <stdio.h>
#include <stdlib.h>
extern Lexeme lexeme_en_cours;
extern char nom_fichier_sortie[100];
extern char nom_fichier_entree[100];


void erreur(type_erreur Err){
    char ligne[100];
    printf("\nErreur \nLigne \033[1mn°%d\033[22m: ",lexeme_en_cours.ligne);
    FILE *entree = fopen(nom_fichier_entree,"r");
    for (int i = 1; i<lexeme_en_cours.ligne;i++){
        fgets(ligne, 100,entree);
        }
    printf("%s",fgets(ligne, 100,entree));  
    printf("Colonne \033[1mn°%d\033[22m\n",lexeme_en_cours.colonne);
    printf("Sur la chaîne : \033[1m%s\033[22m\n",lexeme_en_cours.chaine);
    switch (Err){
        case LEX:
            printf("Erreur Lexicale\n");
            break;
        case S_PARF:
            printf("Paranthèse fermante attendus\n");
            break;
        case S_PARO:
            printf("Paranthèse ouvrante attendus\n");
            break;
        case S_VAR:
            printf("Variable non définie\n");
            break;
        case S_TER:
            printf("ENTIER, PARO ou VAR attendus\n");
            break;
        case S_TABCROO:
            printf("Crochet ouvrant du tableau attendus\n");
            break;
        case S_TABCROF:
            printf("Crochet fermant du tableau attendus\n");
            break;
        case S_NOMF:
            printf("Nom de la fonction attendu\n");
            break;
        case S_TAB_TER:
            printf("Nombre d'élément du tableau invalide\n");
            break;
        case S_TAB_DEF:
            printf("Virgule ou crochet fermant d'un tableau attendu\n");
            break;
        case S_IDF:
            printf("Identificateur invalide\n");
            break;
        case S_AFF:
            printf("Assignateur :=, <- ou = attendus\n");
            break;
        case S_SEP:
            printf("Séparateur invalide\n");
            break; 
        case S_OP:
            printf("Alternance opérateur/terme non respecté\n");
            break; 
        case S_INST:
            printf("La ligne n'est pas une instruction\n");
            break; 
        case S_TAB_NONINI:
            printf("Tableau non initialisé\n");
            break;  
        case S_CMP:
            printf("Un comparateur entre deux expressions attendu\n");
            break;
        case S_CONDI:
            printf("Erreur de construction d'une condition\n");
            break;
        case S_FIN:
            printf("Fin de séquence non atteinte\n");
            break;       
        case S_FSI:
            printf("'sinon' ou 'finsi' attendu\n");
            break;
        case LEX_TEXTE:
            printf("'!' de cloture attendu\n");
            break;
        case S_FDEUXP:
            printf("Deux points ':' en fin de déclaration de fonction attendu\n");
            break;
        case S_RETURN:
            printf("Utilisation d'un return en dehors d'une fonction\n");
            break;
        case S_FINF:
            printf("Déclaration de fin de fonction attendu\n");
            break;
        case S_ARG:
            printf("On attendait un IDF ou une PARF en argument de la fonction\n");
            break;
        case S_FONCTION:
            printf("Fonction inconnue\n");
            break;
        default:
            printf("Erreur inconnue\n");    
    }
    remove(nom_fichier_sortie);
    remove(".tmp");
    fclose(entree);
    exit(0);
}