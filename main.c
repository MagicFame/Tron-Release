#include "proto.h"




int main(int argc, char *argv[])
{

    SDL_Surface * plateau = NULL;  ///ce truc = surface qui va représenter la zone du plateau (en gros tout notre screen)

    SDL_Init(SDL_INIT_VIDEO); // Initialisation de la SDL
    plateau = SDL_SetVideoMode(1600, 900, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // Ouverture de la fenêtre

    SDL_WM_SetCaption("Tron", NULL);
    SDL_WM_SetIcon(SDL_LoadBMP("logo.bmp"), NULL);
    TTF_Font *police = NULL;
    TTF_Init();

    police = TTF_OpenFont("poor.ttf", 65);

    ///SDL_ShowCursor(SDL_DISABLE);
    start(plateau);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit(); // Arrêt de la SDL

    return EXIT_SUCCESS; // Fermeture du programme FINIIIIIIIIIIIIIIIII
}


int jeu(SDL_Surface * plateau, int ***tab, int diff, int hauteur, int largeur, int charger){

    int n = 2, continuer = 1, lenteur, vitesse;///n = Nombre de joueurs (modifier init_joueurs pour plus de joueurs)
    int tempsPrecedent = 0, tempsActuel = 0; ///Variables de TEMPS
    int *direction; ///Tableau qui va stocker les directions
    vitesse = reconnaissanceVitesse();
    if (vitesse == 0) lenteur = 10;
    if (vitesse == 1) lenteur = 8;
    if (vitesse == 2) lenteur = 6;
    if (vitesse == 3) lenteur = 4;
    if (vitesse == 4) lenteur = 2;
    if (vitesse == 5) lenteur = 1;
    int i, k;
    SDL_ShowCursor(SDL_DISABLE);
    SDL_Rect *j; ///Positions des joueurs respectifs.
    SDL_Event event;
    SDL_Surface ** imgPerso = malloc(n * sizeof(SDL_Surface));

    imgPerso = malloc(n * sizeof(SDL_Surface));
    direction = malloc(n * sizeof(int));


    int mur;
    mur = reconnaissanceCouleur();
    if (mur == 1) imgPerso[0] = SDL_LoadBMP("Murs/TEST2.bmp");
    else if (mur == 2) imgPerso[0] = SDL_LoadBMP("Murs/TEST3.bmp");
    else if (mur == 3) imgPerso[0] = SDL_LoadBMP("Murs/TEST4.bmp");
    else if (mur == 4) imgPerso[0] = SDL_LoadBMP("Murs/TEST5.bmp");
    else if (mur == 5) imgPerso[0] = SDL_LoadBMP("Murs/TEST6.bmp");
    else if (mur == 6) imgPerso[0] = SDL_LoadBMP("Murs/TEST7.bmp");
    else if (mur == 7) imgPerso[0] = SDL_LoadBMP("Murs/TEST8.bmp");
    else imgPerso[0] =SDL_LoadBMP("Murs/TEST.bmp"); ///Différents styles de baricades
    imgPerso[1] =SDL_LoadBMP("Murs/TEST1.bmp");


    ///init...
    if(charger == 0){
        init_tab(tab, &j, n, hauteur, largeur);
        init_joueurs(*tab, n, j, hauteur, largeur);
        direction[0] = 8;
        direction[1] = 4;
    }
    else load(plateau, tab, direction, &j, n, hauteur, largeur, diff);
    init_affichage(plateau, *tab, imgPerso, n, hauteur, largeur);


    while (continuer)
    {

        direction[1] = ia_difficulte(plateau, *tab, direction, hauteur, largeur, j, diff);

        for(i = 0; i < n; i++) SDL_BlitSurface(imgPerso[i], NULL, plateau, j+i);
        SDL_Flip(plateau);
        SDL_PollEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){


                    case SDLK_w:
                        //direction[1] = 2;
                        if(direction[0] != 2) direction[0] = 1;
                    break;

                    case SDLK_s:
                       // direction[1] = 1;
                        if(direction[0] != 1) direction[0] = 2;
                    break;

                    case SDLK_a:
                        //direction[1] = 8;
                        if(direction[0] != 8) direction[0] = 4;
                    break;

                    case SDLK_d:
                        //direction[1] = 4;
                        if(direction[0] != 4) direction[0] = 8;
                    break;


                    case SDLK_ESCAPE:

                        continuer = 0;

                        FILE * save_game1 = NULL;
                        FILE * save_game2 = NULL;
                        FILE * save_game3 = NULL;
                        FILE * save_game4 = NULL;

                        save_game1 = fopen("save/chargerTableau.txt", "w+");
                        if(save_game1 != NULL){
                            for(i = 0; i < hauteur*10; i++){
                                for(k = 0; k < largeur*10; k++) fprintf(save_game1, "%d ", (*tab)[i][k]);
                                fprintf(save_game1, "\n");
                            }
                        }
                        fclose(save_game1);

                        save_game2 = fopen("save/chargerPositions.txt", "w+");
                        if(save_game2 != NULL){
                           for(i = 0; i < n; i++) fprintf(save_game2, "%d %d\n", (j[i].y), (j[i].x));
                        }
                        fclose(save_game2);

                        save_game3 = fopen("save/chargerDirections.txt", "w+");
                        if(save_game3 != NULL){
                            for(i = 0; i < n; i++) fprintf(save_game3, "%d\n", direction[i]);
                        }
                        fclose(save_game3);

                        save_game4 = fopen("save/chargerTaille.txt", "w");
                        if(save_game4 != NULL) fprintf(save_game4, "%d %d\n", hauteur, largeur);
                        fclose(save_game4);
                        pause(plateau, tab, diff, hauteur, largeur,1);
                    break;

                    default:
                    break;

                }break;
        }
        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > lenteur && continuer == 1) /* Si 30 ms se sont écoulées depuis le dernier tour de boucle */
        {
            for(i = 0; i < n; i++){

                if(direction[i] == 1 && (j[i].y) > 130 && (*tab)[(j[i].y)-131][(j[i].x)-190] == 0) (j[i].y) -= 1;
                else if(direction[i] == 2 && (j[i].y) < 10*hauteur-1 + 130 && (*tab)[(j[i].y)+1-130][(j[i].x-190)] == 0) (j[i].y) += 1;
                else if(direction[i] == 4 && j[i].x > 190 && (*tab)[(j[i].y)-130][(j[i].x)-191] == 0) (j[i].x)-= 1;
                else if(direction[i] == 8 &&j[i].x < 10 * largeur-1+190 && (*tab)[(j[i].y)-130][(j[i].x)+1-190] == 0) (j[i].x) += 1;
                else return i;

               (*tab)[(j[i].y)-130][(j[i].x)-190] = i+1;

            }
            tempsPrecedent = tempsActuel;
        }
        else if(continuer == 1) SDL_Delay(lenteur - (tempsActuel - tempsPrecedent));

    }

    return -1;
}


