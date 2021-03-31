#include "./prodCons.h"

//As variaveis sao globais
static Buffer b;

static sem_t exclusao; // Semaforos para exclusao mutua no buffer
static sem_t vazio; // Semaforo para espaços vazios
static sem_t cheio; // Semaforo para espaços cheios
static sem_t atualizacao; // Semafora como sinal para exibicao

int option; // Excucao aleatoria ou nao

int consumir; // Quantidade de produtos a ser consumido
int produzir; // Quantidade de produtos a ser produzido

//Abstracao da "interface"
void* exibir(){

    while (1){
        sleep(1);  

        sem_wait(&atualizacao);

        system("tput reset");

        int count;
        sem_getvalue(&cheio, &count);
        

        printf("\n");

        //FILE *arq = fopen("resultados.txt", "w+");
        int i;
        for (i = 0; i < MAX; i++)
        {
            if (b.produtos[i] != NULL)
            {
                printf("x");
                //fwrite("Válido!\n", 1, sizeof("Válido!\n"), arq);
            }
            else
            {
                printf("-");
                //fwrite("NULL!\n", 1, sizeof("NULL!\n"), arq);
            }
        }
        //fclose(arq);

        printf("\n\nProdutos: %d\n", count);
        printf("Espaços: %d\n\n",MAX - count);              
    }
}

//Abstracao do produtor
void* produtor(){
    printf("Inicializando produtor\n");

    while(1){
        sleep(1);
        
        produzir = option ? produzir : rand() % MAX;

        int i;
        for(i = 0; i < produzir; i++){             
            
            sem_wait(&vazio);
            sem_wait(&exclusao);
            // printf("Produtor: %d\n", b.in);

            Produto *produto = criaProduto();
            //Inserir item
            b.produtos[b.in] = produto;

            b.in = (b.in + 1) % MAX;

            sem_post(&exclusao);
            sem_post(&cheio);
            sem_post(&atualizacao);   
        }
        printf("\n");        
    }
}

//Abstracao do consumidor
void* consumidor(){
    printf("Inicializando Consumidor\n");

    while(1){
        sleep(1);

        consumir = option ? consumir : rand() % MAX;

        int i;

        for( i = 0; i < consumir ; i++){
            // while(Cheio == 0);

            sem_wait(&cheio);
            sem_wait(&exclusao);
            // printf("Consumidor: %d\n", b.out);

            int id_produto;

            id_produto = removeProduto();

            b.out = (b.out + 1) % MAX;
            sem_post(&exclusao);
            sem_post(&vazio);   
            sem_post(&atualizacao);
        }
        printf("\n");        
    }
    
}

//Fluxo de execucao
int main(int argc, char** argv){   
    
    //Semente aleatória
    srand(time(NULL));
    
    //Valores iniciais do buffer in: index para entrada, out: index para saída
    b.in = 0;
    b.out = b.in;

    option = atoi(argv[1]); // 0 - consumir e produzir de forma aleatória, caso contrário - parametros

    if( option == 1 ){
        produzir = atoi(argv[2]);
        consumir = atoi(argv[3]);
    }

    // printf("%d, %d", produzir, consumir);

    //Inicializando semáforos
    sem_init(&exclusao,0,1);
    sem_init(&vazio, 0, MAX);
    sem_init(&cheio, 0, 0);

    //Criação das threads posix's
    pthread_t newthread[3];

    pthread_create(&newthread[0],NULL,produtor,NULL);
    pthread_create(&newthread[1],NULL,consumidor,NULL);
    pthread_create(&newthread[2],NULL,exibir,NULL);

    pthread_join(newthread[0],NULL);
    pthread_join(newthread[1],NULL);

    //Liberação dos semáforos
    sem_destroy(&exclusao);
    sem_destroy(&vazio);
    sem_destroy(&cheio);

    return 0;
}

//Cria um produto para ser inserido no buffer
Produto *criaProduto(){
    Produto* new_produto;

    static int id_prod = 0;

    new_produto = (Produto*) malloc(1*sizeof(Produto));

    new_produto->id = ++id_prod;

    return new_produto;
}

//Remove um produto do buffer
int removeProduto(){

    int id_produto = b.produtos[b.out]->id;

    free(b.produtos[b.out]);

    b.produtos[b.out] = NULL;

    return id_produto;
}