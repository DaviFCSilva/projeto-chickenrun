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

TipoFaixa obterTipoFaixa(int indiceFaixa)
{
    /* A sequencia alterna grama, pistas e rios para criar variedade. */
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
    (void)offsetCamera;

    /* Desenha algumas faixas extras para cobrir a camera durante o scroll. */
    for (i = -1; i <= META_VITORIA + 4; i++) {
        float y = obterYFaixa(i);
        TipoFaixa tipo = obterTipoFaixa(i);

        if (tipo == FAIXA_GRAMA) {
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
