#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <winsock2.h>

#define FILE_NAME "saida_parte_" // nome do arquivo de saída
#define Pack 0xba
#define System 0xbb
#define Sequence 0xb3
#define GOP 0xb8
#define Picture 0x00

int main(int argc, char const *argv[])
{
  // string de 4 bytes
  unsigned char byte[4];
  int file_number = 1;
  char output_file_name[256];

  FILE *mpg_file = fopen(argv[1], "rb");
  if (mpg_file == NULL)
  {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  sprintf(output_file_name, "%s%d.mpg", FILE_NAME, file_number);
  printf("Criando arquivo: %s ..\n", output_file_name);
  // abre arquivo de saída parte 1
  FILE *mpg_split_output_file = fopen(output_file_name, "wb");

  // cria buffer com o argv[2] * 1MB
  int max_size = atoi(argv[2]) * 1024 * 1024;
  char *buffer = (char *)malloc(max_size);

  int buffer_offset = 0;

  while (1)
  {
    fread(byte, 1, 4, mpg_file);

    if (memcmp(byte, "\x00\x00\x01\xB3", 4) != 0 && !feof(mpg_file))
    {
      // salva o conteúdo do buffer + 1 byte no buffer
      memcpy(buffer + buffer_offset, byte, 1);
      buffer_offset += 1;
      fseek(mpg_file, -3, SEEK_CUR);
      continue;
    }

    // Se chegar aqui, então temos o início de um novo fluxo Sequence ou o final do arquivo de entrada

    //  se o tamanho do arquivo de saída + o buffer for maior que o tamanho máximo, fecha o arquivo e abre um novo
    if (ftell(mpg_split_output_file) + buffer_offset + 4 > max_size)
    {
      fclose(mpg_split_output_file);
      file_number++;
      sprintf(output_file_name, "%s%d.mpg", FILE_NAME, file_number);
      printf("Criando arquivo: %s ..\n", output_file_name);
      mpg_split_output_file = fopen(output_file_name, "wb");

      fwrite(buffer, buffer_offset, 1, mpg_split_output_file);
      buffer_offset = 0;
    }
    else
    {
      // se não, escreve o buffer no arquivo de saída
      fwrite(buffer, buffer_offset, 1, mpg_split_output_file);
      buffer_offset = 0;
    }

    if (feof(mpg_file))
    {
      break;
    }

    memcpy(buffer + buffer_offset, byte, 4);
    buffer_offset += 4;
  }

  free(buffer);
  fclose(mpg_file);
  fclose(mpg_split_output_file);
  return 0;
}