

CC = gcc -Wall -g

EXECUTABLES = test_lexeme test_table traducteur_python

all: $(EXECUTABLES)

%.o: %.c
	$(CC) -c $<

lecture_caracteres.o: lecture_caracteres.c lecture_caracteres.h 

test_lexeme.o: test_lexeme.c analyse_lexicale.h table_symbole.h

analyse_lexicale.o: analyse_lexicale.c analyse_lexicale.h lecture_caracteres.h gestion_erreurs.h

analyse_syntaxique.o: analyse_syntaxique.c analyse_syntaxique.h analyse_lexicale.h  table_symbole.h 

table_symbole.o: table_symbole.c table_symbole.h

test_table.o: test_table.c table_symbole.h

traducteur_python.o: traducteur_python.c analyse_syntaxique.h analyse_lexicale.h 

gestion_erreurs.o: gestion_erreurs.c gestion_erreurs.h analyse_syntaxique.h analyse_lexicale.h

test_lexeme: test_lexeme.o analyse_lexicale.o lecture_caracteres.o  table_symbole.o gestion_erreurs.o
	$(CC) $^ -o $@ 

test_table: test_table.o table_symbole.o 
	$(CC) $^ -o $@ 

traducteur_python: traducteur_python.o analyse_syntaxique.o analyse_lexicale.o lecture_caracteres.o table_symbole.o gestion_erreurs.o
	$(CC) $^ -o $@ 

clean: 
	rm -f *.o

clean_all: 
	rm -f $(EXECUTABLES) *.o
