/*
Banco de dados para Voos
Autores: Joao Victor Alonso de Mello(turmaB), 
         Natália Monalisa Coelho Mota(turmaA),
         Renan Banci Catarin(turmaB) (2004)

Objetivo: o código a seguir foi criado com o intuito de informatizar
um sistema de cadastro de passageiros para uma companhia aérea.
Como as especificações do projeto ordenam, o sistema não é complexo,
funcionando apenas para um único voo de trecho simples,
além de a reserva de passageiros ser feita individualmente.
Também é possível Consultar, Modificar e Cancelar uma reserva

Sobre os voos, quando um dia é fechado os dados salvos podem ser reabertos em um outro dia para receberem modificações.
Porém, quando o voo é fechado os dados são salvos e nao pode ser mais modificado.
*/

//TODO: fazer a funcao fechar voo fechar permanentemente o voo, implementar o limite de reservas.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Passageiro { /*Dado do tipo passageiro com suas especificações*/
    char *nome, *sobrenome, *cpf, *assento, *classe, *origem, *destino, *numero_voo;
    float valor;
    int data[3];
};
typedef struct Passageiro passageiro;

/*Delacração prévia das funções usadas no código*/
char *alocar_string(int x);
char *enxutarString(char *a);
passageiro *realocar_passageiros(passageiro *p, int x);
void registrar_novo_voo(float informacoes_do_voo[3]); //Ainda não finalizado
passageiro registrar_passageiro(FILE *fp);
void imprimirPassageiro(passageiro pessoa, char *modo);
int acharCPF(char * cpf, passageiro *lista_passageiros, int tamanho_lista_passageiros);
void fechamentoDia(FILE *arquivo, int qtdReservas); // Ainda não finalizado
void deletarCPF(int indice, passageiro **lista_passageiros, int *tamanho_lista_passageiros, float informacoes_do_voo[4]) //Ainda não finalizado
void fechamentoVoo(float informacoes_do_voo[4], int tamanho_lista_passageiros, passageiro *lista_passageiros);
void deletarCPF(int indice, passageiro **lista_passageiros, int *tamanho_lista_passageiros, float informacoes_do_voo[4])
void imprimirPassageiro(passageiro pessoa, char *modo);
void f_imprimirPassageiro(FILE *fp, passageiro pessoa);
void salvarDados(FILE *fp, float informacoes_do_voo[4], int tamanho_lista_passageiros, passageiro *lista_passageiros);
void lerDadosSalvos(FILE *fp, float informacoes_do_voo[4], int *tamanho_lista_passageiros, passageiro **lista_passageiros);

/*
Essa função aloca strings, como por exemplo o nome do passageiro
Entrada: x - Valor que representas quantos caracteres serão alocados
Saída: b - Espaço alocado que será atibuido a string em questão
*/
char *alocar_string(int x){
    char *b = malloc(x * sizeof(char));
    if (b == NULL){
        printf("oops! hehe nao tenho memoria o suficiente senpai ;*_*;");
        exit(1);
    }
    return b;
}

/*
Essa função realoca o vetor de structs passageiro,
Quando o comando RR é chamado antes de executar a função de registrar
um passageiro o vetor é realocado para que possa ser liberado espaço na memória 
Entradas: *p - Representa a lista de passageiros
           x - tamanho a ser realocado
Saída: b - Espaço realocado que será atibuido ao vetor em questão
*/
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

/*
Essa função realoca strings, por exemplo o nome do passageiro, para não ocupar mais memória que o necessário
Entrada: a - String a ser realocada
Saída: b - Espaço realocado que será atibuido a string em questão
*/
char *enxutarString(char *a){
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
    persona.nome = enxutarString(persona.nome);
    persona.sobrenome = enxutarString(persona.sobrenome);
    persona.cpf = enxutarString(persona.cpf);
    persona.numero_voo = enxutarString(persona.numero_voo);
    persona.assento = enxutarString(persona.assento);
    persona.classe = enxutarString(persona.classe);
    persona.origem = enxutarString(persona.origem);
    persona.destino = enxutarString(persona.destino);
    persona.numero_voo = enxutarString(persona.numero_voo);
    return persona;
}

