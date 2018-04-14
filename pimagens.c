#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "pimagens.h"


byte** fundo;


void liberarMatriz2D(byte** p, int y)
{
    int i;
    for(i=0; i<y; i++)
    {
        free(p[i]);
    }
    free(p);
}

float modulo(float x)
{
    if(x<0)
        x = x*(-1);

    return x;
}



byte** separarFundo(byte*** matriz, int  x, int y)
{
    int i, j,k ;
    byte **estruturante;
    fundo = (byte**) alocar_2_dim(y,x,"byte");

    estruturante = (byte**)alocar_2_dim(5,5,"byte");

    for(i=0; i<5; i++)
    {
        for(j=0; j<5; j++)
        {
            estruturante[i][j] = 1;
        }
    }

    for(k=0; k < 3; k++)
    {
        for(i=0; i < y; i++)
        {
            for(j=0; j < x; j++)
            {
                if(matriz[0][i][j] < 14 || matriz[1][i][j] < 14 || matriz[2][i][j] < 14)
                {
                    fundo[i][j] = matriz[0][i][j] = 0;
                    fundo[i][j] = matriz[1][i][j] = 0;
                    fundo[i][j] = matriz[2][i][j] = 0;
                }
                else
                {
                    fundo[i][j] = 255;
                    fundo[i][j] = 255;
                    fundo[i][j] = 255;
                }
            }
        }

    }
    fundo = hoshen_kopelman(fundo, x, y, 200, 255);
    fundo = hoshen_kopelman(fundo, x, y, 200, 0);
    fundo = dilatacao(fundo,x,y,estruturante,5);

    return fundo;
}


// COLOCAR TAMANHO DA JANELA PROPORCIONAL À IMAGEM
byte** coordCentro(byte** matriz, int x, int y, int *xCentro, int *yCentro)
{
    int i, j, k, l, pixelsDentro;
    float maxVariancia=0, minVariancia =256*256, variancia, media, fator;
    float** matrizVarianciaAux;
    byte** matrizVariancia;

    matrizVarianciaAux = (float**)alocar_2_dim(y,x,"float");
    matrizVariancia = (byte**)alocar_2_dim(y,x,"byte");

    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            if(fundo[i][j] == 0)
            {
                pixelsDentro = 0;
                media = 0;
                variancia = 0;
                // DESCOBRE A MÉDIA
                for(k=i-TAMANHO_JANELA/2; k<i+TAMANHO_JANELA/2; k++)
                {
                    for(l=j-TAMANHO_JANELA/2; l<j+TAMANHO_JANELA/2; l++)
                    {
                        if(k<y && k>=0 && l<x && l>=0 && fundo[k][l]==0)
                        {
                            media = media + matriz[k][l];
                            pixelsDentro++;
                        }

                    }
                }

                if(pixelsDentro != 0)
                {
                    media = media/pixelsDentro;
                }
                else
                {
                    media = 0;
                }

                for(k=i-TAMANHO_JANELA/2; k<i+TAMANHO_JANELA/2; k++)
                {
                    for(l=j-TAMANHO_JANELA/2; l<j+TAMANHO_JANELA/2; l++)
                    {
                        if(k<y && k>=0 && l<x && l>=0 && fundo[k][l]==0)
                            variancia += (matriz[k][l]-media)*(matriz[k][l]-media);
                    }
                }

                if(pixelsDentro != 0)
                {
                    variancia = variancia/pixelsDentro;
                }
                else
                {
                    variancia = 0;
                }

//                ///if(variancia>16000)
//                printf("variancia = %f [%d] [%d]\n", variancia, i, j);

//            printf("%f\n", variancia);
                matrizVarianciaAux[i][j] = variancia;
//            printf("v = %f\n", variancia);

                if(i==0 && j==0)
                {
                    maxVariancia = variancia;
                    minVariancia = variancia;
                }

                if(variancia<minVariancia)
                {
                    minVariancia = variancia;
                }

                if(variancia>maxVariancia)
                {
                    maxVariancia = variancia;
                    *xCentro = j+TAMANHO_JANELA/2;
                    *yCentro = i+TAMANHO_JANELA/2;
                }
            }
        }
    }

    if(maxVariancia > 0)
    {
        fator = 255.0/(maxVariancia);
    }
    else
    {
        fator = 1;
    }


    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            if(fundo[i][j] == 0)
            {
                matrizVariancia[i][j] = round(matrizVarianciaAux[i][j]*fator);
            }
            else
            {
                matrizVariancia[i][j] = 0;
            }
        }
    }

    return matrizVariancia;
}

byte** leEstruturante( int* tamanho, char* nome)
{
    int i,j;
    FILE *f = fopen(nome, "r");
    byte** retorno;

    fscanf(f, "%d\n", tamanho);

    retorno = (byte**)alocar_2_dim(*tamanho, *tamanho, "byte");

    for(i=0; i<*tamanho; i++)
        for(j=0; j<*tamanho; j++)
            fscanf(f, "%u \n", &retorno[i][j]);

    fclose(f);

    return retorno;
}

void** alocar_2_dim(int linhas, int colunas, char* tipo)//aloca matrizes de duas dimensoes (tipo = "byte", "float", "int")
{
    int i;
    if(strcmp(tipo,"int") == 0)
    {
        int** matriz;
        matriz = (int**) malloc(linhas*sizeof(int*));
        if(matriz == NULL)
        {
            printf("\nErro na alocacao\n");
            exit(1);
        }
        for(i = 0; i < linhas; i++)
        {
            matriz[i] = (int*) calloc(colunas, sizeof(int));
            if(matriz[i] == NULL)
            {
                printf("\nErro na alocacao %d\n", i);
                exit(1);
            }
        }
        return (void**) matriz;
    }
    if(strcmp(tipo,"float") == 0)
    {
        float** matriz;
        matriz = (float**) malloc(linhas*sizeof(float*));
        if(matriz == NULL)
        {
            printf("\nErro na alocacao\n");
            exit(1);
        }
        for(i = 0; i < linhas; i++)
        {
            matriz[i] = (float*) calloc(colunas,sizeof(float));
            if(matriz[i] == NULL)
            {
                printf("\nErro na alocacao %d\n", i);
                exit(1);
            }
        }
        return (void**) matriz;
    }
    if(strcmp(tipo,"byte") == 0)
    {
        byte** matriz;
        matriz = (byte**) malloc(linhas*sizeof(byte*));
        if(matriz == NULL)
        {
            printf("\nErro na alocacao\n");
            exit(1);
        }
        for(i = 0; i < linhas; i++)
        {
            matriz[i] = (byte*) calloc(colunas,sizeof(byte));
            if(matriz[i] == NULL)
            {
                printf("\nErro na alocacao %d\n", i);
                exit(1);
            }
        }
        return (void**) matriz;
    }
    return NULL;
}

