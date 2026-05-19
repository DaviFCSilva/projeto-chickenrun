#include "visual.h"
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535f

static void desenharRetangulo(float x, float y, float largura, float altura)
{
    glBegin(GL_QUADS);
    glVertex2f(x - largura * 0.5f, y - altura * 0.5f);
    glVertex2f(x + largura * 0.5f, y - altura * 0.5f);
    glVertex2f(x + largura * 0.5f, y + altura * 0.5f);
    glVertex2f(x - largura * 0.5f, y + altura * 0.5f);
    glEnd();
}

static void desenharCirculo(float x, float y, float raio)
{
    int i;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (i = 0; i <= 24; i++) {
        float angulo = (2.0f * PI * (float)i) / 24.0f;
        glVertex2f(x + cosf(angulo) * raio, y + sinf(angulo) * raio);
    }
    glEnd();
}

static void desenharElipse(float x, float y, float raioX, float raioY)
{
    int i;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (i = 0; i <= 28; i++) {
        float angulo = (2.0f * PI * (float)i) / 28.0f;
        glVertex2f(x + cosf(angulo) * raioX, y + sinf(angulo) * raioY);
    }
    glEnd();
}

static void desenharVisualFrango(float x, float y, float largura, float altura,
                                 float corR, float corG, float corB)
{
    float t = altura / 3.0f;

    if (largura / 2.4f < t) {
        t = largura / 2.4f;
    }

    glColor3f(corR, corG, corB);
    desenharElipse(x, y - t * 0.10f, t * 1.10f, t * 0.95f);

    glColor3f(1.0f, 0.96f, 0.45f);
    desenharCirculo(x, y + t * 0.95f, t * 0.75f);

    glColor3f(0.96f, 0.72f, 0.16f);
    desenharElipse(x - t * 0.45f, y - t * 0.10f, t * 0.34f, t * 0.48f);

    glColor3f(1.0f, 0.45f, 0.05f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + t * 0.15f, y + t * 1.05f);
    glVertex2f(x + t * 1.10f, y + t * 0.85f);
    glVertex2f(x + t * 0.15f, y + t * 0.65f);
    glEnd();

    glColor3f(0.95f, 0.06f, 0.04f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - t * 0.28f, y + t * 1.55f);
    glVertex2f(x - t * 0.05f, y + t * 1.18f);
    glVertex2f(x + t * 0.12f, y + t * 1.55f);
    glEnd();

    glColor3f(0.05f, 0.05f, 0.05f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glVertex2f(x + t * 0.25f, y + t * 1.15f);
    glEnd();
    glPointSize(3.0f);

    glColor3f(0.95f, 0.35f, 0.05f);
    glBegin(GL_LINES);
    glVertex2f(x - t * 0.35f, y - t);
    glVertex2f(x - t * 0.35f, y - t * 1.45f);
    glVertex2f(x + t * 0.35f, y - t);
    glVertex2f(x + t * 0.35f, y - t * 1.45f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(x - t * 0.45f, y - t * 1.45f);
    glVertex2f(x - t * 0.25f, y - t * 1.45f);
    glVertex2f(x - t * 0.35f, y - t * 1.32f);
    glVertex2f(x + t * 0.25f, y - t * 1.45f);
    glVertex2f(x + t * 0.45f, y - t * 1.45f);
    glVertex2f(x + t * 0.35f, y - t * 1.32f);
    glEnd();
}

static void desenharVisualCarro(float x, float y, float largura, float altura,
                                float corR, float corG, float corB)
{
    float alturaCorpo = altura * 0.52f;

    glColor3f(corR, corG, corB);
    desenharRetangulo(x, y - altura * 0.05f, largura, alturaCorpo);

    glColor3f(0.75f, 0.90f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - largura * 0.22f, y + altura * 0.16f);
    glVertex2f(x + largura * 0.22f, y + altura * 0.16f);
    glVertex2f(x, y + altura * 0.46f);
    glEnd();

    glColor3f(1.0f, 0.93f, 0.34f);
    desenharRetangulo(x + largura * 0.42f, y - altura * 0.05f,
                      largura * 0.10f, altura * 0.16f);
    desenharRetangulo(x - largura * 0.42f, y - altura * 0.05f,
                      largura * 0.10f, altura * 0.16f);

    glColor3f(1.0f, 1.0f, 1.0f);
    desenharRetangulo(x, y - altura * 0.12f, largura * 0.55f, altura * 0.07f);

    glColor3f(0.02f, 0.02f, 0.02f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex2f(x - largura * 0.32f, y - altura * 0.35f);
    glVertex2f(x + largura * 0.32f, y - altura * 0.35f);
    glEnd();
    glPointSize(3.0f);
}

static void desenharVisualTabua(float x, float y, float largura, float altura,
                                float corR, float corG, float corB)
{
    glColor3f(corR, corG, corB);
    desenharRetangulo(x, y, largura, altura * 0.58f);

    glColor3f(0.72f, 0.42f, 0.16f);
    desenharRetangulo(x, y + altura * 0.13f, largura * 0.84f, altura * 0.08f);

    glColor3f(0.25f, 0.12f, 0.04f);
    glBegin(GL_LINES);
    glVertex2f(x - largura * 0.40f, y - altura * 0.20f);
    glVertex2f(x - largura * 0.40f, y + altura * 0.20f);
    glVertex2f(x, y - altura * 0.24f);
    glVertex2f(x, y + altura * 0.24f);
    glVertex2f(x + largura * 0.40f, y - altura * 0.20f);
    glVertex2f(x + largura * 0.40f, y + altura * 0.20f);
    glEnd();
}

void desenharVisual(TipoVisual visual, float x, float y,
                    float largura, float altura,
                    float corR, float corG, float corB)
{
    if (visual == VISUAL_CARRO) {
        desenharVisualCarro(x, y, largura, altura, corR, corG, corB);
    } else if (visual == VISUAL_TABUA) {
        desenharVisualTabua(x, y, largura, altura, corR, corG, corB);
    } else {
        desenharVisualFrango(x, y, largura, altura, corR, corG, corB);
    }
}
