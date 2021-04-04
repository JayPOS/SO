#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define max_descanso 5

//Vetor que representa os garfos na mesa.
sem_t* garfos;
sem_t n_comendo;

//Variaveis da simulação
int n_filosofos; 
int tempo_comendo = 5;

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

        system("tput reset");
        
        printf("Filosofos comendo: %d\n\nMesa\n", num);

        for( i = 0 ; i < n_filosofos ; i++ ){
            int aux;
            sem_getvalue(&garfos[i],&aux);

            printf("%d ", aux);
        }

        printf("\n");

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

    sleep(tempo_comendo);

    sem_wait(&n_comendo);
}

//Funcao referente a cada filosofo
void* filosofo( void* args ){   
    int id = atoi((char*)args);
    
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
        char* id = (char*) malloc(1*sizeof(char));

        *id = (i+1) + '0';
        
        pthread_create(&newThreads[i],NULL,filosofo, id );
    }

    pthread_create(&newThreads[i],NULL,exibir_mesa, NULL );

    for( i = 0; i < n_filosofos+1; i++){
        pthread_join(newThreads[i],NULL);
    }
}

//Funcao principal
int main(int argc, char** argv){
    srand(time(NULL));
    
    n_filosofos = atoi(argv[1]);

    inicializar_mesa();
    iniciar_jantar();

    return 0;
}