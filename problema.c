#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 100

typedef struct produto
{
    int id;
    //info produto
} Produto;

//As variaveis sao globais
Produto* produtos[MAX] = {NULL};
//

int entrada = 0; // Variavel que demarca o local a ser inserido
int saida = 0; // Vatiavel que demarca o local a ser removido

int Vazio = MAX;
int Cheio = 0;

Produto *criaProduto(/*info produto*/);
int removeProduto(); // retorna id do produto
void* produtor();
void* consumidor();

void* exibir(){
    sleep(1);

    while (1){
        system("tput reset");

        printf("Produtos: %d\n",Cheio);
        printf("Espaços: %d\n",Vazio);

        printf("\n");

        sleep(1);
    }   
}

void* produtor(){
    printf("Inicializando produtor\n");
    sleep(1);

    while(1){
        int quant_produtos = rand() % 5;
        
        int i;

        for(i = 0; i < quant_produtos; i++){
            while(Vazio == 0);                

            Produto *produto = criaProduto();
            //Inserir item
            //Vai ter que ser uma função

            produtos[entrada] = produto;

            entrada = (entrada + 1) % MAX;

            Cheio += 1;
            Vazio -= 1;

        }

        sleep(1);
    }
}

void* consumidor(){
    printf("Inicializando Consumidor\n");
    sleep(1);

    while(1){
        
        int quant_produtos = rand() % 5;

        int i;

        for( i = 0; i < quant_produtos ; i++){
            while(Cheio == 0);
        
            int id_produto;

            id_produto = removeProduto();

            saida = (saida + 1) % MAX;

            Cheio -= 1;
            Vazio += 1;
        }        

        sleep(1);
    }
    
}

int main(){
    srand(time(NULL));

    pthread_t newthread[3];

    pthread_create(&newthread[0],NULL,produtor,NULL);
    pthread_create(&newthread[1],NULL,consumidor,NULL);
    pthread_create(&newthread[2],NULL,exibir,NULL);
    //produtor();
    //consumidor();

    pthread_join(newthread[0],NULL);
    pthread_join(newthread[1],NULL);

    return 0;
}

Produto *criaProduto(){
    Produto* new_produto;

    static int id_prod = 0;

    new_produto = (Produto*) malloc(1*sizeof(Produto));

    new_produto->id = ++id_prod;

    return new_produto;
}

int removeProduto(){

    int id_produto = produtos[saida]->id;

    free(produtos[saida]);

    produtos[saida] = NULL;

    return id_produto;
}