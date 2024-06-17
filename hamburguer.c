/*Banco de dados para Voos
Projeto final do curso de Introdução à Ciência da Computação I, 2024.
Autores: Joao Victor Alonso de Mello(turmaB) - 10951790;
         Nathalia Monalisa Coelho Mota(turmaA) - 15522341e 
         Renan Banci Catarin(turmaB) - 14658181.
Professor: Rudinei Goularte.

Objetivo: o código a seguir foi criado com o intuito de informatizar
um sistema de cadastro de passageiros para uma companhia aérea.
Como as especificações do projeto ordenam, o sistema não é complexo,
funcionando apenas para um único voo de trecho simples,
além de a reserva de passageiros ser feita individualmente.
Também é possível Consultar, Modificar e Cancelar uma reserva.

Sobre os voos, quando um dia é fechado os dados salvos podem ser reabertos
 em um outro dia para receberem modificações.
Porém, quando o voo é fechado os dados são salvos e nao pode ser mais modificado,
 sendo o unico comando válido o de consultar reserva.
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

//Declaração das funções que serão usadas no código:

/*
Entradas Recorrentes:
*lista - representa a lista das estruturas dos passageiros.
n - o tamanho da estrutura da lista de passageiros.
infoVoo[4] - um array de floats com as informacoes de voo inseridas pelo comena do AV
 como o numero de assentos, preço da econômica e executiva além do valor total do voo.
*/

//funções de suporte:
char *alocStr(int x);
char *otimizarEspacoStr(char *a);
Passageiro lerPassageiro();
Passageiro *realocPassageiros(Passageiro *p, int x);
void imprimirPassageiro(Passageiro pessoa, char *modo);
int acharCPF(char cpf[15], Passageiro *lista, int n);
void liberarMemoria(Passageiro *lista, int n);
void freadMelhorado(void *p, int tamanho, int repeticos, FILE *fp, char *nome);

//funções primárias:
void abrirVoo(float infoVoo[4]);
void registrarPassageiro(Passageiro **lista , int *n, float infoVoo[4]);
void consultarReserva(Passageiro *lista, int n);
void cancelarReserva(Passageiro **lista ,int *n, float infoVoo[4]);
void modificarReserva(Passageiro *lista, int n);
void fecharDia(Passageiro *lista, int n, float infoVoo[4]);
void fecharVoo(Passageiro *lista, int n, float infoVoo[4]);
void salvarDados(FILE *fp, int fechado, float informacoes_do_voo[4], int tamanho_lista_passageiros, Passageiro *lista_passageiros);
int lerDadosSalvos(FILE *fp, float informacoes_do_voo[4], int *tamanho_lista_passageiros, Passageiro **lista_passageiros);

/*Função principal do programa*/
int main (void){
    float informacoes_do_voo[4] = {0};
    Passageiro *lista_passageiros = NULL;
    int tamanho_lista_passageiros = 0;

    FILE *fp = fopen("dados.bin", "rb");
    int voo_fechado = 0;
    int voo_encontrado = 0;
    if (fp != NULL){ //Caso o arquivo já esteja escrito.
        voo_encontrado = 1;
        if (lerDadosSalvos(fp, informacoes_do_voo, &tamanho_lista_passageiros, &lista_passageiros)){
            fecharVoo(lista_passageiros, tamanho_lista_passageiros, informacoes_do_voo);
            printf("\n");
            voo_fechado = 1;
        }
        rewind(fp);
        fclose(fp);
    }
    char comando[3] = "NA";
    while (strcmp(comando, "FV") != 0 && (strcmp(comando, "FD") != 0 || voo_fechado)){ 
        scanf("%s", comando); //Ler comando do usuário.
             
        if (!strcmp(comando, "AV") && !voo_encontrado){ 
            abrirVoo(informacoes_do_voo);
        } 
        else if (!strcmp(comando, "RR") && !voo_fechado){ //adiciona um passageiro a lista_passageiros.
           registrarPassageiro(&lista_passageiros, &tamanho_lista_passageiros, informacoes_do_voo);
        }
        else if (!strcmp(comando, "MR") && !voo_fechado){ //Modificar Reserva.
            modificarReserva(lista_passageiros, tamanho_lista_passageiros);
        }
        else if (!strcmp(comando, "CR")){ //Consulta Reserva.
            consultarReserva(lista_passageiros, tamanho_lista_passageiros);
        }
        else if (!strcmp(comando, "CA") && !voo_fechado){ //Cancela Reserva.
            cancelarReserva(&lista_passageiros, &tamanho_lista_passageiros, informacoes_do_voo);
        }
        else if (!strcmp(comando, "FD") && !voo_fechado){ //Encerra o programa e salva as informacoes obtidas.
            fecharDia(lista_passageiros, tamanho_lista_passageiros, informacoes_do_voo);
        }
        else if (!strcmp(comando, "FV") ){ //Encerra o programa e salva as informações obtidas.
            fecharVoo(lista_passageiros, tamanho_lista_passageiros, informacoes_do_voo);
        }
        else printf("comando invalido\n");
    }
    if (lista_passageiros != NULL) liberarMemoria(lista_passageiros, tamanho_lista_passageiros);
    return 0;
}

