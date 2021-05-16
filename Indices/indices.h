#include <stdio.h>

typedef struct infoAlunos
{
    int nUSP;
    char nome[50];
    char sobrenome[50];
    char curso[50];
    float nota;
} infoAlunos;

typedef struct indice
{
    int nUSP_chave;
    long RRN;
} indice;

typedef struct lista_indice
{
    int num_indices;
    indice **indices;
} lista_indice;


char *readline();
void printRegistro();
void readArquive();
void saveRegistroStruct();
char *separarRegistro();
void escreverArquivo();
void read10registros();
void readLoopEntrada();

char *operation();
void armazenar_indice();
void escolher_operacao();
indice **ler_indice();
int encontrar_registro();
int arquivo_vazio();
void novo();