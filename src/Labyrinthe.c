#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Prototypes
typedef struct Element Element;
struct Element
{
    int nombre;
    Element *suivant;
};

typedef struct File File;
struct File
{
    Element *premier;
};

File *initialiser();
void enfiler(File *file, int nvNombre);
int defiler(File *file);


void generationCarte( int carte [60][60], int dimCarte[2]);
void ADonnexy (int XY[1][2],int a,int m);
void searchShortWay (int carte [60][60], int xD, int yD, int xA, int yA);
int xyDonneA (int x, int y , int m);
void searchSon (int carte[60][60] , int xSon, int ySon,File *file);
void affiche (int carte[60][60] , int dimCarte[2]);
void cheminRetour (int carte [60][60], int xA, int yA);
int hasard(int x);


// Fin prototypes



int main()
{
    printf("----------------------------------LABYRINTHE----------------------------------\n");
    printf("\n");
    File *maFile = initialiser();
    srand(time(NULL));
    int continuer = 1;

    while (continuer)
    {
        int carte [60][60];
        int dimCarte[2];

        printf("\n");
        generationCarte(carte, dimCarte);
        affiche(carte,dimCarte);

        printf("\n");
        int xD, yD,xA,yA;
        printf("Entrez le point (x) de depart \n");
        scanf("%d",&xD);
        printf("Entrez le point (y) de depart \n");
        scanf("%d",&yD);
        printf("Entrez le point (x) d'arrive \n");
        scanf("%d",&xA);
        printf("Entrez le point (y) d'arrive \n");
        scanf("%d",&yA);

        searchShortWay (carte  ,xD,  yD,  xA,  yA);
        printf("\n");
        if (carte[xA][yA] == -4) // On affiche la carte que si le chemin existe
        {
            printf(" D -> depart    A -> arrivee  * -> case du chemin \n");
            affiche(carte,dimCarte);
        }
        printf(" \n Un nouveau test ? ( 1 pour continuer, 0 pour quittter)");
        scanf("%d", &continuer);
    }
    return 0;

}


void generationCarte( int carte [60][60], int dimCarte[2])
{
    int s,k,x;
    int tailleCarteM = rand()%(60-10) +10;
    int tailleCarteN = rand()%(60-10) +10;
    dimCarte[0] = tailleCarteM;
    dimCarte[1] = tailleCarteN;

    printf("La taille de la carte est %d x %d \n", tailleCarteM, tailleCarteN);
    printf("\n");
    printf(" [X][Y]   X| Y->         0-> obstacle .-> libre \n");
    printf("\n");

    for (k=0; k <tailleCarteM ; k++)
    {
        for (s=0; s<tailleCarteN;s++)
        {
            if ((k == 0) || (k == (tailleCarteM-1)))
            {
                carte[k][s]= -1;
            }
            else if ((s == 0) || (s == (tailleCarteN-1)))
            {
                carte[k][s]= -1;
            }
            else
            {
                carte[k][s]= hasard(x);
            }
        }
    }
}



int xyDonneA (int x, int y , int m)
{
   return (m*x + y);
}




void ADonnexy (int XY[1][2],int a,int m)
{
    XY [0][0]= a%m; // y
    XY [0][1]= (a -XY [0][0])/m; //x
}




void searchShortWay (int carte [60][60], int xD, int yD, int xA, int yA)
{
    File *maFile = initialiser();

    int x,y,A;

    if (carte[xD][yD] == 0 && carte[xA][yA] == 0 )
    {
        carte [xD][yD] = 1; // mettre 1 ici �nonc� distance

        searchSon ( carte , xD, yD,maFile);
        int XY[1][2] ;


        while (maFile->premier != NULL || ( x == xA && y == yA) )
        {
            A = defiler(maFile);
            ADonnexy( XY,A,60);
            x = XY[0][1];
            y = XY[0][0];
            searchSon ( carte , x, y, maFile);
        }

        if ( carte [xA][yA] != 0 )
        {
            cheminRetour (carte, xA,  yA);
        }
        else
        {
            printf ("ERREUR : IL N'EXISTE AUCUN CHEMIN \n");
        }
    }
    else printf("ERREUR : LE POINT DE DEPART OU D'ARRIVEE N'EST PAS VALIDE \n");

}





void searchSon (int carte[60][60] , int xSon, int ySon,File *file)
{
    int A;
    if (carte [xSon][ySon + 1] == 0)
    {
        carte[xSon][ySon+1] = carte[xSon][ySon]+1;
        enfiler(file, xyDonneA(xSon, ySon+1,60) );
    }


    if (carte [xSon][ySon-1] == 0 )
    {
        carte[xSon][ySon-1] = carte[xSon][ySon]+1;;
        enfiler(file, xyDonneA(xSon, ySon-1,60) );
    }


    if (carte [xSon-1][ySon] == 0 )
    {
        enfiler(file, xyDonneA(xSon-1, ySon,60) );
        carte[xSon-1][ySon] = carte[xSon][ySon]+1;
    }


    if (carte [xSon+1][ySon] == 0  )
    {
        enfiler(file, xyDonneA(xSon+1, ySon,60) );
        carte[xSon+1][ySon]=carte[xSon][ySon]+1;
    }

}







