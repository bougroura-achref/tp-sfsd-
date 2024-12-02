
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define lim 100  // Assuming a fixed block size

// Structure representing a record in the file
typedef struct {
    char info[lim];
    int first_key;
    int last_key;
} record;

// Structure representing the header of the file
typedef struct {
    int nb_block;
    int lastposlastblock; // First free position in the last block
    int nb_eraser;        // Number of deleted characters or sub-existing ones with eff = 1
} header;

// Function to open a file in a specific mode ("N" for new, "E" for existing)
void open(FILE* F, const char nom[], const char mode[1]);

// Function to close a file
void close(FILE *f);

// Function to retrieve a specific header element (nb_block, lastposlastblock, or nb_eraser)
int get_header(char *namef, int n);

// Function to set a specific header element (nb_block, lastposlastblock, or nb_eraser)
void set_header(char *nomf, int n, int x);

// Function to read a block from the file into a buffer
int read_block(char *nomf, int i, record *Buff);

// Function to write a block from a buffer to the file
void write_block(char *nomf, int i, record Buff_write);

// Function to allocate a new block and update the header
void allocate_block(const char namef);

// Function to extract a string of a given size from a block starting at a given position
void get_chaine(int taille_chaine, int *i, int *j, char *nomf, char *chaine);

// Function to write a string into a block at a specific position
void write_chaine(char *nomf, int *i, int *j, char *chaine);

// Function to encrypt a 3-digit string into an integer
void chiffrement(char *nomb, int *x);

// Function to search for a key in the file, return position (i, j) if found
bool searchTOVC(char *nomf, int key, int *taille_chaine, int *i, int *j);
void Dechiffrement(int x , char *nomb);
void Chargement_TOVC(char *nomf, int tai);
void delete_tvs(char *name, int key);
void inserTOVs(char *nomf , char *cle , char *Data);
void Ecrire_pre_der_cle(char *nomf , int i , int n , int x);
void Update_key_amplitudes(char *nomf);
void Suppression_logic_TOVC(char *nomf , int key);
void Reorganisation_TOVC(char *nomf);
void fcpy(char *nomf1 , char *nomf2);
void dsiplay_header(char* name);
void display_block(char* name, int i);
void find_intersection(char *file1, char *file2, char *output_file);
void concatenate_files(const char *file1, const char *file2, const char *output_file);
void overlaping(char* file);

#endif // FUNCTIONS_H
