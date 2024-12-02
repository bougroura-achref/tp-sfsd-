#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifndef _functionTNOVC_H
#define _functionTNOVC_H


#define b 100 // Nb enregistrement par bloc  key 3 ,  eraser 1 , taille 3   ,  max-info (b-7)

typedef struct 
{
 char info[b];
}record;

typedef struct 
{
  int nb_block ;
  int  pos_free ; //la premiere position libre dans le dernier Bloc
  int nb_eraser ;//nbr de caracteres supprim. ou bien sub_existante avec eff = 1 (logiquement supprime)
}header ;

// Forward declarations for functions
void open (FILE* F ,  char *nom , char mode);
void close (FILE *f );
void allocate_block( char *namef);
bool read_Block(char *nomf, int i, record* Buf);
bool write_Block(char *name, int i, record Buf);
int get_header(char *namef, int n);
void set_header(char *nomf, int n, int x);
void write_chaine(char *nomf, int *i, int *j, char *chaine);
void insert_tnovs(char *name, char key[3], char *info);
void search_tnovs(char *name, char cle[3], bool *trouve, int *i, int *j);
void get_chaine(int type, int *i, int *j, char *name, char *key, int max_taille);
void display_block(char* name,int i);
void dsiplay_header(char*name);
void display_file(char* name );
void delete_logic(char *name, char key[3]) ;
void delete_phis_tnovs(char *name , char key[3]);
bool is_reorganization(char *name);
void reorganization(char *old ,char *nw);
void display_chev(char* name);
void Chargement_TNOVC(char *nomf, int tai);
void MENU();
#endif