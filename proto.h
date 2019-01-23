#ifndef prototypes
#define prototypes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <math.h>


typedef struct Element Element;
struct Element
{
    int valeur;
    Element *suivant;
    Element *precedent;
};

typedef struct Liste Liste;
struct Liste
{
    Element *premier;
};


Liste *initialisation(){
    Liste *liste=malloc(sizeof(*liste));
    Element *nouveau=malloc(sizeof(*nouveau));
    if(nouveau==NULL||liste==NULL) exit(EXIT_FAILURE);
    nouveau->valeur=-1;
    nouveau->suivant=NULL;
    liste->premier=nouveau;
    return liste;
}

///PROJET POUSSÉ = FONCTIONS POUSSÉES
void ajouterElement(Liste *liste, int valeur);
int* ouvertureFichierSauvegarde();
void start(SDL_Surface * plateau);
int monnaie();
void blocNote(Liste *maliste);
void achatSkins(int prix);
void personnalisation(SDL_Surface * plateau);
void gagnerTokkens();
int ia_izi(SDL_Surface * plateau, int **tab, int * direction, int hauteur, int largeur, SDL_Rect * pos);
int ia_med(SDL_Surface * plateau, int **tab, int * direction, int hauteur, int largeur, SDL_Rect * pos);
void load(SDL_Surface * plateau, int ***tab, int *directions, SDL_Rect ** j, int n, int hauteur, int largeur, int diff);
void menu2affichage(SDL_Surface * plateau);
int comptageHistoriqueLoose();
void verificationReset(SDL_Surface * plateau);
int comptageHistoriqueWin();
void reprise(SDL_Surface * plateau, int ***tab, int *direction, SDL_Rect **joueur, int n, int hauteur, int largeur, int diff);
int jeu(SDL_Surface *plateau, int ***tab, int diff, int hauteur, int largeur, int charger);
void option(SDL_Surface * plateau);
int ia_difficulte(SDL_Surface * plateau, int **tab, int * direction, int hauteur, int largeur, SDL_Rect * j, int difficulte);
void defaite(SDL_Surface * plateau);
void quitter(SDL_Surface * plateau);
void victoire(SDL_Surface * plateau);
void credits(SDL_Surface * plateau);
void progression(SDL_Surface * plateau, int gagnant, SDL_Surface * texte);
void pause(SDL_Surface * plateau, int ***tab, int diff, int hauteur, int largeur, int charger);
void init_tab(int ***tab, SDL_Rect **joueurs, int n, int hauteur, int largeur);
void init_joueurs(int **tab, int n, SDL_Rect * j , int hauteur, int largeur);
void init_affichage(SDL_Surface * plateau, int **tab, SDL_Surface ** imgPerso, int n, int hauteur, int largeur);
void reset();
int ia_shit();




#endif
