
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "func.h"



void open (FILE* F, const char nom[] , const char mode[1]) {

    if (mode == "N") { // new mode
        F = fopen(nom, "wb+");
            header h ;
        h.nb_block = 0;
        h.nb_eraser = 0;
        h.lastposlastblock= 0;
        fseek(F, 0, SEEK_SET);
    fwrite(&h, sizeof(h), 1, F);


    } else if(mode == "E") { // exist mode
        F = fopen(nom, "rb+");
            if (F == NULL) {
                perror("Error opening file");
            }
        }
        else{perror("Error opening file"); }

}
void close (FILE *f ){
    fclose(f);
    return ;
}

int get_header(char *namef , int n){ //machine abstraitre pour d�terminer les �l�ments de l'ent�te
header e ;
FILE *f = fopen(namef, "rb");
fread(&e , sizeof(e) , 1 , f);
fclose(f) ;

 if(n == 1) return e.nb_block ;
 if(n == 2) return e.lastposlastblock ;
 if(n == 3) return e.nb_eraser ;


 printf("ereur .\n"); //ceci est un sinon be tkhebya
 return 0 ;

}

void set_header(char *nomf , int n , int x){ //machine abstraitre pour affecter des valeurs sur les �l�ments de l'ent�te
header e ;

FILE *f = fopen(nomf , "rb+"); //rb+ pour la modification
 fseek(f , 0 , SEEK_SET);
fread(&e , sizeof(e) , 1 , f); //lire l'ent�te
fseek(f , 0 , SEEK_SET); //puisque on a fait fread , on va retourner le pointeur au d�but

 if(n == 1){
  e.nb_block = x ;
  fwrite(&e , sizeof(e) , 1 , f);
  fclose(f) ;
  return ;
 }
 if(n == 2){
  e.lastposlastblock = x ;
  fwrite(&e , sizeof(e) , 1 , f);
  fclose(f) ;
  return ;
 }
 if(n == 3){
  e.nb_eraser = x ;
  fwrite(&e , sizeof(e) , 1 , f);
  fclose(f) ;
  return ;
 }
 printf("il y a une erreur dans l'aff_entete.\n"); //ceci est un sinon be tkhebya
 fclose(f) ;
 return ;
}


int read_block(char *nomf , int i , record *Buff ){//(Liredir) prendre le ième Block de fichier dans le Buffer et retourne le pointeur vers ce Block

int p ;
FILE *f = fopen(nomf , "rb") ;
 if(f == NULL || i <= 0) return sizeof(header) ;//si le fichier est vide , on se positionne au premier bloque , efficace aux initialisations

p = sizeof(header) + (i - 1)*sizeof(*Buff) ;
fseek(f , p , SEEK_SET) ; //sauter vers ;e ième bloque

fread(Buff , sizeof(*Buff) , 1 , f) ;//lire le bloque

 fclose(f) ;
 return p ;
}

void write_block(char *nomf , int i , record Buff_write){ //ecrire dans un ième bloque le buffer Buff

int p ;

FILE *f = fopen(nomf , "rb+") ;
 if(f == NULL || i < 0) return ;

 p = sizeof(header) + (i - 1)*sizeof(Buff_write) ;
 fseek(f , p , SEEK_SET) ;//positionner à ce Block
 fwrite(&Buff_write , sizeof(Buff_write) , 1 , f); //ecriture

 fclose(f) ;

}

void allocate_block(const char namef)
{
    record a ;
    FILE *f=fopen(namef,"rb+");
     fseek(f, 0, SEEK_END);
     a.first_key=a.last_key=-1;
    fwrite(&a, sizeof(record), 1, f);

header e ;
 fseek(f , 0 , SEEK_SET);
fread(&e , sizeof(e) , 1 , f); //lire l'ent�te
fseek(f , 0 , SEEK_SET); //puisque on a fait fread , on va retourner le pointeur au d�but
 e.nb_block++  ;
  fwrite(&e , sizeof(e) , 1 , f);
}


