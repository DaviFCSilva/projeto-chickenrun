#include "menu.h"
#include "jogo.h"
#include "hud.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static BotaoMenu botaoIniciar = {0.0f, 0.42f, 0.58f, 0.14f, "INICIAR", 0};
static BotaoMenu botaoSair = {0.0f, 0.23f, 0.58f, 0.14f, "SAIR", 0};
static BotaoMenu botaoContinuar = {0.0f, 0.42f, 0.62f, 0.14f, "CONTINUAR", 0};
static BotaoMenu botaoMenu = {0.0f, 0.23f, 0.58f, 0.14f, "MENU", 0};
static BotaoMenu botaoReiniciar = {0.0f, 0.30f, 0.62f, 0.14f, "REINICIAR", 0};
static BotaoMenu botaoMenuFinal = {0.0f, 0.11f, 0.58f, 0.14f, "MENU", 0};

static void desenharRetanguloMenu(float x, float y, float largura, float altura)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + largura, y);
    glVertex2f(x + largura, y + altura);
    glVertex2f(x, y + altura);
    glEnd();
}

static void desenharTextoGrandeCentralizado(float x, float y, const char *texto)
{
    float ajuste = (float)strlen(texto) * 0.011f;
    desenharTextoGrande(x - ajuste, y, texto);
}

static void desenharClassificacaoFinal(void)
{
    int total = obterTotalClassificacao();
    int i;
    char texto[64];

    glColor3f(1.0f, 0.94f, 0.20f);
    desenharTexto(0.42f, 0.58f, "CLASSIFICACAO");

    glColor3f(1.0f, 1.0f, 1.0f);
    if (total == 0) {
        desenharTexto(0.42f, 0.52f, "Sem pontos");
        return;
    }

    for (i = 0; i < total; i++) {
        snprintf(texto, sizeof(texto), "%d. %d pontos",
                 i + 1, obterPontuacaoClassificacao(i));
        desenharTexto(0.42f, 0.52f - (float)i * 0.055f, texto);
    }
}

static void converterMouse(int mouseX, int mouseY, float *x, float *y)
{
    float largura = (float)jogo.larguraJanela;
    float altura = (float)jogo.alturaJanela;

    if (largura <= 0.0f || altura <= 0.0f) {
        *x = 0.0f;
        *y = 0.0f;
        return;
    }
    *x = -1.0f + 2.0f * ((float)mouseX / largura);
    *y = 1.15f - 1.30f * ((float)mouseY / altura);
}

static void desenharFundoMenu(void)
{
    glColor3f(0.06f, 0.18f, 0.25f);
    desenharRetanguloMenu(-1.0f, -0.15f, 2.0f, 1.30f);

    glColor3f(0.16f, 0.55f, 0.23f);
    desenharRetanguloMenu(-1.0f, -0.15f, 2.0f, 0.22f);

    glColor3f(0.16f, 0.16f, 0.17f);
    desenharRetanguloMenu(-1.0f, 0.07f, 2.0f, 0.13f);

    glColor3f(0.06f, 0.34f, 0.78f);
    desenharRetanguloMenu(-1.0f, 0.20f, 2.0f, 0.12f);

    glColor3f(0.95f, 0.92f, 0.30f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.75f, 0.92f);
    glVertex2f(-0.67f, 0.92f);
    glVertex2f(-0.71f, 1.00f);
    glVertex2f(0.72f, 0.82f);
    glVertex2f(0.80f, 0.82f);
    glVertex2f(0.76f, 0.90f);
    glEnd();
}

