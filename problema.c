#include "./problema.h"


//As variaveis sao globais
static Buffer b;

static sem_t exclusao;
static sem_t slot_vazio; // 
static sem_t slot_cheio;

int topo = 0;
//
int bufferCount(const Buffer b)
{
    int count;

    count = b.in - b.out;
    if (count < 0)
        count += MAX;
    return count;
}
bool isValid(int type, const Buffer b)
{
    if (type == PPRODUZIR)
    {
        if(bufferCount(b) > MAX-1) return false;
        else return true;
    }
    else if (type == CONSUMIR)
    {
        if(bufferCount(b) > 0) return true;
        else return false;
    }
}

void* exibir(){
    sleep(1);

    while (1){
        //system("tput reset");

        int count = bufferCount(b);
        printf("Produtos: %d\n", count);
        printf("Espaços: %d\n",MAX - count);

        printf("\n");

        FILE *arq = fopen("resultados.txt", "w+");
        int i;
        for (i = 0; i < MAX; i++)
        {
            if (b.produtos[i] != NULL)
            {
                fwrite("Válido!\n", 1, sizeof("Válido!\n"), arq);
            }
            else
            {
                fwrite("NULL!\n", 1, sizeof("NULL!\n"), arq);
            }
        }
        fclose(arq);

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
            // while(Vazio == 0);                
            if (isValid(PPRODUZIR, b))
            {           
                if (b.produtos[b.in] == NULL)
                {
                    sem_wait(&slot_vazio);
                    sem_wait(&exclusao);
                    printf("Produtor: %d\n", b.in);

                    Produto *produto = criaProduto();
                    //Inserir item
                    //Vai ter que ser uma função
                    b.produtos[b.in] = produto;

                    b.in = (b.in + 1) % MAX;

                    sem_post(&exclusao);
                    sem_post(&slot_cheio);
                }
                else {
                    printf("Erro! Bloco de Memória Cheio!\n");
                    // exit(1);
                }
            }
        }
        printf("\n");

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
            // while(Cheio == 0);
            if (isValid(CONSUMIR, b))
            {
                if (b.produtos[b.out] != NULL)
                {
                    sem_wait(&slot_cheio);
                    sem_wait(&exclusao);
                    printf("Consumidor: %d\n", b.out);

                    int id_produto;

                    id_produto = removeProduto();

                    b.out = (b.out + 1) % MAX;
                    sem_post(&exclusao);
                    sem_post(&slot_vazio);
                }
                else 
                {
                    printf("Erro! Produto inexistente.\n");
                    printf("Topo (Consumidor): %d\n", topo);
                    exit(1);
                }
            }
        }
        printf("\n");

        sleep(1);
    }
    
}

int main(){
    b.in = 0;
    b.out = b.in;

    srand(time(NULL));

    sem_init(&exclusao,0,1);
    sem_init(&slot_vazio, 0, MAX-1);
    sem_init(&slot_cheio, 0, 0);

    pthread_t newthread[3];

    pthread_create(&newthread[0],NULL,produtor,NULL);
    pthread_create(&newthread[1],NULL,consumidor,NULL);
    pthread_create(&newthread[2],NULL,exibir,NULL);
    //produtor();
    //consumidor();

    pthread_join(newthread[0],NULL);
    pthread_join(newthread[1],NULL);

    sem_destroy(&exclusao);
    sem_destroy(&slot_vazio);
    sem_destroy(&slot_cheio);

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

    int id_produto = b.produtos[b.out]->id;

    free(b.produtos[b.out]);

    b.produtos[b.out] = NULL;

    return id_produto;
}