void get_chaine( int taille_chaine , int *i , int *j ,char *nomf , char *chaine){//extraire une chaine dans le fichier d'une taille donnée dans le bloque i du position j

//i , j se mis à jour aprés le dernier caractère extrait
record Buff ;

 if(get_header(nomf , 1) == 0) return ;

read_block(nomf , *i , &Buff) ;//Buffer prend le ième Block , p pointe vers ce bloque

 for(int m = 0 ; m <= taille_chaine-1 ; m++){ //pour chaque itération on écrit un caractère
  chaine[m] = Buff.info[*j] ;
  *j = *j + 1;
   if(*j > lim-1){ //si on est arrivée au fin du Buffer , on va au prochain Block
    *i = *i + 1 ;
    *j = 0 ;
    read_block(nomf , *i , &Buff) ; //lire le suivant block
   }
 }

}
void write_chaine(char *nomf , int *i ,  int *j , char *chaine){ //écrire dans le ième Block de position j la chaine et faire des modifiactions si nécessaire de l'entaite

record Buff ;
int taille_chaine = strlen(chaine) ;

read_block(nomf , *i , &Buff) ;//Buffer prend le ième Block

 for(int m = 0 ; m <= taille_chaine-1 ; m++){ //pour chaque itération on écrit un caractère dans le Buffer
  Buff.info[*j] = chaine[m] ;
  *j = *j + 1;
   if(*j > lim-1){ //si on est arrivée au fin du Buffer , on va au prochain Block en écrivant les dernières modifications
    write_block(nomf , *i , Buff) ;//écriture
    *i = *i + 1 ;

    read_block(nomf , *i , &Buff) ; //lire le suivant block
    *j = 0 ;
   }
 }
 //dernière écriture
if(*j != 0) write_block(nomf , *i , Buff) ; //ecriture
 if(*i > get_header(nomf , 1) || (*i == get_header(nomf , 1) && *j >= get_header(nomf , 2))){ //il a eut une écriture dans la fin de fichier
  set_header(nomf , 1 , *i) ;
  set_header(nomf , 2 , *j) ;
 }

}

void chiffrement(char *nomb , int *x){//chiffrer une chaine de 3 chiffres en CODE_ASCII à un nombre entier
int p = 0 ,car , first = 0 ;
if(nomb[0] == '-') first = 1 ;

 for(int i = first ; i <= 2 ; i++){
  car = nomb[i] ;//prendre une unité ASCII
  car = car - 48 ; //codeASCII de 0 est 48 , car est maintenant le chiffre de l'unité
  p = p + car ; //ecriture de chiffre
  p = p*10 ; //multiplier pour aller au prochain chiffre
 }
 p = p / 10 ;
 if(nomb[0] == '-') p = -p ;
 *x = p ;
 return ;
}

