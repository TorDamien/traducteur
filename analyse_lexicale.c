/* ------------------------------------------------------------------------
-- paquetage analyse_lexicale
--
-- analyse lexicale d'une expression arithmetique
--
-- J-P. Peyrin, G. Serasset (version initiale) : janvier 1999
-- P.Habraken - 18/01/2010
--
-- 10 juin 2006 - PH :
-- remplacement record a champs variants par record "plat"
-- remplacement traitement iteratif par automate
--
-- Aout 2016 - LM : version C
------------------------------------------------------------------------ */

#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h>
#include "lecture_caracteres.h"
#include "analyse_lexicale.h"
#include "gestion_erreurs.h"
/* --------------------------------------------------------------------- */


/* Les variables et fonctions locales au module */
liste_modules modules; /* La liste des modules a appeler */
extern char nom_fichier_sortie[100];
Lexeme lexeme_en_cours ;	/* le lexeme courant */

void ajouter_caractere (char *s, char c);
Nature_Caractere nature_caractere (char c);
int est_separateur(char c ) ;
int est_chiffre(char c ) ;
int est_symbole(char c ) ;
int est_float(char c);
int est_lettre(char c);
void est_condition(char *tab);
void reconnaitre_lexeme();

/* --------------------------------------------------------------------- */

void demarrer(char *nom_fichier) {
   demarrer_car(nom_fichier);
   avancer();
}

/* --------------------------------------------------------------------- */

void avancer() { reconnaitre_lexeme(); } 

/* --------------------------------------------------------------------- */

Lexeme lexeme_courant() { return lexeme_en_cours; } 

/* --------------------------------------------------------------------- */

int fin_de_sequence() { return lexeme_en_cours.nature == FIN_SEQUENCE;}

/* --------------------------------------------------------------------- */

void arreter() { arreter_car() ; }

/* --------------------------------------------------------------------- */

// reconnaissance d'un nouveau lexeme
// etat initial : le caractere courant est soit separateur 
//                soit le 1er caractere d'un lexeme
// etat final : 
//       - un nouveau lexeme est reconnu dans lexeme_en_cours
//       - le caractere courant est soit la fin de fichier, 
//		soit un separateur,  soit le 1er caractere d'un lexeme

