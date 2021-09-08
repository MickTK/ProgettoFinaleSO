//
// Michele Melis 65798 - Progetto Space Invaders
//

#include "header.h"

pthread_t pt_missile_sx, pt_missile_dx;
int centroAstronave;
pthread_mutex_t mutex_stampa = PTHREAD_MUTEX_INITIALIZER;
_Bool shot = true;

/** Astronave giocatore **/
void *astronave(){

    /** Astronave **/
    oggetto_astronave.object_id = ID_ASTRONAVE;
    oggetto_astronave.x_location = DIM_X / 2 - strlen("(=I=)") / 2; // L'astronave viene posizionata al centro dell'asse x
    oggetto_astronave.y_location = DIM_Y - 1;                       // L'astronave viene posizionata in fondo allo schermo
    oggetto_astronave.existence = true;

    /** Missili **/
    pthread_t pt_missile_sx, pt_missile_dx;

    pthread_mutex_lock(&mutex_stampa);
    attron(COLOR_PAIR(3));
    mvprintw(oggetto_astronave.y_location, oggetto_astronave.x_location, "(=I=)");
    refresh();
    pthread_mutex_unlock(&mutex_stampa);

    while(true) {
        switch(getch()) {

            /** L'astronave si muove verso sinistra **/
            case KEY_LEFT:
                if (oggetto_astronave.x_location > 0){
                    pthread_mutex_lock(&mutex_stampa);
                    mvprintw(oggetto_astronave.y_location, oggetto_astronave.x_location, "     ");
                    pthread_mutex_unlock(&mutex_stampa);
                    oggetto_astronave.x_location-=1;
                    pthread_mutex_lock(&mutex_stampa);
                    attron(COLOR_PAIR(3));
                    mvprintw(oggetto_astronave.y_location, oggetto_astronave.x_location, "(=I=)");
                    pthread_mutex_unlock(&mutex_stampa);
                    refresh();
                }
                break;

            /** L'astronave si muove verso destra **/
            case KEY_RIGHT:
                if (oggetto_astronave.x_location + strlen("(=I=)") < DIM_X - 1){
                    pthread_mutex_lock(&mutex_stampa);
                    mvprintw(oggetto_astronave.y_location, oggetto_astronave.x_location, "     ");
                    pthread_mutex_unlock(&mutex_stampa);
                    oggetto_astronave.x_location+=1;
                    pthread_mutex_lock(&mutex_stampa);
                    attron(COLOR_PAIR(3));
                    mvprintw(oggetto_astronave.y_location, oggetto_astronave.x_location, "(=I=)");
                    pthread_mutex_unlock(&mutex_stampa);
                    refresh();
                }
                break;

            /** Vengono sparati i missili **/
            case ' ':
                beep();
                if(shot){
                    shot = false;
                    centroAstronave = oggetto_astronave.x_location+2; // Posizione di partenza dei missili
                    /** Generazione processi dei missili **/
                    pthread_create(&pt_missile_sx,NULL,missileSx,NULL);
                    pthread_create(&pt_missile_dx,NULL,missileDx,NULL);
                }

                break;
        }
    }
}

/** Missile sinistro **/
void *missileSx(){

    /** Inizializzazione missile **/
    oggetto_missile_sx.object_id = ID_MISSILE_SX;
    oggetto_missile_sx.existence = true;
    oggetto_missile_sx.y_location = DIM_Y-2;
    oggetto_missile_sx.x_location = centroAstronave;

    /** Vecchia posizione **/
    pthread_mutex_lock(&mutex_stampa);
    mvaddch(oggetto_missile_sx.y_location, oggetto_missile_sx.x_location, ' ');
    refresh();
    pthread_mutex_unlock(&mutex_stampa);

    /** Prima posizione **/
    pthread_mutex_lock(&mutex_stampa);
    attron(COLOR_PAIR(3));
    mvaddch(oggetto_missile_sx.y_location, oggetto_missile_sx.x_location, '*');
    refresh();
    pthread_mutex_unlock(&mutex_stampa);
    usleep(MISSILE_DELAY);

    /** Vita del missile **/
    while(oggetto_missile_sx.y_location >= 0){
        pthread_mutex_lock(&mutex_stampa);
        mvaddch(oggetto_missile_sx.y_location, oggetto_missile_sx.x_location, ' ');
        refresh();
        pthread_mutex_unlock(&mutex_stampa);
        oggetto_missile_sx.y_location--;
        oggetto_missile_sx.x_location--;

        pthread_mutex_lock(&mutex_stampa);
        attron(COLOR_PAIR(3));
        mvaddch(oggetto_missile_sx.y_location, oggetto_missile_sx.x_location, '*');
        refresh();
        pthread_mutex_unlock(&mutex_stampa);
        usleep(MISSILE_DELAY);
    }
    shot = true;
    pthread_cancel(pt_missile_sx);
}

/** Missile destro **/
void *missileDx(){

    oggetto_missile_dx.object_id = ID_MISSILE_DX;
    oggetto_missile_dx.existence = true;
    oggetto_missile_dx.y_location = DIM_Y-2;
    oggetto_missile_dx.x_location = centroAstronave;

    pthread_mutex_lock(&mutex_stampa);
    mvaddch(oggetto_missile_dx.y_location, oggetto_missile_dx.x_location, ' ');
    refresh();
    pthread_mutex_unlock(&mutex_stampa);
    pthread_mutex_lock(&mutex_stampa);
    attron(COLOR_PAIR(3));
    mvaddch(oggetto_missile_dx.y_location, oggetto_missile_dx.x_location, '*');
    refresh();
    pthread_mutex_unlock(&mutex_stampa);

    usleep(MISSILE_DELAY);

    while(oggetto_missile_dx.y_location >= 0){
        pthread_mutex_lock(&mutex_stampa);
        mvaddch(oggetto_missile_dx.y_location, oggetto_missile_dx.x_location, ' ');
        refresh();
        pthread_mutex_unlock(&mutex_stampa);

        oggetto_missile_dx.y_location--;
        oggetto_missile_dx.x_location++;

        pthread_mutex_lock(&mutex_stampa);
        attron(COLOR_PAIR(3));
        mvaddch(oggetto_missile_dx.y_location, oggetto_missile_dx.x_location, '*');
        refresh();
        pthread_mutex_unlock(&mutex_stampa);

        usleep(MISSILE_DELAY);
    }
    pthread_cancel(pt_missile_dx);
}