void*** alocar_3_dim(int canais, int linhas, int colunas, char* tipo)//aloca matrizes de tres dimensoes (tipo = "byte", "float", "int")
{
    int i, j;
    if(strcmp(tipo,"int") == 0)
    {
        int*** matriz;
        matriz = (int***) malloc(canais*sizeof(int**));
        if(matriz == NULL)
        {
            printf("\nErro na alocacao\n");
            exit(1);
        }
        for(i = 0; i < canais; i++)
        {
            matriz[i] = (int**) malloc(linhas*sizeof(int*));
            if(matriz[i] == NULL)
            {
                printf("\nErro na alocacao %d\n", i);
                exit(1);
            }
            for(j=0; j < linhas; j++)
            {
                matriz[i][j] = (int*) calloc(colunas, sizeof(int));
                if(matriz[i][j] == NULL)
                {
                    printf("\nErro na alocacao %d %d\n", i, j);
                    exit(1);
                }
            }
        }
        return (void***) matriz;
    }
    if(strcmp(tipo,"float") == 0)
    {
        float*** matriz;
        matriz = (float***) malloc(canais*sizeof(float**));
        if(matriz == NULL)
        {
            printf("\nErro na alocacao\n");
            exit(1);
        }
        for(i = 0; i < canais; i++)
        {
            matriz[i] = (float**) malloc(linhas*sizeof(float*));
            if(matriz[i] == NULL)
            {
                printf("\nErro na alocacao %d\n", i);
                exit(1);
            }
            for(j=0; j < linhas; j++)
            {
                matriz[i][j] = (float*) calloc(colunas, sizeof(float));
                if(matriz[i][j] == NULL)
                {
                    printf("\nErro na alocacao %d %d\n", i, j);
                    exit(1);
                }
            }
        }
        return (void***) matriz;
    }
    if(strcmp(tipo,"byte") == 0)
    {
        byte*** matriz;
        matriz = (byte***) malloc(canais*sizeof(byte**));
        if(matriz == NULL)
        {
            printf("\nErro na alocacao\n");
            exit(1);
        }
        for(i = 0; i < canais; i++)
        {
            matriz[i] = (byte**) malloc(linhas*sizeof(byte*));
            if(matriz[i] == NULL)
            {
                printf("\nErro na alocacao %d\n", i);
                exit(1);
            }
            for(j=0; j < linhas; j++)
            {
                matriz[i][j] = (byte*) calloc(colunas, sizeof(byte));
                if(matriz[i][j] == NULL)
                {
                    printf("\nErro na alocacao %d %d\n", i, j);
                    exit(1);
                }
            }
        }
        return (void***) matriz;
    }
    return NULL;
}

void leHeader(byte* buffer, unsigned *x, unsigned *y, unsigned *comeco)
{
    // LENDO OS DADOS ESSENCIAIS DO CABEÇÁRIO BMP

    // INICIO DOS PIXELS
    *comeco = *((unsigned*)(buffer+10));

    // DIMENSÃO X
    *x = *((unsigned*)(buffer+18));

    // DIMENSÃO Y
    *y = *((unsigned*)(buffer+22));

}

void inverteMatriz(byte** matriz, unsigned y)
{
    unsigned i;
    byte* aux;

    // INVERTE AS LINHAS
    for(i=0; i<y/2; i++)
    {
        aux = matriz[i];
        matriz[i] = matriz[y-1-i];
        matriz[y-1-i] = aux;
    }

}

unsigned calculaPadding(unsigned x)
{
    unsigned padding = 4-((x*3)%4);
    if(padding==4) padding = 0;

    return padding;
}

byte*** geraMatriz(byte* buffer, unsigned x, unsigned y, unsigned comeco)
{
    int i, j, padding, puloPadding;
    byte ***matriz;

    // CRIANDO UMA MATRIZ A PARTIR DOS PIXELS NA IMAGEM
    // ALOCA A MATRIZ
    matriz = (byte***)malloc(3*sizeof(byte**));
    if(matriz==NULL) exit(1);

    for(i=0; i<3; i++)
    {
        matriz[i] = (byte**)malloc(y*sizeof(byte*));
        if(matriz[i] == NULL ) exit(1);
        for(j=0; j<y; j++)
        {
            matriz[i][j]=(byte*)malloc(x*sizeof(byte));
            if(matriz[i][j] == NULL) exit(1);
        }

    }

    // CALCULA O "PADDING"

    padding = calculaPadding(x);

    puloPadding = 0;

    //  LÊ DO BUFFER PARA A MATRIZ
    for(i=0; i<x*y; i++)
    {
        matriz[2][i/x][i%x]=buffer[comeco+puloPadding+i*3 + 0];//CANAL B
        matriz[1][i/x][i%x]=buffer[comeco+puloPadding+i*3 + 1];//CANAL G
        matriz[0][i/x][i%x]=buffer[comeco+puloPadding+i*3 + 2];//CANAL R

        // LÊ NORMALMENTE
        if(i%x==x-1 && i!=0) puloPadding=puloPadding+padding;
    }

    inverteMatriz(matriz[0], y);
    inverteMatriz(matriz[1], y);
    inverteMatriz(matriz[2], y);

    return matriz;

}

unsigned geraBuffer(byte** bf, byte*** matriz, unsigned x, unsigned y)
{
    int i, j, k, padding = calculaPadding(x), puloPadding;
    byte* buffer;

    buffer = (byte*)malloc((54+(x+padding)*y*3)*sizeof(byte));

    inverteMatriz(matriz[0], y);
    inverteMatriz(matriz[1], y);
    inverteMatriz(matriz[2], y);

    // MAGIC NUMBER
    buffer[0] = 'B';
    buffer[1] = 'M';

    // TAMANHO DO ARQUIVO
    *((unsigned*)(buffer+2)) = (54+(x+padding)*y*3);

    // NÃO USADO
    *((unsigned*)(buffer+6)) = 0;

    // COMEÇO DO ARRAY DE PIXELS
    *((unsigned*)(buffer+10)) = 54;

    // BYTES ATÉ O ARRAY DE PIXELS A PARTIR DAQUI
    *((unsigned*)(buffer+14)) = 40;

    // QUANTIDADE DE PIXELS NA HORIZONTAL
    *((unsigned*)(buffer+18)) = x;

    // QUANTIDADE DE PIXELS NA VERTICAL
    *((unsigned*)(buffer+22)) = y;

    // COLOR PLANES (SEMPRE 1)
    *((unsigned short*)(buffer+26)) = 1;

    // BITS POR PIXEL
    *((unsigned short*)(buffer+28)) = 24;

    // COMPRESSÃO
    *((unsigned*)(buffer+30)) = 0;

    // QUANTIDADE DE PIXELS USADOS PARA PADDING (OU 0)
    *((unsigned*)(buffer+34)) = 0;

    // RESOLUÇÃO HORIZONTAL (OU 0)
    *((unsigned*)(buffer+38)) = 0;

    // RESOLUÇÃO VERTICAL (OU 0)
    *((unsigned*)(buffer+42)) = 0;

    // QUANTIDADE DE CORES (QUANDO NÃO HÁ MAPA DE CORES É 0)
    *((unsigned*)(buffer+46)) = 0;

    // CORES IMPORTANTES (QUANDO NÃO HÁ MAPA DE CORES É 0)
    *((unsigned*)(buffer+50)) = 0;

    // COLOCA OS PIXELS NO BUFFER
    puloPadding=0;

    k=54;

    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            buffer[k] = matriz[2][i][j];
            k++;
            buffer[k] = matriz[1][i][j];
            k++;
            buffer[k] = matriz[0][i][j];
            k++;
        }

        for(j=0; j<padding; j++)
        {
            buffer[k] = 0;
            //printf("%u\n", k);
            k++;
        }
    }

    *bf = buffer;

    return (54+(x+padding)*y*3);

}

void liberarMatrizLAB(float*** p, int y)
{
    int i, j;

    for(i=0; i < 3; i++)
    {
        for(j=0; j<y; j++)
            free(p[i][j]);
        free(p[i]);
    }
    free(p);
}

void liberarMatriz(byte*** p, int y)
{
    int i, j;

    for(i=0; i < 3; i++)
    {
        for(j=0; j<y; j++)
            free(p[i][j]);
        free(p[i]);
    }
    free(p);
}

float f(float x)
{
    float resultado;
    if( x > 0.008856)
    {
        resultado = pow(x, 0.33333);
    }
    else
    {
        resultado = 7.787*x + 16/116;
    }
    return resultado;
}

byte** obterLuminescencia(byte*** matriz, int x, int y)
{
    int i, j, k;
    float*** resultado = (float***) alocar_3_dim(3, y, x, "float");
    byte** resultado2 = (byte**) alocar_2_dim(y, x, "byte");
    float xyz[3][3] = {{0.490, 0.310, 0.200},{0.177, 0.813, 0.010}, {0.0, 0.01, 0.990}};
    float yn = 1;//white tristimulus (CIE daylight D65 pesquise na wikipedia :))
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            ///CONVERSAO RGB PARA XYZ
            for(k=0; k<3; k++)
            {

                resultado[k][i][j] = (xyz[k][0]*((float)matriz[0][i][j])) / 255.0 + (xyz[k][1]*((float)matriz[1][i][j])) / 255.0 + (xyz[k][2]*((float)matriz[2][i][j])) / 255.0;

            }
            ///OBTER LUMINESCENCIA
            if(resultado[1][i][j]/yn > 0.008856)
            {
                resultado2[i][j] = trunc(116.0*pow(resultado[1][i][j]/yn, 1.0/3.0) - 16.0);
            }
            else
            {
                resultado2[i][j] = trunc(903.3*resultado[1][i][j]/yn);
            }

            if(resultado2[i][j]<0)
            {
                resultado2[i][j] = 0;
            }
            else if(resultado2[i][j]>=100)
            {
                resultado2[i][j] = 99;
            }
        }
    }
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            byte a = 2.5*resultado2[i][j];
            resultado2[i][j] = a;
        }
    }
    liberarMatrizLAB(resultado, y);

    return resultado2;
}