void reconnaitre_lexeme() {
   float pos = 1;
       //Indique le multiplicateur post virgule.
   typedef enum {E_INIT, E_ENTIER, E_CHAINE, E_FIN, E_FLOAT,
      E_AFFDT_DEUXPOINTS, E_AFFDT_FLECHE, E_TEXTE} Etat_Automate;
   Etat_Automate etat=E_INIT;
   // on commence par lire et ignorer les separateurs

   while (est_separateur(caractere_courant())) {
      avancer_car() ;
   } ;
   
   lexeme_en_cours.chaine[0] = '\0' ;

   // on utilise ensuite un automate pour reconnaitre et construire le prochain lexeme

   while (etat != E_FIN) {
      switch (etat) {
         case E_INIT: // etat initial

			   switch(nature_caractere(caractere_courant())) {

      			case C_FIN_SEQUENCE: 
                	lexeme_en_cours.nature = FIN_SEQUENCE;
                  etat = E_FIN;
      				break ;

               case LETTRE: 
                  lexeme_en_cours.nature = IDF;
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant());
                  etat = E_CHAINE;
                  avancer_car();
                  break;

      			case CHIFFRE: 
      	     		lexeme_en_cours.nature = ENTIER;
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
      	     		ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  lexeme_en_cours.valeur = caractere_courant() - '0';
                  etat = E_ENTIER;
      	   		avancer_car();
      				break;

      			case SYMBOLE: 
      	       	lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
      	       	ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  switch (caractere_courant()) {
                  	case '+':
                  		lexeme_en_cours.nature = PLUS;
                  		etat = E_FIN;
      		   			break;
                  	case '-':
                  		lexeme_en_cours.nature = MOINS;
                  		etat = E_FIN;
      		   			break;
                  	case '*':
                  		lexeme_en_cours.nature = MUL;
                  		etat = E_FIN;
      		   			break;
                     case '/': // ajout exo 2 tp1
                        lexeme_en_cours.nature = DIV;
                        etat = E_FIN;
                        break; // fin ajout
                     case '%': // modulo
                        lexeme_en_cours.nature = MODULO;
                        etat = E_FIN;
                        break;
                     case '^': // puissance
                        lexeme_en_cours.nature = POW;
                        etat = E_FIN;
                        break;
                     case '(':
                        lexeme_en_cours.nature = PARO;
                        etat = E_FIN;
                        break; // fin ajout
                     case ')':
                        lexeme_en_cours.nature = PARF;
                        etat = E_FIN;
                        break; // fin ajout
                     case '[':
                        lexeme_en_cours.nature = CROO;
                        etat = E_FIN;
                        break;
                     case ']':
                        lexeme_en_cours.nature = CROF;
                        etat = E_FIN;
                        break;
                     case '=':
                        lexeme_en_cours.nature = AFFEG;
                        etat = E_FIN;
                        break;
                     case ':':
                        etat = E_AFFDT_DEUXPOINTS;
                        break;
                     case '<':
                        lexeme_en_cours.nature = AFFDTF;
                        etat = E_AFFDT_FLECHE;
                        break;
                     case '\n': 
                        lexeme_en_cours.nature = SEP_INST;
                        etat = E_FIN;
                        break;
                     case ',':
                        lexeme_en_cours.nature = SEP_TAB;
                        etat = E_FIN;
                        break;
                     case '#':
                        lexeme_en_cours.nature = PRINT;
                        etat = E_FIN;
                        break;
                     case '!':
                        lexeme_en_cours.nature = TEXTE;
                        strcpy(lexeme_en_cours.chaine,"\0");
                        etat = E_TEXTE;
                        break;
                     case '@':
                        lexeme_en_cours.nature = INPUT;
                        strcpy(lexeme_en_cours.chaine,"@");
                        etat = E_FIN;
                        break;
      	       		default:
                        printf("Erreur lexicale ligne %d, colonne %d : Symbole inconnue\n",numero_ligne(),numero_colonne());
                        remove(".tmp");
                        remove(nom_fichier_sortie);
                        exit(0);
      			 	} ;
      	   		avancer_car() ;
      				break ;

      			default:
                  erreur(LEX);
            } ;
            break ;

   		case E_ENTIER:  // reconnaissance d'un entier
   			switch(nature_caractere(caractere_courant())) {
   			   case CHIFFRE:
   		  		   ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  lexeme_en_cours.valeur = lexeme_en_cours.valeur * 10 + caractere_courant() - '0';
                  avancer_car ();
   					break ;

               case FLOAT: //A modifier
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  etat = E_FLOAT;
                  avancer_car ();
                  break ;

   				default:
                  etat = E_FIN;
                  break;
            } break ;

         case E_FLOAT:
            switch(nature_caractere(caractere_courant())) {
   			   case CHIFFRE:
                  pos = pos * 0.1;
   		  		   ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
                  lexeme_en_cours.valeur = lexeme_en_cours.valeur + (pos * (caractere_courant() - '0'));
                  etat = E_FLOAT;
                  avancer_car ();
   					break ;

               default:
                  etat = E_FIN;
            }
            break;

         case E_CHAINE:
            switch(nature_caractere(caractere_courant())) {
               case LETTRE:
               case CHIFFRE:  
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant());
                  etat = E_CHAINE;
                  avancer_car();
                  break;
               default:
                  est_condition(lexeme_en_cours.chaine);
                  etat = E_FIN;
            } break;

         case E_TEXTE:
            switch(caractere_courant()) {
               case '!':
                  etat = E_FIN;
                  avancer_car();
                  break;
               default:
                  if (caractere_courant() == '\0' || caractere_courant() == '\n'){
                    erreur(LEX);}
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();
                  ajouter_caractere (lexeme_en_cours.chaine, caractere_courant());
                  avancer_car();
                  break;
            } break;

         case E_AFFDT_DEUXPOINTS:
            ajouter_caractere (lexeme_en_cours.chaine, caractere_courant()) ;
            switch (caractere_courant()) {
               case '=':
                  etat = E_FIN;
                  lexeme_en_cours.nature = AFFDTP;
                  avancer_car();
                  break; 
               case '(':
                  etat = E_FIN;
                  lexeme_en_cours.nature = FALSE;
                  avancer_car();
                  break;
               case ')':
                  etat = E_FIN;
                  lexeme_en_cours.nature = TRUE;
                  avancer_car();
                  break;
               default:
                  erreur(LEX);
            }
            break;

         case E_AFFDT_FLECHE:
            ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
            switch (caractere_courant()) {
               case '-':
                  etat = E_FIN;
                  avancer_car();
                  break; 
               default:
                  erreur(LEX_TEXTE);
            }
            break;

         case E_FIN:  // etat final
            break ;
	    
      } ; // fin du switch(etat)
	} ; // fin du while (etat != fin)
}

