//
// Michele Melis 65798 - Progetto Space Invaders
//

#include "header.h"

/** Navicella prima fase **/
void navicella_pf(int pipeout, GameObject object){
    GameObject navicella = object;
    _Bool tipo_l;       // Indica se la navicella fa parte delal colonna di sinistra
    _Bool right_mov;    // Indica se la navicella deve muoversi verso destra

    /* Se la navicella viene posizionata a sinistra dello schermo farà parte della
     * colonna di sinistra, altrimenti di quella di destra
     */
    if(navicella.x_location < DIM_X/2) tipo_l = true;
    else tipo_l = false;

    write(pipeout, &navicella, sizeof(navicella));
    while(true){

        /** Calcolo direzione **/
        // La navicella si muove dal bordo sinistro al centro dello schermo
        if(tipo_l){
            if(navicella.x_location <= 0) right_mov = true;
            else if(navicella.x_location >= DIM_X/2 -3) right_mov = false;
        }
        // La navicella si muove dal bordo destro al centro dello schermo
        else {
            if(navicella.x_location <= DIM_X/2) right_mov = true;
            else if(navicella.x_location+3 >= DIM_X) right_mov = false;
        }

        /** Si muove verso il basso **/
        if(tipo_l){
            if(navicella.x_location <= 0 || navicella.x_location >= DIM_X/2 -3){
                navicella.y_location++;
                write(pipeout, &navicella, sizeof(navicella));
                usleep(NAVICELLE_NEMICHE_DELAY);
            }
        }
        else {
            if(navicella.x_location <= DIM_X/2 || navicella.x_location+3 >= DIM_X){
                navicella.y_location++;
                write(pipeout, &navicella, sizeof(navicella));
                usleep(NAVICELLE_NEMICHE_DELAY);
            }
        }

        /** Movimento laterale **/
        if(right_mov) navicella.x_location++;
        else navicella.x_location--;

        write(pipeout, &navicella, sizeof(navicella));

        /** Delay **/
        usleep(NAVICELLE_NEMICHE_DELAY);
    }
}

/** Navicella seconda fase **/
void navicella_sf(int pipeout, GameObject object){
    GameObject navicella = object;
    _Bool tipo_l, right_mov;   // Indica la colonna in cui si trova la navicella

    if(navicella.x_location < DIM_X/2) tipo_l = true;
    else tipo_l = false;

    write(pipeout, &navicella, sizeof(navicella));
    while(true){

        /** Calcolo direzione **/
        if(tipo_l){
            if(navicella.x_location <= 0) right_mov = true;
            else if(navicella.x_location >= DIM_X/2 -3) right_mov = false;
        }
        else {
            if(navicella.x_location <= DIM_X/2) right_mov = true;
            else if(navicella.x_location+3 >= DIM_X) right_mov = false;
        }

        /** Si muove verso il basso **/
        if(tipo_l){
            if(navicella.x_location <= 0 || navicella.x_location >= DIM_X/2 -3){
                navicella.y_location++;
                write(pipeout, &navicella, sizeof(navicella));
                usleep(NAVICELLE_NEMICHE_DELAY);
            }
        }
        else {
            if(navicella.x_location <= DIM_X/2 || navicella.x_location+3 >= DIM_X){
                navicella.y_location++;
                write(pipeout, &navicella, sizeof(navicella));
                usleep(NAVICELLE_NEMICHE_DELAY);
            }
        }

        /** Movimento laterale **/
        if(right_mov) navicella.x_location++;
        else navicella.x_location--;

        write(pipeout, &navicella, sizeof(navicella));

        /** Delay **/
        usleep(NAVICELLE_NEMICHE_DELAY);
    }
}

/** Bomba **/
void bomba(int pipeout, int pipein){
    GameObject bomba;
    bomba.y_location = -1;
    bomba.object_id = ID_BOMBA;

    /** Genera le coordinate della bomba **/
    do{
        bomba.existence = false;
        usleep(BOMB_DELAY*20);
        write(pipeout, &bomba, sizeof(bomba));
        read(pipein, &bomba, sizeof(bomba));

        do{
            /** La bomba cade verso il basso **/
            if(bomba.existence){
                bomba.y_location++;
                bomba.object_id = ID_BOMBA;
            }
            write(pipeout, &bomba, sizeof(bomba));
            usleep(BOMB_DELAY);
            /** La bomba esce dallo schermo **/
            if(bomba.y_location >= DIM_Y){
                bomba.y_location = -1;
                break;
            }
        }while(true);
    }while(true);
}