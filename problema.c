#include "./problema.h"


//As variaveis sao globais
Produto* produtos[MAX] = {NULL};
//

int topo = 0;
int mutex = PPRODUZIR;

void* exibir(){
    sleep(1);

    while (1){
        // system("tput reset");

        printf("Produtos: %d\n",topo);
        printf("Espaços: %d\n",MAX - topo);

        printf("\n");

        FILE *arq = fopen("resultados.txt", "w+");
        int i;
        for (i = 0; i < MAX; i++)
        {
            if (produtos[i] != NULL)
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
        if (mutex == PPRODUZIR)
        {
            for(i = 0; i < quant_produtos; i++){
                // while(Vazio == 0);                
                if (topo < MAX-1)
                {           
                    if (produtos[topo] == NULL)
                    {
                        printf("Topo (Produtor): %d\n", topo);

                        Produto *produto = criaProduto();
                        //Inserir item
                        //Vai ter que ser uma função
                        produtos[topo] = produto;

                        topo = (topo + 1) % MAX;
                    }
                    else {
                        printf("Erro! Bloco de Memória Cheio!\n");
                        // exit(1);
                    }
                }
            }
            mutex = CONSUMIR;
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
        
        if (mutex == CONSUMIR)
        {
            for( i = 0; i < quant_produtos ; i++){
                // while(Cheio == 0);
                if (topo)
                {
                    if (produtos[topo-1] != NULL)
                    {
                        printf("Topo (Consumidor): %d\n", topo-1);

                        int id_produto;

                        id_produto = removeProduto();

                        topo = (topo - 1) % MAX;
                    }
                    else 
                    {
                        printf("Erro! Produto inexistente.\n");
                        printf("Topo (Consumidor): %d\n", topo);
                        exit(1);
                    }
                }
            }
            mutex = PPRODUZIR;
        }
        printf("\n");

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

    int id_produto = produtos[topo-1]->id;

    free(produtos[topo-1]);

    produtos[topo-1] = NULL;

    return id_produto;
}