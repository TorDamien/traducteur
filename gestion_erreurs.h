#ifndef _GESTION_ERREURS_
#define _GESTION_ERREURS_

typedef enum {
    LEX,
    LEX_TEXTE,
    SYNT,
    S_TAB_NONINI,
    S_OP,      // Enchainement OP Terme non respecté dans eag
    S_PARF, // PARF Attendu
    S_PARO, // PARO Attendu
    S_VAR, // Variable indéfini
    S_TER, // Erreur terme
    S_TABCROO,
    S_TABCROF, // Crochet fermant
    S_TAB_TER, // Terme entre crochet du tableau
    S_IDF, // IDF attendus
    S_AFF, // AFF attendu
    S_SEP, // Manque Séparateur
    S_CONDI,
    S_CMP,
    S_NOMF,
    S_FDEUXP,
    S_TAB_DEF,
    S_INST,
    S_FIN, // Pas de fin de séquence
    S_FSI,  // FINSI ou ALORS 
    S_RETURN,
    S_FINF,
    S_ARG,
    S_FONCTION
} type_erreur;



void erreur(type_erreur Err);
    // Fonction générale des gestions d'erreurs 


#endif