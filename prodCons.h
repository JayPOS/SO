#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <semaphore.h>


#define MAX 20
#define PPRODUZIR 0
#define CONSUMIR 1

typedef struct produto
{
    int id;
    //info produto
} Produto;

typedef struct buffer 
{
    int in;
    int out;
    Produto *produtos[MAX];
} Buffer;

Produto *criaProduto(/*info produto*/);
int removeProduto(); // retorna id do produto
void* produtor();
void* consumidor();

void* exibir();

bool isValid(int type, const Buffer b);
int bufferCount(const Buffer b);
