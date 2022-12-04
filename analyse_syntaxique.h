#ifndef _ANALYSE_SYNTAXIQUE_
#define _ANALYSE_SYNTAXIQUE_

/* Automate actuel pour l'analyse syntaxique */
#include <stdbool.h>

typedef enum {E_INIT, E_ENTIER, E_OPERATEUR, E_FLOAT} Etat_Automate_Syntaxique;


void analyser ();
	// e.i : indifferent
	// e.f : une Expression Arithmetique a ete lue dans fichier
	// si elle ne contient pas d’erreur de syntaxe un message est affiche
	// sinon le pgm termine sur un message d’erreur

void pgm();

void seq_inst();

void suite_seq_inst();

void inst();

void test();

void suite_test();

void suite_cond();

void aff();

void list_print();

void suite_tab_print();

void l_aff();

void eag();

void seq_terme();

void suite_seq_terme();

void terme();

void prolongement(char *nom_var);

void tab(char * nom_var);

void indice();

void aff_tab();

void suite_val_tab();

void argument();

void suite_arg();

void ecrire_nombre(float value);

void ecrit_condi();

void ecrire_module();

void ecrire_espace();

#endif