/*
Comando AV:
 possibilita o usuário informar os dados básicos do voo, sao armazenados em infoVoo.
*/
void abrirVoo(float infoVoo[4]) {
    scanf("%f %f %f", &infoVoo[0], &infoVoo[1], &infoVoo[2]);
}

/*
Comando RR:
 a lista sera realocada para que caiba um novo passageiro,
 em seguida a função ler passageiro será chamada para serem coletados os dados
 após isso ele confere se o número máximo de assentos foi atingido
 caso sim ele chama a funcao de FV.
*/
void registrarPassageiro(Passageiro **lista , int *n, float infoVoo[4]) {
    (*n)++;
    *lista = realocPassageiros(*lista, *n);
    (*lista)[*n - 1] = lerPassageiro();
    infoVoo[3] += (*lista)[*n - 1].valor;
    if (*n == infoVoo[0]){
        fecharVoo(*lista, *n, infoVoo);  
    }
}

/*
Comando CR:
 o usuário digita um cpf que quer buscar, em seguida, 
 a funcao achar CPF retornara o valor referente ao indice do 
 passageiro com esse CPF de acordo com a lista, caso seja inválido
 uma mensagem de erro será exibida, no caso de ser válido 
 os dados do passageiro serão exibidos de maneira "longa".
*/
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

/*
Comando CA:
 realiza o mesmo provesso de buscar o CPF,
 se for válido o passageiro em questao será removido sa liata e a mesma
 irá ser realocada para uma otimizacao da memória do programa.
*/
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

/*
Comando MR:
 faz o mesmo esquema de procura de cliente por CPF
 caso válido o usuário deve colocar os novos dados, eles serão sobrescrevidos
 nos antigos.
*/
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

/*
Comando FD:
 serão exibidas a quantidade de reservas totais e também o valor arrecadado com a venda dos assentos.
 Após isso todos os dados serão salvos em um arquivo binário(.bin)
 e a memória alocada será liberada, enfim encerrando o programa.
 OBS: permite modificações futuras
*/
void fecharDia(Passageiro *lista, int n, float infoVoo[4]) {
    FILE *fp = fopen("dados.bin", "wb");
    if (fp == NULL) printf("FP ta nulo!\n");
    printf("Fechamento do dia:\n");
    printf("Quantidade de reservas: %d\n", n);
    printf("Posicao: %.2f\n", infoVoo[3]);
    printf("--------------------------------------------------");
    salvarDados(fp, 0, infoVoo, n, lista);
    rewind(fp);
    fclose(fp);
}

/*
Comando FV:
 os dados serão salvos e em seguida 
 um aviso que o Voo fechou será exibido junto a "versão curta" dos dados dos passageiros,
 enfim o programa se encerrará.
 OBS: não permitr modificações futuras, apenas os comandos CR e FV estarão válidos para uso.
*/
void fecharVoo(Passageiro *lista, int n, float infoVoo[4]) {
    FILE *fp = fopen("dados.bin", "wb");
    printf("Voo Fechado!\n\n");
    salvarDados(fp, 1, infoVoo, n, lista);
    
    for(int i = 0; i < n; i++) {
        imprimirPassageiro(lista[i], "curto");
    }

    printf("Valor Total: %.2f\n", infoVoo[3]);
    printf("--------------------------------------------------");
    rewind(fp);
    fclose(fp);
}

