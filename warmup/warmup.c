#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct infoAlunos
{
    int nUSP;
    char nome[50];
    char curso[50];
    float nota;
} infoAlunos;


char *readline();
void printArquive();
void readArquive();
void switchOperacao();

int main(){
    
    char *nome_arquivo = NULL;
    int operacao;
    
    infoAlunos *infos = malloc(sizeof(infoAlunos));

    nome_arquivo = readline(stdin);
    scanf("%d", &operacao);

    FILE *registros;
    registros = fopen(nome_arquivo, "r");

    fseek(registros, 0, SEEK_END);
    int numBytesTotal = ftell(registros);

    fseek(registros, 0, SEEK_SET);

    switchOperacao(operacao, numBytesTotal, registros,  infos);

    free(nome_arquivo);
    free(infos);
    fclose(registros);

    return 0;
}

void switchOperacao(int operacao, int numBytesTotal, FILE *registros,  infoAlunos *infos){
    
    // switch case para escolher a operação desejada
    switch (operacao)
        {
            case 1:
            {
                // imprime conteudo do arquivo ate chegar ao fim do arquivo
                while(!feof(registros)){
                    readArquive(infos, registros);
                    if(!feof(registros))
                        printArquive(infos);
                    // se o ponteiro no arquivo nao apontar para o ultimo byte
                    // imprime um pular linha
                    if(ftell(registros) < numBytesTotal)
                        printf("\n");
                }
            }
            break;

            case 2:
            {
                // imprime ate chegar a metade do numero de bytes maximo
                while(ftell(registros) < numBytesTotal/2){
                    readArquive(infos, registros);
                    printArquive(infos);
                    if(ftell(registros) < numBytesTotal/2)
                        printf("\n");
                }
            }
            break;

            case 3:
            {
                fseek(registros, numBytesTotal/2, SEEK_SET);
                // começa a imprimir a partir da metade do numero maximo de bytes
                while(!feof(registros)){
                    readArquive(infos, registros);
                    if(!feof(registros))
                        printArquive(infos); 
                    if(ftell(registros) < numBytesTotal)
                        printf("\n");
                }
            }
            break;

            case 4:
            {
                int initFaixa, fimFaixa;
                scanf("%d %d", &initFaixa, &fimFaixa);

                // como cada registro possui 108 bytes este valor multiplica 
                // a entrada para saber a posição exata do arquivo pretendido
                initFaixa = (initFaixa - 1) * 108;
                fimFaixa = (fimFaixa - 1) * 108;
                fseek(registros, initFaixa, SEEK_SET);
                
                // imprime ate o ponteiro ser igual ao valor de fimFaixa
                while(!feof(registros) && ftell(registros) <= fimFaixa){
                    readArquive(infos, registros);
                    if(!feof(registros))
                        printArquive(infos); 
                    if(ftell(registros) < numBytesTotal && ftell(registros) <= fimFaixa)
                        printf("\n");
                }
            }
            break;

            case 5:
            {
                int posRegistro;
                scanf("%d", &posRegistro);

                fseek(registros, --posRegistro * 108, SEEK_SET);
                readArquive(infos, registros);
                printArquive(infos);
            }
            break;
            
            default:
                break;
        }
}

void readArquive(infoAlunos *infos, FILE *registros){

    fread(&infos->nUSP, sizeof(int), 1, registros);
    fread(&infos->nome, sizeof(char)*50, 1, registros);
    fread(&infos->curso, sizeof(char)*50, 1, registros);
    fread(&infos->nota, sizeof(int), 1, registros);
}

void printArquive(infoAlunos *infos){

    printf("nUSP: %d\n", infos->nUSP);
    printf("Nome: %s\n", infos->nome);
    printf("Curso: %s\n", infos->curso);
    printf("Nota: %.2f\n", infos->nota);
}

char *readline(FILE *imput){
    char *vetor = NULL, letra;
    int tamanho = 0;

    do {
        letra = fgetc(imput);

        vetor = realloc (vetor, sizeof(char) * (tamanho + 1));
        vetor[tamanho++] = letra;
    } while (letra != '\n' && !feof(stdin) && letra != '\r');

    vetor[tamanho - 1] = '\0';

    return vetor;
}