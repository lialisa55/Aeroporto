/*Banco de dados para Voosfiz 
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
typedef struct passageiro{
    char *nome, *sobrenome, *assento, *classe, *origem, *destino, *numero_voo;
    char cpf[15]; //O CPF está estático porque sempre vai ter o mesmo comprimento
    float valor;
    int data[3];
} Passageiro;

//Declaração das funções que serão usadas no código
//Precisam ser modificadas
//int lerDadosSalvos(FILE *fp, float informacoes_do_voo[4], int *tamanho_lista_passageiros, Passageiro **lista_passageiros);
//void salvarDados(FILE *fp, int fechado, float informacoes_do_voo[4], int tamanho_lista_passageiros, Passageiro *lista_passageiros);

//funções de suporte
char *alocStr(int x);
char *realocStr(char *a);
Passageiro lerPassageiro();
Passageiro *realocPassageiros(Passageiro *p, int x);
void imprimirPassageiro(Passageiro pessoa, char *modo);
int acharCPF(char cpf[15], Passageiro *lista, int n);
void liberarMemoria(Passageiro *lista, int n);

//funções primárias
void abrirVoo(float infoVoo[4]);
void registrarPassageiro(Passageiro **lista , int *n, float infoVoo[4]);
void consultarReserva(Passageiro *lista, int n);
void cancelarReserva(Passageiro **lista ,int *n, float infoVoo[4]);
void modificarReserva(Passageiro *lista, int n);
void fecharDia(Passageiro *lista, int n, float infoVoo[4]);
void fecharVoo(Passageiro *lista, int n, float infoVoo[4]);

/*
Como o próprio nome da função diz ela salva os dados já registrados,
esse processo é feito em um arquivo binário passado como parametro da função
Entrada: *fp - arquivo binário
         fechado - 
         informações_do_voo[4] - array com as informações básicas do voo registradas pelo comando AV
         tamanho_lista_passageiros - número de passageiros registrados para o voo
         *lista_passageiros - array contendo as structs de cada passageiro registrado
*/
void salvarDados(FILE *fp, int fechado, float informacoes_do_voo[4], int tamanho_lista_passageiros, Passageiro *lista_passageiros){
     fwrite(&fechado, sizeof(int), 1, fp);
     fwrite(informacoes_do_voo, sizeof(float), 4, fp);
     fwrite(&tamanho_lista_passageiros, sizeof(int), 1, fp);
     fwrite(lista_passageiros, sizeof(Passageiro), tamanho_lista_passageiros, fp);

     for (int i = 0; i < tamanho_lista_passageiros; i++){
	     int tam_nome = strlen(lista_passageiros[i].nome);
	     int tam_sobrenome = strlen(lista_passageiros[i].sobrenome);
	     int tam_assento = strlen(lista_passageiros[i].assento);

	     fwrite(&tam_nome, sizeof(int), 1, fp);
	     fwrite(lista_passageiros[i].nome, 1, tam_nome, fp);

	     fwrite(&tam_sobrenome, sizeof(int), 1, fp);
	     fwrite(lista_passageiros[i].sobrenome, 1, tam_sobrenome, fp);

	     fwrite(lista_passageiros[i].cpf, 1, 15, fp);

	     fwrite(&tam_assento, sizeof(int), 1, fp);
	     fwrite(lista_passageiros[i].assento, 1, tam_assento, fp);

	     fwrite(&lista_passageiros[i].classe, sizeof(int), 1, fp);
     }
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
int lerDadosSalvos(FILE *fp, float informacoes_do_voo[4], int *tamanho_lista_passageiros, Passageiro **lista_passageiros){
     int tmp;
     fread(&tmp, sizeof(int), 1, fp);
     fread(informacoes_do_voo, sizeof(float), 4, fp);
     fread(tamanho_lista_passageiros, sizeof(int), 1, fp);
     *lista_passageiros = realocPassageiros(*lista_passageiros, *tamanho_lista_passageiros);
     fread(*lista_passageiros, sizeof(Passageiro), *tamanho_lista_passageiros, fp);
     return tmp;
 }

/*Função principal do programa*/
int main (void){
    float informacoes_do_voo[4] = {0};
    Passageiro *lista_passageiros = NULL;
    int tamanho_lista_passageiros = 0;

    FILE *fp = fopen("dados.bin", "rb");

    if (fp != NULL){ //Caso o arquivo já esteja escrito
        printf("arquivo encontrado!\n"); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         if (lerDadosSalvos(fp, informacoes_do_voo, &tamanho_lista_passageiros, &lista_passageiros)){
             fecharVoo(lista_passageiros, tamanho_lista_passageiros, informacoes_do_voo);
             rewind(fp);
             fclose(fp);
             free(lista_passageiros);
             return 0;
         };
        rewind(fp);
        fclose(fp);
    }

    char comando[3] = "NA";
    while (1){ 
        scanf("%s", comando); //Ler comando do usuário
             
        if (!strcmp(comando, "AV")){ 
            abrirVoo(informacoes_do_voo);
        } 
        if (!strcmp(comando, "RR")){ //adiciona um passageiro a lista_passageiros
           registrarPassageiro(&lista_passageiros, &tamanho_lista_passageiros, informacoes_do_voo);
        }
        if (!strcmp(comando, "MR")){ //Modificar Reserva
            modificarReserva(lista_passageiros, tamanho_lista_passageiros);
        }
        if (!strcmp(comando, "CR")){ //Consulta Reserva
            consultarReserva(lista_passageiros, tamanho_lista_passageiros);
        }
        if (!strcmp(comando, "CA")){ //Cancela Reserva
            cancelarReserva(&lista_passageiros, &tamanho_lista_passageiros, informacoes_do_voo);
        }
        if (!strcmp(comando, "FD")){ //Encerra o programa e salva as informacoes obtidas
            fecharDia(lista_passageiros, tamanho_lista_passageiros, informacoes_do_voo);
        }
        if (!strcmp(comando, "FV")){ //Encerra o programa e salva as informações obtidas
            fecharVoo(lista_passageiros, tamanho_lista_passageiros, informacoes_do_voo);
        }
    }
    if (lista_passageiros != NULL) free(lista_passageiros);

    return 0;
}

/*
Essa função aloca strings, como por exemplo o nome do passageiro
Entrada: x - Valor que representas quantos caracteres serão alocados
Saída: b - Espaço alocado que será atibuido a string em questão
*/
char *alocStr(int x){
    char *b = malloc(x * sizeof(char));
    if (b == NULL){
        printf("Erro: Falha na alocacao de memoria");
        exit(1);
    }
    return b;
}

/*
Essa função realoca strings, por exemplo o nome do passageiro, para não ocupar mais memória que o necessário
Entrada: a - String a ser realocada
Saída: b - Espaço realocado que será atibuido a string em questão
*/
char *realocStr(char *a){
    char *b = realloc(a, (strlen(a) + 1) * sizeof(char));
    if (b == NULL){
        printf("OWO sumimasen senpai-san! nao tenho memoria o suficiente UWU");
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
Passageiro *realocPassageiros(Passageiro *p, int x){
    if (x == 0){
        free(p);
        return NULL;
    }
    Passageiro *b = realloc(p, x * sizeof(Passageiro));
    if (b == NULL){
        printf("Erro: Falha na alocacao de memoria");
        exit(1);
    }
    return b;
}

void liberarMemoria(Passageiro *lista, int n) {
    for(int i = 0; i < n; i++) {
        free(lista[i].nome);
        free(lista[i].sobrenome);
        free(lista[i].assento);
        free(lista[i].classe);
        free(lista[i].origem);
        free(lista[i].destino);
        free(lista[i].numero_voo);
    }
    free(lista);
}

void abrirVoo(float infoVoo[4]) {
    scanf("%f %f %f", &infoVoo[0], &infoVoo[1], &infoVoo[2]);
}

void registrarPassageiro(Passageiro **lista , int *n, float infoVoo[4]) {
    (*n)++;
    *lista = realocPassageiros(*lista, *n);
    (*lista)[*n - 1] = lerPassageiro();
    infoVoo[3] += (*lista)[*n - 1].valor;
    if (*n == infoVoo[0]){
        fecharVoo(*lista, *n, infoVoo);  
    }
}

void consultarReserva(Passageiro *lista, int n) {
    char cpfBuscado[15];
    scanf(" %s", cpfBuscado);

    int indice = acharCPF(cpfBuscado, lista, n);

    if (indice == -1) {
        printf("\ncpf nao encontrado\n");
    } else {
        imprimirPassageiro(lista[indice], "longo");
    } 
    printf("--------------------------------------------------\n");
}

//Não testei, talvez funcione só com *lista
//Precisa liberar a memória???
void cancelarReserva(Passageiro **lista ,int *n, float infoVoo[4]) {
    char cpfBuscado[15];
    scanf("%s", cpfBuscado);

    int indice = acharCPF(cpfBuscado, *lista, *n);
    
    if (indice < 0 || indice >= *n){
        printf("cpf invalido\n");
        return;
    }

    infoVoo[3] -= (*lista)[indice].valor;
    (*n)--;

    for (int i = indice; i < *n; i++) (*lista)[i] = (*lista)[i + 1];
    *lista = realocPassageiros(*lista, *n);
}

void modificarReserva(Passageiro *lista, int n) {
    char cpfBuscado[15];
    scanf("%s", cpfBuscado);

    int indice = acharCPF(cpfBuscado, lista, n);
    if (indice == -1){
        printf("cpf nao encontrado \n");
        return;
    }

    scanf("%s %s %s %s", lista[indice].nome, lista[indice].sobrenome, lista[indice].cpf, lista[indice].assento);
    printf("Reserva Modificada:\n");
    imprimirPassageiro(lista[indice], "longo");
    printf("--------------------------------------------------\n");
}

void fecharDia(Passageiro *lista, int n, float infoVoo[4]) {
    FILE *fp = fopen("dados.bin", "wb");
    if (fp == NULL) printf("FP ta nulo!\n");
    printf("Fechamento do dia:\n");
    printf("Quantidade de reservas: %d\n", n);
    printf("Posição: %.2f\n", infoVoo[3]);
    printf("--------------------------------------------------");
    printf("tamanho de lista: %d\n", sizeof(Passageiro) * n);
    //salvarDados(fp, 0, infoVoo, n, lista);
    rewind(fp);
    fclose(fp);
    liberarMemoria(lista, n);
    exit (0);
}

void fecharVoo(Passageiro *lista, int n, float infoVoo[4]) {
    FILE *fp = fopen("dados.bin", "wb");
    printf("Voo Fechado!\n\n");
    //salvarDados(fp, 1, infoVoo, n, lista);
    
    for(int i = 0; i < n; i++) {
        imprimirPassageiro(lista[i], "curto");
    }

    printf("Valor Total: %.2f\n", infoVoo[3]);
    printf("--------------------------------------------------");
    rewind(fp);
    fclose(fp);
    liberarMemoria(lista, n);
    exit (0);
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
int acharCPF(char cpf[15], Passageiro *lista, int n){

    for (int i = 0; i < n; i++){
        if (!strcmp(lista[i].cpf, cpf)){
            return i;
        }
    }

    return -1;
}

/*
função responsável por registrar um novo passageiro com dados passados pelos usuários
Entrada: nenhuma
Saída: p - representa o novo passageiro com os valores corretos atgribuídos
*/
Passageiro lerPassageiro(void){
    Passageiro p;

    //Alocando os dados necessários
    p.nome = alocStr(40);
    p.sobrenome = alocStr(40);
    p.assento = alocStr(6);
    p.classe = alocStr(15);
    p.origem = alocStr(4);
    p.destino = alocStr(4);
    p.numero_voo = alocStr(8);

    //Leitura dos dados
    scanf("%s %s %s %d %d %d %s %s %s %f %s %s",
     p.nome, p.sobrenome, p.cpf, &p.data[0],
     &p.data[1], &p.data[2], p.numero_voo, p.assento, 
     p.classe, &p.valor, p.origem, p.destino);
    
    //Realocação dos dados
    p.nome = realocStr(p.nome);
    p.sobrenome = realocStr(p.sobrenome);
    p.assento = realocStr(p.assento);
    p.classe = realocStr(p.classe);
    p.origem = realocStr(p.origem);
    p.destino = realocStr(p.destino);
    p.numero_voo = realocStr(p.numero_voo);
    
    return p;
}

/*
Funcao para consultar reserva, chamada pelo comando CR
Entrada: 
*/
void imprimirPassageiro(Passageiro p, char *modo){
    if (!strcmp(modo, "curto")){
        printf("%s\n", p.cpf);
        printf("%s %s\n", p.nome, p.sobrenome);
        printf("%s\n\n", p.assento);
    }
    else{
        printf("%s\n", p.cpf);
        printf("%s %s\n", p.nome, p.sobrenome);
        printf("%d/%d/%d\n", p.data[0], p.data[1], p.data[2]);
        printf("Voo: %s\n", p.numero_voo);
        printf("Assento: %s\n", p.assento);
        printf("Classe: %s\n", p.classe);
        printf("Trecho: %s %s\n", p.origem, p.destino);
        printf("Valor: %.02f\n", p.valor);
    }
}