byte** dilatacao(byte** matriz, int x, int y, byte** v, int tamEst)
{
    int i, j, m,n, max =0;
    byte** result = (byte**) alocar_2_dim(y, x, "byte");
    for(i=0; i < y; i++)
    {
        for(j=0; j <x; j++)
        {
            max = 0;
            for(m=-tamEst/2; m <= tamEst/2; m++)
            {
                for(n=-tamEst/2; n <= tamEst/2; n++)
                {
                    if(((i+m) >= 0) && ((j+n) >= 0) && ((i+m) < y) &&  ((j+n) < x)  && v[m+tamEst/2][n + tamEst/2])
                    {
                        if(matriz[i+m][j+n] > max)
                        {
                            max = matriz[i+m][j+n];
                        }
                    }
                }
            }
            result[i][j] = max;
        }
    }
    return result;
}

byte** erosao(byte** matriz, int x, int y, byte** v, int tamEst)
{
    int i, j, m,n, min =0;
    byte** result = (byte**)alocar_2_dim(y, x, "byte");
    for(i=0; i < y; i++)
    {
        for(j=0; j <x; j++)
        {
            min = matriz[i][j]+ v[0][0];
            for(m=-tamEst/2; m <= tamEst/2; m++)
            {
                for(n=-tamEst/2; n <= tamEst/2; n++)
                {
                    if(((i+m) >= 0) && ((j+n) >= 0) && ((i+m) < y) &&  ((j+n) < x) && v[m+tamEst/2][n + tamEst/2])
                    {
                        if(matriz[i+m][j+n] < min)
                        {
                            min = matriz[i+m][j+n];
                        }
                    }
                }
            }
            result[i][j] = min;
        }
    }
    return result;
}

//erosao seguida de uma dilatacao
byte** abertura(byte** matriz, int x, int y, byte** v, int tamEst)
{
    byte** resultado, **aux;
    resultado = erosao(matriz, x, y, v, tamEst);
    aux = resultado;
    resultado = dilatacao(resultado, x, y, v, tamEst);
    liberarMatriz2D(aux, y);
    return resultado;
}

//dilatacao seguida de uma erosao
byte** fechamento(byte** matriz, int x, int y, byte** v, int tamEst)
{
    byte** resultado, **aux;
    resultado = dilatacao(matriz, x, y, v, tamEst);
    aux = resultado;
    resultado = erosao(resultado, x, y, v, tamEst);
    liberarMatriz2D(aux, y);
    return resultado;
}

//a imagem menos a sua abertura
byte** top_hatA(byte** matriz, int x, int y, byte** v, int tamEst)
{
    int i, j;
    byte** resultado = (byte**) alocar_2_dim(y,x,"byte"), **abertura_imagem;
    abertura_imagem = abertura(matriz, x, y, v, tamEst);
    for(i =0; i < y; i++)
    {
        for(j =0; j < x; j++)
        {
            resultado[i][j] = matriz[i][j] - abertura_imagem[i][j];
        }
    }

    liberarMatriz2D(abertura_imagem, y);

    return resultado;
}

//a imagem menos o seu fechamento
byte** top_hatF(byte** matriz, int x, int y, byte** v, int tamEst)
{
    int i, j;
    byte** resultado = (byte**)alocar_2_dim(y,x,"byte"), **fechamento_imagem;
    fechamento_imagem = fechamento(matriz, x, y, v, tamEst);
    for(i =0; i < y; i++)
    {
        for(j =0; j < x; j++)
        {
            resultado[i][j] = fechamento_imagem[i][j] - matriz[i][j];
        }
    }

    liberarMatriz2D(fechamento_imagem, y);

    return resultado;
}

//subtrai mat1 - mat2
void subtrai_matriz(byte**mat1,byte**mat2,int x,int y)
{
    int i,j;
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            mat1[i][j] = mat1[i][j] - mat2[i][j];
        }
    }
}

//soma mat1 + mat2
void soma_matriz(byte**mat1,byte**mat2,int x,int y)
{
    int i,j;
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            mat1[i][j] = mat1[i][j] + mat2[i][j];

        }
    }
}

//copia de mat1 para mat2
void copia_matriz(byte** mat1, byte**mat2, int x,int y)
{
    int i,j;
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            mat2[i][j] = mat1[i][j];
        }
    }
}

int compara_matriz(byte** mat1, byte** mat2, int x, int y)
{
    int i,j,result=1;

    for(i=0; i<y && result == 1; i++)
    {
        for(j=0; j<x && result == 1; j++)
        {
            if(mat1[i][j] != mat2[i][j])
            {
                result = 0;
            }
        }
    }
    return result;
}

//retorna matriz com os maiores elementos entre as 2 matrizes
byte** encontra_max(byte**mat1,byte**mat2,int x,int y)
{
    int i,j;
    byte** Fun;
    Fun = (byte**)malloc(y*sizeof(byte*));
    for(i=0; i<y; i++)
    {
        Fun[i] = (byte*)malloc(x*sizeof(byte));
    }

    //Fun recebe o maior valor entre as duas matrizes, faz isso para cada pixel
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            if(mat1[i][j] > mat2[i][j])
            {
                Fun[i][j] = mat1[i][j];
            }
            else
            {
                Fun[i][j] = mat2[i][j];
            }
        }
    }
    return Fun;

}

///retorna matriz com os menores elementos entre as 2 matrizes
byte** encontra_min(byte**mat1,byte**mat2,int x,int y)
{
    int i,j;
    byte** Fun;
    Fun = (byte**)malloc(y*sizeof(byte*));
    for(i=0; i<y; i++)
    {
        Fun[i] = (byte*)malloc(x*sizeof(byte));
    }

    //Fun recebe o maior valor entre as duas matrizes, faz isso para cada pixel
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            if(mat1[i][j] < mat2[i][j])
            {
                Fun[i][j] = mat1[i][j];
            }
            else
            {
                Fun[i][j] = mat2[i][j];
            }
        }
    }
    return Fun;

}

///REALIZA UMA EROSAO NA IMAGEM E DEPOIS COMPARA PIXEL A PIXEL COM A IMAGEM ORIGINAL E
///COMO RESULTADO RETORNA UMA IMAGEM QUE CONTEM O MAIORES PIXELS ENTRE AS DUAS
byte** erosaoGeodesica(byte** matriz, byte** mascara, int x, int y, byte** v, int tamEst)
{
    byte** resultado, **Rerosao, **aux;
    Rerosao = erosao(matriz, x, y, v, tamEst);
    aux = Rerosao;
    resultado = encontra_max(mascara, Rerosao, x, y);
    liberarMatriz2D(aux, y);
    return resultado;
}

///REALIZA UMA DILATACAO NA IMAGEM E DEPOIS COMPARA PIXEL A PIXEL COM A IMAGEM ORIGINAL E
///COMO RESULTADO RETORNA UMA IMAGEM QUE CONTEM OS MENORES PIXELS ENTRE AS DUAS
byte** dilatacaoGeodesica(byte** matriz, byte** mascara,int x, int y, byte** v, int tamEst)
{
    byte** resultado, **Rdilatacao, **aux;
    Rdilatacao = dilatacao(matriz, x, y, v, tamEst);
    aux = Rdilatacao;
    resultado = encontra_min(mascara, Rdilatacao, x, y);
    liberarMatriz2D(aux, y);
    return resultado;
}

