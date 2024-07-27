#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <winsock2.h>

int main(int argc, char const *argv[])
{
  // tipos de stream de vídeo
  const unsigned char Pack = 0xba;
  const unsigned char System = 0xbb;
  const unsigned char Sequence = 0xb3;
  const unsigned char GOP = 0xb8;
  const unsigned char Picture = 0x00;

  FILE *mpg_file = fopen(argv[1], "rb");
  if (mpg_file == NULL)
  {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  while (!feof(mpg_file))
  {
    // ler o arquivo de 3 em 3 bytes até achar "\x00\x00\x01" – em caracteres hexadecimais
    char buffer[3];
    fread(buffer, 3, 1, mpg_file);
    if (buffer[0] == 0x00 && buffer[1] == 0x00 && buffer[2] == 0x01)
    {
      // ler os próximos 2 bytes comparar com picture
      unsigned char byte = fgetc(mpg_file);
      if (byte == Sequence)
      {
        printf("--> Código : % .2x -- Sequence Header ", byte);
        unsigned char byte1 = fgetc(mpg_file);
        unsigned char byte2 = fgetc(mpg_file);
        unsigned char byte3 = fgetc(mpg_file);
        unsigned char byte4 = fgetc(mpg_file);
        unsigned int largura = byte1 * 16 + (byte2 >> 4);
        unsigned int altura = (byte2 & 0x0F) * 256 + byte3;
        unsigned int frame_rate = byte4 & 0x0f;
        double frame_rate_convertido;

        if (frame_rate == 1)
        {
          frame_rate_convertido = 23.976;
        }
        else if (frame_rate == 2)
        {
          frame_rate_convertido = 24;
        }
        else if (frame_rate == 3)
        {
          frame_rate_convertido = 25;
        }
        else if (frame_rate == 4)
        {
          frame_rate_convertido = 29.97;
        }
        else if (frame_rate == 5)
        {
          frame_rate_convertido = 30;
        }
        else if (frame_rate == 6)
        {
          frame_rate_convertido = 50;
        }
        else if (frame_rate == 7)
        {
          frame_rate_convertido = 59.94;
        }
        else if (frame_rate == 8)
        {
          frame_rate_convertido = 60;
        }
        else
        {
          frame_rate_convertido = 0;
        }

        printf("-- Width: %d, Height = %d -- Frame rate = %.3ffps\n", largura, altura, frame_rate_convertido);
      }
      else if (byte == Picture)
      {
        printf("--> Código : % .2x -- Picture ", byte);
        unsigned char byte1 = fgetc(mpg_file);
        unsigned char byte2 = fgetc(mpg_file);
        // pegar o bit 3, 4 e 5 do byte 2 para o tipo
        unsigned char tipo = (byte2 >> 3) & 0x07;
        // converter tipo para char
        char tipo_char;
        if (tipo == 1)
        {
          tipo_char = 'I';
        }
        else if (tipo == 2)
        {
          tipo_char = 'P';
        }
        else if (tipo == 3)
        {
          tipo_char = 'B';
        }
        else
        {
          tipo_char = ' ';
        }
        printf("-- Tipo = %c\n", tipo_char);
      }
      else if (byte == GOP)
      {
        printf("--> Código : % .2x -- Group of Pictures ", byte);
      }
      else if (byte == GOP)
      {
        printf("--> Código : % .2x -- Group of Pictures ", byte);
      }
      else
      {
        printf("-- > Código : % .2x --Tipo de stream não implementado\n", byte);
      }
    }
  }

  return 0;
}
