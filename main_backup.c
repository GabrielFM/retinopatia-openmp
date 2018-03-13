#include <stdio.h>
#include <stdlib.h>
#include "pimagens.h"

int main()
{
    FILE *in, *out;
    byte *buffer;
    unsigned tamanho, comeco, x, y, x1, y1, tamEst1, tamEst2, tamEst3;
    byte ***matriz, **estruturante, **aux1, **aux2, ***matriz1;
    byte **L, **f[QTD_PASSOS+1];//CADA IMAGEM É UM PASSO DO TRABALHO
    int k, tamEst, i, j, xCentro, yCentro;
    char nomeIn[100], nomeOut[100];
    byte **est1 = leEstruturante(&tamEst1,"est1.txt");
    byte **est2 = leEstruturante(&tamEst2,"est2.txt");
    byte **est3 = leEstruturante(&tamEst3,"est3.txt");
    estruturante = leEstruturante(&tamEst, "estruturante.txt");

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
        sprintf(nomeIn, "Imagens\\Entrada\\Im%d.bmp", k);//alterei
        matriz = lerImagem(nomeIn, &x, &y);
//        sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_12\\%d_out.bmp", k);//alterei
//        matriz = lerImagem(nomeIn, &x, &y);
//        f[10] = matriz[0];
//        sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_17\\%d_out.bmp", k);//alterei
//
//        matriz = lerImagem(nomeIn, &x, &y);
//        f[15] = matriz[0];
//        sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_18\\%d_out.bmp", k);//alterei
//        matriz = lerImagem(nomeIn, &x, &y);
//        f[16] = matriz[0];

///PASSO 1 - 2

        printf("PASSO 1 - 2\n");
        separarFundo(matriz,x,y);
        L = obterLuminescencia(matriz, x, y);

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 2, k);
        escreveImagem(L, nomeOut, x, y);


///PASSO 3

        printf("PASSO 3\n");
        f[1] = (byte**)alocar_2_dim(y,x,"byte");
        copia_matriz(L, f[1], x, y);
        soma_matriz(L, top_hatA(L, x, y, estruturante, tamEst), x, y);
        subtrai_matriz(L, top_hatF(f[1], x, y, estruturante, tamEst), x, y);
        copia_matriz(L, f[1], x, y);

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 3, k);
        escreveImagem(f[1], nomeOut, x, y);


///PASSO 4

        printf("PASSO 4\n");
        f[2] = minRegional(f[1],x,y,estruturante,tamEst);
        for(i=0; i<y; i++)
            for(j=0; j<x; j++)
                f[2][i][j] *=255;

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 4, k);
        escreveImagem(f[2], nomeOut, x, y);


///PASSO 5

        printf("PASSO 5\n");
        f[3] = reconstrucaoPorDilatacao(f[2], f[1], x, y, estruturante, tamEst);

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 5, k);
        escreveImagem(f[3], nomeOut, x, y);


///PASSO 6

        printf("PASSO 6\n");
        f[4] = Hminimo(f[3], x, y, estruturante, tamEst, 116);
        aux1 = Hmaximo(f[3], x, y, estruturante, tamEst, 116);
        subtrai_matriz(f[4],aux1,x,y);
//        result = f[2];

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 6, k);
        escreveImagem(f[4], nomeOut, x, y);

        ///ALGO MUITO ESTRANHO ACONTECE
        ///SE RECORTAR E COLAR ESTA ULTIMA FUNÇÃO PARA O PASSO 8 !!!!???????????

///PASSO 7

        printf("PASSO 7\n");
        f[5] = difusao_anisotropica(matriz[1], x, y, 10);
//        f[3] = filtro_da_media(f[3], x, y);
        f[5] = laplaciano_do_gaussiano2(f[5], x,y);
        f[5] = preto_e_branco(f[5], x, y);
        f[5] = hoshen_kopelman(f[5], x, y, 500, 255);
//        f[3] = filtro_da_media(f[3], x, y);
        f[5] = preto_e_branco(f[5], x, y);

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 7, k);
        escreveImagem(f[5], nomeOut, x, y);


///PASSO 8

        printf("PASSO 8\n");
        f[6] = coordCentro(f[4], x, y, &xCentro, &yCentro);

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 8, k);
        escreveImagem(f[6], nomeOut, x, y);

///PASSO 9

        printf("PASSO 9\n");
        f[7] = somarImagens(f[6], matriz[1], x, y);

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 9, k);
        escreveImagem(f[7], nomeOut, x, y);


///PASSO 10

        printf("PASSO 10\n");
        f[8] = maxRegional(f[7], x, y, estruturante, tamEst);
        for(i=0; i<y; i++)
            for(j=0; j<x; j++)
                {
                    f[8][i][j] *=255;
                    if(f[7][i][j] == 255)
                    {
                        f[8][i][j] = 255;
                    }
                }

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 10, k);
        escreveImagem(f[8], nomeOut, x, y);

///PASSO 11

        printf("PASSO 11\n");
        f[9] = geraImagemMarca(f[6], f[5], x, y);

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 11, k);
        escreveImagem(f[9], nomeOut, x, y);


///PASSO 12

        printf("PASSO 12\n");
        f[10] = reconstrucaoPorDilatacao(f[9], f[8], x, y, estruturante, tamEst);

        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 12, k);
        escreveImagem(f[10], nomeOut, x, y);

///REFINAMENTO



///PASSO 13

        printf("PASSO 13\n");
        f[11] = passo11(matriz, f[10], x, y, estruturante, tamEst);
        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 13, k);
        escreveImagem(f[11], nomeOut, x, y);


///PASSO 14

        printf("PASSO 14\n");
        f[12] = reconstrucaoPorDilatacao(f[11], matriz[1], x, y, estruturante, tamEst);
        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 14, k);
        escreveImagem(f[12], nomeOut, x, y);

///PASSO 15

        printf("PASSO 15\n");
        f[13] = subtrairImagens(matriz[1], f[12], x, y);
        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 15, k);
        escreveImagem(f[13], nomeOut, x, y);

///PASSO 16

        printf("PASSO 16\n");
        f[14] = fechamento(f[13],x, y, est3, tamEst3);
        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 16, k);
        escreveImagem(f[14], nomeOut, x, y);


///PASSO 17

        printf("PASSO 17\n");
        f[15] = abertura(f[14],x, y, est3, tamEst3);
        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 17, k);
        escreveImagem(f[15], nomeOut, x, y);

///PASSO 18

        printf("PASSO 18\n");
        f[16] = subtrairImagens(dilatacao(f[15], x, y, estruturante, tamEst), erosao(f[15], x, y, estruturante, tamEst), x, y);
        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 18, k);
        escreveImagem(f[16], nomeOut,x, y);

///PASSO 19

        printf("PASSO 19\n");
        f[17]=dilatacao(subtrairImagens(dilatacao(f[10],x,y,est1,tamEst1),erosao(dilatacao(f[10],x,y,est1,tamEst1),x,y,est2,tamEst2),x,y),x,y,est2,tamEst2);
        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 19, k);
        escreveImagem(f[17], nomeOut,x, y);

///PASSO 20

        printf("PASSO 19\n");
        f[18] = watershed(f[16], f[17], x, y);
        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 20, k);
        escreveImagem(f[18], nomeOut,x, y);
    }

    system("pause");

    return 0;
}
