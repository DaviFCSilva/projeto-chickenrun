#include "cenario.h"
#include "obstaculo.h"
#include <GL/glut.h>

static void desenharRetangulo(float x, float y, float largura, float altura)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + largura, y);
    glVertex2f(x + largura, y + altura);
    glVertex2f(x, y + altura);
    glEnd();
}

static void desenharChegada(float y)
{
    int i;
    float larguraCasa = 0.16f;

    desenharGrama(X_ESQUERDA_MUNDO, y, LARGURA_MUNDO, ALTURA_FAIXA);
    for (i = 0; i < 13; i++) {
        float x = X_ESQUERDA_MUNDO + (float)i * larguraCasa;

        if (i % 2 == 0) {
            glColor3f(0.96f, 0.96f, 0.92f);
        } else {
            glColor3f(0.08f, 0.08f, 0.08f);
        }
        desenharRetangulo(x, y + ALTURA_FAIXA * 0.68f,
                          larguraCasa, ALTURA_FAIXA * 0.22f);
    }
}

TipoFaixa obterTipoFaixa(int indiceFaixa)
{
    if (indiceFaixa <= 0 || indiceFaixa >= META_VITORIA) {
        return FAIXA_GRAMA;
    }
    if (indiceFaixa % 5 == 0) {
        return FAIXA_GRAMA;
    }
    if (indiceFaixa % 3 == 0) {
        return FAIXA_RIO;
    }
    return FAIXA_PISTA;
}

float obterYFaixa(int indiceFaixa)
{
    return (float)indiceFaixa * ALTURA_FAIXA;
}

void desenharCenario(float offsetCamera)
{
    int i;
    int primeiraFaixa = (int)(offsetCamera / ALTURA_FAIXA) - 2;

    if (primeiraFaixa < -1) {
        primeiraFaixa = -1;
    }

    for (i = primeiraFaixa; i <= META_VITORIA + 4; i++) {
        float y = obterYFaixa(i);
        TipoFaixa tipo = obterTipoFaixa(i);

        if (i == META_VITORIA) {
            desenharChegada(y);
        } else if (tipo == FAIXA_GRAMA) {
            desenharGrama(X_ESQUERDA_MUNDO, y, LARGURA_MUNDO, ALTURA_FAIXA);
        } else if (tipo == FAIXA_PISTA) {
            desenharPista(X_ESQUERDA_MUNDO, y, LARGURA_MUNDO, ALTURA_FAIXA);
        } else {
            desenharRio(X_ESQUERDA_MUNDO, y, LARGURA_MUNDO, ALTURA_FAIXA);
        }
    }
}

void desenharGrama(float x, float y, float largura, float altura)
{
    int i;

    glColor3f(0.18f, 0.58f, 0.22f);
    desenharRetangulo(x, y, largura, altura);

    glColor3f(0.14f, 0.48f, 0.18f);
    desenharRetangulo(x, y, largura, altura * 0.18f);

    glColor3f(0.55f, 0.86f, 0.35f);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (i = 0; i < 28; i++) {
        float px = x + 0.04f + (float)(i % 14) * 0.14f;
        float py = y + 0.03f + (float)(i / 14) * 0.08f;
        glVertex2f(px, py);
    }
    glEnd();
}

void desenharPista(float x, float y, float largura, float altura)
{
    glColor3f(0.17f, 0.17f, 0.18f);
    desenharRetangulo(x, y, largura, altura);

    glColor3f(0.24f, 0.24f, 0.25f);
    desenharRetangulo(x, y + altura * 0.08f, largura, altura * 0.08f);
    desenharRetangulo(x, y + altura * 0.84f, largura, altura * 0.08f);

    glColor3f(0.10f, 0.10f, 0.11f);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + largura, y);
    glVertex2f(x, y + altura);
    glVertex2f(x + largura, y + altura);
    glEnd();

    desenharLinhasDePista(y, altura);
}

void desenharRio(float x, float y, float largura, float altura)
{
    float passo;

    glColor3f(0.05f, 0.36f, 0.82f);
    desenharRetangulo(x, y, largura, altura);

    glColor3f(0.04f, 0.24f, 0.60f);
    desenharRetangulo(x, y, largura, altura * 0.18f);

    glColor3f(0.45f, 0.78f, 1.0f);
    glBegin(GL_LINES);
    for (passo = -0.95f; passo < 1.0f; passo += 0.20f) {
        glVertex2f(passo, y + altura * 0.35f);
        glVertex2f(passo + 0.08f, y + altura * 0.45f);
        glVertex2f(passo + 0.10f, y + altura * 0.65f);
        glVertex2f(passo + 0.18f, y + altura * 0.55f);
    }
    glEnd();
}

void desenharLinhasDePista(float y, float altura)
{
    float x;

    glColor3f(0.95f, 0.82f, 0.18f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (x = -0.95f; x < 1.0f; x += 0.28f) {
        glVertex2f(x, y + altura * 0.50f);
        glVertex2f(x + 0.14f, y + altura * 0.50f);
    }
    glEnd();
    glLineWidth(1.0f);
}
