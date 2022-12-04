#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include "analyse_syntaxique.h"
#include "analyse_lexicale.h"
#include "table_symbole.h"
#include "gestion_erreurs.h"


				/************VARIABLE GLOBALE************/ 

extern Lexeme lexeme_en_cours;
extern table_symbole table;
FILE* fichier_sortie;
liste_modules modules;
extern char nom_fichier_sortie[100];
extern char nom_fichier_entree[100];
bool print = false;
int nombre_espace = 0;
bool dans_fonction = false;

				/*************INITIALISATION*************/

void analyser (){	
	fichier_sortie = fopen(nom_fichier_sortie,"w");
	init_modules();
	demarrer(nom_fichier_entree); 
    init_table(&table);
	pgm();
	if (lexeme_en_cours.nature != FIN_SEQUENCE){
		erreur(S_FIN);
	}
	ecrire_module();
	remove("tmp");
	return ;
}

		  /*************FONCTIONS DE LA GRAMMAIRE*************/


/* NOTE : Pour faciliter l'analyse, les fonctions sont rangées dans le même ordre que 
celui de la grammaire syntaxique présente dans le compte rendu situé dans le répertoire 
Documents */

void pgm(){
 	if (lexeme_en_cours.nature == FIN_SEQUENCE) {return;}
 	seq_inst();
}


void seq_inst(){
	switch(lexeme_en_cours.nature) {
		case RETURN:
			if (dans_fonction) {
				ecrire_espace();
				fprintf(fichier_sortie,"return ");
				avancer();
				eag();
				fprintf(fichier_sortie,"\n");
				suite_seq_inst();
			} else {erreur(S_RETURN);}
			break;

		default:
			inst();
			suite_seq_inst();
	}
};


void suite_seq_inst() {
	switch(lexeme_en_cours.nature) {
		case SEP_INST:
			avancer();
			seq_inst();
		default:
			break;
	}
};

void inst(){
	switch (lexeme_en_cours.nature){
		case IDF:
			ecrire_espace();
			fprintf(fichier_sortie,"%s",lexeme_en_cours.chaine);
			char nom_var[100];
			strcpy(nom_var,lexeme_en_cours.chaine);
			avancer();
			aff(nom_var);
			fprintf(fichier_sortie,"\n");
			break;		
		case SI:
			ecrire_espace();
			ecrit_condi();
			avancer();
			test();
			if (lexeme_en_cours.nature != SEP_INST) {erreur(S_INST);}
			fprintf(fichier_sortie,"\n");
			nombre_espace++;
			avancer();
			seq_inst();
			nombre_espace--;
			suite_cond();
			fprintf(fichier_sortie,"\n");
			break;
		case TANTQ:
			ecrire_espace();
			ecrit_condi();
			avancer();
			test();
			if (lexeme_en_cours.nature != SEP_INST) {erreur(S_INST);}
			fprintf(fichier_sortie,"\n");
			nombre_espace++;
			avancer();
			seq_inst();
			if (lexeme_en_cours.nature != FTQ) {erreur(S_CONDI);}
			ecrit_condi();
			nombre_espace--;
			avancer();
			break;
		case DEF:
			dans_fonction = true;
			fprintf(fichier_sortie,"def ");
			avancer();
			if (lexeme_en_cours.nature != IDF) {erreur(S_NOMF);}
			inserer_couple(&table,nom_var, FONCTION);
			fprintf(fichier_sortie,"%s",lexeme_en_cours.chaine);
			avancer();
			if (lexeme_en_cours.nature != PARO) {erreur(S_ARG);}
			fprintf(fichier_sortie,"(");
			avancer();
			argument();
			if (lexeme_en_cours.nature != SEP_INST) {erreur(S_INST);}
			fprintf(fichier_sortie,":\n");
			nombre_espace++;
			avancer();
			seq_inst();
			if (lexeme_en_cours.nature != FINF) {erreur(S_FINF);}
			nombre_espace--;
			dans_fonction = false;
			fprintf(fichier_sortie,"\n");
			avancer();
			break;
		case PRINT:
			ecrire_espace();
			fprintf(fichier_sortie,"print(");
			print = true;
			avancer();
			list_print();
			fprintf(fichier_sortie,")\n");
			print = false;
			break;
		case DATE:
			ecrire_espace();
			fprintf(fichier_sortie,"print(ctime())\n");
			avancer();
			break;
		case ATT:
			ecrire_espace();
			fprintf(fichier_sortie,"sleep(");
			avancer();
			eag();
			fprintf(fichier_sortie,")\n");
			break;
		case AJOUTER:
			ecrire_espace();
			avancer();
			switch (lexeme_en_cours.nature) {
				case IDF:
					fprintf(fichier_sortie, "%s", lexeme_en_cours.chaine);
					fprintf(fichier_sortie, ".append(");
					avancer();
					eag();
					fprintf(fichier_sortie, ")\n");
					break;
				default:
					erreur(S_IDF);
			} 
			break;

		default: 
			break;
	}
}


