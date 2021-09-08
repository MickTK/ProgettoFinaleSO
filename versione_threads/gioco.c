//
// Michele Melis 65798 - Progetto Space Invaders
//

#include "header.h"

int viteNavicellaNemicaSecondaFase[N_NAVICELLE_SF];    // Tiene traccia delle vite rimaste di ciascuna navicella

/** Area gioco **/
void area_gioco() {
    int lastCollisionSx, lastCollisionDx;  // Id dell'ultima navicella eliminata
    _Bool gameInProgress = true;    // Condizione per il while (false == game over)
    int i, j;                       // Variabili per le iterazioni
    int score = 0;                  // Punteggio
    int health_points = HP;         // Punti vita del giocatore
    _Bool won = true;               // true == il giocatore ha vinto, false == il giocatore ha perso

    /* Indica se i missili hanno già colpito una navicella (serve per evitare collisioni multiple) */
    _Bool missileSxVecchio = false;
    _Bool missileDxVecchio = false;

    // Inizializzazione vite navicelle
    for (i = 0; i < N_NAVICELLE_SF; i++) {
        viteNavicellaNemicaSecondaFase[i] = VITE_NAVICELLA;
    }

    do {
        /**
        ------------------------------------------------------------------------------
        Punteggio e vite del giocatore
        ------------------------------------------------------------------------------
        **/

        curs_set(0);
        pthread_mutex_lock(&mutex_stampa);
        attron(COLOR_PAIR(2));
        mvprintw(0, DIM_X/2-4, "SCORE: %d", score);            // Punteggio
        mvprintw(1, DIM_X/2-4, "LIVES: %d", health_points);    // Vite rimaste
        refresh();
        pthread_mutex_unlock(&mutex_stampa);

        /**
        ------------------------------------------------------------------------------
        Collisioni
        ------------------------------------------------------------------------------
        **/

        /** L'astronave viene colpita dalla bomba **/
        if(oggetto_bomba.y_location == oggetto_astronave.y_location && oggetto_bomba.existence) {
            if(oggetto_bomba.x_location >= oggetto_astronave.x_location && oggetto_bomba.x_location <= oggetto_astronave.x_location+4){
                health_points--;            // Decrementa vite giocatore
            }
        }
        // Genera le coordinate della nuova bomba
        else if(!oggetto_bomba.existence){
            oggetto_bomba = initBomba(oggetto_navicella_pf, oggetto_navicella_sf);
            oggetto_bomba.existence = true;
        }

        lastCollisionDx = -1;
        /** Controllo collisione missile con navicella  di primo livello **/
        // Collisione con missile sinistro
        for(i = 0; i < N_NAVICELLE_PF; i++){
            // Coordinata y ed esistenza
            if (oggetto_missile_sx.y_location >= oggetto_navicella_pf[i].y_location && oggetto_missile_sx.y_location <= oggetto_navicella_pf[i].y_location + 2 && oggetto_navicella_pf[i].existence) {
                // Coordinata x
                if (oggetto_missile_sx.x_location >= oggetto_navicella_pf[i].x_location &&
                    oggetto_missile_sx.x_location <= oggetto_navicella_pf[i].x_location + 2) {
                    // Nuovo missile
                    if(!missileSxVecchio && lastCollisionSx != i){
                        lastCollisionSx = i;
                        // Aggiunge i punti guadagni con l'eliminazione della navicella
                        score += PUNTI_NPF;
                        // Cambia lo stato del missile (evita collisioni multiple)
                        missileSxVecchio = true;
                        // Cambia lo stato nella navicella
                        pthread_mutex_lock(&mutex_navicella_1[i]);
                        oggetto_navicella_pf[i].existence = false;
                        pthread_mutex_unlock(&mutex_navicella_1[i]);
                        // Fa spawnare le navicelle di secondo livello
                        if(viteNavicellaNemicaSecondaFase[i] == VITE_NAVICELLA){
                            pthread_mutex_lock(&mutex_navicella_2[i]);
                            oggetto_navicella_sf[i].existence = true;
                            pthread_mutex_unlock(&mutex_navicella_2[i]);
                        }

                        // Evita di rilevare nuovamente la stessa collisione
                        i = -1;
                    }
                }
            }
        }

        // Collisione con missile destro
        for(i = 0; i < N_NAVICELLE_PF; i++){
            // Coordinata y ed esistenza
            if (oggetto_missile_dx.y_location >= oggetto_navicella_pf[i].y_location && oggetto_missile_dx.y_location <= oggetto_navicella_pf[i].y_location + 2 && oggetto_navicella_pf[i].existence) {
                // Coordinata x
                if (oggetto_missile_dx.x_location >= oggetto_navicella_pf[i].x_location &&
                    oggetto_missile_dx.x_location <= oggetto_navicella_pf[i].x_location + 2) {
                    // Nuovo missile
                    if(!missileDxVecchio && lastCollisionDx != i){
                        lastCollisionDx = i;
                        // Aggiunge i punti guadagni con l'eliminazione della navicella
                        score += PUNTI_NPF;
                        // Cambia lo stato del missile (evita collisioni multiple)
                        missileDxVecchio = true;
                        // Cambia lo stato nella navicella
                        pthread_mutex_lock(&mutex_navicella_1[i]);
                        oggetto_navicella_pf[i].existence = false;
                        pthread_mutex_unlock(&mutex_navicella_1[i]);
                        // Fa spawnare le navicelle di secondo livello
                        if(viteNavicellaNemicaSecondaFase[i] == VITE_NAVICELLA){
                            pthread_mutex_lock(&mutex_navicella_2[i]);
                            oggetto_navicella_sf[i].existence = true;
                            pthread_mutex_unlock(&mutex_navicella_2[i]);
                        }
                        // Evita di rilevare nuovamente la stessa collisione
                        i = -1;
                    }
                }
            }
        }

        /** Controllo collisione missile con navicella di secondo livello **/
        // Missile sinistro
        for(i = 0; i < N_NAVICELLE_SF; i++){
            if (oggetto_missile_sx.y_location >= oggetto_navicella_sf[i].y_location && oggetto_missile_sx.y_location <= oggetto_navicella_sf[i].y_location + 2 && oggetto_navicella_sf[i].existence){
                if (oggetto_missile_sx.x_location >= oggetto_navicella_sf[i].x_location && oggetto_missile_sx.x_location <= oggetto_navicella_sf[i].x_location + 2){
                    if(!missileSxVecchio && lastCollisionSx != 0){
                        // Aggiunge i punti guadagni con l'eliminazione della navicella
                        score += PUNTI_NSF;
                        // Cambia lo stato del missile (evita collisioni multiple)
                        missileSxVecchio = true;
                        // Cambia lo stato nella navicella
                        viteNavicellaNemicaSecondaFase[i]--;
                        // Fa spawnare le navicelle di secondo livello
                        if(viteNavicellaNemicaSecondaFase[i] <= 0){
                            pthread_mutex_lock(&mutex_navicella_2[i]);
                            oggetto_navicella_sf[i].existence = false;
                            pthread_mutex_unlock(&mutex_navicella_2[i]);
                        }
                        // Evita di rilevare nuovamente la stessa collisione
                        i = -1;
                    }
                }
            }
        }
        // Missile destro
        for(i = 0; i < N_NAVICELLE_SF; i++){
            if (oggetto_missile_dx.y_location >= oggetto_navicella_sf[i].y_location && oggetto_missile_dx.y_location <= oggetto_navicella_sf[i].y_location + 2 && oggetto_navicella_sf[i].existence){
                if (oggetto_missile_dx.x_location >= oggetto_navicella_sf[i].x_location && oggetto_missile_dx.x_location <= oggetto_navicella_sf[i].x_location + 2){
                    if(!missileDxVecchio && lastCollisionDx != i){
                        lastCollisionDx = i;
                        // Aggiunge i punti guadagni con l'eliminazione della navicella
                        score += PUNTI_NSF;
                        // Cambia lo stato del missile (evita collisioni multiple)
                        missileDxVecchio = true;
                        // Cambia lo stato nella navicella
                        viteNavicellaNemicaSecondaFase[i]--;
                        // Fa spawnare le navicelle di secondo livello
                        if(viteNavicellaNemicaSecondaFase[i] <= 0){
                            pthread_mutex_lock(&mutex_navicella_2[i]);
                            oggetto_navicella_sf[i].existence = false;
                            pthread_mutex_unlock(&mutex_navicella_2[i]);
                        }
                        // Evita di rilevare nuovamente la stessa collisione
                        i = -1;
                    }
                }
            }
        }

        // Il missile esce dallo schermo (ripristina i missili)
        if(oggetto_missile_sx.y_location == 2){
            missileSxVecchio = missileDxVecchio = false;
            lastCollisionSx = lastCollisionDx = -1;
        }

        /**
        ------------------------------------------------------------------------------
        Condizioni per il game over
        ------------------------------------------------------------------------------
        **/

        /* Le navicelle di primo livello arrivano in fondo allo schermo */
        for(i = 0; i < N_NAVICELLE_PF; i++){
            if (oggetto_navicella_pf[i].y_location >= DIM_Y - 3 && oggetto_navicella_pf[i].existence){
                gameInProgress = false;
                won = false;
            }
        }

        /* Le navicelle di secondo livello arrivano in fondo allo schermo */
        for(i = 0; i < N_NAVICELLE_SF; i++)
            if (oggetto_navicella_sf[i].y_location+2 >= DIM_Y - 1 && oggetto_navicella_sf[i].existence){
                gameInProgress = false;
                won = false;
            }

        /* L'astronave finisce le vite */
        if(health_points <= 0){
            gameInProgress = false;
            won = false;
        }

        /* Non ci sono più navicelle di secondo livello in vita */
        j = 0;
        for(i = 0; i < N_NAVICELLE_SF; i++){
            if (viteNavicellaNemicaSecondaFase[i] > 0)
                j++;
        }
        if(j == 0){
            gameInProgress = false;
            won = true;
        }
    }while(gameInProgress);

    game_over(score, won);
    getch();
}

