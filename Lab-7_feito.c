#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <winsock2.h>

struct png_cabecalho_chunk
{
    uint32_t tamanho;
    char tipo[4];
};

struct TiposIHDR
{
    uint32_t width;
    uint32_t height;
    uint8_t bit_depth;
    uint8_t colour_type;
    uint8_t compression_method;
    uint8_t filter_method;
    uint8_t interlace_method;
};

FILE *abrir_arquivo_png(const char *nome_arquivo)
{
    FILE *arquivo_png = fopen(nome_arquivo, "rb");
    if (arquivo_png == NULL)
    {
        fprintf(stderr, "Erro");
        exit(EXIT_FAILURE);
    }
    return arquivo_png;
}

void pular_assinatura_png(FILE *arquivo_png)
{
    fseek(arquivo_png, 8, SEEK_SET);
}

void ler_cabecalho_chunk(FILE *arquivo_png, struct png_cabecalho_chunk *chunk)
{
    fread(&(chunk->tamanho), sizeof(uint32_t), 1, arquivo_png);
    fread(&(chunk->tipo), sizeof(char), 4, arquivo_png);
    chunk->tamanho = ntohl(chunk->tamanho);
}

void ler_TiposIHDR(FILE *arquivo_png, struct TiposIHDR *ihdr)
{
    fread(&(ihdr->width), sizeof(uint32_t), 1, arquivo_png);
    fread(&(ihdr->height), sizeof(uint32_t), 1, arquivo_png);
    fread(&(ihdr->bit_depth), sizeof(uint8_t), 1, arquivo_png);
    fread(&(ihdr->colour_type), sizeof(uint8_t), 1, arquivo_png);
    fread(&(ihdr->compression_method), sizeof(uint8_t), 1, arquivo_png);
    fread(&(ihdr->filter_method), sizeof(uint8_t), 1, arquivo_png);
    fread(&(ihdr->interlace_method), sizeof(uint8_t), 1, arquivo_png);

    ihdr->width = ntohl(ihdr->width);
    ihdr->height = ntohl(ihdr->height);
}

void pular_bytes(FILE *arquivo_png, long bytes_ignorados)
{
    fseek(arquivo_png, bytes_ignorados, SEEK_CUR);
}

void processar_arquivo_png(FILE *arquivo_png)
{
    struct png_cabecalho_chunk *chunk = malloc(sizeof(struct png_cabecalho_chunk));
    struct TiposIHDR *ihdr = malloc(sizeof(struct TiposIHDR));

    if (chunk == NULL || ihdr == NULL)
    {
        fprintf(stderr, "erro");
        exit(EXIT_FAILURE);
    }

    pular_assinatura_png(arquivo_png);

    int cont = 1;
    while (!feof(arquivo_png))
    {
        ler_cabecalho_chunk(arquivo_png, chunk);

        printf("Lendo chunk %d:\n", cont);
        printf("  -->Tamanho: %u\n", chunk->tamanho);
        printf("    --> Tipo:    %.4s\n", chunk->tipo);

        if (strncmp(chunk->tipo, "IHDR", 4) == 0)
        {
            ler_TiposIHDR(arquivo_png, ihdr);
            printf("        --> Largura:  %u\n", ihdr->width);
            printf("        --> Altura:   %u\n", ihdr->height);
            pular_bytes(arquivo_png, 4);
        }
        else if (strncmp(chunk->tipo, "IEND", 4) == 0)
        {
            break;
        }
        else
        {
            pular_bytes(arquivo_png, chunk->tamanho + 4);
        }

        cont++;
    }

    free(chunk);
    free(ihdr);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return EXIT_FAILURE;
    }

    FILE *arquivo_png = abrir_arquivo_png(argv[1]);
    processar_arquivo_png(arquivo_png);
    fclose(arquivo_png);

    return EXIT_SUCCESS;
}