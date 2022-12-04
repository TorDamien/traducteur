#include "table_symbole.h"
#include <stdio.h>

int main(){
    table_symbole table;
    init_table(&table);
    inserer_couple(&table,"Toto",VARIABLE);
    inserer_couple(&table,"Dada",VARIABLE);
    afficher_table(table);
    inserer_couple(&table,"Dada",VARIABLE);
    afficher_table(table);
}
