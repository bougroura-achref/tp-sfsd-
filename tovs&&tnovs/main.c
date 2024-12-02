#include "func.h"
#include"tovc.c"
int main()
{

int i , j , tai , state = 1 , choi , don , key ;

char file[30]="file.bin",file2[30]="file2.bin", Data[lim - 7] , cle[4] ;
FILE *f=fopen(file,"wb+");
set_header(file,1,0);
set_header(file,2,0);
set_header(file,3,0);

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
    case 1://création de fichier TOVC

     system("cls");
      printf("donnez le nombre de donnee qui va etre recu dans ce fichier: ");
      scanf("%d",&tai);

Chargement_TOVC(file,tai);
     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;

    break ;

    case 2://Lecture TOVC

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
     Dechiffrement(key , cle) ;
     cle[3] = '\0' ;
     Data[lim - 6] = '\0' ;

     printf("Ecrivez votre Data (Maximum %d - 7 = %d caracteres): ",lim , lim-7);
     scanf("%s",Data);

     if(searchTOVC(file , key , &don , &i , &j)) printf("\nla cle existe deja pour une autre donnee non supprimee") ;

     inserTOVs(file , cle , Data) ;

     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;

    case 5://Recherche

     system("cls");
     printf("quelle est la cle du donnee pour rechercher dans file.bin: ");
     scanf("%d",&key);
     printf("\n") ;

     if(searchTOVC(file , key , &don , &i , &j)) printf("\nvotre donnee est dans la %dieme position (logique) de %dieme bloc tel que la taille de data est : %d \n" , j+1 , i , don);
     else printf("\nelement pas trouvee , mais votre donnee peut etre inseree dans la %dieme position (logique) de %dieme bloc . \n" , j+1 , i);

     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;

    case 6://Suppression logique

     system("cls");
     printf("ecris la cle pour que la donnee soit supprimee : ");
     scanf("%d",&key);
     Dechiffrement(key , cle) ;
     cle[3] = '\0' ;

     if(!searchTOVC(file , key , &don , &i , &j)) printf("\nla donnee n existe pas ...") ;
     else printf("\nLa donnee a ete bien supprimee ! ");

     Suppression_logic_TOVC(file ,key) ;

     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;

    case 7://Réorganisation
     system("cls");
     if (get_header(file,3)>=0,5*((get_header(file,1)-1)*lim+get_header(file,2))){
     Reorganisation_TOVC(file);

     printf("\nLes donnees ont ete bien Organisee ! ");}else{ printf("Reorganization will satisfy after 50% of characters file are deleted");}

     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;
        case 8://création de fichier TOVC

     system("cls");
      printf("give the key that you want to delet: ");
      scanf("%d",&key);

     Dechiffrement(key , cle) ;
     cle[3] = '\0' ;

     if(!searchTOVC(file , key , &don , &i , &j)) printf("\nla donnee n existe pas ...") ;
     else printf("\nLa donnee a ete bien supprimee ! ");

     delete_tvs(file,key) ;

     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;

    break ;
            case 9://création de fichier TOVC

     system("cls");
      printf("charge the file2 to do intersection and give me how many record you want to charge it\n: ");


    scanf("%d",&tai);
 Chargement_TOVC(file2,tai);
 find_intersection(file,file2,"out.bin");
  for (int k=1 ; k<=get_header("out.bin",1);k++){

    display_block("out.bin",k);}
     fflush(stdin);
     getchar() ;

          break ;
                   case 10://création de fichier TOVC

     system("cls");
      printf("charge the file2 to do MERGE and give me how many record you want to charge it\n: ");
      scanf("%d",&tai);

 Chargement_TOVC(file2,tai);
 concatenate_files(file,file2,"out.bin");
  for (int k=1 ; k<=get_header("out.bin",1);k++){

    display_block("out.bin",k);}

     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;

          break ;
              case 11://Lecture TOVC

     system("cls");
     printf("content of file :\n");

     for (int k=1 ; k<=get_header(file,1);k++){

    display_block(file,k);}

     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;
        case 12://Lecture TOVC

     system("cls");


     printf("content  :\n");
     overlaping(file);
     printf("\n\n Press <ENTER> to Continue...");
     fflush(stdin);
     getchar() ;
    break ;


    case 13://Exit
     state = 0 ; //pour sortir de la grande boucle
    break ;

   }
 }

system("cls");
printf("Thank you for using our Program , see you next time ! ^_^ \n");


return 0;
}
