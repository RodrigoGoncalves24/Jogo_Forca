#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// Assinatura para as funções existentes no arquivo
#include "Froca.h"

// Variáveis globais, olhada por todo programa
char palavraSecreta[TAMANHO_PALAVRA];
char chutes[26];
int chutesDados = 0;

// Funções com pedaços de códigos para ser usados no programa
void abertura()
{
    printf("*******************************\n");
    printf("*     O   Jogo de Forca       *\n");
    printf("*******************************\n\n");
}

// Captura o chute do usuario
void chuta()
{
    char chute;
    scanf(" %c", &chute);

    chutes[chutesDados] = chute; // * pega o CONTEUDO do edereço
    chutesDados++;
}

void desenhaForca()
{
    int erros = chutesErrados();

    // if ternário para evitar repetições de if's - código curto e mais prático
    printf("    __________          \n");
    printf("   |/         |         \n");
    printf("   |         %c%c%c    \n", (erros >= 1 ? '(' : ' '), (erros >= 1 ? '_' : ' '), (erros >= 1 ? ')' : ' '));
    printf("   |         %c%c%c    \n", (erros >= 3 ? '\\' : ' '), (erros >= 2 ? '|' : ' '), (erros >= 4 ? '/' : ' '));
    printf("   |          %c       \n", (erros >= 2 ? '|' : ' '));
    printf("   |         %c %c     \n", (erros >= 5 ? '/' : ' '), (erros >= 6 ? '\\' : ' '));
    printf("   |                    \n");
    printf("  _|__                  \n");
    printf("\n\n");

    for (int i = 0; i < strlen(palavraSecreta); i++)
    {
        int temLetra = jaChutou(palavraSecreta[i]);
        if (temLetra)
        {
            printf("%c ", palavraSecreta[i]);
        }
        else
        {
            printf("_ ");
        }
    }
    printf("\n");
}

void adicionaPalavra()
{
    char quer;
    printf("Deseja adicionar uma nova palavra no jogo (S/N)? ");
    scanf(" %c", &quer);

    if (quer == 'S')
    {
        char novaPalavra[TAMANHO_PALAVRA];

        printf("Escreva a nova palavra, em letras maiusculas: \n");
        scanf("%s", novaPalavra);

        FILE *f;
        f = fopen("PalavrasJogo.txt", "r+");
        if (f == 0) // se der erro ao abrir o arquivo
        {
            printf("Desculpe! Banco de dados nao disponivel!\n\n");
            exit(1); // parar o programa
        }

        // Pega a quantidade de palavras do arquivo
        int qtd;
        fscanf(f, "%d", &qtd);
        qtd++;

        // Muda a quantidade de palavras do existentes no arquivo
        fseek(f, 0, SEEK_SET); // especificamente na linha desejada, ou seja, 0
        fprintf(f, "%d", qtd); // Sobrescreve esse valor

        fseek(f, 0, SEEK_END); // Ir  para o final do arquivo

        // Adiciona a nova palavra ao arquivo
        fprintf(f, "\n%s", novaPalavra);
        fclose(f); // fecha o arquivo
    }
}

void escolhePalavra()
{
    // sprintf(palavraSecreta, "MELANCIA"); // Permite adicionar no array uma palavra

    // Função usada para ler um arquivo externo, sendo "r" (leitura) ou "r+" (leitura e escrita)
    FILE *f;
    f = fopen("PalavrasJogo.txt", "r"); // Devolve um ponteiro para a primeira palavra
    if (f == 0)
    {
        printf("Desculpe! Banco de dados nao disponivel!\n\n");
        exit(1);
    }

    int qtdPalavras;
    fscanf(f, "%d", &qtdPalavras); // A primeira linha diz a quantidade de palavras existentes

    srand(time(0));
    // Sorteia uma palavra qualquer
    int randomico = rand() % qtdPalavras;

    // Vai até a palavra sorteada e escolhe ela como palavra secreta
    for (int i = 0; i <= randomico; i++)
    {
        fscanf(f, "%s", palavraSecreta);
    }

    fclose(f);
}

int chutesErrados()
{
    int erros = 0;

    for (int i = 0; i < chutesDados; i++)
    {
        int existe = 0;
        for (int j = 0; j < strlen(palavraSecreta); j++)
        {
            if (chutes[i] == palavraSecreta[j])
            {
                existe = 1;
                break;
            }
        }

        if (!existe) erros++;
    }
    return erros;
}

int enforcou()
{
    return chutesErrados() >= 6;
}

// Verifica se a letra chutada existe na palavra
int jaChutou(char letra)
{
    int achou = 0;

    for (int j = 0; j < chutesDados; j++)
    {
        if (chutes[j] == letra)
        {
            achou = 1;
            break;
        }
    }
    return achou;
}

int acertou()
{
    for (int i = 0; i < strlen(palavraSecreta); i++)
    {
        if (!jaChutou(palavraSecreta[i]))
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    // \0 indica que a string acabou ao  final de cada palavra
    // %s permite imprimir todas as letras de uma palavra em um array

    escolhePalavra();
    abertura();

    do
    {
        desenhaForca();
        chuta();

    } while (!acertou() && !enforcou());

    if (acertou())
    {
        printf("\nParabens, voce acertou a palavra secreta! \n");
    }
    else
    {
        printf("\nVoce foi enforcado!\n");
        printf("A palavra  era **%s**\n\n", palavraSecreta);
    }

    adicionaPalavra();
}