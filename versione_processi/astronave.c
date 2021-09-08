//
// Michele Melis 65798 - Progetto Space Invaders
//

#include "header.h"

/** Astronave giocatore **/
void astronave(int pipeout){
    /** Astronave **/
    GameObject astronave;
    astronave.object_id = ID_ASTRONAVE;
    astronave.x_location = DIM_X / 2 - strlen("(=I=)") / 2; // L'astronave viene posizionata al centro dell'asse x
    astronave.y_location = DIM_Y - 1;                       // L'astronave viene posizionata in fondo allo schermo
    astronave.existence = true;

    /** Missili **/
    int pid_missile_sinistro, pid_missile_destro;

    write(pipeout, &astronave, sizeof(astronave));
    while(true) {
        switch(getch()) {

            /** L'astronave si muove verso sinistra **/
            case KEY_LEFT:
                if (astronave.x_location > 0)
                    astronave.x_location-=1;
                break;

            /** L'astronave si muove verso destra **/
            case KEY_RIGHT:
                if (astronave.x_location + strlen("(=I=)") < DIM_X - 1)
                    astronave.x_location+=1;
                break;

            /** Vengono sparati i missili **/
            case ' ':
                beep();

                /** Generazione processi dei missili **/
                pid_missile_sinistro = fork();
                switch (pid_missile_sinistro) {
                    case -1: _exit(-1);
                    case 0:
                        missileSx(pipeout, astronave.x_location+2);
                }
                pid_missile_destro = fork();
                switch (pid_missile_destro) {
                    case -1: _exit(-1);
                    case 0:
                        missileDx(pipeout, astronave.x_location+2);
                }

                break;
        }
        write(pipeout, &astronave, sizeof(astronave));
    }
}

/** Missile sinistro **/
void missileSx(int pipeout, int startPosition){
    GameObject missile;

    /** Inizializzazione missile **/
    missile.object_id = ID_MISSILE_SX;
    missile.existence = true;
    missile.y_location = DIM_Y-2;
    missile.x_location = startPosition;

    /** Prima posizione **/
    write(pipeout, &missile, sizeof(missile));
    usleep(MISSILE_DELAY);

    /** Vita del missile **/
    while(missile.y_location >= 0){
        missile.y_location--;
        missile.x_location--;
        write(pipeout, &missile, sizeof(missile));
        usleep(MISSILE_DELAY);
    }
    _exit(0);
}

/** Missile destro **/
void missileDx(int pipeout, int startPosition){
    GameObject missile;

    missile.object_id = ID_MISSILE_DX;
    missile.existence = true;
    missile.y_location = DIM_Y-2;
    missile.x_location = startPosition;

    write(pipeout, &missile, sizeof(missile));
    usleep(MISSILE_DELAY);

    while(missile.y_location >= 0){
        missile.y_location--;
        missile.x_location++;
        write(pipeout, &missile, sizeof(missile));
        usleep(MISSILE_DELAY);
    }
    _exit(0);
}