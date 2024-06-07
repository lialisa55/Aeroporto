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
    scanf("%.0f %f %f", &informacoes_do_voo[0], &informacoes_do_voo[1], &informacoes_do_voo[2]);
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

void imprimirPassageiro(int x, passageiro *lista_passageiros){
    printf("%s\n", lista_passageiros[x].cpf);
    printf("%s %s\n", lista_passageiros[x].nome, lista_passageiros[x].sobrenome);
    printf("%d/%d/%d\n", lista_passageiros[x].data);
    printf("Voo:_%s\n", lista_passageiros[x].numero_voo);
    printf("Assento:_%s\n", lista_passageiros[x].assento);
    printf("Classe:_%s\n", lista_passageiros[x].classe);
    printf("Trecho:_%s %s\n", lista_passageiros[x].origem, lista_passageiros[x].destino);
    printf("Valor:_%.02f\n", lista_passageiros[x].valor);
}

void acharCPF(char * cpf, passageiro *lista_passageiros, int tamanho_lista_passageiros){
    for (int i = 0; i < tamanho_lista_passageiros; i++){
        if (!strcmp(lista_passageiros[i].cpf, cpf)){
            return i;
        }
    }
}

int main (void){
    float informacoes_do_voo[3];
    passageiro *lista_passageiros = NULL;
    int tamanho_lista_passageiros = 0;
    FILE *fp = fopen("dados.txt", "r");
    if (fp != NULL){
        fscanf(fp, "%.0f %f %f", &informacoes_do_voo[0], &informacoes_do_voo[1], &informacoes_do_voo[2]);
        int n;
        fscanf(fp, "%d", &n);
        lista_passageiros = realocar_passageiros(lista_passageiros, n);
        tamanho_lista_passageiros = n;
        for (int i = 0; i < n; i++)
            passageiros[i] = registrar_passageiro();
        }
        fclose(fp);
    }
    char comando[3] = "NA";
    while (strcmp(comando, "FD") != 0 && strcmp(comando, "FV") != 0){
        scanf("%s", comando);
        if (!strcmp(comando, "RR")){
            tamanho_lista_passageiros++;
            lista_passageiros = realocar_passageiros(lista_passageiros, tamanho_lista_passageiros);
            lista_passageiros[tamanho_lista_passageiros - 1] = registrar_passageiro();
        }
        else if (!strcmp(comando, "CR")){
            char cpf[12];
            scanf("%s", cpf);
            int indice = consultarcpf(cpf, lista_passageiros, tamanho_lista_passageiros);
            imprimirPassageiro(indice, lista_passageiros);
        }
    }
    /*FILE *fp;
    fp = fopen("file.txt", "w+");
    char *instrucao;
    char *cpf[20];

    consultarcpf(cpf, fp);*/
