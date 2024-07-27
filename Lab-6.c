#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct zip_file_hdr // Crie a estrutura como mostrado
{
    int signature;
    short version;
    short flags;
    short compression;
    short mod_time;
    short mod_date;
    int crc;
    int compressed_size;
    int uncompressed_size;
    short name_length;
    short extra_field_length;
} __attribute__((packed));

int main(int argc, char **argv)
{
    FILE *zip_file = fopen(argv[1], "rb"); // Abra o arquivo ZIP (fopen – ver exemplo)

    struct zip_file_hdr *file_hdr = malloc(sizeof(struct zip_file_hdr)); // Aloque uma memória para a estrutura criada (malloc – ver exemplo)

    int countArquivo = 0;

    while (fread(file_hdr, sizeof(struct zip_file_hdr), 1, zip_file) == 1)
    {
        if (file_hdr->signature != 0x04034b50) //
        {
            break;
        }
        countArquivo++;
        char *file_name = malloc(file_hdr->name_length + 1);
        fread(file_name, 1, file_hdr->name_length, zip_file) != file_hdr->name_length;
        file_name[file_hdr->name_length] = '\0'; // Adicione o caractere \0 ao final da string com o nome do arquivo

        printf("Arquivo %d ..\n", countArquivo);
        printf(" --> Nome do Arquivo: %s\n", file_name);
        printf(" --> Assinatura: %.8x\n", file_hdr->signature); // Imprime hexadecimal
        printf(" --> Tamanho Compactado: %d\n", file_hdr->compressed_size);

        free(file_name);
        fseek(zip_file, file_hdr->extra_field_length + file_hdr->compressed_size, SEEK_CUR);
    }
    free(file_hdr);
    fclose(zip_file);

    return 0;
}