void init_tab(int ***tab, SDL_Rect **joueurs, int n, int hauteur, int largeur){
    int i,j;

    *tab = malloc(10*hauteur * sizeof(int*)); ///INITIALISATION DU plateau DE JEU
    for(i = 0; i < 10*hauteur; i++) (*tab)[i] = malloc(10*largeur *sizeof(int));
    for(i = 0; i < 10*hauteur; i++){
        for(j = 0; j < 10*largeur; j++) (*tab)[i][j] = 0;
    }


    *joueurs = malloc(n * sizeof(SDL_Rect)); ///INITIALISATION DU NOMBRE DE JOUEURS*/
}

void init_joueurs(int **tab, int n, SDL_Rect * j, int hauteur, int largeur){

    j[0].x = 10*largeur/4+190;
    j[0].y = 10*hauteur/2+130;

    j[1].x = (30*largeur/4)+190;
    j[1].y = 10*hauteur/2+130;

    (tab)[j[0].y][j[0].x] = 1;
    (tab)[j[1].y][j[1].x] = 2;


}


void load(SDL_Surface * plateau, int ***tab, int *directions, SDL_Rect **joueurs, int n, int hauteur, int largeur, int diff){
    int i,j ;
    int a = 0, b = 0;

    FILE *Load_tab = fopen("save/chargerTableau.txt", "r");
    FILE *Load_Positions = fopen("save/chargerPositions.txt", "r");
    FILE *Load_Directions = fopen("save/chargerDirections.txt", "r");

    if(Load_tab == NULL || Load_Directions == NULL) jeu(plateau, tab, diff, hauteur, largeur, 0);
    else{
        ///Chargement des tableau
        *tab = malloc(10*hauteur * sizeof(int*));
        for(i = 0; i < 10*hauteur; i++) (*tab)[i] = malloc(10*largeur * sizeof(int));
        for(i = 0; i < 10*hauteur; i++){
            for(j = 0; j < 10*largeur; j++) {
                fscanf(Load_tab,"%d ", &(*tab)[i][j]);
            }
        }
        *joueurs = malloc(n * sizeof(SDL_Rect));

         ///Chargement des positions
        for(i = 0; i < n; i++){
            fscanf(Load_Positions,"%d %d" , &a, &b);
            ///Ben là on fait un cast. si tu comprends pas ftg et va apprendre à faire de la progr poussée
            (*joueurs)[i].y = (short) a;  ///APRES 5H DE BUG, :), j'ai trouvé, c'est pas joueurs[i]->y mais parce que le pointeur s'applique sur tout sinon. GG FDP DE LANGAGE DE MERDE AIOUCHZANIUCH ZAIUDHAZKHDIAZUKD
            (*joueurs)[i].x = (short) b;
        }
        for(i = 0 ; i < n; i++) fscanf(Load_Directions, "%d\n", &directions[i]); ///Chargements des directions

        fclose(Load_tab);
        fclose(Load_Positions);
        fclose(Load_Directions);
    }



}

int monnaie()
{
    FILE *fichier = NULL;
    int argent;
    fichier = fopen("argent.txt", "r+");
    fscanf(fichier, "%d", &argent);
    fclose(fichier);
    return argent;
}

int reconnaissanceCouleur()
{
    FILE* fichier = NULL;
    fichier = fopen("couleurbase.txt", "r+");
    char valeur;
    int valeurDecimale;
    valeur = fgetc(fichier);
    valeurDecimale = valeur - 48;
    fclose(fichier);
    return valeurDecimale;
}

int reconnaissanceVitesse()
{
    FILE* fichier = NULL;
    fichier = fopen("vitesse.txt", "r+");
    char valeur;
    int valeurDecimale;
    valeur = fgetc(fichier);
    valeurDecimale = valeur - 48;
    fclose(fichier);
    return valeurDecimale;
}



void reset()
{
    FILE* fichier=NULL;
    fichier= fopen("sauvegarde.txt", "w+");
    remove("sauvegarde.txt");
    fclose(fichier);
    fichier = fopen("argent.txt", "w+");
    fprintf(fichier, "0");
    fclose(fichier);
    fichier =fopen("personnalisation.txt", "w+");
    fprintf(fichier,"0000000");
    fclose(fichier);
    fichier =fopen("couleurbase.txt", "w+");
    fprintf(fichier,"0");
    fclose(fichier);
    fichier =fopen("vitesse.txt", "w+");
    fprintf(fichier,"0");
    fclose(fichier);
}

int* ouvertureFichierSauvegarde()
{
    FILE *fichier = NULL;

    static int tab[12];
    int valeur;
    int i;
    char caractereActuel;
    fichier = fopen("personnalisation.txt", "r+");

    if (fichier != NULL)
    {
        for(i = 0; i<12; i++)
        {
            caractereActuel=fgetc(fichier);
            valeur = caractereActuel-48;

            tab[i]=valeur;
        }
    }
    fclose(fichier);
    return tab;
}

