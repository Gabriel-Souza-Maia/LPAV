#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct evento_t
{
  double tempo;
  int alvo;
  int tipo;
} evento_t;

typedef struct lista_eventos_t
{
  evento_t evento;
  struct lista_eventos_t *proximo;
} lista_eventos_t;

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
  bool pacote_enviado;
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

lista_eventos_t *lista = NULL;
dispositivo_t *grafo = NULL;

bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista)
{
  lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t)); // Aloca o novo item
  if (item_novo == NULL)
    return false; // Falta memória?

  item_novo->evento = *evento; // Seta o novo item
  item_novo->proximo = NULL;   // O próximo será Nulo, visto que ele está no final da fila

  if (*lista == NULL)
  {
    // Se a lista estiver vazia, o novo item é inserido na cabeça
    *lista = item_novo;
  }
  else
  {
    lista_eventos_t *item_atual = *lista;
    // printf("Tempo proximo Evento: %3.6f, Tempo Novo Elemento: %3.6f \n", item_atual->evento.tempo, evento->tempo);
    if (item_atual->evento.tempo >= evento->tempo)
    {
      item_novo->proximo = item_atual;
      *lista = item_novo;
    }
    else
    {
      // Percorre até encontrar a posição correta para inserção
      while (item_atual->proximo != NULL && item_atual->proximo->evento.tempo < evento->tempo)
      {
        item_atual = item_atual->proximo;
      }
      // Insere o novo item na posição correta
      item_novo->proximo = item_atual->proximo;
      item_atual->proximo = item_novo;
    }
  }
  return true;
};

void lista_eventos_listar(lista_eventos_t *lista)
{
  while (lista != NULL)
  {
    printf("%3.6f\t%d\t%d\n", lista->evento.tempo, lista->evento.alvo, lista->evento.tipo);
    lista = lista->proximo;
  }
};

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

void simulacao_iniciar()
{
  while (lista != NULL)
  {
    // salve um ponteiro para o primeiro evento
    evento_t *eventoAtual = &lista->evento;
    // avance a lista para o próximo evento
    lista = lista->proximo;
    // imprima o evento
    printf("[%3.6f] Nó %d recebeu pacote.\n", eventoAtual->tempo, eventoAtual->alvo);
    lista_vizinhos_t *vizinho = grafo[eventoAtual->alvo].lista_vizinhos;
    // adiciona eventos nos vizinhos
    if (grafo[eventoAtual->alvo].pacote_enviado == false)
    {
      while (vizinho != NULL)
      {
        printf("\t--> Repassando pacote para o nó %d ...\n", vizinho->id_vizinho);
        printf("tempo Atual: %3.6f\n", eventoAtual->tempo);
        evento_t eventoNovo = {eventoAtual->tempo + (0.1 + (vizinho->id_vizinho * 0.01)), vizinho->id_vizinho, 1};
        lista_eventos_adicionar_ordenado(&eventoNovo, &lista);
        grafo[eventoAtual->alvo].pacote_enviado = true;
        vizinho = vizinho->proximo;
      }
    }
  }
}

int main(int argc, char *argv[])
{
  // lista_eventos_t *lista = NULL;
  double tempo;
  int alvo, tipo;

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

  grafo = grafo_criar(tamanho);

  for (int i = 0; i < tamanho; i++)
  {
    fscanf(arquivo, "%d\t%lf\t%lf\n", &id, &pos_x, &pos_y);
    grafo[i].id = id;
    grafo[i].pos_x = pos_x;
    grafo[i].pos_y = pos_y;
    grafo[i].pacote_enviado = false;
  }

  fclose(arquivo);

  grafo_atualizar_vizinhos(tamanho, grafo, raio);
  // grafo_imprimir(tamanho, grafo);

  // criando evento
  evento_t evento = {1.0, 0, 1};
  lista_eventos_adicionar_ordenado(&evento, &lista);

  // iniciando a simulação
  simulacao_iniciar();

  free(grafo);

  return 0;
}