///REALIZA UMA NOVA DILATACAO GEODESICA ENQUANTO A IMAGEM RESULTANTE DA ULTIMA GEODESICA FOR
///DIFERENTE DA RESULTANTE DA PENULTIMA
byte** reconstrucaoPorDilatacao(byte** matriz, byte** mascara,int x, int y, byte** v, int tamEst)
{
    byte** im1 = (byte**) alocar_2_dim(y, x, "byte"), **im2, **aux;
    copia_matriz(matriz, im1, x, y);
    im2 = dilatacaoGeodesica(im1, mascara,x, y, v, tamEst);
    while(compara_matriz(im1, im2, x, y) == 0)
    {
        copia_matriz(im2, im1,x, y);
        aux = im2;
        im2 = dilatacaoGeodesica(im1, mascara,x, y, v, tamEst);
        liberarMatriz2D(aux, y);
    }

    liberarMatriz2D(im1, y);

    return im2;
}

///REALIZA UMA NOVA EROSAO GEODESICA ENQUANTO A IMAGEM RESULTANTE DA ULTIMA GEODESICA FOR
///DIFERENTE DA RESULTANTE DA PENULTIMA
byte** reconstrucaoPorErosao(byte** matriz, byte** mascara,int x, int y, byte** v, int tamEst)
{
    byte** im1 = (byte**) alocar_2_dim(y, x, "byte"), **im2, **aux;
    copia_matriz(matriz, im1, x, y);
    im2 = erosaoGeodesica(matriz, mascara,x, y, v, tamEst);
    while(compara_matriz(im1, im2, x, y) == 0)
    {
        copia_matriz(im2, im1,x, y);
        aux = im2;
        im2 = erosaoGeodesica(im1, mascara,x, y, v, tamEst);
        liberarMatriz2D(aux, y);
    }

    liberarMatriz2D(im1, y);

    return im2;
}

byte** minRegional(byte** matriz, int x, int y,byte **Est, int TamEst)
{
    int i, j;
    byte** im1 = (byte**) alocar_2_dim(y, x, "byte"), **aux;
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            im1[i][j] = matriz[i][j] + 1;
        }
    }

    aux = im1;

    im1 = reconstrucaoPorErosao(im1, matriz,x, y, Est, TamEst);

    liberarMatriz2D(aux, y);

    subtrai_matriz(im1, matriz, x, y);

    return im1;
}

byte** maxRegional(byte** matriz, int x, int y, byte** Est, int tamEst)
{
    int i, j;
    byte** im1 = (byte**) alocar_2_dim(y, x, "byte"), **aux;
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            if(matriz[i][j] + 1 < 255)
            {
                im1[i][j] = matriz[i][j] +1;
            }
            else
            {
                im1[i][j] = 255;
            }
        }
    }

    aux = reconstrucaoPorDilatacao(matriz, im1,x, y, Est, tamEst);

    subtrai_matriz(im1, aux, x, y);

    liberarMatriz2D(aux, y);

    return im1;
}

byte** Hmaximo(byte** matriz, int x, int y, byte** Est, int tamEst, int limiar)
{
    int i, j;
    byte** im1 = (byte**) alocar_2_dim(y, x, "byte"), **retorno;
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            if(matriz[i][j] - limiar < 0)
            {
                im1[i][j] = 0;
            }
            else
            {
                im1[i][j] = matriz[i][j] - limiar;
            }
        }
    }

    retorno = reconstrucaoPorDilatacao(im1, matriz, x, y, Est, tamEst);

    liberarMatriz2D(im1, y);

    return retorno;
}

byte** Hminimo(byte** matriz, int x, int y, byte** Est, int tamEst, int limiar)
{
    int i, j;
    byte** im1 = (byte**) alocar_2_dim(y, x, "byte"), **retorno;
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            if(matriz[i][j] + limiar > 255)
            {
                im1[i][j] = 255;
            }
            else
            {
                im1[i][j] = matriz[i][j] + limiar;
            }
        }
    }

    retorno = reconstrucaoPorErosao(im1, matriz, x, y, Est, tamEst);

    liberarMatriz2D(im1, y);

    return retorno;
}

typedef struct celula
{
    int quantidade;
    struct celula* ptr;
} celula;

celula* find(celula* p)//Localiza o representante de uma celula
{
    while(p->ptr != p)
    {
        p = p->ptr;
    }
    return p;
}

celula* Union(celula* temp, celula* temp2)//une duas celulas com representantes diferentes
{
    celula* retorno;
    if(temp->quantidade >= temp2->quantidade)
    {
        temp->quantidade = temp->quantidade + temp2->quantidade;
        temp2->ptr = temp;
        retorno = temp;
    }
    else
    {
        temp2->quantidade = temp->quantidade + temp2->quantidade;
        temp->ptr = temp2;
        retorno = temp2;
    }
    return retorno;
}

byte** hoshen_kopelman(byte** matriz, int x, int y, int limiar,  int cor)
{
    int i, j, k, l;
    celula** aux;//mapa de celulas ligadas)
    celula* temp, *temp2;
    byte** resultado;
    aux = (celula**) malloc(y*sizeof(celula*));

    //Alocacao e inicializacao das matrizes aux e resultado
    if(aux == NULL)
    {
        printf("Erro na alocacao da matriz aux em hoshen-kopelman");
        exit(1);
    }
    resultado = (byte**) malloc(y*sizeof(byte*));
    if(resultado == NULL)
    {
        printf("Erro na alocacao da matriz resultado em hoshen-kopelman");
        exit(1);
    }
    for(i=0; i<y; i++)
    {
        aux[i] = (celula*) malloc(x*sizeof(celula));
        resultado[i] = (byte*) malloc(x*sizeof(byte));
        if(resultado[i] == NULL || aux[i] == NULL)
        {
            printf("Erro na alocacao em hoshen-kopelman");
            exit(1);
        }
        for(j=0; j<x; j++)
        {
            aux[i][j].quantidade = -1;
            aux[i][j].ptr = NULL;
        }
    }
    //Parte 1: inicio da segmentacao
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            if(matriz[i][j] == cor)//caso o pixel tenha cor branca(cor da arteria)
            {
                temp = NULL;//ponteiro auxiliar que guarda o endereco de um representante proximo ou caso noa haja nenhum guarda null
                for(k=-1; k <= 1; k++)
                {
                    for(l=-1; k <= 1; k++)
                    {
                        if((i+k >= 0) && (j+l >= 0) && (j+l < x) && (i+k < y))//verifica se a celula sendo verificada nao esta na borda
                        {
                            if(aux[i+k][j+l].ptr != NULL)//caso haja alguma celula nos arredores da celula sendo pesquisada com representante
                            {
                                temp2 = find(aux[i+k][j+l].ptr);//descobre quem e o representante e guarda em temp2
                                if(temp == NULL)//se nao ja havia sido encontrado outro representante para a celula sendo pesquisada
                                {
                                    temp = temp2;//o representante dessa celula passara a ser o representante temp2
                                }
                                else if(temp != temp2)//caso outro representante ja havia sido encontrado
                                {
                                    temp = Union(temp, temp2);// une os dois conjuntos e atribui o representante desse novo conjunto a celula
                                }
                            }
                        }
                    }
                    if(temp == NULL)// se no final nenhum representate foi encontrado entao eh criado um novo representante
                    {
                        aux[i][j].ptr = &aux[i][j];
                        aux[i][j].quantidade = 1;
                    }
                    else
                    {
                        aux[i][j].ptr = temp;//se achou um representante, atribui esse representante na posicao referente a celula pesquisada na matriz mapa
                        temp->quantidade++;//e incrementa a quantidade de celulas ligadas na regiao do representante
                    }
                }
            }
        }
    }
    //Parte 2: apos etiquetar as regioes, e feita uma pesquisa em cada celula do mapa e caso
    //o representate dessa celula tenha uma regiao com quantidade maior do que determinado
    //entao a posicao da celula na imagem aparecera como branca, senao aparecera como preta
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            if(aux[i][j].ptr == NULL)
            {
                resultado[i][j] = 0;
            }
            else
            {
                temp = find(aux[i][j].ptr);
                if(temp->quantidade>limiar)
                {
                    resultado[i][j] = 255;
                }
                else
                {
                    resultado[i][j] = 0;
                }
            }
        }
    }
    for(i=0; i < y; i++)//libera a matriz auxiliar(importante fazer isso, senao ocorre segfault)
    {
        free(aux[i]);
    }
    free(aux);
    return resultado;
}