void modifierSauvegardeSkins(int numeroSkin)
{
    FILE* fichier = NULL;
    static int tab[12];
    int valeur;
    int i;
    char caractereActuel;
    fichier = fopen("personnalisation.txt", "r+");

    if (fichier == NULL) return;
    for(i = 0; i<12; i++)
    {
        caractereActuel=fgetc(fichier);
        valeur = caractereActuel-48;
        tab[i]=valeur;
    }
    tab[numeroSkin] = 1;
    fclose(fichier);
    fichier = fopen("personnalisation.txt", "w");
    for(i = 0; i< 12; i++)
    {
        fprintf(fichier, "%d", tab[i]);
    }
    fclose(fichier);
}

void choisirCouleur(int valeurSkin)
{
    FILE* fichier = NULL;
    fichier = fopen("couleurbase.txt", "w+");
    fprintf(fichier, "%d", valeurSkin);
    fclose(fichier);
}
void choisirVitesse(int valeurVitesse)
{
    FILE* fichier = NULL;
    fichier = fopen("vitesse.txt", "w+");
    fprintf(fichier, "%d", valeurVitesse);
    fclose(fichier);
}


void ajouterElement(Liste *liste, int valeur)
{
    Element *nouveau=malloc(sizeof(*nouveau));
    if(nouveau==NULL) exit(EXIT_FAILURE);
    nouveau->suivant=liste->premier;
    nouveau->valeur=valeur;
    if(nouveau->suivant) nouveau->suivant->precedent = nouveau;
    liste->premier=nouveau;
}

int comptageHistoriqueWin()
{
     int compteurWin = 0, valeur;
     FILE *fichier = NULL;
     char caractereActuel;
     fichier = fopen("sauvegarde.txt", "r");
     rewind(fichier);
     if (fichier != NULL ){
        do
        {
                caractereActuel = fgetc(fichier);
                valeur=caractereActuel-48;
                if (valeur == 1) compteurWin++;

        } while (caractereActuel != EOF);
     }
     fclose(fichier);
     return compteurWin;
 }
 int comptageHistoriqueLoose()
{
     int compteurLoose = 0, valeur;
     FILE *fichier = NULL;
     char caractereActuel;
     fichier = fopen("sauvegarde.txt", "r");
     rewind(fichier);
     if (fichier){
        do{
            caractereActuel = fgetc(fichier);
            valeur = caractereActuel-48;
            if (valeur == 0) compteurLoose++;
        }while(caractereActuel != EOF);
     }
     return compteurLoose;
 }
void blocNote(Liste *liste)
{
    Element *temporaire=liste->premier;
    FILE* fichier=NULL;
    fichier= fopen("sauvegarde.txt", "a");
    if (fichier==NULL) printf("Erreur dans la lecture de la sauvegarde. . .");
    else{
        //fseek(fichier, 0, SEEK_END);

        do{
            fprintf(fichier,"%d\n", temporaire->valeur);
            temporaire=temporaire->suivant;
        }while(temporaire->valeur!=-1);
        fclose(fichier);
    }
    return;
}


void gagnerTokkens()
{
    FILE* fichier = NULL;
    fichier = fopen("argent.txt", "r+");
    int argentActuel;
    fscanf(fichier, "%d", &argentActuel);
    fclose(fichier);
    fichier = fopen("argent.txt", "w+");
    argentActuel=argentActuel+40;
    fprintf(fichier, "%d", argentActuel);
    fclose(fichier);
}

void achatSkins(int prix)
{
    FILE* fichier = NULL;
    fichier = fopen("argent.txt", "r+");
    int argentActuel;
    fscanf(fichier, "%d", &argentActuel);
    fclose(fichier);
    fichier = fopen("argent.txt", "w+");
    argentActuel=argentActuel-prix;
    fprintf(fichier, "%d", argentActuel);
    fclose(fichier);
}
void start(SDL_Surface * plateau)
{

    int continuer = 1;
    SDL_Surface * menu = NULL;
    SDL_Surface *texte = NULL;
    SDL_Rect choix;
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;
    choix.x = 0;
    choix.y = 0;

    menu = SDL_LoadBMP("menus/Menu1.bmp");
    SDL_BlitSurface(menu, NULL, plateau, &positionFond);
    SDL_Flip(plateau);
    SDL_Event event;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
            {
                 case SDLK_ESCAPE:
                     quitter(plateau);
                     continuer = 0;
                     break;
                 default:
                     start(plateau);
                     break;
           }
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONUP:
                choix.x = event.button.x;
                choix.y = event.button.y;
                break;
        }
        if(choix.x >= 950 && choix.x <= 1400 && choix.y >= 180 && choix.y <= 300){ ///On clique sur Jouer
            menu2affichage(plateau);
            continuer = 0;
        }
        else if(choix.x >= 1030 && choix.x <= 1300 && choix.y >= 550 && choix.y <=650){///On clique sur Options
            option(plateau);
            continuer = 0;
        }
        else if(choix.x >= 1000 && choix.x <= 1370 && choix.y >= 380 && choix.y <= 480){
            progression(plateau, -1, texte);
            continuer = 0;
        }
    }
}


void init_affichage(SDL_Surface * plateau, int **tab, SDL_Surface ** imgPerso, int n, int hauteur, int largeur){
    SDL_FillRect(plateau, NULL, SDL_MapRGB(plateau->format, 255, 255, 255));
    SDL_Surface * terrain = SDL_LoadBMP("Terrain.bmp");
    SDL_Surface * contour = SDL_LoadBMP("menus/Affichagejeu.bmp");
    SDL_Rect position;
    SDL_BlitSurface(contour, NULL, plateau, NULL);
    int i, j, k;

    for(i = 0; i < hauteur; i++){
        for(j = 0; j < largeur; j++){
            position.x = j * 10+190;
            position.y = i * 10+130;
            SDL_BlitSurface(terrain, NULL, plateau, &position);
        }
    }

    for(i = 0; i < hauteur*10; i++){
        for(j = 0; j < largeur*10; j++){
            position.x = j+190;
            position.y = i+130;
            if(tab[i][j] != 0){
                for(k = 0; k < n; k++){
                    if(tab[i][j] == k+1) SDL_BlitSurface(imgPerso[k], NULL, plateau, &position);
                }
            }
        }
    }

}