bool searchTOVC(char *nomf , int key , int *taille_chaine , int *i , int *j){ //recherher un élément clé et le trouver dans bloc i et position j si il existe

record Buff ; //Buffer
int Binf = 1 , Bsup = get_header(nomf , 1) , cle_int , first_key , last_key , i_sauv , j_sauv  ; //Binf sera bloc 1 , Bsup sera bloc du derniere clé
char ch[lim] , cle_Buff[3] , taille_Buff[3] , eff ; //pour lire les propriétés du donnée


bool Stop = false ;
if (get_header(nomf,1)>1){// special case the last block dont have first and last key
read_block(nomf,get_header(nomf,1)-1,&Buff);
    int x,y ;
      x=get_header(nomf,1)-1 ;
      y= Buff.last_key;
  get_chaine( 3 , &x , &y , nomf ,cle_Buff) ;
  cle_Buff[3]='\0';
if (key>atoi(cle_Buff)){

      //partie extraction
      get_chaine( 1 , &x , &y ,nomf , &eff );
      get_chaine( 3 , &x , &y ,nomf , taille_Buff );
      chiffrement(taille_Buff , taille_chaine);
      chiffrement(cle_Buff , &cle_int) ;
      get_chaine(*taille_chaine , &x , &y ,nomf, ch );

 if ((y==get_header(nomf,2))&&(x==get_header(nomf,1))){

     *i=x; *j=y;
     return false;
     }}

}
 while( Binf <= Bsup && !Stop){
  *i = (int) (Binf + Bsup) / 2 ;


  read_block(nomf , *i , &Buff) ;//aller vers bloc i , la mediane entre Bsup et Binf et le lire

  i_sauv = *i ;
  j_sauv = Buff.first_key ;


  get_chaine( 3 , &i_sauv , &j_sauv ,nomf ,cle_Buff) ; //on lit la première clé du buffer
  chiffrement(cle_Buff , &first_key) ;




  i_sauv = *i ;
  j_sauv = Buff.last_key;
  get_chaine( 3 , &i_sauv , &j_sauv ,nomf , cle_Buff) ; //on lit la dernière clé du buffer
  chiffrement(cle_Buff , &last_key) ;


   if(key >= first_key && key <= last_key){ //si key est entre ces clés de bloc i
    *j = Buff.first_key ;

     while(*j <= Buff.last_key && !Stop){ //tant que on a pas parcourut jusqu'a la dernière clé
      i_sauv = *i ;
      j_sauv = *j ;
      //partie extraction
      get_chaine( 3 , i , j , nomf ,cle_Buff) ;
      get_chaine( 1 , i , j ,nomf , &eff );
      get_chaine( 3 , i , j ,nomf , taille_Buff );
      chiffrement(taille_Buff , taille_chaine);
      chiffrement(cle_Buff , &cle_int) ;
      get_chaine(*taille_chaine , i , j ,nomf, ch );
      //partie comparaison
          if(cle_int == key && eff == '0'){ //simple comparaison
          *i = i_sauv ;
          *j = j_sauv ;
          return true ; //on a trouvé la donnée avec toute les paramètre mis à jour
         } else if(key < cle_int){ //on arrète si key à rechercher est inférieur aux autres clés
          *i = i_sauv ;
          *j = j_sauv ;
          Stop = true ;
         }
     }
      *i = i_sauv ;
      *j = j_sauv ;
      Stop = true ;
   }
   else if(key > last_key) Binf = *i + 1 ; //sinon bouger vers prochaine interval [Binf ; Bsup] de blocs
   else Bsup = *i - 1 ;
 }
//si on est là , on a pas trouvé la clé , mais on doit affecter le j pour l'insertion plus tard
 if(!Stop){
  if(key > last_key){
   *j = Buff.last_key ;
        get_chaine( 3 , i , j , nomf ,cle_Buff) ;
        get_chaine( 1 , i , j ,nomf , &eff );
        get_chaine( 3 , i , j ,nomf , taille_Buff );
        chiffrement(taille_Buff , taille_chaine);
        chiffrement(cle_Buff , &cle_int) ;
        get_chaine(*taille_chaine , i , j ,nomf, ch );//j maitenant est prète
  } else *j = Buff.first_key ;
 }

 return false ;
}
void Dechiffrement(int x , char *nomb){//déchiffrer un nombre entier de 3 chiffres pour le rendre du tableau en CODE_ASCII
int p = x , last = 2 ;
int reste ;

if(p < 0){
  nomb[0] = '-' ;
  last = 1 ;
  p = -p ;
}

 for(int i = 0 ; i<=last ; i++){
  reste = p % 10 ;//prendre l'unité
  reste = reste + 48 ; //codeASCII de 0 est 48 , reste est maintenant le CODE ASCII de l'unité
  nomb[2-i] = reste ; //ecriture de la droite vers gauche
  p = p/10 ; //pour aller au prochain chiffre
 }
}