/*
Como o próprio nome da função diz ela salva os dados já registrados,
esse processo é feito em um arquivo binário passado como parametro da função
Entrada: *fp - arquivo binário
         fechado - inteiro usado como booleano para se o voo esta fechado ou não 
         informações_do_voo[4] - array com as informações básicas do voo registradas pelo comando AV
         tamanho_lista_passageiros - número de passageiros registrados para o voo
         *lista_passageiros - array contendo as structs de cada passageiro registrado
*/
void salvarDados(FILE *fp, int fechado, float informacoes_do_voo[4], int tamanho_lista_passageiros, Passageiro *lista_passageiros) {
    fwrite(&fechado, sizeof(int), 1, fp);

    fwrite(informacoes_do_voo, sizeof(float), 4, fp);

    fwrite(&tamanho_lista_passageiros, sizeof(int), 1, fp);

    for (int i = 0; i < tamanho_lista_passageiros; i++) {
        int tam_nome = strlen(lista_passageiros[i].nome)+1;
        int tam_sobrenome = strlen(lista_passageiros[i].sobrenome)+1;
        int tam_assento = strlen(lista_passageiros[i].assento)+1;
    	int tam_classe = strlen(lista_passageiros[i].classe)+1;
    	int tam_origem = strlen(lista_passageiros[i].origem) + 1;
    	int tam_destino = strlen(lista_passageiros[i].destino) + 1;
    	int tam_numero_voo = strlen(lista_passageiros[i].numero_voo) + 1;

        fwrite(&tam_nome, sizeof(int), 1, fp);
        fwrite(lista_passageiros[i].nome, sizeof(char), tam_nome, fp);

        fwrite(&tam_sobrenome, sizeof(int), 1, fp);
        fwrite(lista_passageiros[i].sobrenome, sizeof(char), tam_sobrenome, fp);

        fwrite(lista_passageiros[i].cpf, sizeof(char), 15, fp);

        fwrite(&tam_assento, sizeof(int), 1, fp);
        fwrite(lista_passageiros[i].assento, sizeof(char), tam_assento, fp);
	
	    fwrite(&tam_classe, sizeof(int), 1, fp);
        fwrite(lista_passageiros[i].classe, sizeof(char), tam_classe, fp);  //*origem, *destino, *numero_voo, int valor, data[3]
        
        fwrite(&tam_origem, sizeof(int), 1, fp);
        fwrite(lista_passageiros[i].origem, sizeof(char), tam_origem, fp);
        
        fwrite(&tam_destino, sizeof(int), 1, fp);
        fwrite(lista_passageiros[i].destino, sizeof(char), tam_destino, fp);
        
        fwrite(&tam_numero_voo, sizeof(int), 1, fp);
        fwrite(lista_passageiros[i].numero_voo, sizeof(char), tam_numero_voo, fp);
        
        fwrite(&lista_passageiros[i].valor, sizeof(int), 1, fp);
        fwrite(lista_passageiros[i].data, sizeof(int), 3, fp);
    }
}

/*
Responsável por ler os dados salvos no arquivo binário,
Ela serve para continuar o preenchimento dos dados de nvoos passageiros após um dia ter sido fechado
Entrada: *fp - arquivo binário
	informações do voo[4] - array com as informações básicas do voo registrado
        tamanho_lista_passageiros - número de passageiros registrados para o voo
        *lista_passageiros - array contendo as structs de cada passageiro registrado
Saída: tmp - inteiro que retorna se o voo esta fechado ou não 
*/
int lerDadosSalvos(FILE *fp, float informacoes_do_voo[4], int *tamanho_lista_passageiros, Passageiro **lista_passageiros) {
    int tmp;
    freadMelhorado(&tmp, sizeof(int), 1, fp, "fechado");
    freadMelhorado(informacoes_do_voo, sizeof(float), 4, fp, "informacoes_do_voo");
    freadMelhorado(tamanho_lista_passageiros, sizeof(int), 1, fp, "tamanho_lista_passageiros");
    *lista_passageiros = realocPassageiros(*lista_passageiros, *tamanho_lista_passageiros);
    for (int i = 0; i < *tamanho_lista_passageiros; i++) {
        int tam_nome, tam_sobrenome, tam_assento, tam_classe, tam_origem, tam_destino, tam_numero_voo;
        freadMelhorado(&tam_nome, sizeof(int), 1, fp, "tam_nome");
        lista_passageiros[0][i].nome = alocStr(tam_nome);
        freadMelhorado(lista_passageiros[0][i].nome, sizeof(char), tam_nome, fp, "nome");
        
        freadMelhorado(&tam_sobrenome, sizeof(int), 1, fp, "tam_sobrenome");
        lista_passageiros[0][i].sobrenome = alocStr(tam_sobrenome);
        freadMelhorado(lista_passageiros[0][i].sobrenome, sizeof(char), tam_sobrenome, fp, "sobrenome");
        
        freadMelhorado(lista_passageiros[0][i].cpf, sizeof(char), 15, fp, "cpf");
        
        freadMelhorado(&tam_assento, sizeof(int), 1, fp, "tam_assento");
        lista_passageiros[0][i].assento = alocStr(tam_assento);
        freadMelhorado(lista_passageiros[0][i].assento, sizeof(char), tam_assento, fp, "assento");
        
        freadMelhorado(&tam_classe, sizeof(int), 1, fp, "tam_classe");
        lista_passageiros[0][i].classe = alocStr(tam_classe);
        freadMelhorado(lista_passageiros[0][i].classe, sizeof(char), tam_classe, fp, "classe");
        
        freadMelhorado(&tam_origem, sizeof(int), 1, fp, "tam_origem");
        lista_passageiros[0][i].origem = alocStr(tam_origem);
        freadMelhorado(lista_passageiros[0][i].origem, sizeof(char), tam_origem, fp, "origem");
        
        freadMelhorado(&tam_destino, sizeof(int), 1, fp, "tam_destino");
        lista_passageiros[0][i].destino = alocStr(tam_destino);
        freadMelhorado(lista_passageiros[0][i].destino, sizeof(char), tam_destino, fp, "destino");
        
        freadMelhorado(&tam_numero_voo, sizeof(int), 1, fp, "tam_numero_voo");
        lista_passageiros[0][i].numero_voo = alocStr(tam_numero_voo);
        freadMelhorado(lista_passageiros[0][i].numero_voo, sizeof(char), tam_numero_voo, fp, "numero_voo");
        
        freadMelhorado(&lista_passageiros[0][i].valor, sizeof(int), 1, fp, "valor do voo");
        
        freadMelhorado(lista_passageiros[0][i].data, sizeof(int), 3, fp, "data do voo");
    }
    return tmp;
}