void menu2affichage(SDL_Surface * plateau){

    int continuer = 1;
    int **tab;
    int hauteur_prec = 0, largeur_prec = 0;
    int gagnant = -1;

    SDL_ShowCursor(SDL_ENABLE);
    SDL_Surface * menu = SDL_LoadBMP("menus/Menu2.bmp");
    SDL_Rect choix;
    SDL_Rect positionFond;

    positionFond.x = 0;
    positionFond.y = 0;
    choix.x = 0;
    choix.y = 0;

    SDL_BlitSurface(menu, NULL, plateau, &positionFond);
    SDL_Flip(plateau);

    SDL_Event event;
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONUP:
                choix.x = event.button.x;
                choix.y = event.button.y;
                break;
        }
        if(choix.x >= 150 && choix.x <= 350 && choix.y >= 400 && choix.y <= 500){///On clique sur facile
            continuer = 0;
            gagnant = jeu(plateau, &tab, 1, 70, 120, 0); ///plateau, DIFFICULTE, HAUTEUR, lARGEUR
            if (gagnant > 0) victoire(plateau);
            else if(gagnant == 0){
                defaite(plateau);
              //  ajouterElement(maliste, 0);
            }
        }
        else if(choix.x >= 600 && choix.x <= 950 && choix.y >= 400 && choix.y <= 500){///On clique sur facile
            continuer = 0;
            gagnant = jeu(plateau, &tab, 2, 70, 120, 0); ///plateau, DIFFICULTE, HAUTEUR, lARGEUR
            if (gagnant > 0) victoire(plateau);
            else if(gagnant == 0){
                defaite(plateau);
              //  ajouterElement(maliste, 0);
            }
        }
        else if(choix.x >= 390 && choix.x <= 1900 && choix.y >= 640 && choix.y <= 720){///On clique sur charger
            FILE * dimensions = fopen("save/chargerTaille.txt", "r");
            if(dimensions != NULL) fscanf(dimensions, "%d %d", &hauteur_prec, &largeur_prec);
            fclose(dimensions);

            continuer = 0;
            gagnant = jeu(plateau, &tab, 1, hauteur_prec, largeur_prec, 1);
            if (gagnant > 0) victoire(plateau);
            else if(gagnant == 0){
                defaite(plateau);
             //   ajouterElement(maliste, 1);

            }
        }
        else if (choix.x >= 0 && choix.x <= 300 && choix.y >= 20 && choix.y <= 120){//RETOUR
            continuer = 0;
            start(plateau);
        }

    }
}