byte** hoshen_kopelmanMaiorConjunto(byte** matriz, int x, int y, int cor, int tipo)
{
    int i, j, k, l;
    celula** aux;//mapa de celulas ligadas)
    celula* temp, *temp2;
    byte** resultado;
    aux = (celula**) malloc(y*sizeof(celula*));
    int maximo = 0;

    //Alocacao e inicializacao das matrizes aux e resultado
    if(aux == NULL)
    {
        printf("Erro na alocacao da matriz aux em hoshen-kopelman");
        exit(1);
    }
    resultado = (byte**) malloc(y*sizeof(byte*));
    if(resultado == NULL)
    {
        printf("Erro na alocacao da matriz resultado em hoshen-kopelman");
        exit(1);
    }
    for(i=0; i<y; i++)
    {
        aux[i] = (celula*) malloc(x*sizeof(celula));
        resultado[i] = (byte*) malloc(x*sizeof(byte));
        if(resultado[i] == NULL || aux[i] == NULL)
        {
            printf("Erro na alocacao em hoshen-kopelman");
            exit(1);
        }
        for(j=0; j<x; j++)
        {
            aux[i][j].quantidade = -1;
            aux[i][j].ptr = NULL;
        }
    }
    //Parte 1: inicio da segmentacao
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            if(matriz[i][j] == cor)//caso o pixel tenha cor branca(cor da arteria)
            {
                temp = NULL;//ponteiro auxiliar que guarda o endereco de um representante proximo ou caso noa haja nenhum guarda null
                for(k=-1; k <= 1; k++)
                {
                    for(l=-1; k <= 1; k++)
                    {
                        if((i+k >= 0) && (j+l >= 0) && (j+l < x) && (i+k < y))//verifica se a celula sendo verificada nao esta na borda
                        {
                            if(aux[i+k][j+l].ptr != NULL)//caso haja alguma celula nos arredores da celula sendo pesquisada com representante
                            {
                                temp2 = find(aux[i+k][j+l].ptr);//descobre quem e o representante e guarda em temp2
                                if(temp == NULL)//se nao ja havia sido encontrado outro representante para a celula sendo pesquisada
                                {
                                    temp = temp2;//o representante dessa celula passara a ser o representante temp2
                                }
                                else if(temp != temp2)//caso outro representante ja havia sido encontrado
                                {
                                    temp = Union(temp, temp2);// une os dois conjuntos e atribui o representante desse novo conjunto a celula
                                }
                            }
                        }
                    }
                    if(temp == NULL)// se no final nenhum representate foi encontrado entao eh criado um novo representante
                    {
                        aux[i][j].ptr = &aux[i][j];
                        aux[i][j].quantidade = 1;
                    }
                    else
                    {
                        aux[i][j].ptr = temp;//se achou um representante, atribui esse representante na posicao referente a celula pesquisada na matriz mapa
                        temp->quantidade++;//e incrementa a quantidade de celulas ligadas na regiao do representante
                        if(temp->quantidade > maximo) maximo = temp->quantidade;

                    }
                }
            }
        }
    }
    //Parte 2: apos etiquetar as regioes, e feita uma pesquisa em cada celula do mapa e caso
    //o representate dessa celula tenha uma regiao com quantidade maior do que determinado
    //entao a posicao da celula na imagem aparecera como branca, senao aparecera como preta

    if(tipo == 0)
    {
        for(i=0; i < y; i++)
        {
            for(j=0; j < x; j++)
            {
                if(aux[i][j].ptr == NULL)
                {
                    resultado[i][j] = 0;
                }
                else
                {
                    temp = find(aux[i][j].ptr);
                    if(temp->quantidade == maximo)
                    {
                        resultado[i][j] = 255;
                    }
                    else
                    {
                        resultado[i][j] = 0;
                    }
                }
            }
        }
    }
    else if(tipo == 1)
    {
        for(i=0; i < y; i++)
        {
            for(j=0; j < x; j++)
            {
                if(aux[i][j].ptr == NULL)
                {
                    resultado[i][j] = 255;
                }
                else
                {
                    temp = find(aux[i][j].ptr);
                    if(temp->quantidade == maximo)
                    {
                        resultado[i][j] = 0;
                    }
                    else
                    {
                        resultado[i][j] = 255;
                    }
                }
            }
        }
    }

    for(i=0; i < y; i++)//libera a matriz auxiliar(importante fazer isso, senao ocorre segfault)
    {
        free(aux[i]);
    }
    free(aux);
    return resultado;
}


byte** filtro_da_media(byte** matriz, unsigned x, unsigned y)
{

    int i, j, k, l;
    byte** resultado;
    resultado = (byte**) malloc((y)*sizeof(byte*));
    if(resultado == NULL)
    {
        exit(1);
    }
    for(i=0; i< y; i++)
    {
        resultado[i] = (byte*) malloc((x)*sizeof(byte));
        if(resultado[i] == NULL)
        {
            exit(1);
        }
    }
    for(i=0; i< y; i++)
    {
        for(j=0; j< x; j++)
        {
            int media = 0;
            int count =0;
            for(k=-1; k<=1; k++)
            {
                for(l=-1; l<=1; l++)
                {
                    if(i+k >=0 && i+k <y && j+l >= 0 && j+l <x)
                    {
                        if(fundo[i+k][j+l] == 0)
                        {
                            count++;
                            media = media + matriz[i+k][j+l];
                        }
                    }
                }
            }
            if(fundo[i][j] == 0)
                resultado[i][j] = media/count;
            else
                resultado[i][j] = 0;
        }
    }
    return resultado;
}


byte** laplaciano_do_gaussiano2(byte** matriz, unsigned x, unsigned y)//segunda implementacao do laplaciano do gaussiano
{
    int i, j, k, l;
    int mascara[7][7] = {{0,0,-1,-1,-1,0,0},{0,-2,-3,-3,-3,-2,0},{-1,-3,5,5,5,-3,-1},{-1,-3,5,16,5,-3,-1},{-1,-3,5,5,5,-3,-1},{0,-2,-3,-3,-3,-2,0},{0,0,-1,-1,-1,0,0}};
    //float** mascara = gerar_mascara(1);
    byte** resultado;
    resultado = (byte**) malloc((y)*sizeof(byte*));
    float soma;
    int count;
    if(resultado == NULL)
    {
        exit(1);
    }
    for(i=0; i< y; i++)
    {
        resultado[i] = (byte*) malloc((x)*sizeof(byte));
        if(resultado[i] == NULL)
        {
            exit(1);
        }
    }
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            soma = 0;
            count = 0;
            for(k=-3; k<=3; k++)
            {
                for(l=-3; l<=3; l++)
                {
                    if(i+k >=0 && i+k <y && j+l >=0  && j+l < x)
                    {
                        count++;
                        soma = soma + mascara[k+3][-l+3]*matriz[i+k][j+l];
                    }
                }
            }
            resultado[i][j] = soma/(count*8/49);///original: soma/8
        }
    }
    return resultado;
}

byte** preto_e_branco(byte** matriz, int x, int y)
{
    int i, j;
    byte** resultado;
    resultado= (byte**) malloc(y*sizeof(byte*));
    if(resultado == NULL)
    {
        exit(1);
    }
    for(i = 0; i < y; i++)
    {
        resultado[i] = (byte*) malloc(x*sizeof(byte));
        if(resultado[i] == NULL)
        {
            exit(1);
        }
    }
    for(i=0; i < y; i++)
    {
        for(j=0; j<x; j++)
        {
            if(matriz[i][j] <= 40 || fundo[i][j] == 255)//alterado
            {
                resultado[i][j] = 0;
            }
            else
            {
                resultado[i][j] = 255;
            }
        }
    }
    return resultado;
}