void Chargement_TOVC(char *nomf, int tai){ //Créer un LOVC de nombre de datas connus (tai)

FILE *f = fopen(nomf , "wb") ;//faire écraser et créer un nouveau fichier dans la machine d'utilisateur
fclose(f) ;//pas besoin de f ici , dans on fait fclose ici

char Data[lim-7] , cle[4] , length[4] , eff[2] = "0" ; //la donnée et la clé , taille+\0 , eff+\0
int i = 1 , j = 0 , i_sauv , j_sauv , pre , key;
bool New_Block = false ; //sert à traiter qui est la dernière et la première clé dans un bloque
printf(" (%d)",get_header(nomf,1));
//initialisation de l'entaite
 set_header(nomf , 1 , 1); //nbr de bloques initialisé
 set_header(nomf , 2 , 0);//initialiser la dernière position faisable à 0
 set_header(nomf , 3, 0) ;//nbr supprimé (=0 en création)


 Ecrire_pre_der_cle(nomf , 1 , 1 , 0); //initialisation du location du premier clé


 for(int k = 1 ; k <= tai ; k++){//chaque boucle il y aura une donnée prise
  printf("\n**Donnee la cle sous forme d un nombre (maximum 3 chiffres): ");
  scanf("%d",&key) ;
  printf("Ecrivez votre Data (Maximum %d - 7 = %d caracteres): ", lim , lim - 7 );
  scanf("%s",Data);

  if(k == 1 || pre < key){ //pour respecter la structure de TOVC (l'ordre)

  Dechiffrement(key , cle) ;//rendre la clé une chaine ASCII
  Dechiffrement(strlen(Data) , length) ; //Length maitenant a la taille de donnée sous forme d'une chaine
  length[3] = '\0' ; //pour length soit visualiser comme une vrai chaine pour la prochaine ligne
  cle[3] =  '\0' ;
  Data[lim - 6] = '\0' ; //ces affectations en c'est jamais qu'il y aura des fausses donnees et respecter la structure d'une chaine

  if(New_Block){ //si avant d'une ittiration il y avait eut un chevauchement
   New_Block = false ;
   Ecrire_pre_der_cle(nomf , i , 1 , j) ; //si un nouveau bloque est détecté , on sauvegarde la position du premier clé
  }

  i_sauv = i ;
  j_sauv = j ;

  if(k == tai){
   Ecrire_pre_der_cle(nomf , i , 2 , j) ; //on va écrire la dernière position cle
  }

  write_chaine(nomf  , &i , &j , cle) ;
  write_chaine(nomf  , &i , &j , eff) ;
  write_chaine(nomf  , &i , &j , length);
  write_chaine(nomf , &i , &j , Data) ;

  if(i_sauv != i ){ //il a eut un chevanchement ou bien on est dans la dernière data
   New_Block = true ;
   Ecrire_pre_der_cle(nomf , i_sauv , 2 , j_sauv) ;
  }

  pre = key ;//sauvegarder l'ancienne clé
  } else {
    printf("!Creation de TOVC a besoin des elements ordonnees non repetitives ! O_O\n");
    k-- ;//l'utilisateur a donné un élément incorrecte , il doit refaire la boucle .
   }
  }

}
void delete_tvs(char *name,int key)
{
    record buf1,buf2;
    FILE *f=fopen(name,"rb+");
    int i,j;
    int taille ;


if (!searchTOVC(name, key, &taille, &i, &j)){
    printf("this key is not exist %d,%d",i,j);
    return;
}else{
read_block(name,i,&buf1);
  int save_i=i;
  int savej=j,longeur;
  char key_erase[4];
  char lon[3];
  char info[100];
  char test[4] ;

get_chaine(4,&i,&j,name ,key_erase);
get_chaine(3,&i,&j,name,lon);

chiffrement(lon,&longeur);
get_chaine(longeur,&i,&j,name,info) ;
 printf("%d|%d|%d|%d",i,j,get_header(name,1),get_header(name,2));

while (i!=get_header(name,1)||  (j<get_header(name,2)))
{
 get_chaine(longeur+7,&i,&j,name,info) ;
 write_chaine(name,&save_i,&savej,info);

}
int last_pos ;
int long_record=longeur+7;
int nb_block ;
if (get_header(name,2)<=long_record){
    nb_block= get_header(name,1)-1;
}else nb_block=get_header(name,1);
set_header(name,1,nb_block) ;

last_pos=get_header(name,2)-(long_record % lim );
if (last_pos<0) last_pos=last_pos+lim;
set_header(name,2,last_pos) ;

  Update_key_amplitudes(name);
close(f);
}
}
void inserTOVs(char *nomf , char *cle , char *Data) {
    int key, i_write, j_write, i_receive, j_receive,i1=1,j1=0;
    int ta = strlen(Data), t;
    int Old_block = get_header(nomf, 1), Old_Posi = get_header(nomf, 2);
     char Data_write[lim], length[3];
    chiffrement(cle, &key);


Dechiffrement(ta, length);
    if (searchTOVC(nomf, key, &t, &i_receive, &j_receive))  return; // Key exists, exit
    if (get_header(nomf,1)==0){ FILE *f = fopen(nomf , "wb") ;//faire écraser et créer un nouveau fichier dans la machine d'utilisateur
fclose(f) ;
                 Ecrire_pre_der_cle(nomf, 1 , 1 , 0); //initialisation du location du premier clé
                 write_chaine(nomf, &i1 , &j1 , cle) ;
                 write_chaine(nomf, &i1 , &j1 , "0") ;
                 write_chaine(nomf, &i1 , &j1 ,length);
                 write_chaine(nomf, &i1 , &j1 , Data) ;
                 Update_key_amplitudes(nomf);
                 return;
    }

    // Prepare data to insert


    memcpy(Data_write, cle, 3);
    Data_write[3] = '0'; // Eff status
    memcpy(Data_write + 4, length, 3); // Length field
    memcpy(Data_write + 7, Data, ta);
    Data_write[ta + 7] = '\0';

    i_write = i_receive;
    j_write = j_receive;


    bool Stop = false, Final_write = false;
    char Data_receiv[lim];

    while (!Final_write) {
        Final_write = Stop; // Last write operation if Stop is set

        if (i_receive == Old_block - 1 && j_receive + ta + 7 - lim >= Old_Posi && !Final_write) {
            t = Old_Posi - j_receive + lim;
            Stop = true;
        } else if (i_receive == Old_block && j_receive + ta + 7 >= Old_Posi && !Final_write) {
            t = Old_Posi - j_receive;
            Stop = true;
        } else if (!Final_write) {
            t = ta + 7;
        }

        if (!Final_write) {
            get_chaine(t, &i_receive, &j_receive, nomf, Data_receiv);
        }
        write_chaine(nomf, &i_write, &j_write, Data_write);


        // Copy received data into Data_write for the next loop iteration
        memcpy(Data_write, Data_receiv, t);
        Data_write[t] = '\0';
    }

    Update_key_amplitudes(nomf); // Update key amplitudes after insertion
}