void personnalisation(SDL_Surface * plateau)
{

    int *tab;
    tab = ouvertureFichierSauvegarde();
    int continuer = 1;
    int chaine2[30];
    char chaine3[10] = "100";
    char chaine4[10] = "200";
    char chaine5[10] = "300";
    char chaine6[10] = "400";
    char chaine7[10] = "500";
    char chaine8[10] = "600";
    char chaine9[10] = "1000";
    SDL_Surface *texte = NULL;
    SDL_Surface * menu = SDL_LoadBMP("menus/personnalisation.bmp");
    SDL_Surface * impossible = SDL_LoadBMP("menus/impossible.bmp");
    SDL_SetColorKey(impossible, SDL_SRCCOLORKEY, SDL_MapRGB(impossible->format, 0,0, 255));
    SDL_SetAlpha(impossible, SDL_SRCALPHA, 128);
    SDL_Rect choix;
    TTF_Font *police = NULL;
    SDL_Color couleurBlanche = {255, 255, 255};
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;
    choix.x = 0;
    choix.y = 0;
    SDL_BlitSurface(menu, NULL, plateau, &positionFond);
    police = TTF_OpenFont("poor.ttf", 65);
    int argent;
    argent = monnaie();
    snprintf(chaine2, sizeof(chaine2), "Tokkens: %d", argent);
    texte = TTF_RenderText_Blended(police, chaine2, couleurBlanche);
    positionFond.x= 1200;
    positionFond.y= 20;
    SDL_BlitSurface(texte, NULL, plateau, &positionFond);
    if (tab[0]==0)
    {
        texte = TTF_RenderText_Blended(police, chaine3, couleurBlanche);
        positionFond.x= 210;
        positionFond.y= 490;
        SDL_BlitSurface(texte, NULL, plateau, &positionFond);
    }
    if (tab[1] == 0)
    {
        texte = TTF_RenderText_Blended(police, chaine4, couleurBlanche);
        positionFond.x= 410;
        positionFond.y= 490;
        SDL_BlitSurface(texte, NULL, plateau, &positionFond);
    }
    if (tab[2] == 0)
    {
        texte = TTF_RenderText_Blended(police, chaine5, couleurBlanche);
        positionFond.x= 610;
        positionFond.y= 490;
        SDL_BlitSurface(texte, NULL, plateau, &positionFond);
    }
    if (tab[3] == 0)
    {
        texte = TTF_RenderText_Blended(police, chaine6, couleurBlanche);
        positionFond.x= 810;
        positionFond.y= 490;
        SDL_BlitSurface(texte, NULL, plateau, &positionFond);
    }
    if (tab[4] == 0)
    {
        texte = TTF_RenderText_Blended(police, chaine7, couleurBlanche);
        positionFond.x= 1010;
        positionFond.y= 490;
        SDL_BlitSurface(texte, NULL, plateau, &positionFond);
    }
    if (tab[5] == 0)
    {
        texte = TTF_RenderText_Blended(police, chaine8, couleurBlanche);
        positionFond.x= 1210;
        positionFond.y= 490;
        SDL_BlitSurface(texte, NULL, plateau, &positionFond);
    }
     if (tab[6] == 0)
    {
        texte = TTF_RenderText_Blended(police, chaine9, couleurBlanche);
        positionFond.x= 1410;
        positionFond.y= 490;
        SDL_BlitSurface(texte, NULL, plateau, &positionFond);
    }

    SDL_Event event;
    while(continuer)
    {
        SDL_Flip(plateau);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_MOUSEBUTTONUP:
            choix.x=event.button.x;
            choix.y=event.button.y;
            break;
        case SDL_QUIT:
            continuer=0;
            break;
        }


        if (choix.x >= 0 && choix.x <= 300 && choix.y >= 20 && choix.y <= 120){///RETOUR
            continuer = 0;
            progression(plateau, -1, texte);
        }
        else if (choix.x >= 210 && choix.x <= 275 && choix.y >= 418 && choix.y <= 480){///ORANGE
            positionFond.x = 210;
            positionFond.y = 418;
            if(tab[0]==0 && argent >= 100)
                {
                    continuer = 0;
                    achatSkins(100);
                    modifierSauvegardeSkins(0);
                    personnalisation(plateau);
                }
            if(tab[0]==0 && argent < 100) ///PAS ASSEZ DARGENT
            {
                SDL_BlitSurface(impossible, NULL, plateau, &positionFond);
            }
            if(tab[0]==1)
            {
                choisirCouleur(1);
            }
        }
        else if (choix.x >= 412 && choix.x <= 480 && choix.y >= 418 && choix.y <= 480){///ROSE

                positionFond.x = 412;
                positionFond.y = 418;
                if(tab[1]==0 && argent >= 200)
                {
                    continuer = 0;
                    achatSkins(200);
                    modifierSauvegardeSkins(1);
                    personnalisation(plateau);
                }
                if(tab[1]==0 && argent < 200) ///PAS ASSEZ DARGENT
                {
                    SDL_BlitSurface(impossible, NULL, plateau, &positionFond);
                }
                if(tab[1]==1)
                {
                    choisirCouleur(2);
                }
        }
        else if (choix.x >= 612 && choix.x<= 679 && choix.y >= 418 && choix.y <=480){///VERT
            if(tab[2]==0 && argent >= 300)
                {
                    continuer = 0;
                    achatSkins(300);
                    modifierSauvegardeSkins(2);
                    personnalisation(plateau);
                }
                positionFond.x = 612;
                positionFond.y = 418;
                if(tab[2]==0 && argent < 300) ///PAS ASSEZ DARGENT
            {
                SDL_BlitSurface(impossible, NULL, plateau, &positionFond);
            }
            if(tab[2]==1)
            {
                choisirCouleur(3);
            }
        }
        else if (choix.x >= 816 && choix.x <= 882 && choix.y >= 418 && choix.y <= 480){///JAUNE
            if(tab[3]==0 && argent >= 400)
                {
                    continuer = 0;
                    achatSkins(400);
                    modifierSauvegardeSkins(3);
                    personnalisation(plateau);
                }
                positionFond.x = 816;
                positionFond.y = 418;
                if(tab[3]==0 && argent < 400) ///PAS ASSEZ DARGENT
            {
                SDL_BlitSurface(impossible, NULL, plateau, &positionFond);
            }
            if(tab[3]==1)
            {
                choisirCouleur(4);
            }
        }
       else if (choix.x >= 1020 && choix.x <= 1086 && choix.y >= 418 && choix.y <= 480){///GRIS
            if(tab[4]==0 && argent >= 500)
                {
                    continuer = 0;
                    achatSkins(500);
                    modifierSauvegardeSkins(4);
                    personnalisation(plateau);
                }
                positionFond.x = 1020;
                positionFond.y = 418;
                if(tab[4]==0 && argent < 400) ///PAS ASSEZ DARGENT
            {
                SDL_BlitSurface(impossible, NULL, plateau, &positionFond);
            }
            if(tab[4]==1)
            {
                choisirCouleur(5);
            }
        }
        else if (choix.x >= 1211 && choix.x <= 1276 && choix.y >= 418 && choix.y <= 480){///MARRON
            if(tab[5]==0 && argent >= 600)
                {
                    continuer = 0;
                    achatSkins(600);
                    modifierSauvegardeSkins(5);
                    personnalisation(plateau);
                }
                positionFond.x = 1211;
                positionFond.y = 418;
                if(tab[5]==0 && argent < 600) ///PAS ASSEZ DARGENT
            {
                SDL_BlitSurface(impossible, NULL, plateau, &positionFond);
            }
            if(tab[5]==1)
            {
                choisirCouleur(6);
            }
        }
       else if (choix.x >= 1413 && choix.x <= 1479 && choix.y >= 418 && choix.y <= 480){///NOIR
            if(tab[6]==0 && argent >= 1000)
                {
                    continuer = 0;
                    achatSkins(1000);
                    modifierSauvegardeSkins(6);
                    personnalisation(plateau);
                }
                positionFond.x = 1413;
                positionFond.y = 418;
                if(tab[6]==0 && argent < 1000) ///PAS ASSEZ DARGENT
            {
                SDL_BlitSurface(impossible, NULL, plateau, &positionFond);
            }
            if(tab[6]==1)
            {
                choisirCouleur(7);
            }
        }
        if (choix.x >= 230 && choix.x <= 350 && choix.y >= 700 && choix.y <= 760){///x1.2
            choisirVitesse(1);
        }
         else if (choix.x >= 550 && choix.x <= 670 && choix.y >= 700 && choix.y <= 760){///x1.4
            choisirVitesse(2);
        }
        else if (choix.x >= 850 && choix.x <= 970 && choix.y >= 700 && choix.y <= 760){///x1.6
            choisirVitesse(3);
        }
        else if (choix.x >= 1134 && choix.x <= 1240 && choix.y >= 700 && choix.y <= 760){///x1.8
            choisirVitesse(4);
        }
        else if (choix.x >= 1400 && choix.x <= 1475 && choix.y >= 700 && choix.y <= 760){///x2
            choisirVitesse(5);
        }


    }
}
void progression(SDL_Surface * plateau, int gagnant, SDL_Surface * texte)
{
    Liste *maliste=initialisation();
    if (gagnant == 1) ajouterElement(maliste, 1);
    if (gagnant == 0) ajouterElement(maliste, 0);
    if (gagnant == 1 || gagnant == 0) blocNote(maliste);
    int compteurWin = 0, compteurLoose = 0;
    char chaine[30], chaine1[30], chaine2[30];
    compteurLoose = comptageHistoriqueLoose();
    compteurWin = comptageHistoriqueWin();
    TTF_Font *police = NULL;
    police = TTF_OpenFont("poor.ttf", 65);
    SDL_Color couleurBlanche = {255, 255, 255};
    if(compteurWin < 2) snprintf(chaine, sizeof(chaine) , "%d   WIN    ", compteurWin);
    else snprintf(chaine, sizeof(chaine) , "WINS:  %d", compteurWin);
    if(compteurLoose < 2)snprintf(chaine1, sizeof(chaine) , "   LOSS  %d ", compteurLoose);
    else snprintf(chaine1, sizeof(chaine) , "    LOSSES:  %d", compteurLoose);
    strcat(chaine, chaine1);
    texte = TTF_RenderText_Blended(police, chaine, couleurBlanche);
    int continuer = 1;
    SDL_Surface * menu = SDL_LoadBMP("menus/progression.bmp");
    SDL_Rect choix;
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;
    choix.x = 0;
    choix.y = 0;
    SDL_BlitSurface(menu, NULL, plateau, &positionFond);
  //  texte = TTF_RenderText_Blended(police, compteurWin, couleurBlanche);
    positionFond.x = 850;
    positionFond.y = 330;
    SDL_BlitSurface(texte, NULL, plateau, &positionFond); /* Blit du texte */
    int argent;
    argent = monnaie();
    snprintf(chaine2, sizeof(chaine2), "Tokkens: %d", argent);
    texte = TTF_RenderText_Blended(police, chaine2, couleurBlanche);
    positionFond.x= 1200;
    positionFond.y= 50;
    SDL_BlitSurface(texte, NULL, plateau, &positionFond);
    SDL_Flip(plateau);
    SDL_Event event;
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_MOUSEBUTTONUP:
            choix.x=event.button.x;
            choix.y=event.button.y;
            break;
        case SDL_QUIT:
            continuer=0;
            break;
        }

        if (choix.x >= 0 && choix.x <= 300 && choix.y >= 20 && choix.y <= 120){//RETOUR
            continuer = 0;
            start(plateau);
        }
        if (choix.x >= 80 && choix.x <= 920 && choix.y >= 580 && choix.y <= 620){//PERSONNNALISATION
            continuer = 0;
            personnalisation(plateau);
        }
    }
}
void quitter(SDL_Surface * plateau)
{
    SDL_ShowCursor(SDL_ENABLE);
    int continuer = 1;
    SDL_Surface * menu = SDL_LoadBMP("menus/quitter.bmp");
    SDL_Rect choix;
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;
    choix.x = 0;
    choix.y = 0;
    SDL_BlitSurface(menu, NULL, plateau, &positionFond);
    SDL_Flip(plateau);
    SDL_Event event;
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_MOUSEBUTTONUP:
            choix.x=event.button.x;
            choix.y=event.button.y;
            break;
        case SDL_QUIT:
            continuer=0;
            break;
        }

        if (choix.x >= 1020 && choix.x <= 1200 && choix.y >= 470 && choix.y <= 530){/// La personne ne veut plus quitter
            start(plateau);
            continuer=0;
        }
        if (choix.x >= 330 && choix.x <= 470 && choix.y >= 470 && choix.y <= 530){///Quitter
            continuer = 0;
        }
    }
}

