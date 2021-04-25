#include <stdio.h>
typedef struct infoAlunos
{
    int nUSP;
    char nome[50];
    char curso[50];
    float nota;
} infoAlunos;

char *readline(FILE *imput);
void printRegistro(infoAlunos *infos);
void readArquive(infoAlunos *infos, FILE *registros);
void saveRegistroStruct(infoAlunos *infos, char *registro);
char *separarRegistro(char *registro, int *positionRegistro);
void escreverArquivo(FILE *arquivo, infoAlunos *infos);
void read10registros();
void readLoopEntrada();