void Ecrire_pre_der_cle(char *nomf , int i , int n , int x){//ecrire dans le ièeme block Buff.dern/Buff.prem la position x (1 pour prem , 2 pour dern)

record Buff;
int p ;

FILE *f = fopen(nomf , "rb+") ;
 if(f == NULL || i < 0) return ;

 p = read_block(nomf , i , &Buff) ; //aller au ième block
 fseek(f , p , SEEK_SET) ;//positionner à ce Block
/* printf("TEST LIREDIR : Sizeof(ent) : %d    Sizeof(Buff) : %d     i = %d ---> ",sizeof(Ent) , sizeof(Buff) , i);
 printf("p = %d\n",p);*/

 if(n == 1){ //ecrire dans buff.prem
  Buff.first_key = x ;
  fwrite(&Buff , sizeof(Buff) , 1 , f);
  fclose(f) ;
  return ;
 }
 if(n == 2){ //ecrire dans buff.dern
  Buff.last_key = x ;
  fwrite(&Buff , sizeof(Buff) , 1 , f);
  fclose(f) ;
  return ;
 }

 printf("il y a une erreur dans l'Ecri_pre_der.\n"); //ceci est un sinon be tkhebya
 fclose(f) ;
 return ;
}
void Update_key_amplitudes(char *nomf){ //cette fonction va rigller all first and last keys

int i = 1 , j = 0 , i_sauv , j_sauv , taille_chaine ;
bool New_Block = false ; //sert à traiter qui est la dernière et la première clé dans un bloque

char ch[lim] , cle_Buff[3] , taille_Buff[3] , eff ; //pour lire les propriétés du donnée

Ecrire_pre_der_cle(nomf , 1 , 1 , 0); //initialisation du location du premier clé

  while ((i!=get_header(nomf,1))|| (j !=get_header(nomf,2))){ //parcourt des données effaçés ou pas

   if(New_Block){ //si avant d'une ittiration il y avait eut un chevauchement
   New_Block = false ;
   Ecrire_pre_der_cle(nomf , i , 1 , j) ; //si un nouveau bloque est détecté , on sauvegarde la position du premier clé
   }

   i_sauv = i ;
   j_sauv = j ;


   get_chaine( 3 , &i , &j ,nomf , cle_Buff) ;
   get_chaine(1 ,  &i , &j ,nomf , &eff) ;
   get_chaine( 3 , &i , &j ,nomf , taille_Buff);
   chiffrement(taille_Buff,&taille_chaine);
   get_chaine( taille_chaine , &i , &j ,nomf , ch) ;

   if ((j==get_header(nomf,2)) && (i==i_sauv))  Ecrire_pre_der_cle(nomf , i , 2 , j_sauv) ;

    if(i_sauv != i ){ //il a eut un chevanchement ou bien on est dans la dernière data
    New_Block = true ;
    Ecrire_pre_der_cle(nomf , i_sauv , 2 , j_sauv) ;
   }
  }
}
void Suppression_logic_TOVC(char *nomf , int key){

int length, i , j , t ;


char cle_Buff[3] , taille_Buff[3] ;

if(searchTOVC(nomf , key , &t , &i ,&j) == false) return ; //element n'exitse pas
//sinon on desactive le champ effacer

get_chaine( 3 , &i , &j ,nomf , cle_Buff) ;//sauter la clé
write_chaine(nomf , &i , &j , "1");//ecrire eff = 1 pour la suppression logique
get_chaine(3 , &i , &j ,nomf , taille_Buff) ;//sauter la taille
chiffrement(taille_Buff,&length);
set_header(nomf,3,length+7);

}
void Reorganisation_TOVC(char *nomf){

char ch[lim-7] , cle_Buff[4] , taille_Buff[4] , eff ; //pour lire les propriétés du donnée , 4ieme caractère pour \0
int length , i1 = 1 , j1 = 0 , i2 = 1 , j2 = 0 , cpt = 0;
record BUF ;

FILE *f2 = fopen("new.bin" , "wb") ; //écraser tous le contenu de Brouillant pour le réétuliser
fclose(f2) ;
//initialiser l'entète de Brouillant
//Brouillant va ètre le fichier résultat , puis on va le copier vers fichier principal , c'est juste un fichier local
set_header("new.bin" , 1 , 1) ;
set_header("new.bin" , 2 , 0) ;
set_header("new.bin" , 3 , 0) ;


    while ((i1!=get_header(nomf,1))|| (j1 !=get_header(nomf,2))){ //while mal7e9tsh la fin de f1
   //On lit tous les propriétés de l'info
   get_chaine(3 , &i1 , &j1 ,nomf, cle_Buff );
   cle_Buff[3] = '\0' ;
   get_chaine(1 , &i1 , &j1 ,nomf, &eff );
   get_chaine(3 , &i1 , &j1 ,nomf, taille_Buff );
   taille_Buff[3] = '\0' ;
   chiffrement(taille_Buff , &length);
   get_chaine( length , &i1 , &j1 ,nomf, ch );
       ch[length] = '\0' ;
    if(eff == '0'){ //si ellen n'est pas supprimés , on l'écrit

       write_chaine("new.bin" ,&i2 , &j2 , cle_Buff);

       write_chaine("new.bin",&i2 , &j2 , "0") ;
       write_chaine("new.bin" ,&i2 , &j2 , taille_Buff);

       write_chaine("new.bin" ,&i2 , &j2 , ch) ;
       read_block("new.bin",1,&BUF);
     printf("{%s}[%d]",BUF.info,cpt);


    }
  }

  Update_key_amplitudes("new.bin");

  fcpy("new.bin" , nomf) ;

}
void fcpy(char *nomf1 , char *nomf2){//copier de fichier 1 vers 2

header t ;
record Buff ;
FILE *old = fopen( nomf1 , "rb" ) , *neww = fopen( nomf2 , "wb" );

fread(&t , sizeof(t) , 1 , old) ;
fwrite(&t , sizeof(t) , 1 , neww) ;

 for(int i = 1 ; i <= get_header(nomf1 , 1) ; i++){
  fread(&Buff , sizeof(Buff) , 1 , old) ;
  fwrite(&Buff , sizeof(Buff) , 1 , neww) ;
 }

 fclose(old);
 fclose(neww);
}