void pause(SDL_Surface * plateau, int ***tab, int diff, int hauteur, int largeur, int charger)
{
    int continuer = 1;
    SDL_Surface * menu = SDL_LoadBMP("menus/pause.bmp");
    SDL_ShowCursor(SDL_ENABLE);
    SDL_Rect choix;
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;
    choix.x = 0;
    choix.y = 0;
    SDL_BlitSurface(menu, NULL, plateau, &positionFond);
    SDL_Flip(plateau);
    SDL_Event event;
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_MOUSEBUTTONUP:
            choix.x=event.button.x;
            choix.y=event.button.y;
            break;
        case SDL_QUIT:
            continuer=0;
            break;
        }
        if (choix.x >= 500 && choix.x <= 1100 && choix.y >= 350 && choix.y <= 430){
            continuer = 0;
            jeu(plateau, tab, diff, hauteur, largeur,1);
        }
        else if (choix.x >= 400 && choix.x <= 1200 && choix.y >= 550 && choix.y <= 630){
            continuer = 0;
            start(plateau);
        }


    }
}

void verificationReset(SDL_Surface * plateau)
{
        int continuer = 1;
        SDL_Surface * menu = SDL_LoadBMP("menus/reset.bmp");
        SDL_Rect choix;
        SDL_Rect positionFond;

        positionFond.x = 0;
        positionFond.y = 0;
        choix.x = 0;
        choix.y = 0;
        SDL_BlitSurface(menu, NULL, plateau, &positionFond);
        SDL_Flip(plateau);

        SDL_Event event;
        while(continuer)
        {
            SDL_WaitEvent(&event);
            switch(event.type){
                case SDL_QUIT:
                    continuer = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                choix.x=event.button.x;
                choix.y=event.button.y;
                break;
            }
            if (choix.x >= 320 && choix.x <= 470 && choix.y >= 480 && choix.y <= 550){
                continuer = 0;
                reset();
                start(plateau);
            }
            if (choix.x >= 1010 && choix.x <= 1190 && choix.y >= 480 && choix.y <= 550){
                continuer = 0;
                option(plateau);
            }
        }
}
void credits(SDL_Surface * plateau)
{
        int continuer = 1;
        SDL_Surface * menu = SDL_LoadBMP("menus/credits.bmp");
        SDL_Rect choix;
        SDL_Rect positionFond;

        positionFond.x = 0;
        positionFond.y = 0;
        choix.x = 0;
        choix.y = 0;
        SDL_BlitSurface(menu, NULL, plateau, &positionFond);
        SDL_Flip(plateau);

        SDL_Event event;
        while(continuer)
        {
            SDL_WaitEvent(&event);
            switch(event.type){
                case SDL_QUIT:
                    continuer = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                choix.x=event.button.x;
                choix.y=event.button.y;
                break;
            }
            if (choix.x >= 0 && choix.x <= 300 && choix.y >= 20 && choix.y <= 120){
                continuer = 0;
                option(plateau);
            }
        }
}

