#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "indices.h"

#define BYTES_REGISTRO 158
#define NUM_ULTIMOS 10

void readLoopEntrada(){
    char *registro = NULL, *operacao = NULL;
    
    // le todos os registros de entrada ate o final do stdin
    while(!feof(stdin)){
        registro = readline(stdin);
        operacao = operation(registro);

        // apenas retorna se for comando de exit
        if(!strcmp(operacao, "exit")) return;

        if(strlen(operacao))
            escolher_operacao(registro, operacao);
    }
}

// escreve no arquivo cada dado salvo na struct com seus tamanhos estipulados
void escreverArquivo(FILE *arquivo, infoAlunos *infos){

    fseek(arquivo, 0, SEEK_END);

    armazenar_indice(infos->nUSP, ftell(arquivo));

    fwrite(&infos->nUSP, sizeof(int), 1, arquivo);
    fwrite(&infos->nome, sizeof(char)*50, 1, arquivo);
    fwrite(&infos->sobrenome, sizeof(char)*50, 1, arquivo);
    fwrite(&infos->curso, sizeof(char)*50, 1, arquivo);
    fwrite(&infos->nota, sizeof(float), 1, arquivo);
    
}

// lê do arquivo cada dado de um registro e 
// salva cada dado em seu campo especifico da struct
void readArquive(infoAlunos *infos, FILE *registros){

    fread(&infos->nUSP, sizeof(int), 1, registros);
    if(infos->nUSP == -1) return;
    fread(&infos->nome, sizeof(char)*50, 1, registros);
    fread(&infos->sobrenome, sizeof(char)*50, 1, registros);
    fread(&infos->curso, sizeof(char)*50, 1, registros);
    fread(&infos->nota, sizeof(int), 1, registros);

}

void printRegistro(infoAlunos *infos){

    printf("-------------------------------\n");

    printf("USP number: %d\n", infos->nUSP);
    printf("Name: %s\n", infos->nome);
    printf("Surname: %s\n", infos->sobrenome);
    printf("Course: %s\n", infos->curso);
    printf("Test grade: %.2f\n", infos->nota);

    printf("-------------------------------\n");
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
    strcpy(infos->sobrenome, separarRegistro(registro, &position));
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

char *operation(char *linha_de_dado){
    char *token = NULL, letra;
    int posicao = 0;

    // retira a primeira palavra da string passada, que sera o comando pretendido
    letra = linha_de_dado[posicao];
    do{
        
        token = realloc(token, sizeof(char) * posicao + 1);
        token[posicao++] = letra;
        letra = linha_de_dado[posicao];

    }while(letra != ' ' && posicao < strlen(linha_de_dado));
    
    token[posicao] = '\0';
    // copia a string de volta sem o comando
    strcpy(linha_de_dado, linha_de_dado + posicao+1);

    return token;
}

void armazenar_indice(int nUSP_chave, long RRN){
    // abre o arquivo de indice e armazena todos seus registros
    FILE *arq_indices = fopen("arquivo_indices", "a");

    fwrite(&nUSP_chave, sizeof(int), 1, arq_indices);
    fwrite(&RRN, sizeof(long), 1, arq_indices);

    fclose(arq_indices);
}

void escolher_operacao(char *registro, char *operacao){
    int contador = 0;
    infoAlunos *infos = malloc(sizeof(infoAlunos));
    lista_indice *indice_t = malloc(sizeof(lista_indice));
    FILE *arquivo = fopen("arquivo", "a+");

    // le os arquivos de indice 
    indice_t->indices = ler_indice(&contador);
    indice_t->num_indices = contador-1;

    if(!strcmp(operacao, "insert")){
        saveRegistroStruct(infos, registro);

        // verifica se o registro ja existe 
        if(encontrar_registro(indice_t, infos->nUSP, operacao))
            escreverArquivo(arquivo, infos);
        else 
            printf("O Registro ja existe!\n");

    }
    else {
        // verifica se o arquivo será encontrado
        if(encontrar_registro(indice_t, atoi(registro), operacao)){
            printf("Registro nao encontrado!\n");
        }
    }

    free(indice_t);
    free(infos);
    fclose(arquivo);
}

indice **ler_indice(int *contador){
    
    FILE *arq_indices = fopen("arquivo_indices", "a+");
    indice **indice_t = NULL;

    // le todo arquivo de indice ate o seu final e armazena 
    // seus dados em um vetor de structs
    while(!feof(arq_indices)){
        indice_t = realloc(indice_t, sizeof(indice *) * (*contador + 1));
        indice_t[*contador] = malloc(sizeof(indice));
        fread(&indice_t[*contador]->nUSP_chave, sizeof(int), 1, arq_indices);
        fread(&indice_t[*contador]->RRN, sizeof(long), 1, arq_indices);

        (*contador)++;
    }

    fclose(arq_indices);
    return indice_t;
}

int encontrar_registro(lista_indice *indice_t, int registro, char *op){
    int pos = 0;
    infoAlunos *infos = malloc(sizeof(infoAlunos));

    // verifica se a chave procurada existe no arquivo de indices
    while(pos < indice_t->num_indices){
        // se existir ela abre o arquivo e vai ate sua posição
        if(indice_t->indices[pos]->nUSP_chave == registro){
            FILE *arquivo = fopen("arquivo", "r+");
            fseek(arquivo, indice_t->indices[pos]->RRN, SEEK_SET);
            // se o comando for para deletar insere -1 no primeiros 4 bits
            // do arquivo para indicar remoção logica
            if(!strcmp(op, "delete")){
                int delete = -1;
                fwrite(&delete, sizeof(int), 1, arquivo);
            }else if(!strcmp(op, "search")){
                readArquive(infos, arquivo);
                // verifica se o arquivo foi removido
                if(infos->nUSP == -1) return 1;
                    printRegistro(infos); 
            }
            
            fclose(arquivo);
            return 0;
        }
        pos++;
    }

    if(!strcmp(op, "delete")) return 0;
    return 1;
}