/* --------------------------------------------------------------------- */

// cette fonction ajoute le caractere c a la fin de la chaine s 
// (la chaine s est donc modifiee)

void ajouter_caractere (char *s, char c) {
   int l ;

   l = strlen(s) ;
   s[l] = c ;
   s[l+1] = '\0' ;
} ;

/* --------------------------------------------------------------------- */

Nature_Caractere nature_caractere (char c) {
   if (est_lettre(c)) return LETTRE;
   if (fin_de_sequence_car(c)) return C_FIN_SEQUENCE;
   if (est_chiffre(c)) return CHIFFRE;
   if (est_symbole(c)) return SYMBOLE;
   if (est_float(c)) return FLOAT;
   return ERREUR_CAR ;
}

/* --------------------------------------------------------------------- */

// vaut vrai ssi c designe un caractere separateur
int est_separateur(char c) { 
   return c == ' ' || c == '\t';
}

/* --------------------------------------------------------------------- */

// vaut vrai ssi c designe un caractere chiffre
int est_chiffre(char c) {
   return c >= '0' && c <= '9' ;
}

/* --------------------------------------------------------------------- */

// vaut vrai ssi c designe un caractere correspondant a un symbole 
int est_float(char c){
   switch (c) {
      case '.':
      return 1;
   default:
         return 0;
   }
}

/**********************************************************************/

