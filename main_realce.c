#include <stdio.h>
#include <stdlib.h>
#include "pimagens.h"
#define FATOR 30

int main()
{
    FILE *in, *out;
    byte *buffer;
    unsigned tamanho, comeco, x, y;
    byte ***matriz, **estruturante, **aux1, **aux2;
    byte **L, **f[QTD_PASSOS+1];//CADA IMAGEM É UM PASSO DO TRABALHO
    int k, tamEst, i, j, xCentro, yCentro;
    char nomeIn[100], nomeOut[100];
	unsigned aux;
	
    system("mkdir Imagens\\Saida\\1");

    // CRIA AS PASTAS PARA ARMAZENAR OS PASSOS
    for(i=0; i<QTD_PASSOS-1; i++)
    {
        sprintf(nomeOut, "mkdir Imagens\\Saida\\1\\Passo_%d", 2+i);
        system(nomeOut);
    }

    for(k=QTD_IMAGENS; k>=0; k--)///ALTERADO
    {

///LEITURA EM ARQUIVO

        printf("Processando imagem %d\n", k);
        sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_18\\%d_out.bmp", k);//alterei
        in = fopen(nomeIn,"rb");
        if(in == NULL) { exit(2);}
        // DESCOBRE O TAMANHO DO ARQUIVO E VOLTA PRO COMEÇO
        fseek(in, 0, SEEK_END);
        tamanho = ftell(in);
        fseek(in, 0, SEEK_SET);
        // ALOCA O BUFFER
        buffer = (byte*)malloc(tamanho*sizeof(byte));
        if(buffer==NULL) exit(1);
        // LÊ A ENTRADA PARA O BUFFER
        fread(buffer, 1, tamanho, in);
        fflush(in);
        fclose(in);
        // EXTRAI AS INFORMAÇÕES BÁSICAS DO HEADER
        leHeader(buffer, &x, &y, &comeco);
        // GERA A MATRIZ A PARTIR DO ARQUIVO
        matriz = geraMatriz(buffer, x, y, comeco);
        free(buffer);
        estruturante = leEstruturante(&tamEst);

    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            aux = matriz[0][i][j]*FATOR;
            if(aux>255) aux = 255;
            matriz[0][i][j] = aux;

            aux = matriz[1][i][j]*FATOR;
            if(aux>255) aux = 255;
            matriz[1][i][j] = aux;

            aux = matriz[1][i][j]*FATOR;
            if(aux>255) aux = 255;
            matriz[1][i][j] = aux;
        }
    }
        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out2.bmp", 18, k);
        escreveImagem(matriz[0], nomeOut, x, y);
    }
    return 0;
}
