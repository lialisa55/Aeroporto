/*Banco de dados para Voos
Projeto final do curso de Introdução à Ciência da Computação I, 2024.
Autores: Joao Victor Alonso de Mello(turmaB), 
         Natália Monalisa Coelho Mota(turmaA),
         Renan Banci Catarin(turmaB)
Professor: Rudinei Goularte

Objetivo: o código a seguir foi criado com o intuito de informatizar
um sistema de cadastro de passageiros para uma companhia aérea.
Como as especificações do projeto ordenam, o sistema não é complexo,
funcionando apenas para um único voo de trecho simples,
além de a reserva de passageiros ser feita individualmente.
Também é possível Consultar, Modificar e Cancelar uma reserva

Sobre os voos, quando um dia é fechado os dados salvos podem ser reabertos em um outro dia para receberem modificações.
Porém, quando o voo é fechado os dados são salvos e nao pode ser mais modificado.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Criação de uma estrutura para armazenar informações de passageiros
struct Passageiro
{
    char nome[40], sobrenome[40], cpf[15], assento[6], classe[15], origem[4], destino[4], numero_voo[8];
    float valor;
    int data[3];
};
typedef struct Passageiro passageiro;

//Declaração das funções que serão usadas no código
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

/*
Essa função aloca strings, como por exemplo o nome do passageiro
Entrada: x - Valor que representas quantos caracteres serão alocados
Saída: b - Espaço alocado que será atibuido a string em questão
*/
char *alocar_string(int x){
    char *b = malloc(x * sizeof(char));
    if (b == NULL){
        printf("Erro: Falha na alocacao de memoria");
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
    if (x == 0){
        free(p);
        return NULL;
    }
    passageiro *b = realloc(p, x * sizeof(passageiro));
    if (b == NULL){
        printf("Erro: Falha na alocacao de memoria");
        exit(1);
    }
    return b;
}

/*
Chamada pelo comando RR,
função responsável por registrar um novo passageiro com dados passados pelos usuários
Entrada: nenhuma
Saída: persona - representa o novo passageiro com os valores corretos atgribuídos
*/
passageiro registrar_passageiro(void){
    passageiro persona;
    scanf("%s %s %s %d %d %d %s %s %s %f %s %s",
    persona.nome, persona.sobrenome, persona.cpf, &persona.data[0], &persona.data[1], &persona.data[2], persona.numero_voo, persona.assento, persona.classe, &persona.valor, persona.origem, persona.destino);
    return persona;
}

/*
Chamada pelo comando FV,
essa funcao fecha o voo e encerra o programa, imprimindo informacoes como o numero de passageiros e o valor total das passagens 
Entrada: fp - arquivo binário em questão
         informações_do_voo[4] - array com as informações básicas do voo registradas pelo comando AV
         tamanho_lista_passageiros - número de passageiros registrados para o voo
         *lista_passageiros - array contendo as structs de cada passageiro registrado
*/
void fechamentoVoo(FILE *fp, float informacoes_do_voo[4], int tamanho_lista_passageiros, passageiro *lista_passageiros) {
    printf("Voo Fechado!\n\n");
    salvarDados(fp, 1, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
    
    for(int i = 0; i < tamanho_lista_passageiros; i++) {
        imprimirPassageiro(lista_passageiros[i], "curto");
    }
    printf("Valor Total: %.2f\n", informacoes_do_voo[3]);
    printf("--------------------------------------------------");
}

/*
Funcao para consultar reserva, chamada pelo comando CR
Entrada: 
*/
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

/*
Funcao que retorna o indexamento de um cpf específico,
ela é útil para modificar ou deletar a reserva de um passageiro
Entrada: *cpf - o CPF que está sendo procurado
         tamanho_lista_passageiros - número de passageiros registrados para o voo
         *lista_passageiros - array contendo as structs de cada passageiro registrado
Saida: i - referente ao índice do passageiro, na lista de passageiros, com o CPF de entrada
       -1 - caso o CPF não tenha sido encontrado  
*/
int acharCPF(char * cpf, passageiro *lista_passageiros, int tamanho_lista_passageiros){
    for (int i = 0; i < tamanho_lista_passageiros; i++){
        if (!strcmp(lista_passageiros[i].cpf, cpf)){
            return i;
        }
    }
    return -1;
}

/*
Como o próprio nome da função diz ela salva os dados já registrados,
esse processo é feito em um arquivo binário passado como parametro da função
Entrada: *fp - arquivo binário
         fechado - 
         informações_do_voo[4] - array com as informações básicas do voo registradas pelo comando AV
         tamanho_lista_passageiros - número de passageiros registrados para o voo
         *lista_passageiros - array contendo as structs de cada passageiro registrado
*/
void salvarDados(FILE *fp, int fechado, float informacoes_do_voo[4], int tamanho_lista_passageiros, passageiro *lista_passageiros){
    fwrite(&fechado, sizeof(int), 1, fp);
    fwrite(informacoes_do_voo, sizeof(float), 4, fp);
    fwrite(&tamanho_lista_passageiros, sizeof(int), 1, fp);
    fwrite(lista_passageiros, sizeof(passageiro), tamanho_lista_passageiros, fp);
}

/*
Responsável por ler os dados salvos no arquivo binário,
ela serve para continuar o preenchimento dos dados de nvoos passageiros após um dia ter sido fechado
Entrada: *fp - arquivo binário 
         informações_do_voo[4] - array com as informações básicas do voo registradas pelo comando AV
         tamanho_lista_passageiros - número de passageiros registrados para o voo
         *lista_passageiros - array contendo as structs de cada passageiro registrado
Saída: 
*/
int lerDadosSalvos(FILE *fp, float informacoes_do_voo[4], int *tamanho_lista_passageiros, passageiro **lista_passageiros){
    int tmp;
    fread(&tmp, sizeof(int), 1, fp);
    fread(informacoes_do_voo, sizeof(float), 4, fp);
    fread(tamanho_lista_passageiros, sizeof(int), 1, fp);
    *lista_passageiros = realocar_passageiros(*lista_passageiros, *tamanho_lista_passageiros);
    fread(*lista_passageiros, sizeof(passageiro), *tamanho_lista_passageiros, fp);
    return tmp;
}

/*
Caso um cliente tenha cancelado a passagem essa função é chamada.
Ela é capaz de remover da lista de clientes um determinado indivíduo por procura de CPF,
se o indíce do CPF procurado não constar na lista (ser maior que ela ou menor que zero) a 
função simplesmente irá imprimir a mensagem "cpf inválido" e sair,
porém caso o índice for válido o passageiro em questão será removido, excluindo do valor total do voo o valor da sua passagem
e também movendo os indíces dos passageiros a frente para não haver "buracos" no array de clientes
*/
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

/*Função principal do programa*/
int main (void){
    float informacoes_do_voo[4] = {0};
    passageiro *lista_passageiros = NULL;
    int tamanho_lista_passageiros = 0;
    FILE *fp = fopen("dados.bin", "rb");
    if (fp != NULL){ //Caso o arquivo já esteja escrito
        printf("arquivo encontrado!\n");
        if (lerDadosSalvos(fp, informacoes_do_voo, &tamanho_lista_passageiros, &lista_passageiros)){
            fechamentoVoo(fp, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
            rewind(fp);
            fclose(fp);
            free(lista_passageiros);
            return 0;
        };
        rewind(fp);
        fclose(fp);
    }
    char comando[3] = "NA";
    while (strcmp(comando, "FD") != 0 && strcmp(comando, "FV") != 0){ //Esquanto os comandos de fechamento não forem chamados...
        scanf("%s", comando); //Ler comando do usuário
             
        if (!strcmp(comando, "AV")){ //Como essa função é básica ela está contida dentro da própria "main", ela grava as informações básicas do voo na chamada de AV
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
                rewind(fp);
                fclose(fp);  
            }
        }
        else if (!strcmp(comando, "MR")){ //Modificar Reserva
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
            printf("tamanho de lista: %ld\n", sizeof(passageiro) * tamanho_lista_passageiros);
            salvarDados(fp, 0, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
            rewind(fp);
            fclose(fp);
        }
        else if (!strcmp(comando, "FV")){
            FILE *fp = fopen("dados.bin", "wb");
            fechamentoVoo(fp, informacoes_do_voo, tamanho_lista_passageiros, lista_passageiros);
            rewind(fp);
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
