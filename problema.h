#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 100
#define PPRODUZIR 0
#define CONSUMIR 1

typedef struct produto
{
    int id;
    //info produto
} Produto;

Produto *criaProduto(/*info produto*/);
int removeProduto(); // retorna id do produto
void* produtor();
void* consumidor();

void* exibir();

