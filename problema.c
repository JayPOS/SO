#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

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

Produto *criaProduto(/*info produto*/);
int removeProduto(); // retorna id do produto

void produtor(){
    //while(1){
        int quant_produtos = rand() % 5;
        
        int i;

        printf("%d\n", quant_produtos);

        for(i = 0; i < quant_produtos; i++){
            Produto *produto = criaProduto();
            //Inserir item
            //Vai ter que ser uma função

            produtos[entrada] = produto;

            entrada = (entrada + 1) % MAX;
        }
    //}
}

void consumidor(){
    //while(1){
        
        int id_produto;

        id_produto = removeProduto();

        saida = (saida + 1) % MAX;
    //}
    
}

int main(){

    srand(time(NULL));

    produtor();
    
    consumidor();

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