#include "jogo.h"
#include <GL/glut.h>

JogoEstado jogo;

void iniciarJogo(int largura, int altura)
{
    jogo.estadoAtual = ESTADO_MENU;
    jogo.pontuacao = 0;
    jogo.melhorPontuacao = 0;
    jogo.tempoInicio = glutGet(GLUT_ELAPSED_TIME);
    jogo.tempoDecorrido = 0;
    jogo.larguraJanela = largura;
    jogo.alturaJanela = altura;
}

void iniciarPartida(void)
{
    jogo.estadoAtual = ESTADO_JOGANDO;
    jogo.pontuacao = 0;
    jogo.tempoInicio = glutGet(GLUT_ELAPSED_TIME);
    jogo.tempoDecorrido = 0;
}

void atualizarTempoJogo(void)
{
    int agora = glutGet(GLUT_ELAPSED_TIME);
    jogo.tempoDecorrido = agora - jogo.tempoInicio;
}

void somarPontuacao(int pontos)
{
    if (pontos > 0) {
        jogo.pontuacao += pontos;
    }
}

void salvarMelhorPontuacao(void)
{
    if (jogo.pontuacao > jogo.melhorPontuacao) {
        jogo.melhorPontuacao = jogo.pontuacao;
    }
}
