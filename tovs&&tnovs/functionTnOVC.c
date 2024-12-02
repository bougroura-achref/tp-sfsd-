#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
    #include "functionTNOVC.h"





void open (FILE* F ,  char *nom , char mode) {
    
    if (mode == 'N') { // nw mode 
        F = fopen(nom, "wb+");
            header h ;
        h.nb_block = 0;
        h.nb_eraser = 0;
        h.pos_free= 0;
        fseek(F, 0, SEEK_SET);
    fwrite(&h, sizeof(h), 1, F);


    } else if(mode == 'E') { // exist mode 
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


void allocate_block( char *namef)
{
    record a ;
    FILE *f=fopen(namef,"rb");
     fseek(f, 0, SEEK_END);
    fwrite(&a, sizeof(record), 1, f);

header e ;
 fseek(f , 0 , SEEK_SET);
fread(&e , sizeof(e) , 1 , f); //lire l'ent�te
fseek(f , 0 , SEEK_SET); //puisque on a fait fread , on va retourner le pointeur au d�but
 e.nb_block++  ;
  fwrite(&e , sizeof(e) , 1 , f);
  fclose(f);
}



void get_chaine(int n ,int *i,int *j ,char *nomf,char *chaine,int max_taille)
{

//i , j se mis à jour aprés le dernier caractère extrait
record Buff ;

 if(get_header(nomf , 1) == 0) return ;

read_Block(nomf , *i , &Buff) ;//Buffer prend le ième Block , p pointe vers ce bloque

 for(int m = 0 ; m < n ; m++){ //pour chaque itération on écrit un caractère
  chaine[m] = Buff.info[*j] ;
  *j = *j + 1;
   if(*j > max_taille-1){ //si on est arrivée au fin du Buffer , on va au prochain Block
    *i = *i + 1 ;
    *j = 0 ;
    read_Block(nomf , *i , &Buff) ; //lire le suivant block
   }
 }
 chaine[n]='\0';
        //  printf("test of get chaine %s \n",chaine);
}

void write_chaine(char *nomf , int *i ,  int *j , char *chaine){ //écrire dans le ième Block de position j la chaine et faire des modifiactions si nécessaire de l'entaite

record Buff ;
int taille_chaine = strlen(chaine) ;

read_Block(nomf , *i , &Buff) ;//Buffer prend le ième Block

 for(int m = 0 ; m <= taille_chaine-1 ; m++){ //pour chaque itération on écrit un caractère dans le Buffer
  Buff.info[*j] = chaine[m] ;
  *j = *j + 1;
   if(*j > b-1){ //si on est arrivée au fin du Buffer , on va au prochain Block en écrivant les dernières modifications
    write_Block(nomf , *i , Buff) ;//écriture
    *i = *i + 1 ;

    read_Block(nomf , *i , &Buff) ; //lire le suivant block
    *j = 0 ;
   }
 }
 //dernière écriture
if(*j != 0) write_Block(nomf , *i , Buff) ; //ecriture
 if(*i > get_header(nomf , 1) || (*i == get_header(nomf , 1) && *j >= get_header(nomf , 2))){ //il a eut une écriture dans la fin de fichier
  set_header(nomf , 1 , *i) ;
  set_header(nomf , 2 , *j) ;
 }

}

void insert_tnovs( char *name ,  char key[3] ,  char *info )
{ 
     record nw ;
    int k;
       int i,j ;
    bool trouve ;

 // the first insertion 
 if (get_header(name,1)==0)
 {
        printf("the first insertion in the file \n");
    j=0;
i=1;
write_chaine(name,&i,&j,key) ; // write the key 

write_chaine(name,&i,&j,"0") ; // write the erase boolean in the beginig "0"(not erased )
   char longg[4];
  sprintf(longg,"%03d",strlen(info));// we  change the length from int  to string
  write_chaine(name,&i,&j,longg) ; // write the length of the information of the record 
write_chaine(name,&i,&j,info) ;  // write the inforamtion of the record 

set_header(name,2,j); // update the header 
set_header(name,1,i);

return ;
}

 search_tnovs(name,key ,&trouve,&i,&j);
 if (trouve==true)
  {
    printf("\n this key exist \n");
    return ;
    }
    // printf("\n insert ");

i=get_header(name,1);
j=get_header(name,2);
read_Block(name,i,&nw);
 
 
 write_chaine(name,&i,&j,key) ; // write the key 

write_chaine(name,&i,&j,"0") ; // write the erase boolean in the beginig "0"(not erased )
   char longg[4];
  sprintf(longg,"%03d",strlen(info)); // we  change the length from int  to string 
  write_chaine(name,&i,&j,longg) ; // write the length of the information of the record 
write_chaine(name,&i,&j,info) ;  // write the inforamtion of the record 

set_header(name,2,j); // update the header 
set_header(name,1,i);
}

int get_header(char *namef , int n){ //machine abstraitre pour determiner les elements de l'entete
header e ;
FILE *f = fopen(namef, "rb");
fread(&e , sizeof(e) , 1 , f);
fclose(f) ;

 if(n == 1) return e.nb_block ;
 if(n == 2) return e.pos_free ;
 if(n == 3) return e.nb_eraser ;
 

 printf("ereur .\n"); //ceci est un sinon be tkhebya
 return 0 ;

}

void search_tnovs(char *name ,  char cle[3]  ,bool *trouve , int *i,int *j)
{
// FILE *f;
record buf ;
char lon[3];
char erase ;
 char key[3] ;
*trouve=false ;
// open(f,name,'E');
*i=1 ;
*j=0 ;
int savei,savej;
int n=get_header(name,1);
while ((*i < n||*i==n && *j<get_header(name,2)) && (!*trouve))
{
 
   savei=*i;
   savej=*j; 
get_chaine(3,i,j,name,key,b) ;
        // printf("\nkey %s || cle %s ",key,cle);
  //  printf("\n%s --%s\n",key,cle);
 if (strcmp(key,cle)==0 ) {
   get_chaine(1,i,j,name,&erase,b) ;
  //  printf(" the erase %c \n",erase);

    if  (erase=='0') 
     { 
      *i=savei;
      *j=savej;
        
        *trouve=true ;
    }
    else{

    get_chaine(3,i,j,name,lon,b) ;
     int longeur=atoi(lon);
          printf("\n here long %d",longeur);
       *j=*j+longeur ; 
   if (*j>=b)       
   {
    *j=*j%b ;
    *i=*i+(*j/b);
          read_Block(name,*i,&buf);

   }

     }    

    }
 else 
 {
   get_chaine(1,i,j,name,&erase,b) ;
    get_chaine(3,i,j,name,lon,b) ;
     int longeur=atoi(lon);
          // printf("\n here long %d",longeur);
       *j=*j+longeur ; 
   if (*j>=b)       
   {
    *j=*j%b ;
    *i=*i+(*j/b);
          read_Block(name,*i,&buf);
   }
   }
  }
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
  e.pos_free = x ;
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


bool write_Block (char *name, int i, record Buf) {
    FILE* F=fopen(name,"rb+");
    if (F == NULL || i <= 0 || fseek(F, (i - 1) * sizeof(record) + sizeof(header), SEEK_SET) != 0) {
      fclose(F);
        return false;
    }

    if (fwrite(&Buf, sizeof(record), 1, F) != 1) {
              fclose(F);
        return false;  // Check if the write was successful
    }
      fclose(F);
    return true;
}


// Function to read a block from the file
bool read_Block(char *nomf , int i, record* Buf) {
    FILE *F = fopen(nomf , "rb") ;
    if (F == NULL || i <= 0 || fseek(F, (i - 1) * sizeof(record) + sizeof(header), SEEK_SET) != 0) {
             fclose(F) ;
        return false;
    }

    if (fread(Buf, sizeof(record), 1, F) != 1) {
             fclose(F) ;
        return false;  // Check if the read was successful
    }
     fclose(F) ;
    return true;
}

void delete_logic(char *name, char key[3]) 
{
    bool trouve ;
    int i,j ;
    record buf;
     char cle[3] ;
      char lon[3] ;


search_tnovs(name,key,&trouve,&i,&j);
if (trouve==true)
{
get_chaine(3,&i,&j,name,cle,b);
   write_chaine (name,&i,&j,"1"); // 1 means it is erased 
   get_chaine(3,&i,&j,name,lon,b); // we get the number of character in info 
int longeur=atoi(lon);

   set_header(name,3,get_header(name,3)+longeur+7);// we add the number of all character in the record (removed )
        printf("\nLa donnee a ete bien supprimee ! ");
}
else   printf("this key is not exist ");
}



void delete_phis_tnovs(char *name , char key[3])
{
    record buf1,buf2;
    int i,j;
    bool trouve;
search_tnovs(name,key,&trouve,&i,&j);
if (!trouve){
    printf("this key is not exist ");
    return;
}else{
int save_i=i;
int savej=j;
char cle[3];
char erase ;
char lon[3];
bool stop =false ;
get_chaine(3,&i,&j,name,cle,b);
   get_chaine(1,&i,&j,name,&erase,b) ;
get_chaine(3,&i,&j,name,lon,b);

     int longeur=atoi(lon); 
     char rec[longeur];  // we get on rec the inforamtion that we have delete it 
get_chaine(longeur,&i,&j,name,rec,b);
char info[longeur+7];
search_tnovs(name,key,&trouve,&save_i,&savej);
while (!stop)
{ 
get_chaine(longeur+7,&i,&j,name,info,b); 
 write_chaine(name,&save_i,&savej,info);

if ((i==get_header(name,1)&&j>=get_header(name,2))||i>get_header(name,1))
{
    stop=true ; // last iteration 
}

}
int last_pos ;
int long_record=longeur+7;
int nb_block ;
if (get_header(name,2)<=long_record){
    nb_block= get_header(name,1)-1;
}
else
 {nb_block=get_header(name,1);}
set_header(name,1,nb_block) ;


last_pos=get_header(name,2)-(long_record % b );
if (last_pos<0)
{
 last_pos=last_pos+b;
}
set_header(name,2,last_pos) ;
     printf("\nLa donnee a ete bien supprimee ! ");
}
}


void dsiplay_header(char*name)
{

printf("nember of blocks: %d\nthe first position free in the last block: %d\n number of blocks ereased: %d \n",get_header(name,1),get_header(name,2),get_header(name,3));

}

void display_block(char* name,int i)
{
record buf ;
read_Block(name,i,&buf);
char block[b];
int j=0;

if(get_header(name,1)==i){
  int h=get_header(name,2);
  char blo[h];
  get_chaine(h-1,&i,&j,name,blo,b);
   printf("%s",blo);
}else{
  
get_chaine(b,&i,&j,name,block,b);}
 printf("%s",block);

}

void display_file(char* name ){

dsiplay_header(name);
for (int i =1 ; i <= get_header(name,1); i++)
{
display_block(name,i);
}
}

void display_chev(char* name){

  record buf ;
char lon[3];
char erase ;
 char key[3] ;
bool stop=false ;
int i=1 ;
int j=0 ;
int savei,savej;
int n=get_header(name,1);
int max =get_header(name,2) ;
  while (i < n)
{
 
   savei=i;
   savej=j; 
get_chaine(3,&i,&j,name,key,b) ;
   get_chaine(1,&i,&j,name,&erase,b) ;
    get_chaine(3,&i,&j,name,lon,b) ;
     int longeur=atoi(lon);
               char inf [longeur];  
            get_chaine(longeur,&i,&j,name,inf,b);
    if ((i!=savei)&&(erase=='0'))
    {
          char info [longeur+7];  
       get_chaine(longeur+7,&savei,&savej,name,info,b);
       printf(" record in overlaping is : %s  \n ",info);
    }
        
    
     }    

    }
 



bool is_reorganization(char *name)
{

if ( get_header(name,3)>=b*get_header(name,1)*0.5)
{return true ;
}else {return false ;}

}

void reorganization(char *old ,char *nw)
{
    char lon[3];
char erase ;
 char key[3] ;
bool stop=false ;
    int i=1,i2=1;
    int j=0,j2=0 ;
// FILE*f ;
record buf,buf2 ;
FILE *f2 ;
// f=fopen(old,"rb+");
f2=fopen(nw,"wb+");
fclose(f2);
set_header(nw,1,0);
set_header(nw,2,0);
set_header(nw,3,0);
while (i<get_header(old,1) || (i==get_header(old,1)&&j<=get_header(old,2)))
{
  int save_i=i,save_j=j ;  
  get_chaine(3,&i,&j,old,key,b);
  get_chaine(1,&i,&j,old,&erase,b);
   get_chaine(3,&i,&j,old,lon,b);
   int  longeur=atoi(lon);
       char info[longeur+7];
            get_chaine(longeur,&i,&j,old,info,b); 
  if (erase=='0')
  { 

     get_chaine(longeur+7,&save_i,&save_j,old,info,b); 

    write_chaine(nw,&i2,&j2,info);
  }
}}


void Chargement_TNOVC(char *nomf, int tai){ //Créer un L/OVC de nombre de datas connus (tai)

FILE *f = fopen(nomf , "wb") ;//faire écraser et créer un nouveau fichier dans la machine d'utilisateur
fclose(f) ;//pas besoin de f ici , dans on fait fclose ici

char Data[b-7] , cle[4] , length[4] , eff[2] = "0" ; //la donnée et la clé , taille+\0 , eff+\0
int i = 1 , j = 0 , key;

//initialisation de l'entaite
 set_header(nomf , 1 , 1); //nbr de bloques initialisé
 set_header(nomf , 2 , 0);//initialiser la dernière position faisable à 0
 set_header(nomf , 3, 0) ;//nbr supprimé (=0 en création)

 for(int k = 1 ; k <= tai ; k++){//chaque boucle il y aura une donnée prise
  printf("\n**Donnee la cle sous forme d un nombre (maximum 3 chiffres): ");
  scanf("%d",&key) ;
  printf("Ecrivez votre Data san espace (pour erreur de scanf )  (Maximum %d - 7 = %d caracteres): ", b , b - 7 );
  scanf("%s",Data);


    sprintf(cle,"%03d",key);// we  change the key from int  to string 
      sprintf(length,"%03d",strlen(Data));// we recive the lenght of data then change it from int  to string 

  length[3] = '\0' ; //pour length soit visualiser comme une vrai chaine pour la prochaine ligne
  cle[3] =  '\0' ;
  Data[b - 6] = '\0' ; //ces affectations en c'est jamais qu'il y aura des fausses donnees et respecter la structure d'une chaine


  write_chaine(nomf  , &i , &j , cle) ;
  write_chaine(nomf  , &i , &j , eff) ;
  write_chaine(nomf  , &i , &j , length);
  write_chaine(nomf , &i , &j , Data) ;

  }

}


void MENU(){ //a menu
printf("-----------------------------Menu-----------------------------\n");
printf("1)- Creer un fichier binaire TnOVC .\n");
printf("2)- Lire le  block .\n") ;
printf("3)- Lire le Contenue de l entete.\n") ;
printf("4)- Inserer une donnee .\n") ;
printf("5)- Recherche  d une donnee.\n") ;
printf("6)- Suppression logique d une donnee .\n") ;
printf("7)- Reorganisation du fichier TNOVC.\n") ;
printf("8)- DELETED PHISCLY.\n") ;
printf("9)- Lire le Contenue de file.\n") ;
printf("10)- overlaping records.\n") ;
printf("11)- Exit.\n");
printf("--------------------------------------------------------------\n\n");
}

