//
// Michele Melis 65798 - Progetto Space Invaders
//

#include "header.h"

int main() {
    int filedes[2], fd_bomba[2], i;  // Pipe e variabile per le iterazioni
    int pid_astronave, pid_navicelle_pf[N_NAVICELLE_PF], pid_navicelle_sf[N_NAVICELLE_SF], pid_bomba; // Process id

    /** Inizializzazione schermo e colori **/
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);     // Colore navicelle e bomba
    init_pair(2, COLOR_RED, COLOR_BLACK);       // Colore navicelle colpite
    init_pair(3, COLOR_BLUE, COLOR_BLACK);      // Colore astronave e missili

    srand((int)time(0));    // Seme di generazione casuale

    /** Errore nella creazione delle pipe **/
    if(pipe(filedes) == -1) _exit(1);   // Pipe per comunicare al processo padre la posizione degli oggetti
    if(pipe(fd_bomba) == -1) _exit(1);  // Pipe per comunicare al processo bomba la posizione in cui rilasciare la bomba

    schermata_iniziale();   // Schermata visualizzata prima di iniziare il gioco

    /** Processi navicelle di prima fase **/
    GameObject obj;
    const int std_y = 2; // Posizione y delle prime 2 navicelle
    for(i = 0; i < N_NAVICELLE_PF; i++){

        /** Navicelle righe dispari **/
        if(i%2 == 1)     // Dispari -> colonna sinistra
            obj.x_location = 0;
        else obj.x_location = DIM_X-3;

        /** Navicelle righe pari **/
        if((i/2)%2 == 1){
            if(i%2 == 1) // Dispari -> colonna sinistra
                obj.x_location = DIM_X/2-3;
            else obj.x_location = DIM_X/2;
        }

        obj.y_location = std_y + 4*(i/2);       // Posizione y
        obj.object_id = ID_PRIMA_NAVICELLA+i;   // Assegnazione id
        obj.existence = true;                   // Stato navicella

        /** Creazione processo navicella di primo livello **/
        pid_navicelle_pf[i] = fork();
        switch (pid_navicelle_pf[i]) {
            case -1: _exit(-1);
            case 0:
                close(filedes[0]);
                navicella_pf(filedes[1], obj);
        }
    }

    /** Processi navicelle di seconda fase **/
    for(i = 0; i < N_NAVICELLE_SF; i++){

        /** Navicelle righe dispari **/
        if(i%2 == 1) // Dispari -> colonna sinistra
            obj.x_location = 0;
        else obj.x_location = DIM_X-3;

        /** Navicelle righe pari **/
        if((i/2)%2 == 1){
            if(i%2 == 1) // Dispari -> colonna sinistra
                obj.x_location = DIM_X/2-3;
            else obj.x_location = DIM_X/2;
        }

        obj.y_location = std_y + 4*(i/2);         // Posizione y
        obj.object_id = ID_PRIMA_NAVICELLA_SF+i;  // Assegnazione id
        obj.existence = true;                     // Stato navicella

        /** Creazione processo navicella di secondo livello **/
        pid_navicelle_sf[i] = fork();
        switch (pid_navicelle_sf[i]) {
            case -1: _exit(-1);
            case 0:
                close(filedes[0]);
                navicella_sf(filedes[1], obj);
        }
    }

    /** Processo astronave **/
    pid_astronave=fork();
    switch(pid_astronave){
        case -1: _exit(2);
        case 0:
            close(filedes[0]);
            astronave(filedes[1]);
        default:

            /** Processo bomba **/
            pid_bomba=fork();
            switch(pid_bomba) {
                case 0:
                    close(filedes[0]);
                    close(fd_bomba[1]);
                    bomba(filedes[1], fd_bomba[0]);
                default:

                    /** Processo padre **/
                    close(filedes[1]);
                    area_gioco(filedes[0], fd_bomba[1]);
            }
    }

    /** I processi vengono uccisi e viene terminato il programma **/
    for(i = 0; i < N_NAVICELLE_PF; i++) kill(pid_navicelle_pf[i], 1);
    for(i = 0; i < N_NAVICELLE_SF; i++) kill(pid_navicelle_sf[i], 1);
    kill(pid_astronave, 1);
    kill(pid_bomba, 1);
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

    attron(COLOR_PAIR(2));
    mvprintw(0, DIM_X/2-4, "SCORE:");
    mvprintw(1, DIM_X/2-4, "LIVES:");
    refresh();
}