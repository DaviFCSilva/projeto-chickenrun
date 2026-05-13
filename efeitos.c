#include "efeitos.h"
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.1415926535f
#define MAX_TRILHA 28

typedef struct {
    float x, y;
    float vida;
} PontoTrilha;

static Particula particulas[MAX_PARTICULAS];
static PontoTrilha trilha[MAX_TRILHA];
static float tempoFlash = 0.0f;
static int indiceTrilha = 0;

void limparEfeitos(void)
{
    int i;

    tempoFlash = 0.0f;
    indiceTrilha = 0;
    for (i = 0; i < MAX_PARTICULAS; i++) {
        particulas[i].ativa = 0;
    }
    for (i = 0; i < MAX_TRILHA; i++) {
        trilha[i].vida = 0.0f;
    }
}

void criarExplosaoParticulas(float x, float y)
{
    int i;

    for (i = 0; i < MAX_PARTICULAS; i++) {
        float angulo = (2.0f * PI * (float)i) / (float)MAX_PARTICULAS;
        float forca = 0.24f + (float)(rand() % 80) / 260.0f;

        particulas[i].x = x;
        particulas[i].y = y;
        particulas[i].velX = cosf(angulo) * forca;
        particulas[i].velY = sinf(angulo) * forca;
        particulas[i].vida = 1.0f;
        particulas[i].corR = 1.0f;
        particulas[i].corG = 0.68f + (float)(rand() % 30) / 100.0f;
        particulas[i].corB = 0.12f;
        particulas[i].tamanho = 2.0f + (float)(rand() % 5);
        particulas[i].ativa = 1;
    }
}

void atualizarParticulas(float deltaTempo)
{
    int i;

    if (tempoFlash > 0.0f) {
        tempoFlash -= deltaTempo * 3.1f;
    }
    if (tempoFlash < 0.0f) {
        tempoFlash = 0.0f;
    }

    for (i = 0; i < MAX_PARTICULAS; i++) {
        if (!particulas[i].ativa) {
            continue;
        }
        particulas[i].x += particulas[i].velX * deltaTempo;
        particulas[i].y += particulas[i].velY * deltaTempo;
        particulas[i].velY -= 0.42f * deltaTempo;
        particulas[i].vida -= deltaTempo * 1.55f;
        if (particulas[i].vida <= 0.0f) {
            particulas[i].ativa = 0;
        }
    }

    for (i = 0; i < MAX_TRILHA; i++) {
        if (trilha[i].vida > 0.0f) {
            trilha[i].vida -= deltaTempo * 1.8f;
        }
    }
}

void desenharParticulas(void)
{
    int i;

    for (i = 0; i < MAX_PARTICULAS; i++) {
        float t;

        if (!particulas[i].ativa) {
            continue;
        }
        t = particulas[i].tamanho * 0.0028f;
        glColor4f(particulas[i].corR, particulas[i].corG,
                  particulas[i].corB, particulas[i].vida);
        glBegin(GL_QUADS);
        glVertex2f(particulas[i].x - t, particulas[i].y - t);
        glVertex2f(particulas[i].x + t, particulas[i].y - t);
        glVertex2f(particulas[i].x + t, particulas[i].y + t);
        glVertex2f(particulas[i].x - t, particulas[i].y + t);
        glEnd();
    }
}

void ativarFlashTela(void)
{
    tempoFlash = 1.0f;
}

void desenharFlashTela(void)
{
    if (tempoFlash <= 0.0f) {
        return;
    }

    glColor4f(1.0f, 0.05f, 0.05f, tempoFlash * 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.15f);
    glVertex2f(1.0f, -0.15f);
    glVertex2f(1.0f, 1.15f);
    glVertex2f(-1.0f, 1.15f);
    glEnd();
}

void registrarPosicaoTrilha(float x, float y)
{
    trilha[indiceTrilha].x = x;
    trilha[indiceTrilha].y = y;
    trilha[indiceTrilha].vida = 1.0f;
    indiceTrilha = (indiceTrilha + 1) % MAX_TRILHA;
}

void desenharTrilha(void)
{
    int i;

    for (i = 0; i < MAX_TRILHA; i++) {
        float t = 0.020f + trilha[i].vida * 0.010f;

        if (trilha[i].vida <= 0.0f) {
            continue;
        }
        glColor4f(1.0f, 1.0f, 0.30f, trilha[i].vida * 0.45f);
        glBegin(GL_QUADS);
        glVertex2f(trilha[i].x - t, trilha[i].y - t);
        glVertex2f(trilha[i].x + t, trilha[i].y - t);
        glVertex2f(trilha[i].x + t, trilha[i].y + t);
        glVertex2f(trilha[i].x - t, trilha[i].y + t);
        glEnd();
    }
}