static void desenharBotao(BotaoMenu *botao)
{
    float e = botao->x - botao->largura * 0.5f;
    float d = botao->x + botao->largura * 0.5f;
    float b = botao->y - botao->altura * 0.5f;
    float c = botao->y + botao->altura * 0.5f;

    if (botao->destacado) {
        glColor3f(0.98f, 0.76f, 0.22f);
    } else {
        glColor3f(0.22f, 0.52f, 0.42f);
    }

    glBegin(GL_QUADS);
    glVertex2f(e, b);
    glVertex2f(d, b);
    glVertex2f(d, c);
    glVertex2f(e, c);
    glEnd();

    glColor3f(0.04f, 0.12f, 0.14f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(e, b);
    glVertex2f(d, b);
    glVertex2f(d, c);
    glVertex2f(e, c);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    desenharTextoGrandeCentralizado(botao->x, botao->y - 0.02f, botao->rotulo);
}

void desenharMenuInicial(void)
{
    desenharFundoMenu();
    glColor3f(1.0f, 0.94f, 0.20f);
    desenharTextoGrandeCentralizado(0.0f, 0.78f, "CHICKEN RUN");
    glColor3f(0.88f, 0.94f, 1.0f);
    desenharTexto(-0.47f, 0.66f, "Atravesse pistas e rios sem perder as vidas.");
    desenharBotao(&botaoIniciar);
    desenharBotao(&botaoSair);
}

void desenharTelaPausa(void)
{
    glColor4f(0.0f, 0.0f, 0.0f, 0.55f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.15f);
    glVertex2f(1.0f, -0.15f);
    glVertex2f(1.0f, 1.15f);
    glVertex2f(-1.0f, 1.15f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    desenharTextoGrande(-0.15f, 0.66f, "PAUSA");
    desenharBotao(&botaoContinuar);
    desenharBotao(&botaoMenu);
}

void desenharTelaVitoria(int pontuacao, int tempo)
{
    char texto[96];

    desenharFundoMenu();
    glColor3f(0.35f, 1.0f, 0.35f);
    desenharTextoGrande(-0.20f, 0.72f, "VITORIA!");
    snprintf(texto, sizeof(texto), "Pontos: %d   Tempo: %ds", pontuacao, tempo / 1000);
    glColor3f(1.0f, 1.0f, 1.0f);
    desenharTexto(-0.32f, 0.57f, texto);
    desenharClassificacaoFinal();
    desenharBotao(&botaoReiniciar);
    desenharBotao(&botaoMenuFinal);
}

void desenharTelaDerrota(int pontuacao)
{
    char texto[64];

    desenharFundoMenu();
    glColor3f(1.0f, 0.25f, 0.20f);
    desenharTextoGrande(-0.21f, 0.72f, "DERROTA");
    snprintf(texto, sizeof(texto), "Pontos: %d", pontuacao);
    glColor3f(1.0f, 1.0f, 1.0f);
    desenharTexto(-0.13f, 0.57f, texto);
    desenharClassificacaoFinal();
    desenharBotao(&botaoReiniciar);
    desenharBotao(&botaoMenuFinal);
}

int verificarCliqueEmBotao(BotaoMenu *botao, int mouseX, int mouseY)
{
    float x;
    float y;

    converterMouse(mouseX, mouseY, &x, &y);
    return x >= botao->x - botao->largura * 0.5f &&
           x <= botao->x + botao->largura * 0.5f &&
           y >= botao->y - botao->altura * 0.5f &&
           y <= botao->y + botao->altura * 0.5f;
}

void processarCliqueMouse(int mouseX, int mouseY)
{
    if (jogo.estadoAtual == ESTADO_MENU) {
        if (verificarCliqueEmBotao(&botaoIniciar, mouseX, mouseY)) {
            jogo.estadoAtual = ESTADO_JOGANDO;
        } else if (verificarCliqueEmBotao(&botaoSair, mouseX, mouseY)) {
            exit(0);
        }
    } else if (jogo.estadoAtual == ESTADO_PAUSADO) {
        if (verificarCliqueEmBotao(&botaoContinuar, mouseX, mouseY)) {
            jogo.estadoAtual = ESTADO_JOGANDO;
        } else if (verificarCliqueEmBotao(&botaoMenu, mouseX, mouseY)) {
            jogo.estadoAtual = ESTADO_MENU;
        }
    } else if (jogo.estadoAtual == ESTADO_VITORIA ||
               jogo.estadoAtual == ESTADO_DERROTA) {
        if (verificarCliqueEmBotao(&botaoReiniciar, mouseX, mouseY)) {
            jogo.estadoAtual = ESTADO_JOGANDO;
        } else if (verificarCliqueEmBotao(&botaoMenuFinal, mouseX, mouseY)) {
            jogo.estadoAtual = ESTADO_MENU;
        }
    }
}

void atualizarDestaqueMenu(int mouseX, int mouseY)
{
    botaoIniciar.destacado = verificarCliqueEmBotao(&botaoIniciar, mouseX, mouseY);
    botaoSair.destacado = verificarCliqueEmBotao(&botaoSair, mouseX, mouseY);
    botaoContinuar.destacado = verificarCliqueEmBotao(&botaoContinuar, mouseX, mouseY);
    botaoMenu.destacado = verificarCliqueEmBotao(&botaoMenu, mouseX, mouseY);
    botaoReiniciar.destacado = verificarCliqueEmBotao(&botaoReiniciar, mouseX, mouseY);
    botaoMenuFinal.destacado = verificarCliqueEmBotao(&botaoMenuFinal, mouseX, mouseY);
}