void test(){
	if (lexeme_en_cours.nature == TRUE){
		fprintf(fichier_sortie," True ");
		avancer();}
	else if (lexeme_en_cours.nature == FALSE){
		fprintf(fichier_sortie," False ");
		avancer();}
	else {
		eag();
		switch (lexeme_en_cours.nature) {
			case INF:
			case SUP:
			case INF_STRICT:
			case SUP_STRICT:
			case EG:
			case DIF:
				ecrit_condi();
				avancer();
				eag();
				break;
			default:
				erreur(S_CMP);}
			
	}
	suite_test();
}

void suite_test(){
	switch(lexeme_en_cours.nature){
		case ALORS:
			ecrit_condi();
			avancer();
			break;
		case AND:
		case OR:
			ecrit_condi();
			avancer();
			test();
			break;
		default:
			erreur(S_CMP);
	}
}

void suite_cond(){
	switch(lexeme_en_cours.nature){
		case SINON:
			ecrire_espace();
			ecrit_condi();
			avancer();
			if (lexeme_en_cours.nature != SEP_INST) {erreur(S_INST);}
			fprintf(fichier_sortie,"\n");
			nombre_espace++;
			avancer();
			seq_inst();
			if (lexeme_en_cours.nature != FSI) {erreur(S_CONDI);}
			nombre_espace--;
			ecrit_condi();
			avancer();
			break;
		case FSI:
			ecrit_condi();
			avancer();
			break;
		default:
			erreur(S_FSI);
	}

}

void list_print(){
	switch (lexeme_en_cours.nature) {
		case TEXTE:
			fprintf(fichier_sortie,"\"%s\"",lexeme_en_cours.chaine);
			avancer();
			break;
		default:
			eag();
			break;
	}
	suite_tab_print();
}

void suite_tab_print(){
	switch (lexeme_en_cours.nature) {
		case SEP_TAB:
			fprintf(fichier_sortie,",");
			avancer();
			list_print();
			break;
		default:
			return;
	}
}

void aff(char * nom_var){
	switch(lexeme_en_cours.nature){
		case CROO: /* Cas d'un tableau */
			avancer();
			indice(nom_var);
			break;
		case PARO: /* Cas d'une fonction */
			fprintf(fichier_sortie,"(");
			avancer();
			argument();
			break;
		default: /* Cas variable classique */
			inserer_couple(&table,nom_var,VARIABLE);
			l_aff();
			eag();
	}
}

void indice(char * nom_var){
	switch(lexeme_en_cours.nature){
		case CROF: // Déclaration d'un tableau
			inserer_couple(&table,nom_var,TABLEAU);
			avancer();
			l_aff();
			if (lexeme_en_cours.nature != CROO) {erreur(S_TABCROO);}
			fprintf(fichier_sortie,"[");
			avancer();
			aff_tab();
			break;
		default: // Affectation à un tableau
			fprintf(fichier_sortie,"[");
			eag();
			if (lexeme_en_cours.nature != CROF) {erreur(S_TABCROF);}
			fprintf(fichier_sortie,"]");
			avancer();
			l_aff();
			eag();
			break;
	}
}