/*
Aloca strings, como por exemplo o nome do passageiro.
Entrada: x - Valor que representas quantos caracteres serão alocados.
Saída: b - Espaço alocado que será atibuido a string em questão.
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
Realoca strings, por exemplo o nome do passageiro, para não ocupar mais memória que o necessário.
Entrada: a - String a ser realocada.
Saída: b - Espaço realocado que será atibuido a string em questão.
*/
char *otimizarEspacoStr(char *a){
    char *b = realloc(a, (strlen(a) + 1) * sizeof(char));
    if (b == NULL){
        printf("OWO sumimasen senpai-san! nao tenho memoria o suficiente UWU");
        exit(1);
    }
    return b;
}

/*
Realoca o vetor de structs passageiro,
Quando o comando RR é chamado antes de executar a função de registrar
um passageiro o vetor é realocado para que possa ser liberado espaço na memória 
Também é chamada para encurtar a lista no caso de CA ser chamado.
Entradas: *p - Representa a lista de passageiros.
           x - tamanho a ser realocado.
Saída: b - Espaço realocado que será atibuido ao vetor em questão.
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

/*
Previne o vazamento da memória alocada, 
 uma boa pratica adotada para esse código.
*/
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

/* Lê uma informação do arquivo e imprime caso dê algum erro
Entrada: *p - ponteiro sem tipo que aponta para o lugar atual dentro do arquivo
	 tamanho - inteiro que indica o tamanho da informação
	 repeticos - inteiro que indica quantas vezes a informação deve ser lida
	 *fp - arquivo binário
	 *nome - nome da informação sendo lida
*/
void freadMelhorado(void *p, int tamanho, int repeticos, FILE *fp, char *nome){
    if (fread(p, tamanho, repeticos, fp) != repeticos) {
        char c[100] = "Erro ao ler ";
        strcat(c, nome);
        perror(c);
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
    p.nome = otimizarEspacoStr(p.nome);
    p.sobrenome = otimizarEspacoStr(p.sobrenome);
    p.assento = otimizarEspacoStr(p.assento);
    p.classe = otimizarEspacoStr(p.classe);
    p.origem = otimizarEspacoStr(p.origem);
    p.destino = otimizarEspacoStr(p.destino);
    p.numero_voo = otimizarEspacoStr(p.numero_voo);
    
    return p;
}

/*
Imprime os dados dos passageiros de dois modos: curto ou longo:
caso curto: apenas exibidos cpf, nome, sobrenome e assento;
caso longo: todos os dados inseridos em lerPassageiro serão impressos.
Entradas: p - o passageiro a ser impresso
          *modo - "curto" ou "longo"
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
