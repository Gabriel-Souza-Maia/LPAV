#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>  // Biblioteca para tipos uint8_t e uint16_t
#include <winsock2.h> // Biblioteca para conversão de ordem de bytes em Windows

#pragma comment(lib, "ws2_32.lib") // Link com a biblioteca winsock2

typedef struct {
    uint8_t daddr[6]; // Endereco MAC de destino
    uint8_t saddr[6]; // Endereco MAC de origem (source)
    uint16_t protocol; // Protocolo da próxima camada (IP!)
} ethernet_hdr_t;

typedef struct {
    uint8_t hdr_len:4, version:4; // Tamanho do Cabeçalho, Versão do IP
    uint8_t tos; // Tipo de serviço
    uint16_t tot_len; // Tamanho total do IP
    uint16_t id; // Id do pacote
    uint16_t frag_off; // Fragmentado?
    uint8_t ttl; // Tempo de vida
    uint8_t protocol; // Protocolo próxima camada (TCP!)
    uint16_t check; // Checksum
    uint8_t saddr[4]; // Endereço IP de origem
    uint8_t daddr[4]; // Endereço IP de destino
} ip_hdr_t;

typedef struct {
    uint16_t sport; // Porta TCP de origem
    uint16_t dport; // Porta TCP de destino
    uint32_t seq; // Sequência
    uint32_t ack_seq; // Acknowledgement
    uint8_t reservado:4, hdr_len:4; // Não usado, Tamanho do cabecalho
    uint8_t flags; // Flags do TCP
    uint16_t window; // Tamanho da janela
    uint16_t check; // Checksum
    uint16_t urg_ptr; // Urgente
} tcp_hdr_t;

int main(int argc, char **argv) {

    FILE *Ethernet_file = fopen(argv[1], "rb"); // Abre o arquivo Ethernet

    ethernet_hdr_t *ethernet_hdr = malloc(sizeof(ethernet_hdr_t)); // Aloca a Memoria para ler o header da camada da Ethernet
    ip_hdr_t *ip_hdr = malloc(sizeof(ip_hdr_t)); // Aloca a Memoria para ler o header da camada IP
    tcp_hdr_t *tcp_hdr = malloc(sizeof(tcp_hdr_t)); // Aloca a Memoria para ler o header da camada TCP

    printf("Lendo Ethernet ..\n");

    // Lendo a Camada da Ethernet
    fread(ethernet_hdr, sizeof(ethernet_hdr_t), 1, Ethernet_file);

    // IMPRIMINDO ENDEREÇO ORIGEM
    printf("--> MAC de Origem: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
           ethernet_hdr->saddr[0], ethernet_hdr->saddr[1], ethernet_hdr->saddr[2],
           ethernet_hdr->saddr[3], ethernet_hdr->saddr[4], ethernet_hdr->saddr[5]);
    // IMPRIMINDO ENDEREÇO DESTINO
    printf("--> MAC de Destino: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
           ethernet_hdr->daddr[0], ethernet_hdr->daddr[1], ethernet_hdr->daddr[2],
           ethernet_hdr->daddr[3], ethernet_hdr->daddr[4], ethernet_hdr->daddr[5]);

    printf("Lendo IP ..\n");
    // Lendo a Camada IP
    fread(ip_hdr, sizeof(ip_hdr_t), 1, Ethernet_file);

    // IMPRIMINDO VERSÃO E TAMANHO DO CABEÇALHO
    printf("--> Versão do IP: %d\n", ip_hdr->version);
    printf("--> Tamanho do Cabeçalho: %d bytes\n", ip_hdr->hdr_len * 4);

    // IMPRIMINDO TAMANHO TOTAL 
    printf("--> Tamanho do pacote: %d bytes\n", ntohs(ip_hdr->tot_len));

    // IMPRIMINDO ENDEREÇO DE IP DE ORIGEM E DESTINO
    printf("--> Endereço IP de Origem: %d.%d.%d.%d\n",
           ip_hdr->saddr[0], ip_hdr->saddr[1], ip_hdr->saddr[2], ip_hdr->saddr[3]);
    printf("--> Endereço IP de Destino: %d.%d.%d.%d\n",
           ip_hdr->daddr[0], ip_hdr->daddr[1], ip_hdr->daddr[2], ip_hdr->daddr[3]);

    //Para pular as Opções do IP, que possuem tamanho variável, você precisará pular (fseek) ip_hdr.hdr_len*4 – sizeof(ip_hdr_t) bytes
    fseek(Ethernet_file, ip_hdr->hdr_len*4 - sizeof(ip_hdr_t), SEEK_CUR);


    printf("Lendo TCP ..\n");
    // Lendo a Camada TCP
    fread(tcp_hdr, sizeof(tcp_hdr_t), 1, Ethernet_file);

    // IMPRIMINDO PORTAS
    printf("--> Porta de Origem: %d\n", ntohs(tcp_hdr->sport));
    printf("--> Porta de Destino: %d\n", ntohs(tcp_hdr->dport));
    // IMPRIMINDO TAMANHO DO CABEÇALHO
    printf("--> Tamanho do Cabeçalho: %d bytes\n", tcp_hdr->hdr_len * 4);

    //◦ Para pular as Opções do TCP, que possuem tamanho variável, você precisará pular (fseek) tcp_hdr.hdr_len*4 - sizeof(tcp_hdr_t) bytes.

    fseek(Ethernet_file,tcp_hdr->hdr_len*4 - sizeof(tcp_hdr_t), SEEK_CUR);

    printf("Lendo Dados (HTTP) ..\n");

    //IMPRIMINDO TAMANHO DOS DADOS
    int tam_dados = ntohs(ip_hdr->tot_len) - (ip_hdr->hdr_len * 4) - (tcp_hdr->hdr_len * 4);
    printf("--> Tamanho dos dados HTTP: %d bytes\n", tam_dados);

    //IMPRIMINDO DADOS

    printf("--> Dados:\n");
        for (int i = 0; i < tam_dados; i++) {
      int c = fgetc(Ethernet_file);
      if (c == EOF) {
        break;
      }
    putchar(c);
    }


    free(ethernet_hdr);
    free(ip_hdr);
    free(tcp_hdr);
    fclose(Ethernet_file);

    return 0;
}
