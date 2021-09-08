//
// Michele Melis 65798 - Progetto Space Invaders
//

/** Librerie **/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <curses.h>
#include <time.h>

/** Dimensione finestra di gioco **/
#define DIM_X getmaxx(stdscr)
#define DIM_Y getmaxy(stdscr)

/** Macro **/
#define N_NAVICELLE_PF 8                // Numero di navicelle
#define NAVICELLE_NEMICHE_DELAY 100000  // Tempo di movimento delle navicelle
#define HP 1                            // Vite iniziali del giocatore
#define MISSILE_DELAY 20000             // Tempo di movimento dei missili
#define BOMB_DELAY 100000               // Tempo di movimento della bomba
#define N_NAVICELLE_SF N_NAVICELLE_PF   // Numero di navicelle di secondo livello
#define VITE_NAVICELLA 2                // Vite totali di ogni gruppo di navicelle di secondo livello
#define PUNTI_NPF 100                   // Punti guadagnati per ogni navicella di primo livello abbattuta
#define PUNTI_NSF 40                    // Punti guadagnati per ogni gruppo di navicelle di secondo livello abbattute

/** ID oggetti **/
/* Ogni elemento ha il suo id per essere riconosciuto e successivamente stampato a video */
#define ID_ASTRONAVE 0
#define ID_MISSILE_SX 1
#define ID_MISSILE_DX 2
#define ID_BOMBA 3
#define ID_PRIMA_NAVICELLA 4
#define ID_PRIMA_NAVICELLA_SF (ID_PRIMA_NAVICELLA+N_NAVICELLE_PF)

/** Struttura oggetti **/
typedef struct{
    int object_id;      // Indica l'id dell'elemento
    int x_location;     // Indica la coordinata x dell'elemento
    int y_location;     // Indica la coordinata y dell'elemento
    _Bool existence;    // Indica l'esistenza dell'oggetto
} GameObject;

/** Processi/Threads **/
// Processo/Thread che gestisce le coordinate dell'astronave
void astronave(int pipeout);
// Processo/Thread che gestisce le coordinate di una navicella di primo livello
void navicella_pf(int pipeout, GameObject object);
// Processo/Thread che gestisce le coordinate di una navicella di secondo livello
void navicella_sf(int pipeout, GameObject object);
// Processo/Thread che gestisce le coordinate della bomba
void bomba(int pipeout, int pipein);
// Processo/Thread che gestisce la stampa e le collisioni degli oggetti
void area_gioco(int pipein, int pipeout_bomba);
// Processo/Thread che gestisce le coordinate del missile sinistro
void missileSx(int pipeout, int startPosition);
// Processo/Thread che gestisce le coordinate del missile destro
void missileDx(int pipeout, int startPosition);

/** Funzioni **/
// Schermata iniziale
void schermata_iniziale();
// Genera le coordinate iniziali della bomba (sotto una navicella)
GameObject initBomba(GameObject *navicellaNemicaPrimaFase, GameObject *navicellaNemicaSecondaFase);
// Stampa "Game Over" e il punteggio a video
void game_over(int score, _Bool won);