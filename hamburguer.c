//banco de dados para Voos
//autores: Renan Baci Catarin, Monalisa, joao mello, onlinegdb.com
//TODO: fazer a funcao fechar voo fechar permanentemente o voo, implementar o limite de reservas.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Passageiro
{
    char nome[40], sobrenome[40], cpf[15], assento[6], classe[15], origem[4], destino[4], numero_voo[8];
    float valor;
    int data[3];
};
typedef struct Passageiro passageiro;

void imprimirPassageiro(passageiro pessoa, char *modo);
char *alocar_string(int x);
passageiro *realocar_passageiros(passageiro *p, int x);
char *enxutarString(char *a);
passageiro registrar_passageiro();
void fechamentoVoo(FILE *fp, float informacoes_do_voo[4], int tamanho_lista_passageiros, passageiro *lista_passageiros);
void imprimirPassageiro(passageiro pessoa, char *modo);
int acharCPF(char * cpf, passageiro *lista_passageiros, int tamanho_lista_passageiros);
void salvarDados(FILE *fp, int fechado, float informacoes_do_voo[4], int tamanho_lista_passageiros, passageiro *lista_passageiros);
int lerDadosSalvos(FILE *fp, float informacoes_do_voo[4], int *tamanho_lista_passageiros, passageiro **lista_passageiros);

char *alocar_string(int x){
    char *b = malloc(x * sizeof(char));
    if (b == NULL){
        printf("oops! hehe nao tenho memoria o suficiente senpai ;*_*;");
        exit(1);
    }
    return b;
}

passageiro *realocar_passageiros(passageiro *p, int x){
    if (x == 0){
        free(p);
        return NULL;
    }
    passageiro *b = realloc(p, x * sizeof(passageiro));
    if (b == NULL){
        printf("oops! hehe nao tenho memoria o suficiente senpai ;*_*;");
        exit(1);
    }
    return b;
}

/*chamado pelo comando RR*/
passageiro registrar_passageiro(){
    passageiro persona;
    scanf("%s %s %s %d %d %d %s %s %s %f %s %s",
    persona.nome, persona.sobrenome, persona.cpf, &persona.data[0], &persona.data[1], &persona.data[2], persona.numero_voo, persona.assento, persona.classe, &persona.valor, persona.origem, persona.destino);
    return persona;
}