/*chamado pelo comando FV*/
void fechamentoVoo(float informacoes_do_voo[4], int tamanho_lista_passageiros, passageiro *lista_passageiros) {
    //qtdPassageiros é obtido por uma váriavel que é incrimentada quando chama o comando RR;
    FILE *fp = fopen("dados.txt", "w");
    salvarDados(fp, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
    //Ainda não sei como achar essa parte específica no arquivo;
    //MAS EU SEI KKKKKKKKKKKK
    
    for(int i = 0; i < tamanho_lista_passageiros; i++) {
        //Aqui tem que pegar os dados dos passageiros e printar eles. Juro que vou descobrir como;
        //eu descobri primeiro kkkkkkkk
        imprimirPassageiro(lista_passageiros[i], "curto");
    }
    printf("Valor Total: %.2f\n", informacoes_do_voo[3]);
    printf("--------------------------------------------------");
    fclose(fp);
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

void salvarDados(FILE *fp, float informacoes_do_voo[4], int tamanho_lista_passageiros, passageiro *lista_passageiros){
    fprintf(fp, "%.0f %.2f %.2f %.2f\n", informacoes_do_voo[0], informacoes_do_voo[1], informacoes_do_voo[2], informacoes_do_voo[3]);
    fprintf(fp, "%d\n", tamanho_lista_passageiros);
    for (int i = 0; i < tamanho_lista_passageiros; i++){
        f_imprimirPassageiro(fp, lista_passageiros[i]);
    }
}

void lerDadosSalvos(FILE *fp, float informacoes_do_voo[4], int *tamanho_lista_passageiros, passageiro **lista_passageiros){
    fscanf(fp, "%f %f %f %f", &informacoes_do_voo[0], &informacoes_do_voo[1], &informacoes_do_voo[2], &informacoes_do_voo[3]);
    fscanf(fp, "%d", tamanho_lista_passageiros);f
    *lista_passageiros = realocar_passageiros(*lista_passageiros, *tamanho_lista_passageiros);
    for (int i = 0; i < *tamanho_lista_passageiros; i++){
        (*lista_passageiros)[i] = registrar_passageiro(fp);
    }
}


void deletarCPF(int indice, passageiro **lista_passageiros, int *tamanho_lista_passageiros, float informacoes_do_voo[4]){
    if (indice < 0 || indice >= *tamanho_lista_passageiros){
        printf("cpf invalido\n");
        return;
    }
    informacoes_do_voo[3] -= (*lista_passageiros)[indice].valor;
    (*tamanho_lista_passageiros)--;
    (*lista_passageiros)[indice] = (*lista_passageiros)[*tamanho_lista_passageiros];
    if (*tamanho_lista_passageiros > 1) *lista_passageiros = realocar_passageiros(*lista_passageiros, *tamanho_lista_passageiros);
    else{
        free(*lista_passageiros);
        *lista_passageiros = NULL;
    }
}

int main (void){
    float informacoes_do_voo[4] = {0};
    passageiro *lista_passageiros = NULL;
    int tamanho_lista_passageiros = 0;
    FILE *fp = fopen("dados.txt", "r");
    if (fp != NULL){ //le dados salvos anteriormente (caso nao esteja claro o suficiente pra voce :)))))))))))))))))))))))))
        printf("arquivo encontrado!\n");
        lerDadosSalvos(fp, informacoes_do_voo, &tamanho_lista_passageiros, &lista_passageiros);
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
            lista_passageiros[tamanho_lista_passageiros - 1] = registrar_passageiro(NULL);
            informacoes_do_voo[3] += lista_passageiros[tamanho_lista_passageiros - 1].valor;
            if (tamanho_lista_passageiros == informacoes_do_voo[0]){
                comando = "FD";
                fechamentoVoo(informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
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
            free(lista_passageiros[indice].nome);
            free(lista_passageiros[indice].sobrenome);
            lista_passageiros[indice].nome = alocar_string(100);
            lista_passageiros[indice].sobrenome = alocar_string(100);
            scanf("%s %s %s %s", lista_passageiros[indice].nome, lista_passageiros[indice].sobrenome, lista_passageiros[indice].cpf, lista_passageiros[indice].assento);
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
            FILE *fp = fopen("dados.txt", "w");
            if (fp == NULL) printf("FP ta nulo!\n");
            printf("Fechamento do dia:\n");
            printf("Quantidade de reservas: %d\n", tamanho_lista_passageiros);
            printf("Posição: %f\n", informacoes_do_voo[3]);
            printf("--------------------------------------------------");
            salvarDados(fp, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
            fclose(fp);
        }
        else if (!strcmp(comando, "FV")){
            fechamentoVoo(informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
        }

        return 0;
    }
    /*FILE *fp;
    fp = fopen("file.txt", "w+");
    char *instrucao;
    char *cpf[20];

    consultarcpf(cpf, fp);*/
}
