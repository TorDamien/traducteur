#ifndef _TABLE_SYMBOLE_
#define _TABLE_SYMBOLE_
#include <stdbool.h>

typedef enum {
    VARIABLE,
    TABLEAU,
    FONCTION
} Nature_valeur;

typedef struct {
    char IDF[50];
    Nature_valeur NAT; 
    int nb_argument; // Seulement si l'élément stocké est le nom d'une fonction 
} symbole;

typedef struct {
    symbole tab[100];
    int nb_elem;
} table_symbole;


void init_table(table_symbole* table);
    // Initialise une table vide

int inserer_couple(table_symbole *table, char *nom, Nature_valeur n);
    // Vérifie si le nom est déjà dans la table
    //-> Si oui, met à jour sa valeur
    //-> Si non, ajoute le couple nom valeur à la table


bool chercher(char* chaine, int type_chaine);
    // Renvoie la valeur en fonction de l'identificateun dans la table, 
    // ou une erreur si celui ci est non présente
    // type_chaine = 0 -> tableau ou non
    // type_chaine = 1 -> on cherche un tableau
    // type_chaine = 2 -> on cherche une variable
    // type_chaine = 3 -> on cherche une fonction

void afficher_table(table_symbole);
    // Affichage de la table

#endif