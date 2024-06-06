//fcsadfasfasfas
#include <stdio.h>
struct Passageiro
{
    char* nome, sobrenome, cpf, assento, classe, origem, destino, numero_voo;
    float valor;
    int data[3];
}
typedef struct Passageiro passageiro;

void fechamentoDia(FILE *arquivo, int qtdReservas) {
    arquivo = fopen(nomeDoArquivo, "r");
    int valorTotal
    fscanf(arquivo, "%d", &valorTotal);
    //Ainda não sei como achar essa parte específica no arquivo, acontece neh, kk;
    printf("--------------------------------------------------\n");
    printf("Fechamento do dia:\n");
    printf("Quantidade de reservas: %d\n", qtdReservas);
    printf("Posição: %.2f\n", valorTotal);
    printf("--------------------------------------------------");
}
