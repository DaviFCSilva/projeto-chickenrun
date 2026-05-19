#include "jogo.h"
#include <GL/glut.h>
#include <stdio.h>

#define MAX_CLASSIFICACAO 5

JogoEstado jogo;
static int classificacao[MAX_CLASSIFICACAO];
static int totalClassificacao = 0;

static void ordenarClassificacao(void)
{
    int i;
    int j;

    for (i = 0; i < totalClassificacao - 1; i++) {
        for (j = i + 1; j < totalClassificacao; j++) {
            if (classificacao[j] > classificacao[i]) {
                int troca = classificacao[i];
                classificacao[i] = classificacao[j];
                classificacao[j] = troca;
            }
        }
    }
}

static void atualizarMelhorClassificacao(void)
{
    if (totalClassificacao > 0) {
        jogo.melhorPontuacao = classificacao[0];
    }
}

static void adicionarPontuacaoClassificacao(int pontos)
{
    if (pontos < 0) {
        return;
    }

    if (totalClassificacao < MAX_CLASSIFICACAO) {
        classificacao[totalClassificacao] = pontos;
        totalClassificacao++;
    } else if (pontos > classificacao[MAX_CLASSIFICACAO - 1]) {
        classificacao[MAX_CLASSIFICACAO - 1] = pontos;
    } else {
        return;
    }

    ordenarClassificacao();
    atualizarMelhorClassificacao();
}

static void gravarClassificacao(void)
{
    FILE *arquivo = fopen("classificacao.txt", "w");
    int i;

    if (!arquivo) {
        return;
    }

    fprintf(arquivo, "Classificacao Chicken Run\n");
    for (i = 0; i < totalClassificacao; i++) {
        fprintf(arquivo, "%d. %d pontos\n", i + 1, classificacao[i]);
    }

    fclose(arquivo);
}

void carregarClassificacao(void)
{
    FILE *arquivo = fopen("classificacao.txt", "r");
    char linha[128];

    totalClassificacao = 0;
    jogo.melhorPontuacao = 0;

    if (!arquivo) {
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) &&
           totalClassificacao < MAX_CLASSIFICACAO) {
        int pontos;

        if (sscanf(linha, "%*d. %d", &pontos) == 1 ||
            sscanf(linha, "%d", &pontos) == 1) {
            adicionarPontuacaoClassificacao(pontos);
        }
    }

    fclose(arquivo);
    ordenarClassificacao();
    atualizarMelhorClassificacao();
}

void salvarPontuacaoClassificacao(void)
{
    adicionarPontuacaoClassificacao(jogo.pontuacao);
    gravarClassificacao();
}

int obterTotalClassificacao(void)
{
    return totalClassificacao;
}

int obterPontuacaoClassificacao(int indice)
{
    if (indice < 0 || indice >= totalClassificacao) {
        return 0;
    }
    return classificacao[indice];
}

void iniciarJogo(int largura, int altura)
{
    jogo.estadoAtual = ESTADO_MENU;
    jogo.pontuacao = 0;
    jogo.melhorPontuacao = 0;
    jogo.tempoInicio = glutGet(GLUT_ELAPSED_TIME);
    jogo.tempoDecorrido = 0;
    jogo.larguraJanela = largura;
    jogo.alturaJanela = altura;
    carregarClassificacao();
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
