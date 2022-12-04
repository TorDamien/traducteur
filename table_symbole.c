#include "table_symbole.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

table_symbole table;

bool meme_nom(char *nom1, char *nom2){
    if (strcmp(nom1, nom2) == 0){
        return true;
    }
    else { 
        return false;
    }
}

extern void init_table(table_symbole* table){
    table->nb_elem=0;
}

extern int inserer_couple(table_symbole *table, char *nom, Nature_valeur n){
    for (int i = 0; i<table->nb_elem;i++){
        //Cas IDF présent
        if (meme_nom(nom,table->tab[i].IDF)){
                table->tab[i].NAT = n;
                return 1;
            } 
    }
    //Cas IDF non présent
    symbole new_elem;
    strcpy(new_elem.IDF,nom);
    table->tab[table->nb_elem] = new_elem;
    table->tab[table->nb_elem].NAT = n;
    table->nb_elem++;
    return 2;
}

extern void afficher_table(table_symbole table){
    printf("\nTable de %d elements\n*******************\n",table.nb_elem);
    for (int i = 0; i<table.nb_elem;i++){
        printf("{ %s }\n",table.tab[i].IDF);
    }
}


bool chercher(char* chaine, int type_chaine){
    if (type_chaine == 0) { 
        for (int i=0; i<table.nb_elem;i++) {
            if  (strcmp(table.tab[i].IDF,chaine) == 0)  {
                return true;
            }
        }
    } else if (type_chaine == 1) { 
        for (int j=0; j<table.nb_elem;j++) {
            if ( (strcmp(table.tab[j].IDF,chaine) == 0)  && (table.tab[j].NAT == TABLEAU) ) {
                return true;
            }
        }
    } else if (type_chaine == 2) {
    	for (int k=0; k<table.nb_elem;k++){
    		if ( (strcmp(table.tab[k].IDF,chaine) == 0) && (table.tab[k].NAT == VARIABLE) ) {
    			return true;
    		}
    	}
    } else if (type_chaine == 3) {
        for (int k=0; k<table.nb_elem;k++){
            if ( (strcmp(table.tab[k].IDF,chaine) == 0) && (table.tab[k].NAT == FONCTION) ) {
                return true;
            }
        }
    }

	return false;
}