/*chamado pelo comando FV*/
void fechamentoVoo(FILE *fp, float informacoes_do_voo[4], int tamanho_lista_passageiros, passageiro *lista_passageiros) {
    printf("Voo Fechado!\n\n");
    //qtdPassageiros é obtido por uma váriavel que é incrimentada quando chama o comando RR;
    salvarDados(fp, 1, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
    //Ainda não sei como achar essa parte específica no arquivo;
    //MAS EU SEI KKKKKKKKKKKK
    
    for(int i = 0; i < tamanho_lista_passageiros; i++) {
        //Aqui tem que pegar os dados dos passageiros e printar eles. Juro que vou descobrir como;
        //eu descobri primeiro kkkkkkkk
        imprimirPassageiro(lista_passageiros[i], "curto");
    }
    printf("Valor Total: %.2f\n", informacoes_do_voo[3]);
    printf("--------------------------------------------------");
}

void imprimirPassageiro(passageiro pessoa, char *modo){
    if (!strcmp(modo, "curto")){
        printf("%s\n", pessoa.cpf);
        printf("%s %s\n", pessoa.nome, pessoa.sobrenome);
        printf("%s\n\n", pessoa.assento);
    }
    else{
        printf("%s\n", pessoa.cpf);
        printf("%s %s\n", pessoa.nome, pessoa.sobrenome);
        printf("%d/%d/%d\n", pessoa.data[0], pessoa.data[1], pessoa.data[2]);
        printf("Voo: %s\n", pessoa.numero_voo);
        printf("Assento: %s\n", pessoa.assento);
        printf("Classe: %s\n", pessoa.classe);
        printf("Trecho: %s %s\n", pessoa.origem, pessoa.destino);
        printf("Valor: %.02f\n", pessoa.valor);
    }
}

int acharCPF(char * cpf, passageiro *lista_passageiros, int tamanho_lista_passageiros){
    for (int i = 0; i < tamanho_lista_passageiros; i++){
        if (!strcmp(lista_passageiros[i].cpf, cpf)){
            return i;
        }
    }
    return -1;
}

void salvarDados(FILE *fp, int fechado, float informacoes_do_voo[4], int tamanho_lista_passageiros, passageiro *lista_passageiros){
    fwrite(&fechado, sizeof(int), 1, fp);
    fwrite(informacoes_do_voo, sizeof(float), 4, fp);
    fwrite(&tamanho_lista_passageiros, sizeof(int), 1, fp);
    fwrite(lista_passageiros, sizeof(passageiro), tamanho_lista_passageiros, fp);
}

int lerDadosSalvos(FILE *fp, float informacoes_do_voo[4], int *tamanho_lista_passageiros, passageiro **lista_passageiros){
    int tmp;
    fread(&tmp, sizeof(int), 1, fp);
    fread(informacoes_do_voo, sizeof(float), 4, fp);
    fread(tamanho_lista_passageiros, sizeof(int), 1, fp);
    *lista_passageiros = realocar_passageiros(*lista_passageiros, *tamanho_lista_passageiros);
    fread(*lista_passageiros, sizeof(passageiro), *tamanho_lista_passageiros, fp);
    return tmp;
}


void deletarCPF(int indice, passageiro **lista_passageiros, int *tamanho_lista_passageiros, float informacoes_do_voo[4]){
    if (indice < 0 || indice >= *tamanho_lista_passageiros){
        printf("cpf invalido\n");
        return;
    }
    informacoes_do_voo[3] -= (*lista_passageiros)[indice].valor;
    (*tamanho_lista_passageiros)--;
    for (int i = indice; i < *tamanho_lista_passageiros; i++) (*lista_passageiros)[i] = (*lista_passageiros)[i + 1];
    *lista_passageiros = realocar_passageiros(*lista_passageiros, *tamanho_lista_passageiros);
}

int main (void){
    float informacoes_do_voo[4] = {0};
    passageiro *lista_passageiros = NULL;
    int tamanho_lista_passageiros = 0;
    FILE *fp = fopen("dados.bin", "rb");
    if (fp != NULL){ //le dados salvos anteriormente (caso nao esteja claro o suficiente pra voce :)))))))))))))))))))))))))
        printf("arquivo encontrado!\n");
        if (lerDadosSalvos(fp, informacoes_do_voo, &tamanho_lista_passageiros, &lista_passageiros)){
            fechamentoVoo(fp, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
            fclose(fp);
            free(lista_passageiros);
            return 0;
        };
        fclose(fp);
    }
    char comando[3] = "NA";
    while (strcmp(comando, "FD") != 0 && strcmp(comando, "FV") != 0){
        scanf("%s", comando);
        if (!strcmp(comando, "AV")){
            scanf("%f %f %f", &informacoes_do_voo[0], &informacoes_do_voo[1], &informacoes_do_voo[2]);
        }
        else if (!strcmp(comando, "RR")){ //adiciona um passageiro a lista_passageiros
            tamanho_lista_passageiros++;
            lista_passageiros = realocar_passageiros(lista_passageiros, tamanho_lista_passageiros);
            lista_passageiros[tamanho_lista_passageiros - 1] = registrar_passageiro();
            informacoes_do_voo[3] += lista_passageiros[tamanho_lista_passageiros - 1].valor;
            if (tamanho_lista_passageiros == informacoes_do_voo[0]){
                strcpy(comando, "FV");
                FILE *fp = fopen("dados.bin", "wb");
                fechamentoVoo(fp, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
                fclose(fp);
            }
        }
        else if (!strcmp(comando, "MR")){
            char cpf[100];
            scanf("%s", cpf);
            int indice = acharCPF(cpf, lista_passageiros, tamanho_lista_passageiros);
            if (indice == -1){
                printf("cpf nao encontrado \n");
                continue;
            }
            scanf("%s %s %s %s", lista_passageiros[indice].nome, lista_passageiros[indice].sobrenome, lista_passageiros[indice].cpf, lista_passageiros[indice].assento);
            printf("Reserva Modificada:\n");
            imprimirPassageiro(lista_passageiros[indice], "longo");
            printf("--------------------------------------------------\n");
        }
        else if (!strcmp(comando, "CR")){ //encontra um cpf e imprime as informacoes de seu dono
            char cpf[100];
            scanf("%s", cpf);
            int indice = acharCPF(cpf, lista_passageiros, tamanho_lista_passageiros);
            if (indice == -1) printf("\ncpf nao encontrado\n");
            else imprimirPassageiro(lista_passageiros[indice], "longo");
            printf("--------------------------------------------------\n");
        }
        else if (!strcmp(comando, "CA")){
            char cpf[100];
            scanf("%s", cpf);
            int indice = acharCPF(cpf, lista_passageiros, tamanho_lista_passageiros);
            deletarCPF(indice, &lista_passageiros, &tamanho_lista_passageiros, informacoes_do_voo);
        }
        else if (!strcmp(comando, "FD")){ //encerra o programa e salva as informacoes obtidas
            FILE *fp = fopen("dados.bin", "wb");
            if (fp == NULL) printf("FP ta nulo!\n");
            printf("Fechamento do dia:\n");
            printf("Quantidade de reservas: %d\n", tamanho_lista_passageiros);
            printf("Posição: %f\n", informacoes_do_voo[3]);
            printf("--------------------------------------------------");
            salvarDados(fp, 0, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
            fclose(fp);
        }
        else if (!strcmp(comando, "FV")){
            FILE *fp = fopen("dados.bin", "wb");
            fechamentoVoo(fp, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
            fclose(fp);
        }
    }
    if (lista_passageiros != NULL) free(lista_passageiros);
}
/*AV 200 1200.00 2500.00
RR Carlos Massa 555.555.333-99 12 12 2024
V001 A27 economica 1200.00 CGH RAO
RR Maria Massa 444.555.333-93 12 12 2024
V001 A31 economica 1200.00 CGH RAO
RR Roberto Carlos 555.333.333-89 12 12 2024
V001 P12 executiva 2500.00 CGH RAO
MR 555.555.333-99 Carlos Massa 555.555.333-99 A30
FD*/
