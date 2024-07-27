#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista)
{
    lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t)); // Aloca o novo item
    if (item_novo == NULL)
        return false;            // Falta memória?
    item_novo->evento = *evento; // Seta o novo item
    item_novo->proximo = *lista; // O próximo do novo item será a lista
    *lista = item_novo;          // Aqui, estamos mudando o ponteiro da lista
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

bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista)
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
        while (item_atual->proximo != NULL) // Percorre até o fim da Lista
        {
            item_atual = item_atual->proximo;
        }
        // Agora temp aponta para o último item da lista
        item_atual->proximo = item_novo; // Adiciona o novo item no final da lista
    }

    return true;
};

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
            printf("Entrou \n");
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

int main()
{
    lista_eventos_t *lista = NULL;

    float tempo;
    int alvo, tipo;

    // FILE *arquivo = fopen("arquivo_teste_pequeno.in", "r"); // Abre o arquivo de entrada em modo de leitura
    FILE *arquivo = fopen("arquivo_teste.in", "r"); // Abre o arquivo de entrada em modo de leitura

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    printf("Lendo eventos do arquivo...\n");

    while (fscanf(arquivo, "%3.6f %d %d", &tempo, &alvo, &tipo) == 3)
    {
        evento_t evento = {tempo, alvo, tipo};
        lista_eventos_adicionar_ordenado(&evento, &lista);
    }

    fclose(arquivo); // Fecha o arquivo após a leitura

    printf("\nEventos inseridos na lista:\n");
    lista_eventos_listar(lista);

    return 0;
}