void retiraVasos(byte*** matriz, byte** vasos, int x, int y)
{
    int estrutura[21][21];
//    int estrutura[7][5] = {{0,0,0,1,0,0,0},{0,0,1,1,1,0,0},{0,1,1,1,1,1,0},{1,1,1,1,1,1,1},{0,1,1,1,1,1,0},{0,0,1,1,1,0,0},{0,0,0,1,0,0,0}};
    int i, j, k, l;
    int count;
    int soma[3];
    byte** vasos2;
    vasos2 = (byte**) malloc(y*sizeof(byte*));
    for(i=0; i < 21; i++)
    {
        for(j=0; j < 21; j++)
        {
            estrutura[i][j] = 1;
        }
    }

    for(i=0; i < y; i++)
    {
        vasos2[i] = (byte*) malloc(x*sizeof(byte));
        for(j=0; j < x; j++)
        {
            vasos2[i][j] = vasos[i][j];
        }
    }
    for(i=0; i < y; i++)
    {
        for(j=0; j<x; j++)
        {
            if(vasos2[i][j] == 255)
            {
                count = 0;
                soma[0] = soma[1] = soma[2] = 0;
                for(k=-10; k <= 10; k++)
                {
                    for(l=-10; l <= 10; l++)
                    {
                        if(i+k>=0 && i+k<y && j+l >= 0 && j+l <x && estrutura[k+10][l+10])
                        {
                            if(vasos2[i+k][j+l] != 255)
                            {
                                count++;
                                soma[0] += matriz[0][i+k][j+l];
                                soma[1] += matriz[1][i+k][j+l];
                                soma[2] += matriz[2][i+k][j+l];
                            }
                        }
                    }
                }
                if(count != 0)
                {
                    matriz[0][i][j] = soma[0]/count;
                    matriz[1][i][j] = soma[1]/count;
                    matriz[2][i][j] = soma[2]/count;
                    vasos2[i][j] = 0;
                }
                else
                {
                    vasos2[i][j] =0;
                }
            }
        }
    }
}

byte** aumentarVasos(byte** vasos, int x, int y)
{
    int i, j, k, l;
    int estrutura[5][5] = {{0,0,1,0,0},{0,0,1,0,0},{1,1,1,1,1},{0,0,1,0,0},{0,0,1,0,0}};
    byte** result;
    result = (byte**) alocar_2_dim(y, x, "byte");
    for(i=0; i < y; i++)
    {
        for(j=0; j<x; j++)
        {
            if(vasos[i][j] == 255)
            {
                for(k=-2; k<=2; k++)
                {

                    for(l=-2; l<=2; l++)
                    {
                        if(i+k>= 0 && i+k <y && j+l >= 0 && j+l < x && estrutura[k+2][l+2])
                        {
                            result[i+k][j+l] = 255;
                        }
                    }
                }
            }
        }
    }
    return result;
}

float perona_malik(float x, float sigma)
{
    return exp(-x*x/(sigma*sigma));
}

byte** difusao_anisotropica(byte** matriz, unsigned x, unsigned y, unsigned iter)
{
    int t, i, j, k, l;
    float total;
    float gradiente, coeficiente;
    byte** resultado = (byte**) alocar_2_dim(y, x, "byte");
    copia_matriz(matriz, resultado, x, y);
    for(t=0; t< iter; t++)
    {

        for(i=1; i < y-1; i++)
        {
            for(j=1; j< x-1; j++)
            {

                total = 0;
                if(fundo[i][j] == 0)
                {
                    for(k=-1; k <= 1; k++)
                    {
                        for(l=-1; l <=1; l++)
                        {
                            if(k!=0 && l !=0)
                            {
                                if(k+i >= 0 && j+l >=0 && k+i <y && j+l <x)
                                {

                                    gradiente = resultado[i+k][l+j] - resultado[i][j];
                                    coeficiente = perona_malik(gradiente, 50);
                                    if(abs(k-l) == 1)
                                    {
                                        total += gradiente*coeficiente;
                                    }
                                    else
                                    {
                                        total += gradiente*coeficiente/sqrt(2);
                                    }

                                }
                            }
                        }
                    }
                    resultado[i][j] +=  round(1.0/8.0*total);
                }
            }

        }

    }
    return resultado;
}

byte** somarImagens(byte** m1, byte** m2, int x, int y)
{
    int i, j, soma;
    byte** resultado = (byte**) alocar_2_dim(y, x, "byte");

    #pragma omp parallel for private(i,j,soma) shared(resultado) num_threads(4)
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            soma = m1[i][j] + m2[i][j];
            if(soma > 255)
            {
                resultado[i][j] = 255;
            }
            else if(soma < 0)
            {
                resultado[i][j] = 0;
            }
            else
            {
                resultado[i][j] = soma;
            }
        }
    }

    return resultado;
}

byte** subtrairImagens(byte** m1, byte** m2, int x, int y)
{
    int i, j, subtr;
    byte** resultado = (byte**) alocar_2_dim(y, x, "byte");
    #pragma omp parallel for private(i,j,subtr) shared(resultado) num_threads(4)
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            subtr = m1[i][j] - m2[i][j];
            if(subtr > 255)
            {
                resultado[i][j] = 255;
            }
            else if(subtr < 0)
            {
                resultado[i][j] = 0;
            }
            else
            {
                resultado[i][j] = subtr;
            }
        }
    }
    return resultado;
}


byte** retornaFundo()
{
    return fundo;
}

byte** geraImagemMarca(byte** variancia, byte** vasos, int x, int y)
{
    int i, j, maior = 0;
    byte** resultado = alocar_2_dim(y, x, "byte");
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            if(variancia[i][j] > maior)
            {
                maior = variancia[i][j];
            }
        }
    }
    for(i=0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            if(variancia[i][j] >= maior-10 && vasos[i][j] == 255)
            {
                resultado[i][j] = 255;
            }
            else
            {
                resultado[i][j] = 0;
            }
        }
    }
    return resultado;
}

void geraTexto(char *nomeArquivo, byte** img, int x, int y)
{
    FILE *saida = fopen(nomeArquivo, "w");
    int i,j;

    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            fprintf(saida,"%u ", img[x][y]);
        }
        fprintf(saida,"\n");
    }
}

void escreveImagem(byte** imagem, char *nomeOut, int x, int y)
{
    FILE *out;
    byte **matriz[3];
    byte* buffer;
    int tamanho;

    matriz[0] = imagem;
    matriz[1] = imagem;
    matriz[2] = imagem;

    buffer = (byte*)malloc((54+(x+calculaPadding(x))*y*3)*sizeof(byte));
    tamanho = geraBuffer(&buffer, matriz, x, y);
    out = fopen(nomeOut, "wb");
    // ESCREVE O RESULTADO NO ARQUIVO DE SAÍDA
    fseek(out, 0, SEEK_SET);
    fwrite(buffer, 1, tamanho, out);
    fflush(out);
    fclose(out);
    free(buffer);
    buffer = NULL;
    inverteMatriz(imagem, y);
}

byte** passo11(byte*** matriz, byte** im, int x, int y, byte** est, int tamEst)
{
    int i, j;
    byte** aux, **result = alocar_2_dim(y, x, "byte");
    aux= dilatacao(im , x, y, est, tamEst);
    for(i = 0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            if(aux[i][j] == 255)
            {
                result[i][j] = 0;
            }
            else if(aux[i][j] == 0)
            {
                result[i][j] = matriz[1][i][j];
            }
        }
    }
    return result;
}

byte*** lerImagem(char* nomeIn, unsigned *x, unsigned *y)
{
    int tamanho;
    unsigned comeco;
    byte* buffer, ***matriz;
    FILE *in;
    in = fopen(nomeIn,"rb");
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
    leHeader(buffer, x, y, &comeco);
    // GERA A MATRIZ A PARTIR DO ARQUIVO
    matriz = geraMatriz(buffer, *x, *y, comeco);
    free(buffer);
    return matriz;
}

void geraCentroide(byte** im, int x, int y, int *Cx, int* Cy)
{
    int i, j, contador = 0;
    *Cx = *Cy = 0;
    for(i =0; i < y; i++)
    {
        for(j=0; j < x; j++)
        {
            if(im[i][j] != 0)
            {
                *Cx += i;
                *Cy += j;
                contador++;
            }

        }
    }
    *Cx /= contador;
    *Cy /= contador;
}

