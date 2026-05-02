#include "frango.h"
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535f

/* Mantem o jogador dentro dos limites horizontais do mundo visivel. */
static void limitarFrangoNaTela(Frango *f)
{
    if (f->x < -LIMITE_X_FRANGO) {
        f->x = -LIMITE_X_FRANGO;
    }
    if (f->x > LIMITE_X_FRANGO) {
        f->x = LIMITE_X_FRANGO;
    }
}

static void desenharCirculo(float x, float y, float raio)
{
    int i;

    /* Aproxima um circulo usando um leque de triangulos do OpenGL. */
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (i = 0; i <= 24; i++) {
        float angulo = (2.0f * PI * (float)i) / 24.0f;
        glVertex2f(x + cosf(angulo) * raio, y + sinf(angulo) * raio);
    }
    glEnd();
}

void iniciarFrango(Frango *f)
{
    resetarFrango(f);
    f->vidas = MAX_VIDAS;
    f->corR = 1.0f;
    f->corG = 0.92f;
    f->corB = 0.25f;
}

void desenharFrango(Frango *f)
{
    float t = TAMANHO_FRANGO;

    glColor3f(f->corR, f->corG, f->corB);
    glBegin(GL_QUADS);
    glVertex2f(f->x - t, f->y - t);
    glVertex2f(f->x + t, f->y - t);
    glVertex2f(f->x + t, f->y + t);
    glVertex2f(f->x - t, f->y + t);
    glEnd();

    glColor3f(1.0f, 0.96f, 0.45f);
    desenharCirculo(f->x, f->y + t * 0.95f, t * 0.75f);

    glColor3f(1.0f, 0.45f, 0.05f);
    glBegin(GL_TRIANGLES);
    glVertex2f(f->x + t * 0.15f, f->y + t * 1.05f);
    glVertex2f(f->x + t * 1.10f, f->y + t * 0.85f);
    glVertex2f(f->x + t * 0.15f, f->y + t * 0.65f);
    glEnd();

    glColor3f(0.05f, 0.05f, 0.05f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glVertex2f(f->x + t * 0.25f, f->y + t * 1.15f);
    glEnd();

    glColor3f(0.95f, 0.35f, 0.05f);
    glBegin(GL_LINES);
    glVertex2f(f->x - t * 0.35f, f->y - t);
    glVertex2f(f->x - t * 0.35f, f->y - t * 1.45f);
    glVertex2f(f->x + t * 0.35f, f->y - t);
    glVertex2f(f->x + t * 0.35f, f->y - t * 1.45f);
    glEnd();
}

void moverFrangoFrente(Frango *f)
{
    if (!f->estaVivo) {
        return;
    }
    f->y += PASSO_FRANGO;
    f->faixaAtual++;
}

void moverFrangoTras(Frango *f)
{
    if (!f->estaVivo) {
        return;
    }
    if (f->faixaAtual > 0) {
        f->y -= PASSO_FRANGO;
        f->faixaAtual--;
    }
}

void moverFrangoEsquerda(Frango *f)
{
    if (!f->estaVivo) {
        return;
    }
    f->x -= PASSO_FRANGO;
    limitarFrangoNaTela(f);
}

void moverFrangoDireita(Frango *f)
{
    if (!f->estaVivo) {
        return;
    }
    f->x += PASSO_FRANGO;
    limitarFrangoNaTela(f);
}

void resetarFrango(Frango *f)
{
    f->x = 0.0f;
    f->y = Y_INICIAL_FRANGO;
    f->faixaAtual = 0;
    f->estaVivo = 1;
}