void option(SDL_Surface * plateau)
{
    int continuer = 1;
    SDL_Surface * menu = SDL_LoadBMP("menus/option.bmp");
    SDL_Rect choix;
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;
    choix.x = 0;
    choix.y = 0;
    SDL_BlitSurface(menu, NULL, plateau, &positionFond);
    SDL_Flip(plateau);
    SDL_Event event;
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_MOUSEBUTTONUP:
            choix.x=event.button.x;
            choix.y=event.button.y;
            break;
        case SDL_QUIT:
            continuer=0;
            break;
        }
        if (choix.x >= 0 && choix.x <= 300 && choix.y >= 20 && choix.y <= 120){//RETOUR
            continuer = 0;
            start(plateau);
        }
        if (choix.x >= 90 && choix.x <= 270 && choix.y >= 710 && choix.y <= 790){//RESET
            continuer = 0;
            verificationReset(plateau);
            start(plateau);
        }
        if (choix.x >= 90 && choix.x<= 280 && choix.y >= 480 && choix.y <=580){//CREDIT
            continuer = 0;
            credits(plateau);
        }


    }
}


void victoire(SDL_Surface * plateau)
{
        SDL_Surface *texte = NULL;
        //ajouterElement(maliste, 1);
        char chaine[30] = "40", chaine2[30];
        SDL_ShowCursor(SDL_ENABLE);
        int continuer = 1;
        SDL_Surface * menu = SDL_LoadBMP("menus/victoire.bmp");
        SDL_Rect choix;
        SDL_Rect positionFond;
        positionFond.x = 0;
        positionFond.y = 0;
        choix.x = 0;
        choix.y = 0;
        SDL_BlitSurface(menu, NULL, plateau, &positionFond);
        TTF_Font *police = NULL;
        police = TTF_OpenFont("poor.ttf", 65);
        SDL_Color couleurBlanche = {255, 255, 255};
        texte = TTF_RenderText_Blended(police, chaine, couleurBlanche);
        positionFond.x= 1285;
        positionFond.y= 530;
        SDL_BlitSurface(texte, NULL, plateau, &positionFond);
        gagnerTokkens();
        int argent;
        argent = monnaie();
        snprintf(chaine2, sizeof(chaine2), "%d", argent);
        texte = TTF_RenderText_Blended(police, chaine2, couleurBlanche);
        positionFond.x = 820;
        positionFond.y = 535;
        SDL_BlitSurface(texte, NULL, plateau, &positionFond);
        SDL_Flip(plateau);
        SDL_Event event;
        while(continuer)
            {
                SDL_WaitEvent(&event);
                switch(event.type)
                {
                case SDL_MOUSEBUTTONUP:
                    choix.x=event.button.x;
                    choix.y=event.button.y;
                    break;
                case SDL_QUIT:
                    continuer=0;
                    break;
                }
                if (choix.x >= 390 && choix.x <= 1220 && choix.y >= 720 && choix.y <= 810){//RETOUR
                    continuer = 0;
                    progression(plateau, 1, texte);
                }
            }

}

void defaite(SDL_Surface * plateau)
{
    SDL_Surface *texte=NULL;
    SDL_ShowCursor(SDL_ENABLE);
    int continuer = 1;
    SDL_Surface * menu = SDL_LoadBMP("menus/defaite.bmp");
    SDL_Rect choix;
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;
    choix.x = 0;
    choix.y = 0;
    SDL_BlitSurface(menu, NULL, plateau, &positionFond);
    SDL_Flip(plateau);
    SDL_Event event;
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_MOUSEBUTTONUP:
            choix.x=event.button.x;
            choix.y=event.button.y;
            break;
        case SDL_QUIT:
            continuer=0;
            break;
        }
        if (choix.x >= 390 && choix.x <= 1210 && choix.y >= 750 && choix.y <= 820){//Appuyez pour continuer
            continuer = 0;
            progression(plateau, 0, texte);
        }
    }
}





int ia_difficulte(SDL_Surface * plateau, int **tab, int * direction, int hauteur, int largeur, SDL_Rect * j, int difficulte){
    if(difficulte == 0) return ia_shit();
    else if(difficulte == 1) return ia_izi(plateau, tab, direction, hauteur, largeur, j);
    else if(difficulte == 2) return ia_med(plateau, tab, direction, hauteur, largeur, j);

    else return -1;
}

