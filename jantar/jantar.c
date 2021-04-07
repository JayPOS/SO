#include "jantar.h"

//Window do ncurses
WINDOW *win;

//Vetor que representa os garfos na mesa.
sem_t* garfos;
sem_t n_comendo;

//Variaveis da simulação
int n_filosofos; 
int tempo_comendo = 5;

//Vetor de estados dos filósofos
int *filosofos;

//Inicia a mesa de acordo com a quantidade de filososfos
void inicializar_mesa(){
    int i;
    
    sem_init(&n_comendo,0,0);
    garfos = (sem_t*) malloc(n_filosofos*sizeof(sem_t));

    for( i = 0; i < n_filosofos ; i++){
        sem_init(&garfos[i],0,1);
    }  
}

//Funcao para exibir a disposicao dos garfos a cada segundo
void* exibir_mesa(void* args){
    int i;

    while(1){
        int num;

        sem_getvalue(&n_comendo, &num);
        
        wprintw(win, "Filosofos comendo: %d\n", num);
        

        int i;
        for (i = 0; i < n_filosofos; i++)
        {
             if (filosofos[i])
            {
                wprintw(win, " ");
                wattron(win, COLOR_PAIR(OCCUPED));
                wprintw(win, " F ");
                wattroff(win, COLOR_PAIR(OCCUPED));
                wprintw(win, " ");
                //fwrite("Válido!\n", 1, sizeof("Válido!\n"), arq);
            }
            else
            {
                 wprintw(win, " ");
                wattron(win, COLOR_PAIR(FREE));
                wprintw(win, " F ");
                wattroff(win, COLOR_PAIR(FREE));
                wprintw(win, " ");
                //fwrite("NULL!\n", 1, sizeof("NULL!\n"), arq);
            }
        }

        wprintw(win,"\n\n\nMesa:\n");

        int old_i = i;

        for( i = 0 ; i < n_filosofos ; i++ ){
            int aux;
            sem_getvalue(&garfos[i],&aux);

            if (aux)
            {
                wprintw(win, " ");
                wattron(win, COLOR_PAIR(FREE));
                wprintw(win, " G ");
                wattroff(win, COLOR_PAIR(FREE));
                wprintw(win, " ");
            }
            else
            {
                wprintw(win, " ");
                wattron(win, COLOR_PAIR(OCCUPED));
                wprintw(win, " G ");
                wattroff(win, COLOR_PAIR(OCCUPED));
                wprintw(win, " ");
            }
        }
        wprintw(win, "\n");
        wrefresh(win);
        wclear(win);

        sleep(1);
    }    
}

//Abstracao do filosofo pensando
void pensar(int id){
    int pensar = rand() % max_descanso;

    //printf("%d - Pensarei por: %d\n", id, pensar );

    sleep(pensar);
}

//Abstracao do filosofo comendo
void comer(int id){
    //printf("%d - Comendo por: %d\n", id, tempo_comendo);
    sem_post(&n_comendo);
    filosofos[id-1] = 1;
    sleep(tempo_comendo);
    filosofos[id-1] = 0;
    sem_wait(&n_comendo);
}

//Funcao referente a cada filosofo
void* filosofo( void* args ){   
    int id = *(int *)args;
    
    while(1){
        //Pensar
        pensar(id);

        //printf("%d - Acordei\n", id);

        //Pegar o garfo direito (id - 1)
        if( sem_trywait( &garfos[id-1] ) != 0 ) continue;
        //Pegar o garfo esquerdo ( id )
        if( sem_trywait( &garfos[id%n_filosofos] ) != 0 ){
            //coloca o garfo direito de volta na mesa
            sem_post( &garfos[id-1] );
            continue;
        }

        //Comer
        comer(id);

        //Colocar os garfos de volta na mesa
        //Direito
        sem_post( &garfos[id-1] );
        //Esquerdo
        sem_post( &garfos[id%n_filosofos] );
    }
}

//Inicializacao dos filosofos e execucao do jantar
void iniciar_jantar(){
    int i;
    pthread_t newThreads[n_filosofos + 1];
       
    for( i = 0; i < n_filosofos; i++){
        int* id = (int*) malloc(1*sizeof(int));

        *id = (i+1);
        
        pthread_create(&newThreads[i],NULL,filosofo, id );
    }

    pthread_create(&newThreads[i],NULL,exibir_mesa, NULL );

    for( i = 0; i < n_filosofos+1; i++){
        pthread_join(newThreads[i],NULL);
    }
}

//Funcao principal
int main(int argc, char** argv){

    //inicializando
    initscr();
    noecho();
    win = newwin(100, 100, 0, 0);

    if (!has_colors)
    {
        wprintw(win, "Terminal dont have colors!\n");
        return -1;
    }
    else
    {
        start_color();
    }

    // colors:
    init_pair(FREE, COLOR_WHITE, COLOR_BLUE);
    init_pair(OCCUPED, COLOR_WHITE, COLOR_RED);
    refresh();

    srand(time(NULL));
    
    n_filosofos = atoi(argv[1]);

    filosofos = (int *)calloc(n_filosofos, sizeof(int));

    inicializar_mesa();
    iniciar_jantar();

    endwin();

    return 0;
}