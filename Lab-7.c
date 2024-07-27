#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <winsock2.h>
// COMPILAR USANDO O COMANDO: gcc -o Lab-7 Lab-7.c -lws2_32

// Png header chunk
struct png_chunk_hdr
{
  uint32_t length;
  char tipo[4];
};

// Png header types
struct IHDR_t
{
  uint32_t width;
  uint32_t height;
  uint8_t bit_depth;
  uint8_t colour_type;
  uint8_t compression_method;
  uint8_t filter_method;
  uint8_t interlace_method;
};

int main(int argc, char const *argv[])
{
  int count = 0;

  FILE *png_file = fopen(argv[1], "rb");
  if (png_file == NULL)
  {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  struct png_chunk_hdr *png_chunk = malloc(sizeof(struct png_chunk_hdr));
  struct IHDR_t *ihdr = malloc(sizeof(struct IHDR_t));

  // pula assinatura do arquivo
  fseek(png_file, 8, SEEK_SET);

  while (!feof(png_file))
  {
    fread(png_chunk, sizeof(struct png_chunk_hdr), 1, png_file); // Lê um chunk
    count++;
    printf("Lendo chunk  %d\n", count);
    printf("\t --> Tamanho: %d\n", ntohl(png_chunk->length)); // Converte e imprime o tam.
    printf("\t --> Tipo: %.4s\n", png_chunk->tipo);           // Imprime o tipo

    // Se for um chunk IHDR
    if (strncmp(png_chunk->tipo, "IHDR", 4) == 0)
    {
      fread(ihdr, sizeof(struct IHDR_t), 1, png_file); // Lê o chunk IHDR
      printf("\t\t --> Largura: %d\n", ntohl(ihdr->width));
      printf("\t\t --> Altura: %d\n", ntohl(ihdr->height));
      // pular o CRC
      fseek(png_file, 4, SEEK_CUR);
    }
    else if (strncmp(png_chunk->tipo, "IEND", 4) == 0)
    {
      // quebra o loop
      break;
    }
    else
    {
      // pula o chunk
      fseek(png_file, ntohl(png_chunk->length) + 4, SEEK_CUR);
    }
  }

  fclose(png_file);
  free(png_chunk);
  free(ihdr);

  return 0;
}
