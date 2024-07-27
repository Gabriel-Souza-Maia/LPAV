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

dispositivo_t *grafo_criar(int tam)
{
  dispositivo_t *grafo = malloc(tam * sizeof(dispositivo_t));
  for (int i = 0; i < tam; i++)
  {
    grafo[i].id = 0;
    grafo[i].lista_vizinhos = NULL;
    grafo[i].pos_x = 0;
    grafo[i].pos_y = 0;
  }
  return grafo;
}

// // função que imprime a lista de vizinhos
void lista_vizinhos_imprimir(lista_vizinhos_t *lista)
{
  while (lista != NULL)
  {
    printf("\t%d", lista->id_vizinho);
    lista = lista->proximo;
  }
  printf("\n");
}

// função que imprime o grafo
void grafo_imprimir(int tam, dispositivo_t *grafo)
{
  for (int i = 0; i < tam; i++)
  {
    if (grafo[i].lista_vizinhos != NULL)
    {
      printf("NÓ %d:", grafo[i].id);
      lista_vizinhos_imprimir(grafo[i].lista_vizinhos);
    }
    else
    {
      printf("NÓ %d: sem vizinhos\n", grafo[i].id);
    }
  }
}

// função que adiciona um vizinho à lista de vizinhos
bool lista_vizinhos_adicionar(int id_vizinho, lista_vizinhos_t **lista)
{
  lista_vizinhos_t *novo_vizinho = malloc(sizeof(lista_vizinhos_t));
  novo_vizinho->id_vizinho = id_vizinho;
  novo_vizinho->proximo = *lista;
  *lista = novo_vizinho;
  return true;
}

// função que atualiza a lista de vizinhos
void grafo_atualizar_vizinhos(int tam, dispositivo_t *grafo, double raio_comunicacao)
{
  for (int i = 0; i < tam; i++)
  {
    for (int j = 0; j < tam; j++)
    {
      if (i != j)
      {
        double distancia = sqrt(pow(grafo[i].pos_x - grafo[j].pos_x, 2) + pow(grafo[i].pos_y - grafo[j].pos_y, 2));
        if (distancia <= raio_comunicacao)
        {
          lista_vizinhos_adicionar(grafo[j].id, &grafo[i].lista_vizinhos);
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  int tamanho = 0;
  double raio = 0;

  double pos_x, pos_y;
  int id;

  FILE *arquivo = fopen(argv[1], "r"); // Abre o arquivo de entrada em modo de leitura
  if (arquivo == NULL)
  {
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }

  fscanf(arquivo, "%d\t%lf\n", &tamanho, &raio);

  dispositivo_t *grafo = grafo_criar(tamanho);

  for (int i = 0; i < tamanho; i++)
  {
    fscanf(arquivo, "%d\t%lf\t%lf\n", &id, &pos_x, &pos_y);
    grafo[i].id = id;
    grafo[i].pos_x = pos_x;
    grafo[i].pos_y = pos_y;
  }

  fclose(arquivo);

  grafo_atualizar_vizinhos(tamanho, grafo, raio);
  grafo_imprimir(tamanho, grafo);

  free(grafo);

  return 0;
}
