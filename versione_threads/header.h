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
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

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
#define LOAD_TIME 30

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

/** Oggetti condivisi **/
GameObject oggetto_astronave;
GameObject oggetto_missile_sx, oggetto_missile_dx;
GameObject oggetto_navicella_pf[N_NAVICELLE_PF], oggetto_navicella_sf[N_NAVICELLE_SF];
GameObject oggetto_bomba;

// Thread
extern pthread_t pt_astronave, pt_missile_sx, pt_missile_dx, pt_navicella_nemica_pf[N_NAVICELLE_PF], pt_navicella_nemica_sf[N_NAVICELLE_SF], pt_bomba;
// Posizione centro astronave (per la comunicazione con i missili)
extern int centroAstronave;
// Mutex
extern pthread_mutex_t mutex_stampa, mutex_navicella_1[N_NAVICELLE_PF], mutex_navicella_2[N_NAVICELLE_SF];
// Vite delle navicelle di secondo livello
extern int viteNavicellaNemicaSecondaFase[N_NAVICELLE_SF];

/** Processi/Threads **/
// Processo/Thread che gestisce le coordinate dell'astronave
void *astronave();
// Processo/Thread che gestisce le coordinate di una navicella di primo livello
void *navicella_pf(void *genericObject);
// Processo/Thread che gestisce le coordinate di una navicella di secondo livello
void *navicella_sf(void *genericObject);
// Processo/Thread che gestisce le coordinate della bomba
void *bomba();
// Processo/Thread che gestisce la stampa e le collisioni degli oggetti
void area_gioco();
// Processo/Thread che gestisce le coordinate del missile sinistro
void *missileSx();
// Processo/Thread che gestisce le coordinate del missile destro
void *missileDx();

/** Funzioni **/
// Schermata iniziale
void schermata_iniziale();
// Genera le coordinate iniziali della bomba (sotto una navicella)
GameObject initBomba(GameObject *navicellaNemicaPrimaFase, GameObject *navicellaNemicaSecondaFase);
// Stampa "Game Over" e il punteggio a video
void game_over(int score, _Bool won);
// Disegna o cancella la navicella dallo dallo schermo
void disegnaNPF(_Bool disegno, GameObject navicella);
// Disegna o cancella la navicella dallo dallo schermo
void disegnaNSF(_Bool disegno, GameObject navicella, int colore);