void dsiplay_header(char*name)
{
FILE*f=fopen(name,"rb");

header e ;
 fseek(f , 0 , SEEK_SET);
fread(&e , sizeof(e) , 1 , f); //lire l'ent�te
printf("nember of blocks: %d\nthe first position free in the last block: %d\n number of char ereased: %d",e.nb_block,e.lastposlastblock,e.nb_eraser);
close(f);
}

void display_block(char* name,int i)
{
record buf ;
int j=0;
read_block(name,i,&buf);
printf("BLOCK %d  : ",i);
while((j<lim)&&((get_header(name,1)!=i||get_header(name,2)>j))){


 printf("%c|",buf.info[j]);

 j++;
}
 printf("\n");
}
void find_intersection(char *file1, char *file2, char *output_file) {
    int i1 = 1, j1 = 0, taille1,length,length2;
    int i2, j2, i3,j3,taille2,key;
    char key_buff[4], data1[lim], data2[lim];
    char ch[lim] , cle_Buff[4] , taille_Buff[4] , eff ,ch2[lim] , cle_Buff2[4] , taille_Buff2[4] , eff2 ;

    // Create the output file
    FILE *out = fopen(output_file, "wb");
    fclose(out);
    set_header(output_file, 1, 0); // Initialize header

    set_header(output_file, 2, 0);
    set_header(output_file, 3, 0);

    // Iterate through all records in the first file
    while ((i1!=get_header(file1,1))|| (j1 !=get_header(file1,2))){

   get_chaine(3 , &i1 , &j1 ,file1, cle_Buff );
   cle_Buff[3] = '\0' ;
   get_chaine(1 , &i1 , &j1 ,file1, &eff );
   get_chaine(3 , &i1 , &j1 ,file1, taille_Buff );
   taille_Buff[3] = '\0' ;
   chiffrement(taille_Buff , &length);
   get_chaine( length , &i1 , &j1 ,file1, ch );
       ch[length] = '\0' ;


            // Check if the key exists in the second file
            if (searchTOVC(file2, atoi(cle_Buff), &taille2, &i2, &j2)) {

                // Read data from second file
                  get_chaine(3 , &i2 , &j2 ,file2, cle_Buff2 );
                  cle_Buff2[3] = '\0' ;
                  get_chaine(1 , &i2 , &j2 ,file2, &eff2 );
                  get_chaine(3 , &i2 , &j2 ,file2, taille_Buff2 );
                  taille_Buff2[3] = '\0' ;
                  chiffrement(taille_Buff2 , &length2);
                  get_chaine( length2 , &i2 , &j2 ,file2, ch2 );
                  ch2[length2] = '\0' ;
                      if ((strcmp(ch2,ch)==0)){
               inserTOVs(output_file,cle_Buff,ch2);
            }}


    }


    }
    void concatenate_files(const char *file1, const char *file2, const char *output_file) {


  int i2 = 1, j2 = 0, taille1,length,length2;
    int  i3,j3,taille2,key;
    char key_buff[4], data1[lim], data2[lim];
    char ch[lim] , cle_Buff[4] , taille_Buff[4] , eff ,ch2[lim] , cle_Buff2[4] , taille_Buff2[4] , eff2 ;

    // Create the output file
    FILE *out = fopen(output_file, "wb");
    fclose(out);
    set_header(output_file, 1, 0); // Initialize header

    set_header(output_file, 2, 0);
    set_header(output_file, 3, 0);

    //copy the file 1
    fcpy(file1,output_file);

   //INSERT all record of 2nd if not exist in our out put file
            while( i2!=get_header(file2,1)||j2!=get_header(file2,2)) {


                // Read data from second file
                  get_chaine(3 , &i2 , &j2 ,file2, cle_Buff2 );
                  cle_Buff2[3]='\0';
                  get_chaine(1 , &i2 , &j2 ,file2, &eff2 );
                  get_chaine(3 , &i2 , &j2 ,file2, taille_Buff2 );

                  chiffrement(taille_Buff2 , &length2);
                  get_chaine( length2 , &i2 , &j2 ,file2, ch2 );
                  ch2[length2]='\0';

               inserTOVs(output_file,cle_Buff2,ch2);
            }


    }
    void overlaping(char* file)
{
record buf ;
int length ;
char ch[lim] , cle_Buff[4] , taille_Buff[4]  , eff;
for (int i=1;i<=get_header(file,1)-1;i++){
        int k;
        k=i;
read_block(file,k,&buf);
int j=buf.last_key;

                 get_chaine(3 , &k , &j ,file, cle_Buff );
                  cle_Buff[3]='\0';
                  get_chaine(1 , &k , &j ,file, &eff );

                  get_chaine(3 , &k , &j ,file, taille_Buff );

                  chiffrement(taille_Buff , &length);
                  get_chaine( length , &k , &j ,file, ch );
                  ch[length]='\0';

if (k!=i) { printf("|%s",cle_Buff);
printf("|%c",eff);
printf("|%s",taille_Buff);
printf("|%s",ch);}}
}


void MENU() { // A menu
    printf("-----------------------------Menu-----------------------------\n");
    printf("1) Create a binary TOVC file.\n");
    printf("2) Read a block.\n");
    printf("3) Read the content of the header.\n");
    printf("4) Insert data.\n");
    printf("5) Binary search for data.\n");
    printf("6) Logical deletion of data.\n");
    printf("7) Reorganize the TOVC file.\n");
    printf("8) Physical deletion.\n");
    printf("9) Intersection of two files.\n");
    printf("10) Concatenate two files.\n");
    printf("11) Read the content of a file.\n");
    printf("12) Overlapping records.\n");
    printf("13) Exit.\n");
    printf("--------------------------------------------------------------\n\n");
}
