#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct lista_vizinhos_t
{
    int id_vizinho;
    struct lista_vizinhos_t *proximo;
} lista_vizinhos_t;

typedef struct dispositivo_t
{
    int id;
    double pos_x;
    double pos_y;
    lista_vizinhos_t *lista_vizinhos;
} dispositivo_t;

typedef struct grafo_t
{
    dispositivo_t *grafo;
} grafo_t;

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista)
{
    lista_vizinhos_t *item_novo = malloc(sizeof(lista_vizinhos_t)); // Aloca o novo item
    if (item_novo == NULL)
        return false;                // Falta memória?
    item_novo->id_vizinho = vizinho; // Seta o novo item
    item_novo->proximo = *lista;     // O próximo do novo item será a lista
    *lista = item_novo;              // Aqui, estamos mudando o ponteiro da lista
    return true;
}

void lista_vizinhos_imprimir(lista_vizinhos_t *lista)
{
    while (lista != NULL)
    {
        printf("\t%d", lista->id_vizinho);
        lista = lista->proximo;
    }
    printf("\n");
}

grafo_t *grafo_criar(int tam)
{
    grafo_t *grafo = malloc(sizeof(grafo_t));

    grafo->grafo = malloc(tam * sizeof(dispositivo_t *));

    // grafo->tamanho = tam;
    for (int i = 0; i < tam; i++)
    {
        grafo->grafo[i].id = 0;
        grafo->grafo[i].lista_vizinhos = NULL;
        grafo->grafo[i].pos_x = 0;
        grafo->grafo[i].pos_y = 0;
    }
    return grafo;
}

void grafo_imprimir(int tam, grafo_t grafo)
{
    for (int i = 0; i < tam; i++)
    {
        if (grafo.grafo[i].lista_vizinhos != NULL)
        {
            printf("NÓ %d:", grafo.grafo[i].id);
            lista_vizinhos_imprimir(grafo.grafo[i].lista_vizinhos);
        }
    }
}

void grafo_atualizar_vizinhos(int tam, grafo_t *grafo, double raio_comunicacao)
{
    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            if (i != j)
            {
                double distancia = sqrt(pow(grafo->grafo[i].pos_x - grafo->grafo[j].pos_x, 2) +
                                        pow(grafo->grafo[i].pos_y - grafo->grafo[j].pos_y, 2));
                if (distancia <= raio_comunicacao)
                {
                    lista_vizinhos_adicionar(grafo->grafo[j].id, &grafo->grafo[i].lista_vizinhos);
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    int tamanho;
    double raio;

    double pos_x, pos_y;
    int id;

    FILE *arquivo = fopen(argv[1], "r"); // Abre o arquivo de entrada em modo de leitura
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fscanf(arquivo, "%d %lf", &tamanho, &raio);

    grafo_t *grafo = grafo_criar(tamanho);

    for (int i = 0; i < tamanho; i++)
    {
        if (fscanf(arquivo, "%d %lf %lf", &id, &pos_x, &pos_y) == 3)
        {
            grafo->grafo[i].id = id;
            grafo->grafo[i].pos_x = pos_x;
            grafo->grafo[i].pos_y = pos_y;
            grafo->grafo[i].lista_vizinhos = NULL;
        }
        else
        {
            printf("Deu ruim");
            fclose(arquivo);
            free(grafo->grafo);
            free(grafo);
            return 1;
        }
    }

    fclose(arquivo); // Fecha o arquivo após a leitura

    grafo_atualizar_vizinhos(tamanho, grafo, raio);

    grafo_imprimir(tamanho, *grafo);

    for (int i = 0; i < tamanho; i++)
    {
        lista_vizinhos_t *atual = grafo->grafo[i].lista_vizinhos;
        while (atual != NULL)
        {
            lista_vizinhos_t *tmp = atual;
            atual = atual->proximo;
            free(tmp);
        }
    }
    free(grafo->grafo);
    free(grafo);
}