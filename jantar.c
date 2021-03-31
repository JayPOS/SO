#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define max_descanso 5

int* garfos;
int n_filosofos; 

int tempo_comendo = 5;

//Inicia a mesa de acordo com a quantidade de filososfos
void inicializar_mesa(){
    int i;
    
    garfos = (int*) malloc(n_filosofos*sizeof(int));

    for( i = 0 ; i < n_filosofos ; i++){
        garfos[i] = 1;
    } 
       
}

//Funcao para exibir a disposicao dos garfos a cada segundo
void* exibir_mesa(void* args){
    int i;

    while(1){
        system("tput reset");

        for( i = 0 ; i < n_filosofos ; i++ ){
            printf("%d ", garfos[i]);
        }

        printf("\n");

        sleep(1);
    }    
}

//Abstracao do filoso pensando
void pensar(){
    int pensar = rand() % max_descanso;

    //printf("Pensarei por: %d\n", pensar );

    sleep(pensar);
}

//Abstracao do filosofo comendo
void comer(){
    sleep(tempo_comendo);
}

//Funcao referente a cada filosofo
void* filosofo( void* args ){   
    while(1){
        //Pensar
        pensar();

        //printf("Acordei %c\n", *(char*)args );

        //Pegar o garfo esquerdo
        //Pegar o garfo direito
        //Comer
        comer();
        //Colocar um o gafor na mesa
        //Colocar o outro garfo na mesa
    }
}

//Funcao principal
int main(int argc, char** argv){
    srand(time(NULL));
    
    n_filosofos = atoi(argv[1]);

    inicializar_mesa();

    pthread_t newThreads[n_filosofos + 1];
   
    int i;
    
    for( i = 0; i < n_filosofos; i++){
        char* id = (char*) malloc(1*sizeof(char));

        *id = i + '0';
        
        pthread_create(&newThreads[i],NULL,filosofo, id );
    }

    pthread_create(&newThreads[i],NULL,exibir_mesa, NULL );

    for( i = 0; i < n_filosofos+1; i++){
        pthread_join(newThreads[i],NULL);
    }

    return 0;
}