//byte** geraMarcadores(byte** im, int x, int y)
//{
//    int cx, cy, i, j, ehBorda, l ,m;
//    byte** result = (byte**) alocar_2_dim(y, x, "byte");
//    geraCentroide(im, x, y, &cx, &cy);
//    result[cx][cy] = 255;
//    for(i=0; i < y; i++)
//    {
//        for(j=0; j <x; j++)
//        {
//            if(im[i][j] == 255)
//            {
//               ehBorda = 0;
//                for(l=-1; l<=1 && !ehBorda; l++)
//                {
//                    for(m=-1; m<=1 && !ehBorda; m++)
//                    {
//                        if(i+l>=0 && i+l<x && j+m>=0 && j+m<y)
//                        {
//                            ehBorda = (im[i+l][j+m] == 0);
//                        }
//                    }
//                }
//                if(ehBorda)
//                {
//                    result[i][j] = 255;
//                }
//            }
//        }
//    }
//    return result;
//}

void swap(Ponto **a, int i, int j)
{
    Ponto *t = a[i];
    a[i] = a[j];
    a[j] = t;
}

int partition(Ponto **a, int left,int right,int pivot)
{
    int pos, i;
    swap(a, pivot, right);
    pos = left;
    for(i = left; i < right; i++)
    {
        if (a[i]->valor < a[right]->valor)
        {
            swap(a, i, pos);
            pos++;
        }
    }
    swap(a, right, pos);
    return pos;
}

void quick(Ponto **a, int left, int right)
{
    if (left < right)
    {
        int pivot = (left+right)/2;
        int pos = partition(a,left,right,pivot);
        quick(a,left,pos-1);
        quick(a,pos+1,right);
    }
}


byte** watershed(byte** imagem, byte **marcadores, int x, int y)
{
    int i, j, k, l, tamVet=0;
    unsigned comeco = 0, fim = 0, qtdRep = 0;
    byte **retorno = alocar_2_dim(y,x,"byte");
    Ponto **vetMarcadores = (Ponto**) calloc(x*y, sizeof(Ponto*));
    Ponto **matMarcadores = (Ponto**) calloc(y, sizeof(Ponto*));

    if(matMarcadores == NULL)
    {
        printf("Erro alocacao watershed");
        exit(1);
    }

    for(i =0; i < y; i++)
    {
        matMarcadores[i] = (Ponto*) calloc(x, sizeof(Ponto));
        if(matMarcadores[i] == NULL)
        {
            printf("Erro alocacao watershed");
            exit(1);
        }
    }

    ///PERCORRE AS MATRIZES
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            ///SE FOR O REPRESENTANTE
            if(marcadores[i][j]==255)
            {
                qtdRep++;
                matMarcadores[i][j].marcado = 1;
                matMarcadores[i][j].repY = i;
                matMarcadores[i][j].repX = j;
                matMarcadores[i][j].valor = imagem[i][j];
                matMarcadores[i][j].y = i;
                matMarcadores[i][j].x = j;
                matMarcadores[i][j].limite = 0;

                ///COLOCA OS QUE ESTAO AO REDOR EM UM VETOR
                for(k=-1; k<=1; k++)
                {
                    for(l=-1; l<=1; l++)
                    {
                        /// SE ESTA DENTRO DA MATRIZ E NAO FOI MARCADO, COLOCA NO VETOR
                        if(i+k>=0 && i+k<y && j+l>=0 && j+l<x && !matMarcadores[i+k][j+l].marcado)
                        {
                            vetMarcadores[tamVet] = &matMarcadores[i+k][j+l];
                            vetMarcadores[tamVet]->repY = i;
                            vetMarcadores[tamVet]->repX = j;
                            vetMarcadores[tamVet]->y = i+k;
                            vetMarcadores[tamVet]->x = j+l;
                            vetMarcadores[tamVet]->valor = imagem[i+k][j+l];
                            vetMarcadores[tamVet]->marcado = 1;
                            tamVet++;
                        }
                    }
                }
            }
        }
    }

    comeco = 0;
    fim = tamVet-1;

    while(tamVet<(x*y-qtdRep) && comeco!=fim)
    {
        ///ORDENA O VETOR
        quick(vetMarcadores, comeco, fim);

        ///DEFINE SE OS ELEMENTOS DO VETOR SÃO LIMITES
        for(i=comeco; i<=fim; i++)
        {
            for(k=-1; k <= 1; k++)
            {
                for(l=-1; l <= 1; l++)
                {
                    ///VERIFICA SE NAO VAI PASSAR DOS LIMITES DA MATRIIZ
                    if(vetMarcadores[i]->x+l>=0 && vetMarcadores[i]->x+l <x && vetMarcadores[i]->y+k>=0 && vetMarcadores[i]->y+k<y)
                    {
                        /// PROCURA UM REPRESENTANTE DIFERENTE DENTRE OS ELEMENTOS AO REDOR
                        if( (matMarcadores[vetMarcadores[i]->y+k][vetMarcadores[i]->x+l].repX != vetMarcadores[i]->repX) || (matMarcadores[vetMarcadores[i]->y+k][vetMarcadores[i]->x+l].repY != vetMarcadores[i]->repY))
                        {
                            vetMarcadores[i]->limite = 1;
                        }

                        /// SE NAO FOI COLOCADO NO VETOR ANTES, COLOCA O ELEMENTO NO VETOR
                        if(!matMarcadores[vetMarcadores[i]->y+k][vetMarcadores[i]->x+l].marcado)
                        {
                            vetMarcadores[tamVet] = &matMarcadores[vetMarcadores[i]->y+k][vetMarcadores[i]->x+l];
                            vetMarcadores[tamVet]->y = vetMarcadores[i]->y+k;
                            vetMarcadores[tamVet]->x = vetMarcadores[i]->x+l;
                            vetMarcadores[tamVet]->valor = imagem[vetMarcadores[i]->y+k][vetMarcadores[i]->x+l];
                            vetMarcadores[tamVet]->marcado = 1;
                            tamVet++;
                        }
                    }
                }
            }
        }

        comeco = fim;
        fim = tamVet-1;
    }

    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            retorno[i][j] = matMarcadores[i][j].limite * 255;
        }
    }

    return retorno;

}

unsigned CalculaDiametro(byte **imagem,int x,int y)
{
    int i,j,sai=0,achouBranco=0;
    int contar = 0;
    unsigned Diametro=0, MaiorDiametro=0;

    for(i=0;i<y;i++)
    {
        for(j=0;j<x;j++)
        {
            if(imagem[i][j]==255 && imagem[i][j+1]==0 && j+1<x)
            {
                contar=1;
            }

            if(contar==1)
            {
                Diametro++;
                if(imagem[i][j]==0 && imagem[i][j+1]==255)
                {
                    contar=0;

                    if(Diametro > MaiorDiametro)
                    {
                        MaiorDiametro = Diametro;
                    }
                }
            }
        }
        Diametro=0;
        contar=0;
    }

    return MaiorDiametro;
}

byte **RegiaoInteresse(byte **imagem2,int x,int y, int xCentro, int yCentro)
{
    int i,j,l,m;
    int novoX;
    int Diametro;

    byte **ROI = (byte**)alocar_2_dim(XYINTERE,XYINTERE,"byte");

    int distancia = 200;

//    printf("Diametro: %d\n", CalculaDiametro(imagem17, x, y));

    ///printf("%d %d \n",xCentro,yCentro);

    if(xCentro > x/2)
    {
        novoX = (xCentro)-distancia;
    }
    else
    {
        novoX= (xCentro)+distancia;
    }

//    printf("NovoX: %d / NovoY: %d\n", novoX, *yCentro);

    for(i=(yCentro)-(XYINTERE/2), l=0; i<(yCentro)+(XYINTERE/2) ; i++,l++)
    {
        for(j=novoX - (XYINTERE/2), m=0;j<novoX+(XYINTERE/2);j++,m++)
        {
            ROI[l][m]=imagem2[i][j];
        }
    }


    return ROI;
}

