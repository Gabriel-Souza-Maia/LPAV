#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Classe Pessoa campos: nome, cpf, idade
typedef struct pessoa_t
{
    char nome[51];
    long long int cpf;
    int idade;
} pessoa_t;

// Classe Lista Encadeada de Pessoas campos: pessoa, ponteiro do próximo da lista
typedef struct lista_pessoas_t
{
    pessoa_t pessoa;
    struct lista_pessoas_t *proximo;
} lista_pessoas_t;

// Classe da Hash de pessoas campos: Array de Listas Encadeadas, Tamanho (A ser definido pelo prompt de comando)
typedef struct hash_pessoas_t
{
    lista_pessoas_t **hash;
    int tamanho;
} hash_pessoas_t;

bool lista_pessoas_adicionar_inicio(pessoa_t *pessoa, lista_pessoas_t **lista)
{
    lista_pessoas_t *item_novo = malloc(sizeof(lista_pessoas_t)); // Aloca o novo item
    if (item_novo == NULL)
        return false;            // Falta memória?
    item_novo->pessoa = *pessoa; // Seta o novo item
    item_novo->proximo = *lista; // O próximo do novo item será a lista
    *lista = item_novo;          // Aqui, estamos mudando o ponteiro da lista
    return true;
};

void lista_pessoas_listar(lista_pessoas_t *lista)
{
    while (lista != NULL)
    {
        printf("Nome: %s, CPF: %lld, Idade: %d\n", lista->pessoa.nome, lista->pessoa.cpf, lista->pessoa.idade);
        lista = lista->proximo;
    }
}

int tabela_hash_pessoas_funcao(pessoa_t *pessoa, int tam)
{
    return pessoa->cpf % tam;
}

hash_pessoas_t *tabela_hash_pessoas_criar(int tam)
{
    hash_pessoas_t *tabela = malloc(sizeof(hash_pessoas_t));

    tabela->hash = malloc(tam * sizeof(lista_pessoas_t *));

    tabela->tamanho = tam;
    for (int i = 0; i < tam; i++)
    {
        tabela->hash[i] = NULL;
    }
    return tabela;
}

void tabela_hash_pessoas_listar(hash_pessoas_t *hash)
{
    for (int i = 0; i < hash->tamanho; i++)
    {
        if (hash->hash[i] != NULL)
        {
            printf("POSIÇÃO %d DA TABELA HASH:\n", i);
            lista_pessoas_listar(hash->hash[i]);
        }
    }
}

bool tabela_hash_pessoas_inserir(hash_pessoas_t *hash, pessoa_t *pessoa)
{
    int index = tabela_hash_pessoas_funcao(pessoa, hash->tamanho);
    return lista_pessoas_adicionar_inicio(pessoa, &hash->hash[index]);
}

int main(int argc, char **argv)
{
    lista_pessoas_t *lista = NULL;

    int tabela_hash_tam = atoi(argv[1]);

    hash_pessoas_t *tabela_hash = tabela_hash_pessoas_criar(tabela_hash_tam);

    FILE *arquivo = fopen(argv[2], "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    pessoa_t pessoa;
    while (fscanf(arquivo, "%50[^\t]\t%lld\t%d\n", pessoa.nome, &pessoa.cpf, &pessoa.idade) == 3)
    {
        if (!tabela_hash_pessoas_inserir(tabela_hash, &pessoa))
        {
            printf("Erro ao inserir pessoa na tabela hash.\n");
            fclose(arquivo);
            return EXIT_FAILURE;
        }
    }

    fclose(arquivo); // Fecha o arquivo após a leitura

    tabela_hash_pessoas_listar(tabela_hash);

    return 0;
}