void affiche (int carte[60][60] , int dimCarte[2])
{

    int j,d,s,u,w,z,y;
    int tailleCarteM = dimCarte[0];
    int tailleCarteN = dimCarte[1];
    for (j=0; j < tailleCarteM; j++)
    {
        // Bord horizontal haut
        if ( j == 0)
        {
            // Affiche rep�re haut
            z = 0;
            printf("  ");
            while (z < tailleCarteN)
            {
                y = 0;
                while ( y < 10 && z < tailleCarteN)
                {
                    printf("%d ",y);
                    z++;
                    y++;
                }
            }
            printf("\n");

            for ( w = 0 ; w < tailleCarteN +2; w++)
            {
                printf("- ");
            }
            printf("\n");
        }
        for (s=0 ; s < tailleCarteN ; s++)
        {
            // Bord vertical droit
            if ( s == 0)
            {
                printf("- ");
            }
            // on affiche ICI
            if ( carte[j][s] == - 1)
            {
                printf("0 ");
            }
            else if (carte[j][s] == -2)
            {
                printf("* ");
            }
            else if (carte[j][s] == 0)
            {
                printf(". ");
            }
            else if (carte[j][s] == -3)
            {
                printf("D ");
            }
            else if (carte[j][s] == -4)
            {
                printf("A ");
            }
            else if (carte[j][s] != -1 && carte[j][s] != -2  )
            {
                printf(". ");
            }
            // on affiche ICI FIN

            // Bord vertical gauche
            if ( s == tailleCarteN -1)
            {
                printf("- %d", j);
            }
        }
        printf("\n");

        // Bord horizontal bas
        if ( j == tailleCarteM -1)
        {
            for ( w = 0 ; w < tailleCarteN+2; w++)
            {
                printf("- ");
            }
            printf("\n");
            z = 0;
            printf("  ");
            while (z < tailleCarteN)
            {
                y = 0;
                while ( y < 10 && z < tailleCarteN)
                {
                    printf("%d ",y);
                    z++;
                    y++;
                }
            }
            printf("\n");
        }
    }
}





void cheminRetour (int carte [60][60], int xA, int yA)
{
    int x = xA;
    int y = yA;
    while ( ( carte[x][y] != 1))
    {
        if ( carte [x + 1][y] == carte[x][y] - 1)
        {
            carte [x][y] = -2;
            x = x + 1 ;
        }
        else if ( carte [x - 1][y] == carte[x][y] - 1)
        {
            carte [x][y] = -2;
            x = x - 1 ;
        }
        else if ( carte [x][y + 1] == carte[x][y] - 1)
        {
            carte [x][y] = -2;
            y = y + 1;
        }
        else if ( carte [x][y-1] == carte[x][y] - 1)
        {
            carte [x][y] = -2;
            y = y - 1;
        }
    }
    carte[x][y] = -3; // Pour afficher D
    carte[xA][yA] = -4; //Pour afficher A
}


int hasard(int x) // Donne la proba qu'une case soit libre ou non
{
    int y = rand()%(10-1) +1;
    if ( y == 1 || y == 2 || y == 3 )
    {
        x = -1;
    }
    else
    {
        x = 0 ;
    }
    return x;
}


// File avec cha�ne de pointeurs et ses fonctions

File *initialiser()
{
    File *file = malloc(sizeof(*file));
    file->premier = NULL;

    return file;
}


void enfiler(File *file, int nvNombre)
{
    Element *nouveau = malloc(sizeof(*nouveau));
    if (file == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau->nombre = nvNombre;
    nouveau->suivant = NULL;

    if (file->premier != NULL) /* La file n'est pas vide */
    {
        /* On se positionne � la fin de la file */
        Element *elementActuel = file->premier;
        while (elementActuel->suivant != NULL)
        {
            elementActuel = elementActuel->suivant;
        }
        elementActuel->suivant = nouveau;
    }
    else /* La file est vide, notre �l�ment est le premier */
    {
        file->premier = nouveau;
    }
}


int defiler(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int nombreDefile = 0;

    /* On v�rifie s'il y a quelque chose � d�filer */
    if (file->premier != NULL)
    {
        Element *elementDefile = file->premier;

        nombreDefile = elementDefile->nombre;
        file->premier = elementDefile->suivant;
        free(elementDefile);
    }

    return nombreDefile;
}
