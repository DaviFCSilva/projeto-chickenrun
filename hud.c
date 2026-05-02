#include "hud.h"
#include "jogo.h"
#include "frango.h"
#include <GL/glut.h>
#include <stdio.h>

extern Frango frangoJogador;

/* Escreve texto bitmap nas coordenadas ortograficas da tela. */
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

void desenharHUD(void)
{
    int segundos = jogo.tempoDecorrido / 1000;
    char melhor[64];

    glColor3f(1.0f, 1.0f, 1.0f);
    desenharPontuacao(jogo.pontuacao);
    desenharTimer(segundos);
    desenharVidas(frangoJogador.vidas);

    snprintf(melhor, sizeof(melhor), "Melhor: %d", jogo.melhorPontuacao);
    desenharTexto(0.63f, 1.02f, melhor);
}
