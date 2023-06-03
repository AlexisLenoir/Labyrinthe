//
//  main.c
//  LabyrintheC
//
//  Created by Alexis Lenoir on 25/04/2018.
//  Copyright © 2018 Alexis Lenoir. All rights reserved.
//

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Prototype
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

File *initialiser(void);
void enfiler(File *file, int nvNombre);
int defiler(File *file);

int hasard(int x);
void generationCarte( int carte [60][60]);
void searchShortWay (int carte [60][60], int xD, int yD, int xA, int yA);
int xyDonneA (int x, int y , int m);
void searchSon (int carte[60][60] , int xSon, int ySon,File *file);
void cheminRetour (int carte [60][60], int xA, int yA);
void ADonnexy (int XY[1][2],int a,int m);

int testRect (SDL_Point mouse_position,SDL_Rect cases[60][60], int carte[60][60],  int CooDepartArriv[] , int m);
void affiche(SDL_Renderer *render, SDL_Rect cases[60][60],int carte[60][60], int err,SDL_Rect errRect);

// FIN Prototype


int main (){
    srand(time(NULL));
    SDL_Window *fenetre = NULL;
    SDL_Renderer *render = NULL;
    
    
    
    SDL_Init(SDL_INIT_VIDEO);
    
    
    fenetre = SDL_CreateWindow("LABYRINTHE", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, 800, 800, 0);
    render = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(render, 0, 115, 229, 255);
    SDL_RenderClear(render);
    
    int carte [60][60];
    int CooDepartArriv [4];
    int i,j;
    int m = 0;
  
    
    
    
    generationCarte(carte);
    
    SDL_Event event;
    SDL_bool programm_launched = SDL_TRUE;
    SDL_bool appelAffiche = SDL_TRUE;
    SDL_bool err = SDL_FALSE;
    SDL_Point depart_position;
    SDL_Point arivee_position;
    SDL_Rect cases[60][60];
    SDL_Rect errRect;
    errRect.x = 200;
    errRect.y = 50;
    errRect.h = 30;
    errRect.w = 400;
    
    for( i = 0; i < 60 ; i++)
    {
        
        for ( j = 0; j < 60; j++)
        {
            cases [i][j].x = 100 + i*10;
            cases [i][j].y = 100 + j*10;
            cases [i][j].h = 10;
            cases [i][j].w = 10;
        }
    }
    
    
    
    
    while (programm_launched) // Le programme se passe dans une boucle infinie
    {
        
        
        // AFFICHAGE
        if (appelAffiche) // on affiche que si c'est nécéssaire
        {
            affiche(render, cases,carte,err, errRect);
            appelAffiche = SDL_FALSE;
        }
        
        
        //ÉVENEMENTS
        
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                
                
                
                
                case SDL_MOUSEBUTTONDOWN :
                    
                    
                    if ((event.button.clicks == 2) && (m == 0)) //Pour récupérer le rectangle de départ
                    {
                        
                        depart_position.x = event.button.x;
                        depart_position.y = event.button.y;
                        
                        
                        if (testRect (depart_position,cases,carte,CooDepartArriv,m))
                        {
                            m = 1;
                        }
                        
                        
                    }
                    
                    else if ((event.button.clicks == 2) && (m == 1))  //Pour récupérer le rectangle d'arrivé
                    {
                        
                        arivee_position.x = event.button.x;
                        arivee_position.y = event.button.y;
                        
                        if(testRect (arivee_position,cases,carte,CooDepartArriv,m))
                        {
                            
                            searchShortWay (carte  ,CooDepartArriv[0] ,  CooDepartArriv[1] ,  CooDepartArriv[2]  , CooDepartArriv[3] );
                            
                            if ( carte[CooDepartArriv[2]][CooDepartArriv[3]]  != -2)
                            {
                                
                                err = SDL_TRUE; // erreur car le chemin n'a pas été trouver
                            }
                            
                            appelAffiche = SDL_TRUE;
                            
                        }
                        
                        m = 2;
                        
                    }
                    break;
                
                case SDL_KEYDOWN:
                    
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_SPACE :
                            
                            // On réinitialise les valeurs par défaut pour faire un restart
                            generationCarte(carte);
                            m = 0;
                            appelAffiche = SDL_TRUE;
                            err = SDL_FALSE;
                            
                            break;
                    
                            
                    }
                    break;
                    
                    
                case SDL_QUIT:
                    
                    programm_launched = SDL_FALSE;
            }
        }
       
    }
   
    
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(fenetre);
    
    SDL_Quit();
    
    
    
    return EXIT_SUCCESS;
}





