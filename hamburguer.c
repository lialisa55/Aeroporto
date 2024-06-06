//fcsadfasfasfas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Passageiro
{
    char *nome, *sobrenome, *cpf, *assento, *classe, *origem, *destino, *numero_voo;
    float valor;
    int data[3];
};
typedef struct Passageiro passageiro;

char *alocar_string(int x){
    char *b = malloc(x * sizeof(char));
    if (b == NULL){
        printf("oops! hehe nao tenho memoria o suficiente senpai ;*_*;");
        exit(1);
    }
    return b;
}

/*chamado pelo comando AV*/
void registrar_novo_voo(int *assentos, float *valor_da_economica, float *valor_BUSINESS){
    scanf("%d %f %f", assentos, valor_da_economica, valor_BUSINESS);
    //insira jeito de registrar num arquivo
}

char *enxutar_string(char *a){
    char *b = realloc(a, (strlen(a) + 1) * sizeof(char));
    if (b == NULL){
        printf("OWO sumimasen senpai-san! nao tenho memoria o suficiente UWU");
        exit(1);
    }
    return b;
}
/*chamado pelo comando RR*/
passageiro registrar_passageiro(){
    passageiro persona;
    persona.nome = alocar_string(100);
    persona.sobrenome = alocar_string(100);
    persona.cpf = alocar_string(100);
    persona.numero_voo = alocar_string(100);
    persona.assento = alocar_string(100);
    persona.classe = alocar_string(100);
    persona.origem = alocar_string(100);
    persona.destino = alocar_string(100);
    persona.numero_voo = alocar_string(100);
    scanf("%s %s %s %d %d %d %s %s %s %f %s %s",
        persona.nome, persona.sobrenome, persona.cpf, &persona.data[0], 
        &persona.data[1], &persona.data[2], persona.numero_voo, persona.assento, persona.classe, &persona.valor, persona.origem, persona.destino);
    persona.nome = enxutar_string(persona.nome);
    persona.sobrenome = enxutar_string(persona.sobrenome);
    persona.cpf = enxutar_string(persona.cpf);
    persona.numero_voo = enxutar_string(persona.numero_voo);
    persona.assento = enxutar_string(persona.assento);
    persona.classe = enxutar_string(persona.classe);
    persona.origem = enxutar_string(persona.origem);
    persona.destino = enxutar_string(persona.destino);
    persona.numero_voo = enxutar_string(persona.numero_voo);
    return persona;
}

/*chamado pelo comando FV*/
void fechamentoVoo(FILE *arquivo, int qtdPassageiros) {
    //qtdPassageiros é obtido por uma váriavel que é incrimentada quando chama o comando RR;
    arquivo = fopen("nomeDoArquivo.txt", "r");
    float valorTotal;
    fscanf(arquivo, "%f", &valorTotal);
    //Ainda não sei como achar essa parte específica no arquivo;
    printf("--------------------------------------------------\n");
    printf("Voo Fechado!\n");
    for(int i = 0; i < qtdPassageiros; i++) {
        //Aqui tem que pegar os dados dos passageiros e printar eles. Juro que vou descobrir como;
    }
    printf("Valor Total: %.2f\n", valorTotal);
    printf("--------------------------------------------------");
    fclose(arquivo);
}

/*chamado pelo comando FD*/
void fechamentoDia(FILE *arquivo, int qtdReservas) {
     //qtdReservas é obtido por uma váriavel que é incrimentada quando chama o comando RR;
    arquivo = fopen("nomeDoArquivo.txt", "r");
    float valorTotal;
    fscanf(arquivo, "%f", &valorTotal);
    //Ainda não sei como achar essa parte específica no arquivo;
    printf("--------------------------------------------------\n");
    printf("Fechamento do dia:\n");
    printf("Quantidade de reservas: %d\n", qtdReservas);
    printf("Posição: %.2f\n", valorTotal);
    printf("--------------------------------------------------");
    fclose(arquivo);
}

int main()
{
    passageiro persona = registrar_passageiro();
    printf("%s %s %s %d %d %d %s %s %s %.02f %s %s",
        persona.nome, persona.sobrenome, persona.cpf, persona.data[0], 
        persona.data[1], persona.data[2], persona.numero_voo, persona.assento, persona.classe, persona.valor, persona.origem, persona.destino);

    return 0;
}

