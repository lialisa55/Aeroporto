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

passageiro *realocar_passageiros(passageiro *p, int x){
    passageiro *b = realloc(p, x * sizeof(passageiro));
    if (b == NULL){
        printf("oops! hehe nao tenho memoria o suficiente senpai ;*_*;");
        exit(1);
    }
    return b;
}

/*chamado pelo comando AV*/
void registrar_novo_voo(float informacoes_do_voo[3]){
    scanf("%f %f %f", &informacoes_do_voo[0], &informacoes_do_voo[1], &informacoes_do_voo[2]);
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
passageiro registrar_passageiro(FILE *fp){ //se fp for NULL, use scanf normal
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
    if (fp != NULL){
        fscanf(fp, "%s %s %s %d %d %d %s %s %s %f %s %s",
        persona.nome, persona.sobrenome, persona.cpf, &persona.data[0], 
        &persona.data[1], &persona.data[2], persona.numero_voo, persona.assento, persona.classe, &persona.valor, persona.origem, persona.destino);
    }
    else{
        scanf("%s %s %s %d %d %d %s %s %s %f %s %s",
        persona.nome, persona.sobrenome, persona.cpf, &persona.data[0], 
        &persona.data[1], &persona.data[2], persona.numero_voo, persona.assento, persona.classe, &persona.valor, persona.origem, persona.destino);
    }
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

void imprimirPassageiro(passageiro pessoa){
    printf("%s\n", pessoa.cpf);
    printf("%s %s\n", pessoa.nome, pessoa.sobrenome);
    printf("%d/%d/%d\n", pessoa.data[0], pessoa.data[1], pessoa.data[2]);
    printf("Voo: %s\n", pessoa.numero_voo);
    printf("Assento: %s\n", pessoa.assento);
    printf("Classe: %s\n", pessoa.classe);
    printf("Trecho: %s %s\n", pessoa.origem, pessoa.destino);
    printf("Valor: %.02f\n", pessoa.valor);
}

void f_imprimirPassageiro(FILE *fp, passageiro pessoa){
    fprintf(fp, "%s %s ", pessoa.nome, pessoa.sobrenome);
    fprintf(fp, "%s ", pessoa.cpf);
    fprintf(fp, "%d %d %d ", pessoa.data[0], pessoa.data[1], pessoa.data[2]);
    fprintf(fp, "%s ", pessoa.numero_voo);
    fprintf(fp, "%s ", pessoa.assento);
    fprintf(fp, "%s ", pessoa.classe);
    fprintf(fp, "%.02f ", pessoa.valor);
    fprintf(fp, "%s %s ", pessoa.origem, pessoa.destino);
}

int acharCPF(char * cpf, passageiro *lista_passageiros, int tamanho_lista_passageiros){
    for (int i = 0; i < tamanho_lista_passageiros; i++){
        if (!strcmp(lista_passageiros[i].cpf, cpf)){
            return i;
        }
    }
    return -1;
}

int main (void){
    float informacoes_do_voo[4] = {0};
    passageiro *lista_passageiros = NULL;
    int tamanho_lista_passageiros = 0;
    FILE *fp = fopen("dados.txt", "r");
    if (fp != NULL){
        printf("arquivo encontrado!\n");
        fscanf(fp, "%f %f %f %f", &informacoes_do_voo[0], &informacoes_do_voo[1], &informacoes_do_voo[2], &informacoes_do_voo[3]);
        fscanf(fp, "%d", &tamanho_lista_passageiros);
        lista_passageiros = realocar_passageiros(lista_passageiros, tamanho_lista_passageiros);
        for (int i = 0; i < tamanho_lista_passageiros; i++){
            printf("alo");
            lista_passageiros[i] = registrar_passageiro(fp);
        }
        fclose(fp);
    }
    char comando[3] = "NA";
    while (strcmp(comando, "FD") != 0 && strcmp(comando, "FV") != 0){
        scanf("%s", comando);
        if (!strcmp(comando, "AV")){
            scanf("%f %f %f", &informacoes_do_voo[0], &informacoes_do_voo[1], &informacoes_do_voo[2]);
        }
        else if (!strcmp(comando, "RR")){
            tamanho_lista_passageiros++;
            lista_passageiros = realocar_passageiros(lista_passageiros, tamanho_lista_passageiros);
            lista_passageiros[tamanho_lista_passageiros - 1] = registrar_passageiro(NULL);
            informacoes_do_voo[3] += lista_passageiros[tamanho_lista_passageiros - 1].valor;
        }
        else if (!strcmp(comando, "CR")){
            char *cpf = malloc(100 * sizeof(char));
            scanf("%s", cpf);
            cpf = enxutar_string(cpf);
            int indice = acharCPF(cpf, lista_passageiros, tamanho_lista_passageiros);
            if (indice == -1) printf("cpf nao encontrado\n");
            else imprimirPassageiro(lista_passageiros[indice]);
            free(cpf);
        }
        else if (!strcmp(comando, "FD")){
            FILE *fp = fopen("dados.txt", "w");
            if (fp == NULL) printf("FP ta nulo!\n");
            fprintf(fp, "%.0f %.2f %.2f %.2f\n", informacoes_do_voo[0], informacoes_do_voo[1], informacoes_do_voo[2], informacoes_do_voo[3]);
            fprintf(fp, "%d\n", tamanho_lista_passageiros);
            printf("Fechamento do dia:\n");
            printf("Quantidade de reservas: %d\n", tamanho_lista_passageiros);
            printf("Posição: %f\n", informacoes_do_voo[3]);
            for (int i = 0; i < tamanho_lista_passageiros; i++){
                f_imprimirPassageiro(fp, lista_passageiros[i]);
            }
            fclose(fp);
        }
    }
    /*FILE *fp;
    fp = fopen("file.txt", "w+");
    char *instrucao;
    char *cpf[20];

    consultarcpf(cpf, fp);*/
}
