#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include "pimagens.h"

int main()
{
    FILE *in, *out;
    byte *buffer;
    unsigned tamanho, comeco, x, y, x1, y1;
    int tamEst1, tamEst2, tamEst3,tamEst4, tamEst5,tamEst9,tamEst17,tamEst13,tamEstAlterado;
    int tamEstCentral, tamEstIntermediario, tamEstExterno;
    int distancia = 200;
    byte ***matriz,***matrizGrande, **aux1, **aux2, ***matriz1;
    byte **L, **f[QTD_PASSOS+1];//CADA IMAGEM É UM PASSO DO TRABALHO
    int k, tamEst, i, j, xCentro, yCentro,xCentro2,yCentro2,Diametro, xCentroFovea, yCentroFovea;
    char nomeIn[100], nomeOut[100];
    byte **est1 = leEstruturante(&tamEst1,"est1.txt");
    byte **est2 = leEstruturante(&tamEst2,"est35.txt");
    byte **est3 = leEstruturante(&tamEst3,"est3.txt");
    byte **est4 = leEstruturante(&tamEst4,"est4.txt");
    byte **est5 = leEstruturante(&tamEst5,"est5.txt");
    byte **est9 = leEstruturante(&tamEst9,"est9.txt");
    byte **est17 = leEstruturante(&tamEst17,"est17.txt");
    byte **est13 = leEstruturante(&tamEst13,"est13.txt");
    byte **estruturante = leEstruturante(&tamEst, "estruturante.txt");
    byte **est3alterado = leEstruturante(&tamEstAlterado,"est3alterado.txt");
    byte **estCentral = leEstruturante(&tamEstCentral,"estCentral.txt");
    byte **estIntermediario = leEstruturante(&tamEstIntermediario,"estIntermediario.txt");
    byte **estExterno = leEstruturante(&tamEstExterno,"estExterno.txt");
    FILE* relatorio = fopen("relatorio.txt", "w");
    FILE *tempo = fopen("tempo.txt","w");
    int qtdCentral;
    int qtdInter;
    int qtdExt;

    clock_t begin,end;

    int Cx, Cy;

    system("mkdir Imagens/Saida/1");

    omp_set_dynamic(0);
    omp_set_nested(1);

    // CRIA AS PASTAS PARA ARMAZENAR OS PASSOS
    for(i=0; i<QTD_PASSOS-1; i++)
    {
        sprintf(nomeOut, "mkdir Imagens/Saida/1/Passo_%d", 2+i);
        system(nomeOut);
    }


    for(k = 1 ; k <= 25 ; k ++)///ALTERADO
    {
        begin = clock();
        fprintf(relatorio, "Imagem %d\n", k);
        fprintf(tempo, "Imagem %d\n", k);
        #pragma omp parallel num_threads(2)
        {
            #pragma omp sections
            {

                /*
                printf("Imagem %d\n",k);
                /// LE DO PASSO 2
                sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_2\\%d_out.bmp", k);//alterei
                matriz = lerImagem(nomeIn, &x, &y);
                f[2] = matriz[0];

                /// LE DO PASSO 12
                sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_12\\%d_out.bmp", k);//alterei
                matriz = lerImagem(nomeIn, &x, &y);
                f[10] = matriz[0];

                /// LE DO PASSO 15
                sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_15\\%d_out.bmp", k);//alterei
                matriz = lerImagem(nomeIn, &x, &y);
                f[13] = matriz[0];

                /// LE DO PASSO 16
                sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_16\\%d_out.bmp", k);//alterei
                matriz = lerImagem(nomeIn, &x, &y);
                f[14] = matriz[0];


                /// LE DO PASSO 17
                sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_17\\%d_out.bmp", k);//alterei
                matriz = lerImagem(nomeIn, &x, &y);
                f[15] = matriz[0];

                /// LE DO PASSO 18
                sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_18\\%d_out.bmp", k);//alterei
                matriz = lerImagem(nomeIn, &x, &y);
                f[16] = matriz[0];

                ///PASSO 19
                /// MARCADOR EXTERNO

                sprintf(nomeIn, "Imagens\\Saida\\1\\Passo_19\\%d_out.bmp", k);//alterei
                matriz = lerImagem(nomeIn, &x, &y);
                f[17] = matriz[0];
                geraCentroide(f[17],x,y,&xCentro,&yCentro);

                ///PASSO 20

                //        printf("PASSO 20\n");
                //        f[18] = watershed(f[16], f[17], x, y);
                //        sprintf(nomeOut, "Imagens\\Saida\\1\\Passo_%d\\%d_out.bmp", 20, k);
                //        escreveImagem(f[18], nomeOut,x, y);

                */
///-=-=-=-=-=-=-=-=-=-=-=-=-=-=Começo da Detecção do Disco Optico-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-///
                //goto pulo;
                #pragma omp section
                {
                    printf("Processando imagem %d\n", k);
                    sprintf(nomeIn, "Imagens/Entrada/Im%d.bmp", k);//alterei
                    matriz = lerImagem(nomeIn, &x, &y);

                    ///PASSO 28

                    printf("PASSO 28\n");
                    //separarFundo(matriz,x,y);
                    deixaFundoPreto(matriz, x, y);

                    f[1] = alocar_2_dim(y, x, "byte");
                    copia_matriz(matriz[1], f[1], x, y);

                    //L = obterLuminescencia(matriz, x, y);
                    L = alocar_2_dim(y, x, "byte");
                    copia_matriz(matriz[1], L, x, y);

                    f[2] = passo27(L, x, y, estruturante, tamEst);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_28/%d_out.bmp", k);
                    escreveImagem(f[2],nomeOut, x, y);

                    ///PASSO 29

                    printf("PASSO 29\n");
                    f[3] = minRegional(f[2], x, y, est2, tamEst2);

                    for(i=0; i<y; i++)
                        for(j=0; j<x; j++)
                            f[3][i][j] *=255;

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_29/%d_out.bmp", k);
                    escreveImagem(f[3],nomeOut, x, y);

                    ///PASSO 30

                    printf("PASSO 30\n");
                    f[4] = reconstrucaoPorDilatacao(f[3], f[2], x, y, estruturante, tamEst);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_30/%d_out.bmp", k);
                    escreveImagem(f[4],nomeOut, x, y);

                    ///PASSO 31

                    printf("PASSO 31\n");

                    f[5] = subtrairImagens(f[2], f[4], x, y);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_31/%d_out.bmp", k);
                    escreveImagem(f[5],nomeOut, x, y);

                    ///PASSO 32

                    printf("PASSO 32\n");


                    //O valor do limiar = 20 foi empiricamente obtido de acordo com a tese.
                    f[6] = Hmaximo(f[5], x, y, estruturante, tamEst, 7);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_32/%d_out.bmp", k);
                    escreveImagem(f[6],nomeOut, x, y);

                    ///PASSO 33

                    printf("PASSO 33\n");

                    //O valor do limiar = 15 foi empiricamente obtido de acordo com a tese.
                    f[7] = passo33(f[6], x, y, 8);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_33/%d_out.bmp", k);
                    escreveImagem(f[7],nomeOut, x, y);

                    ///-> HÁ PERDA DE REFERENCIA AQUI!


                    printf("PASSO 34 - Hoshen\n");

                    f[7] = hoshen_kopelmanMaiorConjunto(f[7], x, y, 255, 0);


                    f[7] = dilatacao(f[7], x, y, est1, tamEst1);
                    f[7] = erosao(f[7], x, y, est1, tamEst1);

                    f[7] = hoshen_kopelmanMaiorConjunto(f[7], x, y, 0, 1);


                    sprintf(nomeOut, "Imagens/Saida/1/Passo_34/%d_out.bmp", k);
                    escreveImagem(f[7],nomeOut, x, y);

                    printf("PASSO 35 - Gera Centroide\n");

                    geraCentroide(f[7], x, y, &Cx, &Cy);
                    printf("Cx: %d\nCy: %d\n", Cx, Cy);

                    //  f[7][Cx][Cy] = 0;
                    sprintf(nomeOut, "Imagens/Saida/1/Passo_35/%d_out.bmp", k);
                    escreveImagem(f[7],nomeOut, x, y);
                    ///-=-=-=-=-=-=-=-=-=-=-=-=FIM DA DETECÇÂO DO DISCO-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//


///-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-Começo da Detecção do Centro da Fovea-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-///
                    printf("CENTRO DA FOVEA\n");

                    sprintf(nomeIn, "Imagens/Entrada/Normal/Im%d.bmp", k);//alterei
                    matriz = lerImagem(nomeIn, &x, &y);

///Passo 21 - Região de Interesse
                    int tempmax,tempmin;
                    printf("PASSO 21\n");
                    ///TROCAR PELA FUNÇÂO QUE PROCURA ESSA REGIÃO
                    f[8] = RegiaoInteresse(matriz[0],x,y, Cy, Cx);
                    passo21(f[8], XYINTERE, XYINTERE);
                    sprintf(nomeOut,"Imagens/Saida/1/Passo_36/%d_out.bmp", k);
                    escreveImagem(f[8], nomeOut, XYINTERE, XYINTERE);

/// Passo 22 - Remover Imagens Brilhosas
                    printf("PASSO 22\n");
                    f[9] = RemoverRegioesBrilhosas(f[8], XYINTERE, XYINTERE,est1,tamEst1);
                    sprintf(nomeOut, "Imagens/Saida/1/Passo_37/%d_out.bmp", k);
                    escreveImagem(f[9],nomeOut,XYINTERE,XYINTERE);

/// Passo 23 - Filtro de Volumes mininos
                    printf ("PASSO 23\n");
                    f[10] = fechamento(f[9],XYINTERE, XYINTERE, est1, tamEst1);
                    f[10] = fechamento(f[9],XYINTERE, XYINTERE, est4, tamEst4);
                    sprintf(nomeOut, "Imagens/Saida/1/Passo_38/%d_out.bmp", k);
                    escreveImagem(f[10],nomeOut,XYINTERE,XYINTERE);

///Segundo a tese, essa etapa seria necessária, segundo nossos testes, essa etapa pode ser dispensada, pois não usamos o filto de volumes mínimos, porque foi encontrado
///um método mais eficiente.
///PASSO 24
//    printf("PASSO 24\n");
//    f[11] = minRegional(f[10],XYINTERE, XYINTERE,est3,tamEst3);
//    for(j = 0 ; j < XYINTERE ; j++)
//    {
//        for(i = 0 ; i < XYINTERE ; i++)
//        {
//            f[11][i][j]*=255;
//        }
//    }
//    sprintf(nomeOut, "Imagens/Saida/1/Passo_39/%d_out.bmp", k);
//    escreveImagem(f[11],nomeOut,XYINTERE, XYINTERE);

/////PASSO 25
//Obs: A tese diz que geralmente o centro da fóvea está abaixo do centro do disco ótico, mas nossos resultados não comprovam isso.
//    printf("PASSO 25\n");
//    f[12] = passo25(f[11], XYINTERE, XYINTERE, xCentro, yCentro);
//    sprintf(nomeOut, "Imagens/Saida/1/Passo_40/%d_out.bmp", k);
//    escreveImagem(f[11],nomeOut,XYINTERE,XYINTERE);

///PASSO 26
                    printf("PASSO 26\n");
                    f[13] = passo26(f[10], XYINTERE, XYINTERE);
                    sprintf(nomeOut, "Imagens/Saida/1/Passo_41/%d_out.bmp", k);
                    escreveImagem(f[13],nomeOut, XYINTERE, XYINTERE);

                    ///PASSO 27
                    ///(Depois de achar a região onde o centro da fóvea está, acharemos o ponto central)

                    printf("PASSO 27\n");
                    f[14] = matriz[0];

                    f[16] = CentroDaFovea(f[13], XYINTERE, XYINTERE, f[7], x, y, distancia, f[14], &xCentroFovea, &yCentroFovea);

                    printf("Centro da fovea: \nX:%d\nY:%d\n", xCentroFovea, yCentroFovea);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_42/%d_out.bmp", k);
                    escreveImagem(f[16],nomeOut, x, y);
                }

                ///-=-=-=-=-=-=-=-=-=-=-=-=FIM DA DETECÇÂO DO CENTRO DA FÓVEA-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//


                ///-=-=-=-=-=-=-=-=-=-=-=-=COMEÇO DA DETECÇÃO DOS EXSUDATOS-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
//pulo:
                #pragma omp section
                {
                    ///PASSO 34.2

                    printf("Carrega a imagem normal\n");
                    sprintf(nomeIn, "Imagens/Entrada/Normal/Im%d.bmp", k);//alterei
                    matriz = lerImagem(nomeIn, &x, &y);

                    ///PASSO 28.2

                    printf("PASSO 28.2\n");
                    //separarFundo(matriz,x,y);
                    deixaFundoPreto(matriz, x, y);

                    f[17] = alocar_2_dim(y, x, "byte");
                    copia_matriz(matriz[1], f[17], x, y);

                    //L = obterLuminescencia(matriz, x, y);
                    L = obterLuminescencia(matriz,x,y);

                    f[18] = passo27(L, x, y, estruturante, tamEst);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_43/%d_out.bmp", k);
                    escreveImagem(f[18],nomeOut, x, y);

                    ///PASSO 29.2

                    printf("PASSO 29.2\n");
                    f[19] = minRegional(f[18], x, y, est13, tamEst13);

                    for(i=0; i<y; i++)
                        for(j=0; j<x; j++)
                            f[19][i][j] *=255;

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_44/%d_out.bmp", k);
                    escreveImagem(f[19],nomeOut, x, y);

                    ///PASSO 30.2

                    printf("PASSO 30.2\n");
                    f[20] = reconstrucaoPorDilatacao(f[19], f[18], x, y, estruturante, tamEst);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_45/%d_out.bmp", k);
                    escreveImagem(f[20],nomeOut, x, y);

                    ///PASSO 31.2

                    printf("PASSO 31.2\n");

                    f[21] = subtrairImagens(f[18], f[20], x, y);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_46/%d_out.bmp", k);
                    escreveImagem(f[21],nomeOut, x, y);

                    ///PASSO 32.2

                    printf("PASSO 32.2\n");


                    //O valor do limiar = 20 foi empiricamente obtido de acordo com a tese.
                    f[22] = Hmaximo(f[21], x, y, estruturante, tamEst, 10); //15

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_47/%d_out.bmp", k);
                    escreveImagem(f[22],nomeOut, x, y);

                    ///PASSO 33.2

                    printf("PASSO 33.2\n");

                    //O valor do limiar = 15 foi empiricamente obtido de acordo com a tese.
                    f[23] = passo33(f[22], x, y, 5);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_48/%d_out.bmp", k);
                    escreveImagem(f[23],nomeOut, x, y);
                }
            }
///=-=-=-=-=-=-=-=-=--=-=-Refinamento dos Exsudados-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-///
            #pragma omp barrier
            {
                #pragma omp single
                {

                    printf("PASSO 34.2\n");

                    f[24] = dilatacao(f[23], x, y, est1, tamEst1);
                    f[25] = passo34(matriz[1], f[24], x, y);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_49/%d_out.bmp", k);
                    escreveImagem(f[25],nomeOut, x, y);

                    ///    PASSO 35.2
                    printf("PASSO 35.2\n");

                    f[26] = reconstrucaoPorDilatacao(f[25], matriz[1], x, y, est5, tamEst5);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_50/%d_out.bmp", k);
                    escreveImagem(f[26],nomeOut, x, y);

                    ///PASSO 36
                    printf("PASSO 36\n");

                    f[27] = subtrairImagens(matriz[1], f[26], x, y);

                    //O valor do limiar = 2 foi empiricamente obtido de acordo com a tese.
                    f[28] = passo33(f[27], x, y, 2);

                    sprintf(nomeOut, "Imagens/Saida/1/Passo_51/%d_out.bmp", k);
                    escreveImagem(f[28],nomeOut, x, y);

                    ///PASSO 37 - Tirar os falsos exsudatos
                    f[29] = erosao(f[28], x, y, est3alterado, tamEstAlterado);
                    f[30] = dilatacao(f[29], x, y, est3alterado, tamEstAlterado);
                    sprintf(nomeOut, "Imagens/Saida/1/Passo_52/%d_out.bmp", k);
                    escreveImagem(f[30],nomeOut, x, y);

                    ///PASSO 38 - Descarta a area do disco optico da imagem dos exsudatos

                    f[31] = passo38(f[30], f[7], x, y);
                    sprintf(nomeOut, "Imagens/Saida/1/Passo_53/%d_out.bmp", k);
                    escreveImagem(f[31],nomeOut, x, y);

                    ///PASSO 39 - Contagem dos pixels ao redor do centro da fóvea

                    qtdCentral = passo39(f[31], x, y, estCentral, tamEstCentral, xCentroFovea, yCentroFovea, 1, k);
                    printf("\nContagem Central: %d\n", qtdCentral);
                    fprintf(relatorio, "Contagem Central: %d\n", qtdCentral);

                    qtdInter = passo39(f[31], x, y, estIntermediario, tamEstIntermediario, xCentroFovea, yCentroFovea, 2, k);
                    printf("\nContagem Intermediario: %d\n", qtdInter);
                    fprintf(relatorio, "Contagem Intermediario: %d\n", qtdInter);

                    qtdExt = passo39(f[31], x, y, estExterno, tamEstExterno, xCentroFovea, yCentroFovea, 3, k);
                    printf("\nContagem Externo: %d\n", qtdExt);
                    fprintf(relatorio, "Contagem Externo: %d\n\n", qtdExt);
                }
            }

            end = clock();
            if(qtdExt < 5 && qtdInter < 5 && qtdCentral < 5)
            {
                printf("\nDiagnostico: Ausente\n");
                fprintf(relatorio, "Diagnostico: Ausente\n\n");
            }
            else if(((qtdCentral + qtdInter + qtdExt) > 700 && (0.62*(qtdCentral + qtdInter + qtdExt) > qtdExt)) || qtdCentral > 70)
            {
                printf("\nDiagnostico: Grave\n");
                fprintf(relatorio, "Diagnostico: Grave\n\n");
            }
            else if(qtdCentral < 6 && qtdInter < 20 && qtdExt < 290)
            {
                printf("\nDiagnostico: Leve\n");
                fprintf(relatorio, "Diagnostico: Leve\n\n");
            }
            else
            {
                printf("\nDiagnostico: Moderado\n");
                fprintf(relatorio, "Diagnostico: Moderado\n\n");
            }
        }





//    int CXX;
//    int CYY;
//
//    geraCentroide(f[24], XYINTERE, XYINTERE, &CXX, &CYY);
//    printf("CENT X:%d\nCENT Y: %d\n ", CXX, CYY);
//
//    xDistancia = CXX -




        // liberarMatriz(matriz, y);
        //liberarMatriz2D(L, y);
        //liberarMatriz2D(f[1], y);

        ///PASSO 36 Liberação das matrizes
//
//    printf("PASSO 34\n");
//
//    for(i = 26 ; i < 32 ; i++)
//    {
//        liberarMatriz2D(f[i], y);
//    }

        fprintf(tempo,"Tempo: %f\n",(double)(end - begin) / CLOCKS_PER_SEC);


        printf(" FIM - Imagem %d \n\n", k);

    }

    fclose(relatorio);

//system("pause");

    return 0;
}
