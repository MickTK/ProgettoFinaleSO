//
// Michele Melis 65798 - Progetto Space Invaders
//

#include "header.h"

/** Navicella prima fase **/
void *navicella_pf(void *genericObject) {
    GameObject n = *(GameObject *) genericObject;
    int index = n.object_id - 4;

    pthread_mutex_lock(&mutex_navicella_1[index]);
    oggetto_navicella_pf[index] = *(GameObject *) genericObject;
    pthread_mutex_unlock(&mutex_navicella_1[index]);

    _Bool tipo_l;       // Indica se la navicella fa parte della colonna di sinistra
    _Bool right_mov;    // Indica se la navicella deve muoversi verso destra

    /* Se la navicella viene posizionata a sinistra dello schermo farà parte della
     * colonna di sinistra, altrimenti di quella di destra
     */
    if (oggetto_navicella_pf[index].x_location < DIM_X / 2) tipo_l = true;
    else tipo_l = false;

    while (true){
        if(oggetto_navicella_pf[index].existence){
            /** Calcolo direzione **/
            // La navicella si muove dal bordo sinistro al centro dello schermo
            if (tipo_l) {
                if (oggetto_navicella_pf[index].x_location <= 0) right_mov = true;
                else if (oggetto_navicella_pf[index].x_location >= DIM_X / 2 - 3) right_mov = false;
            }
            // La navicella si muove dal bordo destro al centro dello schermo
            else {
                if (oggetto_navicella_pf[index].x_location <= DIM_X / 2) right_mov = true;
                else if (oggetto_navicella_pf[index].x_location + 3 >= DIM_X) right_mov = false;
            }

            /** Si muove verso il basso **/
            if (tipo_l) {
                if (oggetto_navicella_pf[index].x_location <= 0 || oggetto_navicella_pf[index].x_location >= DIM_X / 2 - 3) {
                    // Cancella la vecchia posizione
                    disegnaNPF(false, oggetto_navicella_pf[index]);
                    // Aggiorna la coordinata
                    pthread_mutex_lock(&mutex_navicella_1[index]);
                    oggetto_navicella_pf[index].y_location++;
                    pthread_mutex_unlock(&mutex_navicella_1[index]);
                    // Disegna la navicelal nella nuova posizione
                    disegnaNPF(true, oggetto_navicella_pf[index]);
                    usleep(NAVICELLE_NEMICHE_DELAY);
                }
            } else {
                if (oggetto_navicella_pf[index].x_location <= DIM_X / 2 || oggetto_navicella_pf[index].x_location + 3 >= DIM_X) {
                    disegnaNPF(false, oggetto_navicella_pf[index]);

                    pthread_mutex_lock(&mutex_navicella_1[index]);
                    oggetto_navicella_pf[index].y_location++;
                    pthread_mutex_unlock(&mutex_navicella_1[index]);

                    disegnaNPF(true, oggetto_navicella_pf[index]);
                    usleep(NAVICELLE_NEMICHE_DELAY);
                }
            }

            /** Movimento laterale **/
            disegnaNPF(false, oggetto_navicella_pf[index]);

            pthread_mutex_lock(&mutex_navicella_1[index]);
            if (right_mov) oggetto_navicella_pf[index].x_location++;
            else oggetto_navicella_pf[index].x_location--;
            pthread_mutex_unlock(&mutex_navicella_1[index]);

            disegnaNPF(true, oggetto_navicella_pf[index]);

            /** Delay **/
            usleep(NAVICELLE_NEMICHE_DELAY);
        }
        else{
            disegnaNPF(false, oggetto_navicella_pf[index]);
            break;
        }
    }
}

/** Navicella seconda fase **/
void *navicella_sf(void *genericObject){
    GameObject n = *(GameObject *) genericObject;
    int index = n.object_id - 12, j;

    pthread_mutex_lock(&mutex_navicella_2[index]);
    oggetto_navicella_sf[index] = *(GameObject *) genericObject;
    pthread_mutex_unlock(&mutex_navicella_2[index]);

    _Bool tipo_l, right_mov;   // Indica la colonna in cui si trova la navicella

    if(oggetto_navicella_sf[index].x_location < DIM_X/2) tipo_l = true;
    else tipo_l = false;

    while(true){

        /** Calcolo direzione **/
        if(tipo_l){
            if(oggetto_navicella_sf[index].x_location <= 0) right_mov = true;
            else if(oggetto_navicella_sf[index].x_location >= DIM_X/2 -3) right_mov = false;
        }
        else {
            if(oggetto_navicella_sf[index].x_location <= DIM_X/2) right_mov = true;
            else if(oggetto_navicella_sf[index].x_location+3 >= DIM_X) right_mov = false;
        }

        /** Si muove verso il basso **/
        if(tipo_l){
            if(oggetto_navicella_sf[index].x_location <= 0 || oggetto_navicella_sf[index].x_location >= DIM_X/2 -3){
                disegnaNSF(false, oggetto_navicella_sf[index], viteNavicellaNemicaSecondaFase[index]);

                pthread_mutex_lock(&mutex_navicella_2[index]);
                oggetto_navicella_sf[index].y_location++;
                pthread_mutex_unlock(&mutex_navicella_2[index]);

                disegnaNSF(true, oggetto_navicella_sf[index], viteNavicellaNemicaSecondaFase[index]);
                usleep(NAVICELLE_NEMICHE_DELAY);
            }
        }
        else {
            if(oggetto_navicella_sf[index].x_location <= DIM_X/2 || oggetto_navicella_sf[index].x_location+3 >= DIM_X){
                disegnaNSF(false, oggetto_navicella_sf[index], viteNavicellaNemicaSecondaFase[index]);

                pthread_mutex_lock(&mutex_navicella_2[index]);
                oggetto_navicella_sf[index].y_location++;
                pthread_mutex_unlock(&mutex_navicella_2[index]);

                disegnaNSF(true, oggetto_navicella_sf[index], viteNavicellaNemicaSecondaFase[index]);
                usleep(NAVICELLE_NEMICHE_DELAY);
            }
        }

        /** Movimento laterale **/
        disegnaNSF(false, oggetto_navicella_sf[index], viteNavicellaNemicaSecondaFase[index]);

        pthread_mutex_lock(&mutex_navicella_2[index]);
        if(right_mov) oggetto_navicella_sf[index].x_location++;
        else oggetto_navicella_sf[index].x_location--;
        pthread_mutex_unlock(&mutex_navicella_2[index]);

        disegnaNSF(true, oggetto_navicella_sf[index], viteNavicellaNemicaSecondaFase[index]);

        /** Delay **/
        usleep(NAVICELLE_NEMICHE_DELAY);

        // Cancella la navicella per l'ultima volta
        if(viteNavicellaNemicaSecondaFase[index] == 0){
            pthread_mutex_lock(&mutex_stampa);
            for(j = 0; j < 3; j++)
                mvprintw(oggetto_navicella_sf[index].y_location + j, oggetto_navicella_sf[index].x_location, "   ");
            refresh();
            pthread_mutex_unlock(&mutex_stampa);
            break;
        }
    }
}