void l_aff(){
	switch (lexeme_en_cours.nature){
		case AFFDTF:
		case AFFDTP:
		case AFFEG:
			fprintf(fichier_sortie," = ");
			avancer();
			break;
		default:
			erreur(S_AFF);
	}
}

void aff_tab(){
	switch (lexeme_en_cours.nature){
		case CROF:
			fprintf(fichier_sortie,"]");
			avancer();
			break;

		default: 
			eag();
			suite_val_tab();
	}
}

void suite_val_tab(){
	switch (lexeme_en_cours.nature){
		case SEP_TAB:
			fprintf(fichier_sortie,",");
			avancer();
			eag();
			suite_val_tab();
			break;
		case CROF:
			fprintf(fichier_sortie,"]");
			avancer();
			break;
		default:
			erreur(S_TAB_DEF);
	}
}

void argument(){
	switch (lexeme_en_cours.nature){
		case PARF: 
			fprintf(fichier_sortie,")");
			avancer();
			break;
		default:
			eag();
			suite_arg();
	}
}

void suite_arg(){
	switch (lexeme_en_cours.nature){
		case SEP_TAB:
			fprintf(fichier_sortie,",");
			avancer();
			if (lexeme_en_cours.nature != IDF) {erreur(S_ARG);}
			/* compter les arguments */
			fprintf(fichier_sortie,"%s",lexeme_en_cours.chaine);
			avancer();
			suite_arg();
			break;
		case PARF:
			fprintf(fichier_sortie,")");
			avancer();
			break;
		default:
			erreur(S_ARG);
	}
}


void eag(){
	seq_terme();
}

void seq_terme(){
	terme();
	suite_seq_terme();
}


void suite_seq_terme(){
	switch (lexeme_en_cours.nature){
		case PLUS:
			fprintf(fichier_sortie," + ");
			break;
		case MOINS:
			fprintf(fichier_sortie," - ");
			break;
		case MUL:
			fprintf(fichier_sortie," * ");
			break;
		case DIV:
			fprintf(fichier_sortie," / ");
			break;
		case MODULO:
			fprintf(fichier_sortie,"%%");
			break;
		case POW:
			fprintf(fichier_sortie," ** ");
			break;
		default: // cas ɛ
			return;
	}
	avancer();
	terme();
	suite_seq_terme();
}



void terme(){
	char nom_var2[100];
	switch (lexeme_en_cours.nature){
		case ENTIER:
			ecrire_nombre(lexeme_en_cours.valeur);
			avancer();
			break;
		case TRUE:
			fprintf(fichier_sortie,"%s","True");
			avancer();
			break;
		case FALSE:
			fprintf(fichier_sortie,"%s","False");
			avancer();
			break;
		case PARO:
			fprintf(fichier_sortie,"(");
			avancer();
			eag();
			if (lexeme_en_cours.nature != PARF){
				erreur(S_PARF);
			} 
			fprintf(fichier_sortie,")");
			avancer();
			break;
		case IDF:
			strcpy(nom_var2,lexeme_en_cours.chaine);
			fprintf(fichier_sortie,"%s",lexeme_en_cours.chaine);
			avancer();
			prolongement(nom_var2);
			break;
		case INPUT:
			fprintf(fichier_sortie,"%s","float(input(\"Entrez un nombre: \"))");
			avancer();
			break;
		case ROUND:
            fprintf(fichier_sortie,"%s","round(");
            avancer();
            if (lexeme_en_cours.nature != PARO){
                erreur(S_PARO);
            } 
            avancer();
            eag();
            if (lexeme_en_cours.nature != PARF){
                erreur(S_PARF);
            } 
            fprintf(fichier_sortie,")");
            avancer();
            break;
		case TAILLE:
		    fprintf(fichier_sortie,"%s","len(");
            avancer();
            if (lexeme_en_cours.nature != PARO){
                erreur(S_PARO);
            } 
            avancer();
            eag();
            if (lexeme_en_cours.nature != PARF){
                erreur(S_PARF);
            } 
            fprintf(fichier_sortie,")");
            avancer();
            break;
		default:
			erreur(S_TER);
	}
}

