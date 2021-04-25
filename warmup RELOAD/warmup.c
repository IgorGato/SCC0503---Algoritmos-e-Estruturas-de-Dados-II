#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "warmup.h"

#define BYTES_REGISTRO 108
#define NUM_ULTIMOS 10

void readLoopEntrada(){
    char *registro = NULL;
    FILE *arquivo = fopen("arquivo", "w+");
    infoAlunos *infos = malloc(sizeof(infoAlunos));

    // le todos os registros de entrada ate o final do stdin
    while(!feof(stdin)){
        registro = readline(stdin);
        saveRegistroStruct(infos, registro);
        escreverArquivo(arquivo, infos);
    }

    free(infos);
    fclose(arquivo);
}

void read10registros(){
    
    int dezUltimos, numBytesArquivo;
    FILE *arquivo = fopen("arquivo", "r+");
    infoAlunos *infos = malloc(sizeof(infoAlunos));

    fseek(arquivo, 0, SEEK_END);
    numBytesArquivo = ftell(arquivo);

    // sabendo o numero de bytes do arquivo com ftell, faz uma conta simples de
    // subtração para saber a posição onde começa os 10 ultimos registros
    dezUltimos = numBytesArquivo - (BYTES_REGISTRO * NUM_ULTIMOS);
    fseek(arquivo, dezUltimos, SEEK_SET);

    // imprime estes registros até o fim do arquivo 
    while(!feof(arquivo)){
        readArquive(infos, arquivo);
        if(!feof(arquivo))
            printRegistro(infos); 
        if(ftell(arquivo) < numBytesArquivo)
            printf("\n");
    }

    free(infos);
    fclose(arquivo);
}

// escreve no arquivo cada dado salvo na struct com seus tamanhos estipulados
void escreverArquivo(FILE *arquivo, infoAlunos *infos){
    fwrite(&infos->nUSP, sizeof(int), 1, arquivo);
    fwrite(&infos->nome, sizeof(char)*50, 1, arquivo);
    fwrite(&infos->curso, sizeof(char)*50, 1, arquivo);
    fwrite(&infos->nota, sizeof(float), 1, arquivo);
}

// lê do arquivo cada dado de um registro e 
// salva cada dado em seu campo especifico da struct
void readArquive(infoAlunos *infos, FILE *registros){

    fread(&infos->nUSP, sizeof(int), 1, registros);
    fread(&infos->nome, sizeof(char)*50, 1, registros);
    fread(&infos->curso, sizeof(char)*50, 1, registros);
    fread(&infos->nota, sizeof(int), 1, registros);
}

void printRegistro(infoAlunos *infos){

    printf("nUSP: %d\n", infos->nUSP);
    printf("Nome: %s\n", infos->nome);
    printf("Curso: %s\n", infos->curso);
    printf("Nota: %.2f\n", infos->nota);
}

char *readline(FILE *imput){
    char *vetor = NULL, letra;
    int tamanho = 0;

    // lê uma linha inteira do arquivo até o fim deste, 
    // ou uma quebra de linha seja encontrada
    do {
        letra = fgetc(imput);

        vetor = realloc (vetor, sizeof(char) * (tamanho + 1));
        vetor[tamanho++] = letra;
    } while (letra != '\n' && !feof(stdin) && letra != '\r');

    vetor[tamanho - 1] = '\0';

    return vetor;
}

void saveRegistroStruct(infoAlunos *infos, char *registro){

    int position = 0;
    // faz a separação de cada dado, e os salvam nos campos da struct
    // já fazendo as tranformaçoes necessarias em int e float
    infos->nUSP = atoi(separarRegistro(registro, &position));
    strcpy(infos->nome, separarRegistro(registro, &position));
    strcpy(infos->curso, separarRegistro(registro, &position));
    infos->nota = atof(separarRegistro(registro, &position));
}

char *separarRegistro(char *registro, int *positionRegistro){

    char letra, *palavra = NULL;
    int tamPalavra = 0;

    // salva a palavra do registro a partir da posição do ultimo registro já
    // salvo, utilizando as virgulas como separadores
    do{
        letra = registro[(*positionRegistro)++];
        palavra = realloc(palavra, sizeof(char) * (tamPalavra+1));
        if(letra != ',')
            palavra[tamPalavra++] = letra; 

    }while(letra != ',' && letra != '\0');

    palavra[tamPalavra] = '\0';

    return palavra;
}

