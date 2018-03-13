#ifndef PIMAGENS_H_INCLUDED
#define PIMAGENS_H_INCLUDED



#endif // PIMAGENS_H_INCLUDED

#define B 0.11
#define G 0.59
#define R 0.3
#define PI 3.141592
#define E  2.718281
#define TAMANHO_JANELA 80
#define QTD_PASSOS 60
#define QTD_IMAGENS 20
#define XYINTERE 100

typedef unsigned char byte;

typedef struct bmpheader
{
    byte magicNumber[2];
    byte tamanhoArquivo[4];
    byte naoUsado[4];
    byte comecoPixels[4];
    byte bytesAPartirDaqui[4];
    byte x[4];
    byte y[4];
    byte colorPlanes[2];
    byte bitsPerPixel[2];
    byte compressao[4];
    byte qtdLixo[4];
    byte resHorizontal[4];
    byte resVertical[4];
    byte qtdCoresNaPaleta[4];
    byte qtdCoresImportantes[4];
} BMPHeader;

typedef struct ponto
{
    byte valor;
    unsigned x;
    unsigned y;
    unsigned repX;
    unsigned repY;
    byte limite;
    byte marcado;
} Ponto;

byte** leEstruturante(int* tamanho, char* nome);

void** alocar_2_dim(int linhas, int colunas, char* tipo);//aloca matrizes de duas dimensoes (tipo = "byte", "float", "int")

void*** alocar_3_dim(int canais, int linhas, int colunas, char* tipo);//aloca matrizes de tres dimensoes (tipo = "byte", "float", "int")

void liberarMatriz2D(byte** p, int y);

void leHeader(byte* buffer, unsigned *x, unsigned *y, unsigned *comeco);

void inverteMatriz(byte** matriz, unsigned y);

unsigned calculaPadding(unsigned x);

byte*** geraMatriz(byte* buffer, unsigned x, unsigned y, unsigned comeco);

unsigned geraBuffer(byte** bf, byte*** matriz, unsigned x, unsigned y);

void liberarMatrizLAB(float*** p, int y);

void liberarMatriz(byte*** p, int y);

float f(float x);

byte** obterLuminescencia(byte*** matriz, int x, int y);

byte** dilatacao(byte** matriz, int x, int y, byte** v, int tamEst);

byte** erosao(byte** matriz, int x, int y, byte** v, int tamEst);

byte** abertura(byte** matriz, int x, int y, byte** v, int tamEst);

byte** fechamento(byte** matriz, int x, int y, byte** v, int tamEst);

byte** top_hatA(byte** matriz, int x, int y, byte** v, int tamEst);

byte** top_hatF(byte** matriz, int x, int y, byte** v, int tamEst);

void subtrai_matriz(byte**mat1,byte**mat2,int x,int y);

void copia_matriz(byte** mat1, byte**mat2, int x,int y);

int compara_matriz(byte** mat1, byte** mat2, int x, int y);

byte** encontra_max(byte**mat1,byte**mat2,int x,int y);

byte** minRegional(byte** matriz, int x, int y,byte **Est, int TamEst);

void soma_matriz(byte**mat1,byte**mat2,int x,int y);

byte** reconstrucaoPorErosao(byte** matriz, byte** mascara, int x, int y, byte** v, int tamEst);

byte** reconstrucaoPorDilatacao(byte** matriz,byte** mascara,int x, int y, byte** v, int tamEst);

byte** dilatacaoGeodesica(byte** matriz,byte** mascara,int x, int y, byte** v, int tamEst);

byte** erosaoGeodesica(byte** matriz, byte** mascara, int x, int y, byte** v, int tamEst);

byte** maxRegional(byte** matriz,int x, int y, byte** Est, int tamEst);

byte** coordCentro(byte** matriz, int x, int y, int *xCentro, int *yCentro);

byte** Hmaximo(byte** matriz, int x, int y, byte** Est, int tamEst, int limiar);

byte** Hminimo(byte** matriz, int x, int y, byte** Est, int tamEst, int limiar);

byte** separarFundo(byte*** matriz, int  x, int y);

byte** hoshen_kopelman(byte** matriz, int x, int y, int limiar, int cor);
byte** hoshen_kopelmanMaiorConjunto(byte** matriz, int x, int y, int cor, int tipo);

byte** filtro_da_media(byte** matriz, unsigned x, unsigned y);

byte** laplaciano_do_gaussiano2(byte** matriz, unsigned x, unsigned y);

byte** preto_e_branco(byte** matriz, int x, int y);

byte** difusao_anisotropica(byte** matriz, unsigned x, unsigned y, unsigned iter);

byte** somarImagens(byte** m1, byte** m2, int x, int y);

byte** subtrairImagens(byte** m1, byte** m2, int x, int y);

byte** geraImagemMarca(byte** variancia, byte** vasos, int x, int y);

void escreveImagem(byte** imagem, char *nomeOut, int x, int y);

byte** passo11(byte*** matriz, byte** im, int x, int y, byte** est, int tamEst);

byte*** lerImagem(char* nomeIn, unsigned *x, unsigned *y);

unsigned CalculaDiametro(byte **imagem,int x,int y);

void passo21(byte **img,int x, int y);

byte ** RegiaoInteresse(byte **imagem2,int x,int y, int xCentro, int yCentro);

void geraCentroide(byte** im, int x, int y, int *Cx, int* Cy);

byte **CentroDaFovea(byte **imgP,int xP,int yP,byte **imgG,int xG,int yG,int Distancia, byte** imgRGB, int* xCentroFovea, int* yCentroFovea);

byte** passo26(byte** imgCONTR, int x, int y);

byte **RemoverRegioesBrilhosas(byte **imagem,int x,int y,byte **Est, int TamEst);

byte** passo27(byte** componenteL, int x, int y, byte** est, int tamEst);

byte** passo33(byte** imagem, int x, int y, int limiar);

void deixaFundoPreto(byte*** matriz, int x, int y);

byte** passo34(byte** canalVerde, byte** imagemExsudatos, int x, int y);

byte** passo38(byte** imagemExsudatos, byte** imagemDiscoOptico, int x, int y);

int passo39(byte** matriz, int x, int y, byte** v, int tamEst, int xCentroFovea, int yCentroFovea, int ordem, int k);
