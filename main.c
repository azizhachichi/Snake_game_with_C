#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#define TETE '0'
#define QUEUE 'o'
#define VIDE ' '
#define MUR '='
#define FRUIT '*'

// Dimensions du plateau
#define HAUTEUR 20
#define LARGEUR 40

struct Tete_serpent {
    int x, y;
};

struct Partie_Queue_serpent {
    int x, y;
};

struct Queue_serpent {
    int longueur_queue;
    struct Partie_Queue_serpent tab[HAUTEUR * LARGEUR];
};

void initialiserPlateau(char plateau[HAUTEUR][LARGEUR]) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            plateau[i][j] = (i == 0 || i == HAUTEUR - 1 || j == 0 || j == LARGEUR - 1)? MUR : VIDE ;
        }
    }
}

void miseAJourQueue(struct Queue_serpent* queue, int save_x, int save_y) {
    for (int i = queue->longueur_queue - 1; i > 0; i--) {
        queue->tab[i] = queue->tab[i - 1];
    }
    if (queue->longueur_queue > 0) {
        queue->tab[0].x = save_x;
        queue->tab[0].y = save_y;
    }
}

void genererFruit(char plateau[HAUTEUR][LARGEUR], int* fruit_x, int* fruit_y) {
    do {
        *fruit_x = rand() % (HAUTEUR - 2) + 1;
        *fruit_y = rand() % (LARGEUR - 2) + 1;
    } while (plateau[*fruit_x][*fruit_y] != VIDE);
    plateau[*fruit_x][*fruit_y] = FRUIT;
}

int collisionAvecQueue(struct Queue_serpent* queue, int tete_x, int tete_y) {
    for (int i = 0; i < queue->longueur_queue; i++) {
        if (queue->tab[i].x == tete_x && queue->tab[i].y == tete_y) {
            return 1;
        }
    }
    return 0;
}


void jeuSerpent(char plateau[HAUTEUR][LARGEUR]) {
    struct Tete_serpent tete = {4, 5};
    struct Queue_serpent queue = {1, {{5, 5}}};
    int fruit_x, fruit_y, score = 0;
    char touche = 'd';

    srand((unsigned int)time(NULL));
    genererFruit(plateau, &fruit_x, &fruit_y);
    plateau[tete.x][tete.y] = TETE;
    plateau[queue.tab[0].x][queue.tab[0].y] = QUEUE;

    while (1) {
        if (_kbhit()) {
            char first_char = _getch();
            if (first_char == -32 || first_char == 224) {
                switch (_getch()) {
                    case 72: touche = 'z'; break; // Haut
                    case 80: touche = 'x'; break; // Bas
                    case 75: touche = 'q'; break; // Gauche
                    case 77: touche = 'd'; break; // Droite
                }
            }
        }

        int prev_x = tete.x, prev_y = tete.y;
        switch (touche) {
            case 'z': tete.x--; break;
            case 'x': tete.x++; break;
            case 'q': tete.y--; break;
            case 'd': tete.y++; break;
        }

        if (tete.x < 1 || tete.x >= HAUTEUR - 1 || tete.y < 1 || tete.y >= LARGEUR - 1 || collisionAvecQueue(&queue, tete.x, tete.y)) {
            printf("\nGame Over! Score final: %d\n", score);
            break;
        }

        if (tete.x == fruit_x && tete.y == fruit_y) {
            queue.longueur_queue++;
            score++;
            genererFruit(plateau, &fruit_x, &fruit_y);
        }

        miseAJourQueue(&queue, prev_x, prev_y);
        for (int i = 1; i < HAUTEUR - 1; i++) {
            for (int j = 1; j < LARGEUR - 1; j++) {
                plateau[i][j] = VIDE;
            }
        }

        plateau[fruit_x][fruit_y] = FRUIT;
        plateau[tete.x][tete.y] = TETE;
        for (int i = 0; i < queue.longueur_queue; i++) {
            plateau[queue.tab[i].x][queue.tab[i].y] = QUEUE;
        }

        afficherPlateauStable(plateau, score);
        Sleep(100);
    }
}


// Fonction pour positionner le curseur
void positionnerCurseur(int x, int y) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(console, (COORD){.X = (SHORT)y, .Y = (SHORT)x});
}

// Fonction pour masquer le curseur
void masquerCurseur() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Rendre le curseur invisible
    SetConsoleCursorInfo(console, &cursorInfo);
}

// Affichage du plateau sans clignotement
void afficherPlateauStable(char plateau[HAUTEUR][LARGEUR], int score) {
    positionnerCurseur(0, 0); // Remettre le curseur au coin sup�rieur gauche
    printf("Score: %d\n", score);
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            printf("%c", plateau[i][j]);
        }
        printf("\n");
    }
}


int main() {
    system("chcp 437 > nul");
    system("cls");

    char plateau[HAUTEUR][LARGEUR];
    initialiserPlateau(plateau);

    masquerCurseur();
    jeuSerpent(plateau);

    return 0;
}