void tab(char * nom_var) {
	switch (lexeme_en_cours.nature) {
		case CROO:
			avancer();
			if (lexeme_en_cours.nature != CROF || !print){
				fprintf(fichier_sortie,"[");
				eag();
				fprintf(fichier_sortie,"]");
			}
			if (lexeme_en_cours.nature == CROF) {
				avancer();
			} else {erreur(S_PARF);}
			break;
		default:
			break;
	}
}

void prolongement(char *nom_var) {
	switch (lexeme_en_cours.nature) {
		case PARO:
			fprintf(fichier_sortie,"(");
			avancer();
			argument();
			break;
		default:
			tab(nom_var);
	}
}

					/*************Fonctions Annexes*************/

void ecrire_nombre(float value){
	if(value - (int)value == 0 ){fprintf(fichier_sortie,"%.0f",value);}
	else if (value*10 - (int)(value*10) == 0){fprintf(fichier_sortie,"%.1f",value);}
	else if (value*100 - (int)(value*100) == 0){fprintf(fichier_sortie,"%.2f",value);}
	else if (value*1000 - (int)(value*1000) == 0){fprintf(fichier_sortie,"%.3f",value);}
	else if (value*10000 - (int)(value*10000) == 0){fprintf(fichier_sortie,"%.4f",value);}
	else if (value*100000 - (int)(value*100000) == 0){fprintf(fichier_sortie,"%.5f",value);}
	else {fprintf(fichier_sortie,"%f",value);}
}


void ecrit_condi() {
	switch (lexeme_en_cours.nature) {
		case SI:
			fprintf(fichier_sortie,"if ");
			break;
		case TANTQ:
			fprintf(fichier_sortie,"while ");
			break;
		case ALORS:
			fprintf(fichier_sortie,":");
			break;
		case SINON:
			fprintf(fichier_sortie,"else:");
			break;
		case INF:
			fprintf(fichier_sortie," <= ");
			break;
		case SUP:
			fprintf(fichier_sortie," >= ");
			break;
		case INF_STRICT:
			fprintf(fichier_sortie," < ");
			break;
		case SUP_STRICT:
			fprintf(fichier_sortie," > ");
			break;
		case EG:
			fprintf(fichier_sortie," == ");
			break;
		case DIF:
			fprintf(fichier_sortie," != ");
			break;
		case AND:
			fprintf(fichier_sortie," and ");
			break;
		case OR:
			fprintf(fichier_sortie," or ");
			break;
		default:
			break;
	}
}


void ecrire_espace(){
	for (int i=0; i<nombre_espace;i++){
		fprintf(fichier_sortie,"\t");
	}
}

void ecrire_module(){
	char c;
	if (modules.nombre_elem == 0){ return;}
	FILE *tmp = fopen("mod.tmp","w");
	for (int i=0; i<modules.nombre_elem;i++){
		switch (modules.liste[i]){
			case SLEEP:
				fprintf(tmp,"from time import sleep\n");
				break;
			case CTIME:
				fprintf(tmp,"from time import ctime\n");
				break;
			default:
				erreur(S_FONCTION);
		}
	}
	fclose(fichier_sortie);
	fopen(nom_fichier_sortie,"r");
	while((c=fgetc(fichier_sortie))!=EOF)
        fprintf(tmp, "%c",c);
	fclose(tmp);
	tmp = fopen("mod.tmp","r");
	fclose(fichier_sortie);
	fopen(nom_fichier_sortie,"w");
	while((c=fgetc(tmp))!=EOF)
        fprintf(fichier_sortie, "%c",c);
	remove("mod.tmp");
}