int ia_med(SDL_Surface * plateau, int **tab, int * direction, int hauteur, int largeur, SDL_Rect * pos){

    int choix[4] = {0}, i, j, k = 1, max = 0, c = 0;
    int dX = abs((pos[1].x-190) - (pos[0].x-190)), dY = abs((pos[1].y-130) - (pos[0].y-130));
    int possibilites = 0; ///IA
    float distance;

    ///
    if((((pos[1].y) - 130) - k) < 1) choix[0] = 0;
    else{
        for(i = ((pos[1].y)-130) - k ; i > 0 ; i--){
            if(tab[i][(pos[1].x) - 190] == 0) choix[0] +=1;
            else i = 0;
        }
    }

    if((((pos[1].y) - 130) + k) > hauteur*10) choix[1] = 0;
    else{
        for(i = ((pos[1].y)-130 + k); i < 10*hauteur ; i++){
            if(tab[i][(pos[1].x) - 190] == 0) choix[1] += 1;
            else i = 10*hauteur;
        }
    }

    if((((pos[1].x)-190) - k) < 1) choix[2] = 0;
    else{
        for(j = ((pos[1].x) - 190) - k ; j > 0 ; j--){
            if(tab[(pos[1].y)-130][j] == 0) choix[2] += 1;
            else j = 0;
        }
    }

    if((((pos[1].x) - 190) + k) > largeur*10) choix[3] = 0;
    else{
        for(j = ((pos[1].x) - 190) + k ; j < 10*largeur ; j++){
            if(tab[(pos[1].y)-130][j] == 0) choix[3] += 1;
            else j = 10*largeur;
        }
    }

    k = 0;
    max = choix[0];
    for(i = 1; i < 4; i++){
        if(max <= choix[i]){
            c = ((rand()%2)+1);
            if(max == choix[i]){
                if(c == 2) k = i;
                ///Sinon on reste sur le valeur de k !
            }
            else k = i ;
            max = choix[i];
        }
    }

    ///k = indice de la direction avec le plus de possibilités (en ligne droite)

    if(k == 0) k = 1;
    else if(k == 1) k = 2;
    else if(k == 2) k = 4;
    else if(k == 3) k = 8;

    distance = sqrt(abs((pos[0].x-190) - (pos[1].x-190)) * abs((pos[0].x-190) - (pos[1].x-190))  + abs((pos[0].y-130 - (pos[1].y-130)) * abs((pos[0].y-130 - (pos[1].y-130)))));


///Combat rapproché

///5h de réflexion/debug pour pondre ces 20 lignes
///#AuBoutDeMaVie


        if(distance < 300){
            if(pos[1].x-190 >= pos[0].x-190 && pos[1].y -130 <= pos[0].y-130){/// J en bas à gauche (donc attaque en allant en bas... etc..

                if(dX < dY && choix[2] > 1 && direction[0] == 1) return 4;
                if(dX > dY && choix[1] > 1 && direction[0] == 8) return 2;
            }
            else if(pos[1].x-190 <= pos[0].x-190 && pos[1].y -130 <=pos[0].y-130){/// J en bas à droite
                if(dX < dY && choix[3] > 1 && direction[0] == 1) return 8;
                if(dX > dY &&choix[1] > 1 && direction[0] == 4) return 2;
            }
            else if(pos[1].x-190 >= pos[0].x-190 && pos[1].y -130 >= pos[0].y-130){/// J en haut à gauche
                if(dX < dY &&  choix[2] > 1 && direction[0] == 2) return 4;
                if(dX > dY &&choix[0] > 1 && direction[0] == 8) return 1;
            }
            else if(pos[1].x-190 <= pos[0].x-190 && pos[1].y -130 >= pos[0].y-130){/// J en haut à droite
                if(dX < dY && choix[3] > 1 && direction[0] == 4) return 8;
                if(dX > dY &&choix[0] > 1 && direction[0] == 2) return 1;
            }
            possibilites = 0;
            for(i = 0; i < 4; i++) if(choix[i] == 0 || choix[i] == 1) possibilites++;
            if(possibilites > 1) return k;

            return direction[1];
        }
        if(choix[0] == 1 || choix[1] == 1 || choix[2] == 1 || choix[3] == 1) return k;

        else return direction[1];


}


int ia_izi(SDL_Surface * plateau, int **tab, int * direction, int hauteur, int largeur, SDL_Rect * pos){

    int choix[4] = {0}, i, j, k = 1, max = 0, c = 0;
    ///k = verification de pixel;

    if((((pos[1].y) - 130) - k) < 1) choix[0] = 0;
    else{
        for(i = ((pos[1].y)-130) - k ; i > 0 ; i--){
            if(tab[i][(pos[1].x) - 190] == 0) choix[0] +=1;
            else i = 0;
        }
    }

    if((((pos[1].y) - 130) + k) > hauteur*10) choix[1] = 0;
    else{
        for(i = ((pos[1].y)-130 + k); i < 10*hauteur ; i++){
            if(tab[i][(pos[1].x) - 190] == 0) choix[1] += 1;
            else i = 10*hauteur;
        }
    }

    if((((pos[1].x)-190) - k) < 1) choix[2] = 0;
    else{
        for(j = ((pos[1].x) - 190) - k ; j > 0 ; j--){
            if(tab[(pos[1].y)-130][j] == 0) choix[2] += 1;
            else j = 0;
        }
    }

    if((((pos[1].x) - 190) + k) > largeur*10) choix[3] = 0;
    else{
        for(j = ((pos[1].x) - 190) + k ; j < 10*largeur ; j++){
            if(tab[(pos[1].y)-130][j] == 0) choix[3] += 1;
            else j = 10*largeur;
        }
    }


    k = 0;
    max = choix[0];
    for(i = 1; i < 4; i++){
        if(max <= choix[i]){
            c = ((rand()%2)+1);
            if(max == choix[i]){
                if(c == 2) k = i;
                ///Sinon on reste sur le valeur de k !
            }
            else k = i ;
            max = choix[i];
        }
    }


    if(k == 0) return 1;
    else if(k == 1) return 2;
    else if(k == 2) return 4;
    else if(k == 3) return 8;
    return -1;
}

int ia_shit(){ ///NE JAMAIS UTLIISER CETTE MERDE

    int choix;

    choix = (rand()%4)+1;

    if(choix == 1) return 1 ;
    else if(choix == 2) return 4;
    else if (choix == 3) return 2;
    else if(choix == 4) return 8;

    return 0;
}











