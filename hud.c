#include "hud.h"
#include "configuracao.h"
#include "jogo.h"
#include "frango.h"
#include "obstaculo.h"
#include <GL/glut.h>
#include <stdio.h>

extern Frango frangoJogador;

static void desenharRetanguloHUD(float x, float y, float largura, float altura)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + largura, y);
    glVertex2f(x + largura, y + altura);
    glVertex2f(x, y + altura);
    glEnd();
}

void desenharTexto(float x, float y, const char *texto)
{
    const unsigned char *p = (const unsigned char *)texto;

    glRasterPos2f(x, y);
    while (*p != '\0') {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)*p);
        p++;
    }
}

void desenharTextoGrande(float x, float y, const char *texto)
{
    const unsigned char *p = (const unsigned char *)texto;

    glRasterPos2f(x, y);
    while (*p != '\0') {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)*p);
        p++;
    }
}

void desenharPontuacao(int pontos)
{
    char texto[64];

    snprintf(texto, sizeof(texto), "Pontos: %d", pontos);
    desenharTexto(-0.96f, 1.08f, texto);
}

void desenharTimer(int segundos)
{
    char texto[64];

    snprintf(texto, sizeof(texto), "Tempo: %02d:%02d", segundos / 60, segundos % 60);
    desenharTexto(-0.18f, 1.08f, texto);
}

void desenharVidas(int vidas)
{
    char texto[64];

    snprintf(texto, sizeof(texto), "Vidas: %d", vidas);
    desenharTexto(0.63f, 1.08f, texto);
}

static void desenharProgresso(void)
{
#if JOGO_INVERTIDO
    float progresso = (float)(META_VITORIA - frangoJogador.faixaAtual) /
                      (float)META_VITORIA;
#else
    float progresso = (float)frangoJogador.faixaAtual / (float)META_VITORIA;
#endif

    if (progresso < 0.0f) {
        progresso = 0.0f;
    }
    if (progresso > 1.0f) {
        progresso = 1.0f;
    }

    glColor4f(0.08f, 0.10f, 0.12f, 0.78f);
    desenharRetanguloHUD(-0.96f, 1.015f, 0.42f, 0.025f);
    glColor3f(0.95f, 0.78f, 0.20f);
    desenharRetanguloHUD(-0.96f, 1.015f, 0.42f * progresso, 0.025f);
}

void desenharHUD(void)
{
    int segundos = jogo.tempoDecorrido / 1000;
    char melhor[64];

    glColor4f(0.02f, 0.04f, 0.05f, 0.72f);
    desenharRetanguloHUD(-1.0f, 0.985f, 2.0f, 0.165f);

    glColor3f(1.0f, 1.0f, 1.0f);
    desenharPontuacao(jogo.pontuacao);
    desenharTimer(segundos);
    desenharVidas(frangoJogador.vidas);
    desenharProgresso();

    snprintf(melhor, sizeof(melhor), "Melhor: %d", jogo.melhorPontuacao);
    desenharTexto(0.63f, 1.02f, melhor);
}