void affiche(SDL_Renderer *render, SDL_Rect cases[60][60], int carte[60][60], int err, SDL_Rect errRect)
{
    int i,j;
    SDL_SetRenderDrawColor(render, 0, 115, 229, 255);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    
    for( i = 0; i < 60 ; i++)
    {
        for ( j = 0; j < 60; j++)
        {
            SDL_RenderFillRect(render, &cases [i][j]);
        }
    }
    
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    
    for (i = 0; i < 60 ; i++)
    {
        for (j = 0; j < 60; j++)
        {
            if (carte [i][j] == -1)
            {
                
                SDL_RenderFillRect(render, &cases [i][j]);
            }
            if (carte [i][j] == -2)
            {
                SDL_SetRenderDrawColor(render, 0, 166, 88, 255);
                SDL_RenderFillRect(render, &cases [i][j]);
                SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
                
                
            }
        }
        
    }
    if (err )
    {
        SDL_SetRenderDrawColor(render, 238, 10, 18, 255);
        SDL_RenderFillRect(render, &errRect);
    }
    SDL_RenderPresent(render);
}





int testRect (SDL_Point point_position, SDL_Rect cases[60][60], int carte[60][60] , int CooDepartArriv[] , int m)
{
    
   
    int i,j;

    int c = 0;
    for ( i = 0; i < 60; i++)
    {
 
        for (j = 0 ; j < 60; j++)
        {
          
            if (SDL_PointInRect (&point_position, &cases[i][j] ) && carte [i][j] == 0)
            {
                c = 1;
                if (m == 0)
                {
                    CooDepartArriv [0] = i; // Position de la case de Départ x puis y
                    CooDepartArriv [1] = j;
                }
                if (m == 1)
                {
                    CooDepartArriv [2] = i; // Position de la case de arrivéé x puis y
                    CooDepartArriv [3] = j;
                }
                
            }
        }
    }
    
    if (c == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
    
    
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


void generationCarte( int carte [60][60])
{
    int x,s,k;
    
    
    
    
    for (k=0; k <60 ; k++)
    {
        for (s=0; s<60;s++)
        {
            if ((k == 0) || (k == (60-1)))
            {
                carte[k][s]= -1;
            }
            else if ((s == 0) || (s == (60-1)))
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
    printf ("cool 1");
    if (carte[xD][yD] == 0 && carte[xA][yA] == 0 )
    {
        carte [xD][yD] = 1; // mettre 1 ici ÈnoncÈ distance
        
        searchSon ( carte , xD, yD,maFile);
        printf ("cool 2");
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
   
}
// File avec chaîne de pointeurs et ses fonctions

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
        /* On se positionne ‡ la fin de la file */
        Element *elementActuel = file->premier;
        while (elementActuel->suivant != NULL)
        {
            elementActuel = elementActuel->suivant;
        }
        elementActuel->suivant = nouveau;
    }
    else /* La file est vide, notre ÈlÈment est le premier */
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
    
    /* On vÈrifie s'il y a quelque chose ‡ dÈfiler */
    if (file->premier != NULL)
    {
        Element *elementDefile = file->premier;
        
        nombreDefile = elementDefile->nombre;
        file->premier = elementDefile->suivant;
        free(elementDefile);
    }
    
    return nombreDefile;
}




