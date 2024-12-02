#include "functionTNOVC.h"
        #include "functionTnOVC.c"



int main()
{

int i , j , tai , state = 1 , choi , don , key ;
bool trouve;

char file[30]="file.bin",file2[30]="file2.bin", Data[b - 7] , cle[4] ;
FILE *f=fopen(file,"wb+");
set_header(file,1,0);
set_header(file,2,0);
set_header(file,3,0);
fclose(f);
 while(state){
   system("cls") ;
   printf("voici votre menu : \n\n");
   MENU() ;

     printf("votre choix : ");
  scanf("%d",&choi);
     while( choi < 1 || choi > 13 || ( (choi != 1 && choi != 13) && get_header(file , 1) == 0) ){
      printf("Erreur de choix ");
      if(get_header(file, 1) == 0) printf("(Votre fichier est vide !! le seul choix restant est 1 pour créer nouvelles donnees)");
      printf(":") ;
      scanf("%d",&choi);
     } //ce simple while est pour donner un choix correcte

   switch (choi){
    case 1://création de fichier TnOVC

     system("cls");
      printf("donnez le nombre de donnee qui va etre recu dans ce fichier: ");
      scanf("%d",&tai);

Chargement_TNOVC(file,tai);
     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;

    break ;

    case 2://Lecture TnOVC

     system("cls");
     printf("wich block you want :\n");
     int k ;
     scanf("%d",&k);

    display_block(file,k);
     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;

    case 3://Lecture de l'entète

     dsiplay_header(file);

     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;

    case 4://Insertion

     system("cls");
     printf("\n**Donnee la cle sous forme d un entier ( 3 chiffres maxs): ");
     scanf("%d",&key) ;
    sprintf(cle,"%03d",key);
     cle[3] = '\0' ;
     Data[b - 6] = '\0' ;

     printf("Ecrivez votre Data (Maximum %d - 7 = %d caracteres): ",b , b-7);
     scanf("%s",Data);

     insert_tnovs(file , cle , Data) ;
     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;

    case 5://Recherche

     system("cls");
     printf("quelle est la cle du donnee pour rechercher dans file.bin: ");
     scanf("%d",&key);
    sprintf(cle,"%03d",key);     
     cle[3] = '\0' ;
 
       printf("\n") ;

     search_tnovs(file , cle , &trouve , &i , &j);
     if (trouve) printf("\nvotre donnee est dans la %dieme position (logique) de %dieme bloc  \n" , j+1, i );

     else printf("\nelement pas trouvee , mais votre donnee peut etre inseree a la fin " );

     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;

    case 6://Suppression logique

     system("cls");
     printf("ecris la cle pour que la donnee soit supprimee : ");
     scanf("%d",&key);
    sprintf(cle,"%03d",key);
     cle[3] = '\0' ;

     delete_logic(file ,cle) ;
     if (is_reorganization(file))
     {
      printf(" \n you should reorganize the file select  7 ");
     }
     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;

    case 7://Réorganisation
     system("cls");
     reorganization(file,file2);
     strcpy(file,file2);

     printf("\nLes donnees ont ete bien Organisee ! ");
     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;
        case 8:// delete phis TNOVC

     system("cls");
      printf("give the key that you want to delet: ");
     scanf("%d",&key);
    sprintf(cle,"%03d",key);
     cle[3] = '\0' ;


    delete_phis_tnovs(file,cle) ;

     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;

    break ;
           
              case 9://Lecture TNOVC

     system("cls");
     printf("content of file :\n");
     display_file(file);
     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;
        case 10:// display overlaping  TNOVC

     system("cls");
     printf("content  :\n");
      display_chev(file);
       printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;


    case 11://Exit
     state = 0 ; //pour sortir de la grande boucle
    break ;

   }
 }

system("cls");
printf("Thank you for using our Program , see you next time ! ^_^ \n");


return 0;
}