int PixelMax(byte **img,int x,int y)
{
    int i,j;
    int temp=0;
    for(i=0;i<y;i++)
    {
        for(j=0;j<x;j++)
        {
            if(img[i][j]>temp)
            {
                temp = img[i][j];
            }
        }
    }
    return temp;
}

int PixelMin(byte **img,int x,int y)
{
    int i,j;
    int temp=255;
    for(i=0;i<y;i++)
    {
        for(j=0;j<x;j++)
        {
            if(img[i][j]<temp)
            {
                temp = img[i][j];
            }
        }
    }
    return temp;
}

void passo21(byte **img,int x, int y)
{
    int tempmax,tempmin;
    int temp1,i,j;
    tempmax=PixelMax(img,x,y);
    tempmin=PixelMin(img,x,y);

    temp1=tempmax-tempmin;
    for(i=0;i<y;i++)
    {
        for(j=0;j<x;j++)
        {
            img[i][j]= (img[i][j]-tempmax)*(255/(temp1));
        }
    }
}

byte **RemoverRegioesBrilhosas(byte **imagem,int x,int y,byte **Est, int TamEst)
{
    int i,j;
    byte ** aux1=(byte**)alocar_2_dim(y,x,"byte"),**retorno;

    byte ** temp = minRegional(imagem,x,y,Est,TamEst);
    for(i=0;i<y;i++)
    {
        for(j=0;j<x;j++)
        {
            temp[i][j] *= 255;
        }
    }
    retorno = reconstrucaoPorDilatacao(temp,imagem,x,y,Est,TamEst);
    return retorno;
}

byte **passo25(byte **img,int x,int y,int xCentro,int yCentro)
{
    int i,j;
    byte **img2 = alocar_2_dim(y,x,"byte");

    for(i=0; i < y ;i++)
    {
        for(j=0 ; j < x ; j++)
        {
            if(i < (XYINTERE/2))
            {
                img2[i][j] = 0;
            }
            else
            {
                img2[i][j] = img[i][j];
            }
        }
    }
    return img2;
}

byte **CentroDaFovea(byte **imgP,int xP,int yP,byte **imgG,int xG,int yG,int Distancia, byte** imgRGB, int* xCentroFovea, int* yCentroFovea)
{
    int xCentro,xCentro2,yCentro,yCentro2;
    int novoX;

    geraCentroide(imgG,xG,yG,&yCentro, &xCentro);
    geraCentroide(imgP,xP,yP,&yCentro2, &xCentro2);

//    printf("xCentro:%d yCentro:%d\n",xCentro,yCentro);
//    printf("xCentro2:%d yCentro2:%d\n",xCentro2,yCentro2);
//    printf("Distancia: %d\n", Distancia);

    xCentro2-=(XYINTERE/2);
    yCentro2-=(XYINTERE/2);


    if(xCentro > xG/2)
    {
        novoX = xCentro-Distancia;
    }
    else
    {
        novoX = xCentro+Distancia;
    }

    novoX+=xCentro2;
    yCentro+=yCentro2;

    imgRGB[yCentro][novoX] = 255;
    *xCentroFovea = novoX;
    *yCentroFovea = yCentro;

    return imgRGB;
}

byte** passo26(byte** imgCONTR, int x, int y)
{
    int menorContraste;
    byte** retorno = alocar_2_dim(XYINTERE, XYINTERE, "byte");
    int i, j;

    menorContraste = PixelMin(imgCONTR, x, y);

    for(i = 0 ; i < XYINTERE ; i++)
    {
        for(j = 0 ; j < XYINTERE ; j++)
        {
            if(menorContraste != imgCONTR[i][j])
            {
                retorno[i][j] = 0;
            }
            else
            {
                retorno[i][j] = 255;
            }
        }
    }

    return retorno;

}

byte** passo27(byte** componenteL, int x, int y, byte** est, int tamEst)
{
    byte** retorno;
    byte** resultAbertura;
    byte** resultFechamento;
    byte** aux;

    resultAbertura = top_hatA(componenteL, x, y, est, tamEst);
    resultFechamento = top_hatF(componenteL, x, y, est, tamEst);

    retorno = somarImagens(componenteL, resultAbertura, x, y);
    aux = retorno;
    retorno = subtrairImagens(retorno, resultFechamento, x, y);

    liberarMatriz2D(aux, y);
    liberarMatriz2D(resultAbertura, y);
    liberarMatriz2D(resultFechamento, y);

    return retorno;

}

byte** passo33(byte** imagem, int x, int y, int limiar)
{
    byte** novaImagem = alocar_2_dim(y, x, "byte");
    int i, j;

    for(i = 0 ; i < y ; i++)
    {
        for(j = 0 ; j < x ; j++)
        {
            if(imagem[i][j] <= limiar)
            {
                novaImagem[i][j] = 0;
            }
            else
            {
                novaImagem[i][j] = 255;
            }
        }
    }

    return novaImagem;

}

void deixaFundoPreto(byte*** matriz, int x, int y)
{
    int i;
    int j;

    #pragma omp parallel for private(i,j) num_threads(4)
    for(i = 0 ; i < y ; i++)
    {
        for(j = 0 ; j < x ; j++)
        {
            if(matriz[0][i][j] < 14 && matriz[1][i][j] < 14 && matriz[2][i][j] < 14)
            {
                matriz[0][i][j] = 0;
                matriz[1][i][j] = 0;
                matriz[2][i][j] = 0;
            }
        }
    }
}

byte** passo34(byte** canalVerde, byte** imagemExsudatos, int x, int y)
{
    byte** resultado;
    int i, j;

    resultado = alocar_2_dim(y, x, "byte");

    for(i = 0 ; i < y ; i++)
    {
        for(j = 0 ; j < x ; j++)
        {
            if(imagemExsudatos[i][j] == 255)
            {
                resultado[i][j] = 0;
            }
            else
            {
                resultado[i][j] = canalVerde[i][j];
            }
        }
    }

    return resultado;
}

byte** passo38(byte** imagemExsudatos, byte** imagemDiscoOptico, int x, int y)
{
    byte** resultado;
    int i, j;

    resultado = alocar_2_dim(y, x, "byte");

    for(i = 0 ; i < y ; i++)
    {
        for(j = 0 ; j < x ; j++)
        {
            if(imagemDiscoOptico[i][j] == 255)
            {
                resultado[i][j] = 0;
            }
            else
            {
                resultado[i][j] = imagemExsudatos[i][j];
            }
        }
    }

    return resultado;
}

int passo39(byte** matriz, int x, int y, byte** v, int tamEst, int xCentroFovea, int yCentroFovea, int ordem, int k)
{
    int i = yCentroFovea;
    int j = xCentroFovea;
    int m;
    int n;

    int contagem = 0;
    char nomeOut[100];
    byte** confere;
    confere = alocar_2_dim(y, x, "byte");

    copia_matriz(matriz, confere, x, y);

    #pragma omp parallel for private(m,n) num_threads(4)
    for(m = -tamEst/2; m <= tamEst/2; m++)
    {
        for(n = -tamEst/2; n <= tamEst/2; n++)
        {
            if(((i+m) >= 0) && ((j+n) >= 0) && ((i+m) < y) && ((j+n) < x) && v[m+tamEst/2][n + tamEst/2])
            {
                if(matriz[i+m][j+n] > 0)
                {
                    contagem++;
                    confere[i+m][j+n] = 0;
                }
                else
                {
                    confere[i+m][j+n] = 255;
                }

            }
        }
    }

    if(ordem == 1)
    {
        sprintf(nomeOut, "Imagens/%d_1_out.bmp", k);
        escreveImagem(confere,nomeOut, x, y);
    }
    else if(ordem == 2)
    {
        sprintf(nomeOut, "Imagens/%d_2_out.bmp", k);
        escreveImagem(confere,nomeOut, x, y);
    }
    else
    {
        sprintf(nomeOut, "Imagens/%d_3_out.bmp", k);
        escreveImagem(confere,nomeOut, x, y);
    }


    return contagem;
}
