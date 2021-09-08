//
// Michele Melis 65798 - Progetto Space Invaders
//

#include "header.h"

pthread_t pt_astronave, pt_navicella_nemica_pf[N_NAVICELLE_PF], pt_navicella_nemica_sf[N_NAVICELLE_SF], pt_bomba;
pthread_mutex_t mutex_navicella_1[N_NAVICELLE_PF], mutex_navicella_2[N_NAVICELLE_SF];

int main() {
    /** Inizializzazione schermo e colori **/
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);     // Colore navicelle e bomba
    init_pair(2, COLOR_RED, COLOR_BLACK);       // Colore navicelle colpite
    init_pair(3, COLOR_BLUE, COLOR_BLACK);      // Colore astronave e missili
    int i;

    srand((int)time(0));    // Seme di generazione casuale
    schermata_iniziale();   // Schermata visualizzata prima di iniziare il gioco

    for(i = 0; i < N_NAVICELLE_PF; i++){
        pthread_mutex_init(&mutex_navicella_1[i], NULL);
    }
    for(i = 0; i < N_NAVICELLE_SF; i++){
        pthread_mutex_init(&mutex_navicella_2[i], NULL);
    }

    /** Thread astronave **/
    pthread_create(&pt_astronave,NULL,&astronave,NULL);

    GameObject genericObject, *o;
    /** Thread navicelle di prima fase **/
    const int std_y = 2; // Posizione y delle prime 2 navicelle
    for(i = 0; i < N_NAVICELLE_PF; i++){

        /** Navicelle righe dispari **/
        if(i%2 == 1)     // Dispari -> colonna sinistra
            genericObject.x_location = 0;
        else genericObject.x_location = DIM_X-3;

        /** Navicelle righe pari **/
        if((i/2)%2 == 1){
            if(i%2 == 1) // Dispari -> colonna sinistra
                genericObject.x_location = DIM_X/2-3;
            else genericObject.x_location = DIM_X/2;
        }

        genericObject.y_location = std_y + 4*(i/2);       // Posizione y
        genericObject.object_id = ID_PRIMA_NAVICELLA+i;   // Assegnazione id
        genericObject.existence = true;                   // Stato navicella

        /** Creazione thread navicella di primo livello **/
        o = &genericObject;
        pthread_create(&pt_navicella_nemica_pf[i],NULL,&navicella_pf, (void *) o);
        usleep(LOAD_TIME);
    }

    /** Thread navicelle di seconda fase **/
    for(i = 0; i < N_NAVICELLE_SF; i++){

        /** Navicelle righe dispari **/
        if(i%2 == 1) // Dispari -> colonna sinistra
            genericObject.x_location = 0;
        else genericObject.x_location = DIM_X-3;

        /** Navicelle righe pari **/
        if((i/2)%2 == 1){
            if(i%2 == 1) // Dispari -> colonna sinistra
                genericObject.x_location = DIM_X/2-3;
            else genericObject.x_location = DIM_X/2;
        }

        genericObject.y_location = std_y + 4*(i/2);         // Posizione y
        genericObject.object_id = ID_PRIMA_NAVICELLA_SF+i;  // Assegnazione id
        genericObject.existence = false;                    // Stato navicella

        /** Creazione thread navicella di secondo livello **/
        o = &genericObject;
        pthread_create(&pt_navicella_nemica_sf[i],NULL,&navicella_sf, (void *) o);
        usleep(LOAD_TIME);
    }

    /** Thread bomba **/
    pthread_create(&pt_bomba,NULL,&bomba,NULL);

    area_gioco();
    endwin();
    return 0;
}
void schermata_iniziale(){
    attron(COLOR_PAIR(1));

    mvprintw(DIM_Y/2-2, DIM_X/2-29, " ___ ___  _   ___ ___   ___ _  ___   ___   ___  ___ ___  ___ \n");
    mvprintw(DIM_Y/2-1, DIM_X/2-29, "/ __| _ \\/_\\ / __| __| |_ _| \\| \\ \\ / /_\\ |   \\| __| _ \\/ __|\n");
    mvprintw(DIM_Y/2, DIM_X/2-29, "\\__ \\  _/ _ \\ (__| _|   | || .` |\\ V / _ \\| |) | _||   /\\__ \\\n");
    mvprintw(DIM_Y/2+1, DIM_X/2-29, "|___/_|/_/ \\_\\___|___| |___|_|\\_| \\_/_/ \\_\\___/|___|_|_\\|___/");
    mvprintw(DIM_Y/2+2, DIM_X/2-10, "Press any key to start");

    refresh();

    getch();
    beep();
    clear();
}