void game_over(int score, _Bool won){
    oggetto_missile_sx.y_location = oggetto_missile_dx.y_location = -1;
    int i;
    /** I thread vengono terminati e viene terminato il programma **/
    for(i = 0; i < N_NAVICELLE_PF; i++) pthread_cancel(pt_navicella_nemica_pf[i]);
    for(i = 0; i < N_NAVICELLE_SF; i++) pthread_cancel(pt_navicella_nemica_sf[i]);
    pthread_cancel(pt_astronave);
    pthread_cancel(pt_bomba);

    clear();
    curs_set(0);
    refresh();
    // Stampa "You Win" se il giocatore vince
    if(won){
        attron(COLOR_PAIR(1));
        mvprintw(DIM_Y/2+2, DIM_X/2-10, "FINAL SCORE: %d", score);
        mvprintw(DIM_Y/2-2, DIM_X/2-20, "__  ______  __  ___      _______  __\n");
        mvprintw(DIM_Y/2-1, DIM_X/2-20, "\\ \\/ / __ \\/ / / / | /| / /  _/ |/ /\n");
        mvprintw(DIM_Y/2, DIM_X/2-20, " \\  / /_/ / /_/ /| |/ |/ // //    /\n");
        mvprintw(DIM_Y/2+1, DIM_X/2-20, " /_/\\____/\\____/ |__/|__/___/_/|_/  ");
    }
    // Stampa "You Lose" se il giocatore perde
    else{
        attron(COLOR_PAIR(2));
        mvprintw(DIM_Y/2+2, DIM_X/2-10, "FINAL SCORE: %d", score);
        mvprintw(DIM_Y/2-2, DIM_X/2-25, "  ___   __   _  _  ____  __   _  _  ____  ____ \n");
        mvprintw(DIM_Y/2-1, DIM_X/2-25, " / __) / _\\ ( \\/ )(  __)/  \\ / )( \\(  __)(  _ \\\n");
        mvprintw(DIM_Y/2, DIM_X/2-25, "( (_ \\/    \\/ \\/ \\ ) _)(  O )\\ \\/ / ) _)  )   /\n");
        mvprintw(DIM_Y/2+1, DIM_X/2-25, " \\___/\\_/\\_/\\_)(_/(____)\\__/  \\__/ (____)(__\\_)");
    }
    mvprintw(DIM_Y/2+3, DIM_X/2-12, "Press any key to close");

    refresh();
}

GameObject initBomba(GameObject *navicellaNemicaPrimaFase, GameObject *navicellaNemicaSecondaFase){
    int x;
    GameObject bomba;
    bomba.existence = true;

    /** Viene decisa casualmente la navicella che farà cadere la bomba **/
    while(true){

        x = rand()%N_NAVICELLE_PF;

        while(x < N_NAVICELLE_PF){
            if(navicellaNemicaPrimaFase[x].existence){
                /** Coordinate bomba **/
                bomba.x_location = navicellaNemicaPrimaFase[x].x_location + 1;
                bomba.y_location = navicellaNemicaPrimaFase[x].y_location + 3;
                return bomba;
            }

            if(navicellaNemicaSecondaFase[x].existence){
                /** Coordinate bomba **/
                bomba.x_location = navicellaNemicaSecondaFase[x].x_location + 1;
                bomba.y_location = navicellaNemicaSecondaFase[x].y_location + 3;
                return bomba;
            }

            x++;
        }
    }
}