int est_lettre(char c) {
   return (c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z');
}


/**********************************************************************/

int est_symbole(char c)  {
   switch (c) {
      case '+':  
      case '-':  
      case '*':
      case '%':
      case '^':
      case '/':
      case '(':
      case ')':
      case '[':
      case ']':
      case '=':
      case ':':
      case '<':
      case ',':
      case '#':
      case '!':
      case '\n':
      case '@':
         return 1;

      default:
         return 0;
   } 
}

/* --------------------------------------------------------------------- */

void est_condition(char *tab) {
   if (strcmp(tab,"si") == 0) {lexeme_en_cours.nature = SI;}
   else if (strcmp(tab,"alors") == 0) {lexeme_en_cours.nature = ALORS;}
   else if (strcmp(tab,"sinon") == 0) {lexeme_en_cours.nature = SINON;}
   else if (strcmp(tab,"tantq") == 0) {lexeme_en_cours.nature = TANTQ;}
   else if (strcmp(tab,"sup") == 0) {lexeme_en_cours.nature = SUP;}
   else if (strcmp(tab,"inf") == 0) {lexeme_en_cours.nature = INF;}
   else if (strcmp(tab,"supstr") == 0) {lexeme_en_cours.nature = SUP_STRICT;}
   else if (strcmp(tab,"infstr") == 0) {lexeme_en_cours.nature = INF_STRICT;}
   else if (strcmp(tab,"eg") == 0) {lexeme_en_cours.nature = EG;}
   else if (strcmp(tab,"dif") == 0) {lexeme_en_cours.nature = DIF;}
   else if (strcmp(tab,"finsi") == 0) {lexeme_en_cours.nature = FSI;}
   else if (strcmp(tab,"fintq") == 0) {lexeme_en_cours.nature = FTQ;}
   else if (strcmp(tab,"et") == 0) {lexeme_en_cours.nature = AND;}
   else if (strcmp(tab,"ou") == 0) {lexeme_en_cours.nature = OR;}
   else if (strcmp(tab,"fonction") == 0) {lexeme_en_cours.nature = DEF;}
   else if (strcmp(tab,"retourne") == 0) {lexeme_en_cours.nature = RETURN;}
   else if (strcmp(tab,"ff") == 0) {lexeme_en_cours.nature = FINF;}
   else if (strcmp(tab,"arrondir") == 0) {lexeme_en_cours.nature = ROUND;}
   else if (strcmp(tab,"attendre") == 0) {lexeme_en_cours.nature = ATT;append(SLEEP);}
   else if (strcmp(tab,"date") == 0) {lexeme_en_cours.nature = DATE;append(CTIME);}
   else if (strcmp(tab,"ajouter") == 0) {lexeme_en_cours.nature = AJOUTER;}
   else if (strcmp(tab,"taille") == 0) {lexeme_en_cours.nature = TAILLE;}
};

/* --------------------------------------------------------------------- */


// renvoie la chaine de caracteres correspondant a la nature du lexeme
char *Nature_vers_Chaine (Nature_Lexeme nature) {
   switch (nature) {
   	case ENTIER: return "ENTIER";
   	case PLUS: return "PLUS";
      case MOINS: return "MOINS";            
      case MUL: return "MUL"; 
      case DIV : return "DIV"; // ajout exo 2 tp1  
      case AFFEG : return "SYMBOLE =";
      case AFFDTF : return "SYMBOLE <-";
      case AFFDTP : return "SYMBOLE :=";
      case IDF : return "VARIABLE OU TABLEAU";
      case SEP_INST : return "RETOUR_LIGNE";   
      case SEP_TAB : return "SEPARATEUR TABLEAU";        
      case FIN_SEQUENCE: return "FIN_SEQUENCE";    
      case PARO: return "PARANTHESE_OUVRANTE";
      case PARF: return "PARANTHESE_FERMANTE";
      case CROO: return "CROCHET_OUVRANT";
      case CROF: return "CROCHET_FERMANT";
      case SI: return "CONDI SI";
      case ALORS: return "CONDI ALORS";
      case SINON: return "CONDI SINON";
      case TANTQ: return "CONDI TANT QUE";
      case SUP: return "CMP SUPERIEUR";
      case INF: return "CMP INFERIEUR";
      case INF_STRICT: return "CMP INFERIEUR STRICT";
      case SUP_STRICT: return "CMP SUPERIEUR STRICT";
      case EG: return "CMP EGALE";
      case DIF: return "CMP DIFFERENT";
      case FSI: return "FIN SI";
      case FTQ: return "FIN TANT QUE";
      case PRINT: return "PRINT";
      case TEXTE: return "TEXTE";
      case MODULO: return "MODULO";
      case POW: return "PUISSANCE";
      case TRUE: return "TRUE";
      case FALSE: return "FALSE";
      case AND: return "ET";
      case OR: return "OU";
      case DEF: return "DECLARATION FONCTION";
      case RETURN: return "RETURN";
      case FINF: return "FIN FONCTION";
      case ROUND: return "ARRONDIR";
      case DATE: return "DATE";
      case ATT: return "ATTENDRE";  
      case TAILLE: return "TAILLE"; 
      case AJOUTER: return "AJOUTER";
      default: return "ERREUR";            
   } ; 
} 

/* --------------------------------------------------------------------- */

// affiche a l'ecran le lexeme l
void afficher(Lexeme l) {

   switch (l.nature) {
      case FIN_SEQUENCE: 
         break;
      default: 
         printf("(ligne %d, ", l.ligne);
         printf("colonne %d) : ",l.colonne);
         printf("[") ;
         printf("nature = %s", Nature_vers_Chaine(l.nature)) ;
         printf(", chaine = %s, ", l.chaine) ;
         switch(l.nature) {
              case ENTIER:
                   printf("valeur = %.2f", l.valeur);
              default:
                   break;
         } ;
      printf("]") ;
   } ;
}

void init_modules(){
   modules.nombre_elem=0;
}

void append(nom_module n_module){
   modules.liste[modules.nombre_elem] = n_module;
   modules.nombre_elem++;
}

/* --------------------------------------------------------------------- */

