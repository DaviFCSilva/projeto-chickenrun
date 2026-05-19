#include "captura_tela.h"
#include "jogo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

#define CAPTURA_ESQUERDA 0.89f
#define CAPTURA_BAIXO 1.03f
#define CAPTURA_DIREITA 0.96f
#define CAPTURA_CIMA 1.10f

static void desenharQuadrado(float esquerda, float baixo, float direita, float cima)
{
    glBegin(GL_QUADS);
    glVertex2f(esquerda, baixo);
    glVertex2f(direita, baixo);
    glVertex2f(direita, cima);
    glVertex2f(esquerda, cima);
    glEnd();
}

static void desenharCirculo(float centroX, float centroY, float raio)
{
    int i;
    int segmentos = 36;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centroX, centroY);
    for (i = 0; i <= segmentos; i++) {
        float angulo = 6.2831853f * (float)i / (float)segmentos;
        glVertex2f(centroX + cosf(angulo) * raio,
                   centroY + sinf(angulo) * raio);
    }
    glEnd();
}

void desenharBotaoCaptura(void)
{
    float esquerda = CAPTURA_ESQUERDA;
    float baixo = CAPTURA_BAIXO;
    float direita = CAPTURA_DIREITA;
    float cima = CAPTURA_CIMA;

    glColor4f(0.0f, 0.0f, 0.0f, 0.55f);
    desenharQuadrado(esquerda, baixo, direita, cima);

    glColor3f(1.0f, 1.0f, 1.0f);
    desenharQuadrado(esquerda, baixo, direita, cima);

    glColor3f(0.0f, 0.0f, 0.0f);
    desenharQuadrado(esquerda + 0.01f, baixo + 0.01f,
                     direita - 0.01f, cima - 0.01f);

    glColor3f(1.0f, 1.0f, 1.0f);
    desenharCirculo((esquerda + direita) * 0.5f,
                   (baixo + cima) * 0.5f,
                   0.020f);
}

int clicouNoBotaoCaptura(int mouseX, int mouseY)
{
    float x;
    float y;

    if (jogo.larguraJanela <= 0 || jogo.alturaJanela <= 0) {
        return 0;
    }

    x = -1.0f + 2.0f * ((float)mouseX / (float)jogo.larguraJanela);
    y = 1.15f - 1.30f * ((float)mouseY / (float)jogo.alturaJanela);

    return x >= CAPTURA_ESQUERDA && x <= CAPTURA_DIREITA &&
           y >= CAPTURA_BAIXO && y <= CAPTURA_CIMA;
}

static void escreverNumero(unsigned char *lugar, unsigned int numero, int bytes)
{
    int i;

    for (i = 0; i < bytes; i++) {
        lugar[i] = (unsigned char)((numero >> (8 * i)) & 255);
    }
}

static void criarNomeArquivo(char *nomeArquivo, int tamanho)
{
    const char *pastaUsuario = getenv("USERPROFILE");
    time_t agora = time(NULL);
    struct tm *dataHora = localtime(&agora);

    if (pastaUsuario && dataHora) {
        snprintf(nomeArquivo, (size_t)tamanho,
                 "%s\\Downloads\\captura_chicken_run_%04d%02d%02d_%02d%02d%02d.bmp",
                 pastaUsuario,
                 dataHora->tm_year + 1900, dataHora->tm_mon + 1, dataHora->tm_mday,
                 dataHora->tm_hour, dataHora->tm_min, dataHora->tm_sec);
    } else {
        snprintf(nomeArquivo, (size_t)tamanho, "captura_chicken_run.bmp");
    }
}

void salvarCapturaTelaDownloads(void)
{
    int largura = jogo.larguraJanela;
    int altura = jogo.alturaJanela;
    int bytesPorLinha = largura * 3;
    int sobraLinha = (4 - (bytesPorLinha % 4)) % 4;
    int tamanhoImagem = (bytesPorLinha + sobraLinha) * altura;
    int tamanhoArquivo = 54 + tamanhoImagem;
    unsigned char cabecalho[54];
    unsigned char zeros[3] = {0, 0, 0};
    unsigned char *pixels;
    unsigned char *linhaBmp;
    char nomeArquivo[512];
    FILE *arquivo;
    int y;
    int x;

    if (largura <= 0 || altura <= 0) {
        return;
    }

    pixels = (unsigned char *)malloc((size_t)bytesPorLinha * (size_t)altura);
    if (!pixels) {
        return;
    }

    linhaBmp = (unsigned char *)malloc((size_t)bytesPorLinha);
    if (!linhaBmp) {
        free(pixels);
        return;
    }

    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, largura, altura, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    criarNomeArquivo(nomeArquivo, sizeof(nomeArquivo));
    arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        free(linhaBmp);
        free(pixels);
        return;
    }

    memset(cabecalho, 0, sizeof(cabecalho));
    cabecalho[0] = 'B';
    cabecalho[1] = 'M';
    escreverNumero(cabecalho + 2, (unsigned int)tamanhoArquivo, 4);
    escreverNumero(cabecalho + 10, 54, 4);
    escreverNumero(cabecalho + 14, 40, 4);
    escreverNumero(cabecalho + 18, (unsigned int)largura, 4);
    escreverNumero(cabecalho + 22, (unsigned int)altura, 4);
    escreverNumero(cabecalho + 26, 1, 2);
    escreverNumero(cabecalho + 28, 24, 2);
    escreverNumero(cabecalho + 34, (unsigned int)tamanhoImagem, 4);

    fwrite(cabecalho, 1, sizeof(cabecalho), arquivo);
    for (y = 0; y < altura; y++) {
        for (x = 0; x < largura; x++) {
            int origem = y * bytesPorLinha + x * 3;
            int destino = x * 3;

            linhaBmp[destino] = pixels[origem + 2];
            linhaBmp[destino + 1] = pixels[origem + 1];
            linhaBmp[destino + 2] = pixels[origem];
        }
        fwrite(linhaBmp, 1, (size_t)bytesPorLinha, arquivo);
        fwrite(zeros, 1, (size_t)sobraLinha, arquivo);
    }

    fclose(arquivo);
    free(linhaBmp);
    free(pixels);
}
