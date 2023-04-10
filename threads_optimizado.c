#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#define MAX 10000 // tamanho maximo das linhas e colunas
#define MAXT 2 // numero de threads

int matA[MAX][MAX], matB[MAX][MAX], matres[MAX][MAX]; //matrizes operando e resultado
int lin_col = 0; //numero de linhas e colunas das matrizes
int linhas_por_thread = 0; //numero de linhas que cada thread vai calcular

void *mult(void *arg)
{
    int *inicio_ptr = (int *)arg; // transformar arg de void* de volta para int*
    int i = 0, j = 0, k = 0, operacao = 0;
    int final;

    final = linhas_por_thread + *inicio_ptr; //multiplicação deve terminar na linha correspondente
                                             //a (linhas_por_thread + inicio)


    for ( i = *inicio_ptr; i < final; i++) //começa na linha indicada por inicio
    {                                                      
        for ( j = 0; j < lin_col; j++)                     
        {
            for ( k = 0; k < lin_col; k++)
            {
                operacao += matA[i][k] * matB[k][j]; //multiplica os valores e soma á variavel operação
            }

            matres[i][j] = operacao; //insere o valor obtido na matriz resultado
            operacao = 0; //retorna a variavel a zero para começar de novo
        }
    }
}

int main()
{
    int i = 0, j = 0, k = 0, status;
    int maxt = MAXT; //numero de threads
    float segundos;

    srand(time(NULL)); //seed para a geração aleatoria
    // criação da matriz aleatoriamente
    printf("insira o tamanho das matrizes\n");
    scanf("%d", &lin_col);

    // geracao da matriz A
    printf("\nMatriz A gerada!\n");
    for (i = 0; i < lin_col; i++)
    {
        for (j = 0; j < lin_col; j++)
        {
            matA[i][j] = (rand() % (9 + 1 - 0) + 0);
            //printf("%d\n", matA[i][j]);
        }
        //printf("\n");
    }
    // geracao da matriz B
    printf("\nMatriz B gerada!\n");
    for (i = 0; i < lin_col; i++)
    {
        for (j = 0; j < lin_col; j++)
        {
            matB[i][j] = (rand() % (9 + 1 - 0) + 0);
            //printf("%d\n", matB[i][j]);
        }
        //printf("\n");
    }

    // estabelecer quantas linhas cada thread vai calcular (numero de linhas a dividir pelo numero de threads)
    linhas_por_thread = lin_col / maxt;

    // criacao do array de threads
    pthread_t *threads; // declaração de variavel do tipo pthread_t*
    threads = (pthread_t *)malloc(maxt*sizeof(pthread_t)); // array do tamanho maxt

    int inicio[maxt]; //criação de vetor que contem os numeros da linha onde começar a multiplicação

    clock_t start = clock(); //iniciar o cronometro
    for (i = 0; i < maxt; i++) //um ciclo para cada thread
    {
        inicio[i] = i * linhas_por_thread; // exemplo: se i = 0 multiplicação começa na linha zero
        
        // cria os threads e envia ponteiro do inicio
        status = pthread_create(&threads[i], NULL, mult, (void*)(inicio+i)); 
        if (status != 0)
        {
            printf("Oops. pthread_create returned error code %d\n", status); // em caso de criação correr mal
            exit(-1);
        }
    }

    for (i = 0; i < maxt; i++)
    {
        // Joining de cada thread, ou seja, espera que cada thread termine a sua tarefa
        pthread_join(threads[i], NULL);
    }

    //impressao da porção da matriz resultado (matres)
    /*
    for ( j = 0; j < lin_col; j++)
    {
        for ( k = 0; k < lin_col; k++)
        {
            printf("%d\t", matres[j][k]);
        }
        printf("\n");
    }
    */
   printf("\nmultiplicação terminada!\n");
        
    //codigo para apresentar o tempo de execução
    clock_t end = clock();
    segundos = (float)(end - start) / CLOCKS_PER_SEC;
    segundos /= 10;
    printf("\nExecutado em %0.4f segundos\n", segundos);
    return 0;
}