/** Bomba **/
void *bomba(){
    oggetto_bomba.y_location = -1;
    oggetto_bomba.object_id = ID_BOMBA;

    /** Genera le coordinate della bomba **/
    do{
        usleep(BOMB_DELAY*20);
        pthread_mutex_lock(&mutex_stampa);
        attron(COLOR_PAIR(1));
        mvaddch(oggetto_bomba.y_location, oggetto_bomba.x_location, ' ');
        refresh();
        pthread_mutex_unlock(&mutex_stampa);

        oggetto_bomba.existence = false;
        while(!oggetto_bomba.existence){}

        do{
            /** La bomba cade verso il basso **/
            pthread_mutex_lock(&mutex_stampa);
            attron(COLOR_PAIR(1));
            mvaddch(oggetto_bomba.y_location, oggetto_bomba.x_location, ' ');
            refresh();
            pthread_mutex_unlock(&mutex_stampa);
            if(oggetto_bomba.existence){
                oggetto_bomba.y_location++;
                oggetto_bomba.object_id = ID_BOMBA;
            }
            pthread_mutex_lock(&mutex_stampa);
            attron(COLOR_PAIR(1));
            mvaddch(oggetto_bomba.y_location, oggetto_bomba.x_location, 'o');
            refresh();
            pthread_mutex_unlock(&mutex_stampa);

            /** La bomba esce dallo schermo **/
            if(oggetto_bomba.y_location >= DIM_Y || !oggetto_bomba.existence){
                pthread_mutex_lock(&mutex_stampa);
                attron(COLOR_PAIR(3));
                clear();
                mvprintw(oggetto_astronave.y_location, oggetto_astronave.x_location, "(=I=)");
                refresh();
                pthread_mutex_unlock(&mutex_stampa);
                oggetto_bomba.y_location = -1;
                break;
            }

            /** Delay **/
            usleep(BOMB_DELAY);

        }while(true);
    }while(true);
}

/* Disegna la navicella di primo livello */
void disegnaNPF(_Bool disegno, GameObject navicella){
    int j;

    /* Disegna la navicella */
    if(disegno){
        pthread_mutex_lock(&mutex_stampa);
        attron(COLOR_PAIR(1));      // Colore verde
        for(j = 0; j < 3; j++){
            switch(j){
                case 0:
                    mvprintw(navicella.y_location + j, navicella.x_location, "(-)");
                    break;
                case 1:
                    mvprintw(navicella.y_location + j, navicella.x_location, ")v(");
                    break;
                case 2:
                    mvprintw(navicella.y_location + j, navicella.x_location, "(-)");
                    break;
            }
        }
        refresh();
        pthread_mutex_unlock(&mutex_stampa);
    }
    /* Cancella la navicella */
    else{
        pthread_mutex_lock(&mutex_stampa);
        for(j = 0; j < 3; j++)
            mvprintw(navicella.y_location + j, navicella.x_location, "   ");
        refresh();
        pthread_mutex_unlock(&mutex_stampa);
    }
}

/* Disegna la navicella di secondo livello */
void disegnaNSF(_Bool disegno, GameObject navicella, int colore){
    int j;

    /* Disegna la navicella */
    if(disegno && navicella.existence){
        pthread_mutex_lock(&mutex_stampa);
        // Ultima vita
        if(colore <= 1)
            attron(COLOR_PAIR(2));
        // 2 vite
        else if(colore > 1)
            attron(COLOR_PAIR(1));

        for(j = 0; j < 3; j++){
            switch(j){
                case 0:
                    mvprintw(navicella.y_location + j, navicella.x_location, " v ");
                    break;
                case 1:
                    mvprintw(navicella.y_location + j, navicella.x_location, "v v");
                    break;
                case 2:
                    mvprintw(navicella.y_location + j, navicella.x_location, " v ");
                    break;
            }
        }
        refresh();
        pthread_mutex_unlock(&mutex_stampa);
    }
    /* Cancella la navicella */
    else if(navicella.existence){
        pthread_mutex_lock(&mutex_stampa);
        for(j = 0; j < 3; j++)
            mvprintw(navicella.y_location + j, navicella.x_location, "   ");
        refresh();
        pthread_mutex_unlock(&mutex_stampa);
    }
}