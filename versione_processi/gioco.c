//
// Michele Melis 65798 - Progetto Space Invaders
//

#include "header.h"

/** Area gioco **/
void area_gioco(int pipein, int pipeout_bomba) {
    GameObject valoreLetto;         // Variabile utilizzata come contenitore per la pipe
    int lastCollisionSx, lastCollisionDx;   // Id dell'ultima navicella eliminata
    _Bool gameInProgress = true;    // Condizione per il while (false == game over)
    int i, j;                       // Variabili di flag
    int score = 0;                  // Punteggio
    int health_points = HP;         // Punti vita del giocatore
    _Bool won = true;               // true == il giocatore ha vinto, false == il giocatore ha perso

    /* Inizializzazione astronave (giocatore) */
    GameObject astronave, missileSinistro, missileDestro;
    astronave.x_location = astronave.y_location = -1;
    missileSinistro.x_location = missileSinistro.y_location = -1;
    missileDestro.x_location = missileDestro.y_location = -1;
    // Un missile diventa vecchio appena colpisce una navicella (serve per evitare collisioni multiple di più navicelle)
    _Bool missileSxVecchio = false;
    _Bool missileDxVecchio = false;

    /* Inizializzazione navicelle nemiche prima fase */
    GameObject navicellaNemicaPrimaFase[N_NAVICELLE_PF];
    _Bool npm[N_NAVICELLE_PF]; // Cancella per l'ultima volte la navicella di primo livello
    for (i = 0; i < N_NAVICELLE_PF; i++) {
        navicellaNemicaPrimaFase[i].y_location = -1;
        navicellaNemicaPrimaFase[i].existence = npm[i] = true;
    }

    /* Inizializzazione navicelle nemiche seconda fase */
    GameObject navicellaNemicaSecondaFase[N_NAVICELLE_SF];
    int viteNavicellaNemicaSecondaFase[N_NAVICELLE_SF];    // Tiene traccia delle vite rimaste di ciascuna navicella
    for (i = 0; i < N_NAVICELLE_SF; i++) {
        navicellaNemicaSecondaFase[i].y_location = -1;
        navicellaNemicaSecondaFase[i].existence = false;
        viteNavicellaNemicaSecondaFase[i] = VITE_NAVICELLA;
    }

    /* Inizializzazione bomba */
    GameObject bomba;
    bomba.existence = false;

    do {
        /**
        ------------------------------------------------------------------------------
        Stampa a video l'oggetto
        ------------------------------------------------------------------------------
        **/

        /** Legge l'oggetto dalla pipe **/
        read(pipein, &valoreLetto, sizeof(valoreLetto));

        /** Gestione stampa astronave **/
        if (valoreLetto.object_id == ID_ASTRONAVE) {
            // Cancella la vecchia posizione
            if (astronave.y_location >= 0)
                mvprintw(astronave.y_location, astronave.x_location, "     ");
            astronave = valoreLetto;
            attron(COLOR_PAIR(3));
            mvprintw(valoreLetto.y_location, valoreLetto.x_location, "(=I=)");  // Sprite astronave
        }

        /** Gestione stampa missile sinistro **/
        else if (valoreLetto.object_id == ID_MISSILE_SX) {
            // Cancella la vecchia posizione
            if (missileSinistro.y_location >= 0)
                mvprintw(missileSinistro.y_location, missileSinistro.x_location, " ");
            missileSinistro = valoreLetto;
            attron(COLOR_PAIR(3));
            mvprintw(valoreLetto.y_location, valoreLetto.x_location, "*");  // Sprite missile
        }

        /** Gestione stampa missile destro **/
        else if (valoreLetto.object_id == ID_MISSILE_DX) {
            // Cancella la vecchia posizione
            if (missileDestro.y_location >= 0)
                mvprintw(missileDestro.y_location, missileDestro.x_location, " ");
            missileDestro = valoreLetto;
            attron(COLOR_PAIR(3));
            mvprintw(valoreLetto.y_location, valoreLetto.x_location, "*");  // Sprite missile
        }

        /** Gestione stampa bomba **/
        else if (valoreLetto.object_id == ID_BOMBA) {
            if (bomba.y_location >= 0 && bomba.existence){
                mvprintw(bomba.y_location, bomba.x_location, " ");  // Cancella la vecchia posizione della bomba
                attron(COLOR_PAIR(1));
                mvprintw(valoreLetto.y_location, valoreLetto.x_location, "o");  // Sprite bomba
                bomba = valoreLetto;
            }
            /* Se la bomba non esiste viene mandata una richiesta al processo della bomba
             * per la generazione di quest'ultima
             */
            if(!valoreLetto.existence){
                bomba = initBomba(navicellaNemicaPrimaFase, navicellaNemicaSecondaFase);
                bomba.existence = true;
                write(pipeout_bomba, &bomba, sizeof(bomba));
            }

        }

        /** Gestione stampa navicelle di secondo livello **/
        else if (valoreLetto.object_id >= ID_PRIMA_NAVICELLA_SF && valoreLetto.object_id < N_NAVICELLE_SF + ID_PRIMA_NAVICELLA_SF) {

            i = valoreLetto.object_id - ID_PRIMA_NAVICELLA_SF;      // Recupero indice navicella

            // Cancella la vecchia posizione delle navicelle
            for(j = 0; j < 3; j++)
                mvprintw(navicellaNemicaSecondaFase[i].y_location + j, navicellaNemicaSecondaFase[i].x_location, "   ");

            valoreLetto.existence = navicellaNemicaSecondaFase[i].existence; // Valore esistenza navicella

            /* Ridisegna le navicelle (verdi se hanno ancora tutte le vite, rosse se hanno una vita sola) */
            if (valoreLetto.y_location >= 0 && navicellaNemicaSecondaFase[i].existence){
                if(viteNavicellaNemicaSecondaFase[i] < VITE_NAVICELLA){
                    attron(COLOR_PAIR(2));  // Colore rosso
                    navicellaNemicaSecondaFase[i] = valoreLetto;
                }
                else{
                    attron(COLOR_PAIR(1));  // Colore verde
                    navicellaNemicaSecondaFase[i] = valoreLetto;
                }
                /* Sprite navicelle */
                for(j = 0; j < 3; j++){
                    switch(j){
                        case 0:
                            mvprintw(valoreLetto.y_location + j, valoreLetto.x_location, " v ");
                            break;
                        case 1:
                            mvprintw(valoreLetto.y_location + j, valoreLetto.x_location, "v v");
                            break;
                        case 2:
                            mvprintw(valoreLetto.y_location + j, valoreLetto.x_location, " v ");
                            break;
                    }
                }
                attron(COLOR_PAIR(1));
            }
        }

        /** Gestione stampa navicella nemica di primo livello **/
        else {
            /* Cancella la vecchia posizione delle navicelle */
            if (valoreLetto.object_id >= ID_PRIMA_NAVICELLA && valoreLetto.object_id < N_NAVICELLE_PF + ID_PRIMA_NAVICELLA) {
                i = valoreLetto.object_id - ID_PRIMA_NAVICELLA;    // Recupero indice navicella

                // Cancella la vecchia posizione delle navicelle
                if (navicellaNemicaPrimaFase[i].y_location >= 0 && navicellaNemicaPrimaFase[i].existence){
                    for(j = 0; j < 3; j++)
                        mvprintw(navicellaNemicaPrimaFase[i].y_location + j, navicellaNemicaPrimaFase[i].x_location, "   ");
                }
                else if(navicellaNemicaPrimaFase[i].y_location >= 0 && npm[i]){
                    for(j = 0; j < 3; j++)
                        mvprintw(navicellaNemicaPrimaFase[i].y_location + j, navicellaNemicaPrimaFase[i].x_location, "   ");
                    npm[i] = false;
                }
                valoreLetto.existence = navicellaNemicaPrimaFase[i].existence; // Valore esistenza navicella
                navicellaNemicaPrimaFase[i] = valoreLetto;

                /* Disegna la navicella */
                if(valoreLetto.existence){
                    attron(COLOR_PAIR(1));      // Colore verde
                    for(j = 0; j < 3; j++){
                        switch(j){
                            case 0:
                                mvprintw(valoreLetto.y_location + j, valoreLetto.x_location, "(-)");
                                break;
                            case 1:
                                mvprintw(valoreLetto.y_location + j, valoreLetto.x_location, ")v(");
                                break;
                            case 2:
                                mvprintw(valoreLetto.y_location + j, valoreLetto.x_location, "(-)");
                                break;
                        }
                    }
                }
            }
        }

        /**
        ------------------------------------------------------------------------------
        Punteggio e vite del giocatore
        ------------------------------------------------------------------------------
        **/

        curs_set(0);
        refresh();
        attron(COLOR_PAIR(2));
        mvprintw(0, DIM_X/2-4, "SCORE: %d", score);            // Punteggio
        mvprintw(1, DIM_X/2-4, "LIVES: %d", health_points);    // Vite rimast
        refresh();

        /**
        ------------------------------------------------------------------------------
        Collisioni
        ------------------------------------------------------------------------------
        **/

        /* L'astronave viene colpita dalla bomba */
        if(bomba.y_location == astronave.y_location && bomba.existence) {
            if(bomba.x_location >= astronave.x_location && bomba.x_location <= astronave.x_location+4){
                health_points--;            // Decrementa vite giocatore
                bomba.existence = false;    // Evita collisioni multiple
            }
        }

        /** Controllo collisione missile con navicella  di primo livello **/
        // Collisione con missile sinistro
        if(valoreLetto.object_id == missileSinistro.object_id)
            for(i = 0; i < N_NAVICELLE_PF; i++){
                // Coordinata y ed esistenza
                if (missileSinistro.y_location >= navicellaNemicaPrimaFase[i].y_location && missileSinistro.y_location <= navicellaNemicaPrimaFase[i].y_location + 2 && navicellaNemicaPrimaFase[i].existence) {
                    // Coordinata x
                    if (missileSinistro.x_location >= navicellaNemicaPrimaFase[i].x_location &&
                            missileSinistro.x_location <= navicellaNemicaPrimaFase[i].x_location + 2) {
                        // Nuovo missile
                        if(!missileSxVecchio){
                            lastCollisionSx = i;
                            // Aggiunge i punti guadagni con l'eliminazione della navicella
                            score += PUNTI_NPF;
                            // Cambia lo stato del missile (evita collisioni multiple)
                            missileSxVecchio = true;
                            // Cambia lo stato nella navicella
                            navicellaNemicaPrimaFase[i].existence = false;
                            // Fa spawnare le navicelle di secondo livello
                            if(viteNavicellaNemicaSecondaFase[i] == VITE_NAVICELLA)
                                navicellaNemicaSecondaFase[i].existence = true;

                            // Evita di rilevare nuovamente la stessa collisione
                            i = -1;
                        }
                    }
                }
            }

        // Collisione con missile destro
        if(valoreLetto.object_id == missileDestro.object_id)
            for(i = 0; i < N_NAVICELLE_PF; i++){
                // Coordinata y ed esistenza
                if (missileDestro.y_location >= navicellaNemicaPrimaFase[i].y_location && missileDestro.y_location <= navicellaNemicaPrimaFase[i].y_location + 2 && navicellaNemicaPrimaFase[i].existence) {
                    // Coordinata x
                    if (missileDestro.x_location >= navicellaNemicaPrimaFase[i].x_location &&
                            missileDestro.x_location <= navicellaNemicaPrimaFase[i].x_location + 2) {
                        // Nuovo missile
                        if(!missileDxVecchio){
                            lastCollisionDx = i;
                            // Aggiunge i punti guadagni con l'eliminazione della navicella
                            score += PUNTI_NPF;
                            // Cambia lo stato del missile (evita collisioni multiple)
                            missileDxVecchio = true;
                            // Cambia lo stato nella navicella
                            navicellaNemicaPrimaFase[i].existence = false;
                            // Fa spawnare le navicelle di secondo livello
                            if(viteNavicellaNemicaSecondaFase[i] == VITE_NAVICELLA)
                                navicellaNemicaSecondaFase[i].existence = true;
                            // Evita di rilevare nuovamente la stessa collisione
                            i = -1;
                        }
                    }
                }
            }

        /** Controllo collisione missile con navicella di secondo livello **/
        // Missile sinistro
        if(valoreLetto.object_id == missileSinistro.object_id)
            for(i = 0; i < N_NAVICELLE_SF; i++){
                if (missileSinistro.y_location >= navicellaNemicaSecondaFase[i].y_location && missileSinistro.y_location <= navicellaNemicaSecondaFase[i].y_location + 2 && navicellaNemicaSecondaFase[i].existence){
                    if (missileSinistro.x_location >= navicellaNemicaSecondaFase[i].x_location && missileSinistro.x_location <= navicellaNemicaSecondaFase[i].x_location + 2){
                        if(!missileSxVecchio && lastCollisionSx != i){
                            lastCollisionSx = i;
                            // Aggiunge i punti guadagni con l'eliminazione della navicella
                            score += PUNTI_NSF;
                            // Cambia lo stato del missile (evita collisioni multiple)
                            missileSxVecchio = true;
                            // Cambia lo stato nella navicella
                            viteNavicellaNemicaSecondaFase[i]--;
                            // Elimina la navicella di secondo livello
                            if(viteNavicellaNemicaSecondaFase[i] <= 0)
                                navicellaNemicaSecondaFase[i].existence = false;
                            // Evita di rilevare nuovamente la stessa collisione
                            i = -1;
                        }
                    }
                }
            }
        // Missile destro
        if(valoreLetto.object_id == missileDestro.object_id)
            for(i = 0; i < N_NAVICELLE_SF; i++){
                if (missileDestro.y_location >= navicellaNemicaSecondaFase[i].y_location && missileDestro.y_location <= navicellaNemicaSecondaFase[i].y_location + 2 && navicellaNemicaSecondaFase[i].existence){
                    if (missileDestro.x_location >= navicellaNemicaSecondaFase[i].x_location && missileDestro.x_location <= navicellaNemicaSecondaFase[i].x_location + 2){
                        if(!missileDxVecchio && lastCollisionDx != i){
                            lastCollisionDx = i;
                            // Aggiunge i punti guadagni con l'eliminazione della navicella
                            score += PUNTI_NSF;
                            // Cambia lo stato del missile (evita collisioni multiple)
                            missileDxVecchio = true;
                            // Cambia lo stato nella navicella
                            viteNavicellaNemicaSecondaFase[i]--;
                            // Elimina la navicella di secondo livello
                            if(viteNavicellaNemicaSecondaFase[i] <= 0)
                                navicellaNemicaSecondaFase[i].existence = false;
                            // Evita di rilevare nuovamente la stessa collisione
                            i = -1;
                        }
                    }
                }
            }


        /* Ripristina lo stato dei missili */
        if((valoreLetto.object_id == ID_MISSILE_DX || valoreLetto.object_id == ID_MISSILE_SX) && valoreLetto.y_location == 3){
            missileSxVecchio = missileDxVecchio = false;
            lastCollisionSx = lastCollisionDx = -1;
        }

        /**
        ------------------------------------------------------------------------------
        Condizioni per il game over
        ------------------------------------------------------------------------------
        **/

        /* Le navicelle di primo livello arrivano in fondo allo schermo */
        if (valoreLetto.object_id >= ID_PRIMA_NAVICELLA && valoreLetto.object_id < N_NAVICELLE_PF + ID_PRIMA_NAVICELLA)
            if (valoreLetto.y_location >= DIM_Y - 3 && valoreLetto.existence){
                gameInProgress = false;
                won = false;
            }

        /* Le navicelle di secondo livello arrivano in fondo allo schermo */
        if (valoreLetto.object_id >= ID_PRIMA_NAVICELLA_SF && valoreLetto.object_id < N_NAVICELLE_SF + ID_PRIMA_NAVICELLA_SF)
            if (valoreLetto.y_location+2 >= DIM_Y - 1 && valoreLetto.existence){
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
            if (viteNavicellaNemicaSecondaFase[i] == 0)
                j++;
        }
        if(j == N_NAVICELLE_SF){
            gameInProgress = false;
            won = true;
        }

    }while(gameInProgress);

    game_over(score, won);
    getch();
}

void game_over(int score, _Bool won){
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

    /** Viene decisa casualmente la navicella che farà cadere la bomba **/
    while(1){
        x = rand()%N_NAVICELLE_PF;
        if(navicellaNemicaPrimaFase[x].existence){
            /** Coordinate bomba **/
            bomba.x_location = navicellaNemicaPrimaFase[x].x_location + 1;
            bomba.y_location = navicellaNemicaPrimaFase[x].y_location + 3;
            return bomba;
        }
        if(navicellaNemicaSecondaFase[x].existence){
            /** Coordinate bomba **/
            bomba.x_location = navicellaNemicaSecondaFase[x].x_location+1;
            bomba.y_location = navicellaNemicaSecondaFase[x].y_location+2;
            return bomba;
        }
    }
}