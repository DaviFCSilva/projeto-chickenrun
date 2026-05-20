#include "frango.h"
#include "cenario.h"
#include "configuracao.h"
#include "obstaculo.h"
#include "visual.h"

#define APARENCIA_FRANGO VISUAL_FRANGO

static int obterFaixaInicialFrango(void)
{
    if (FAIXA_INICIAL_FRANGO < 0) {
        return 0;
    }
    if (FAIXA_INICIAL_FRANGO > META_VITORIA) {
        return META_VITORIA;
    }
    return FAIXA_INICIAL_FRANGO;
}

static void limitarFrangoNaTela(Frango *f)
{
#if JOGO_LATERAL
    if (f->y < Y_BAIXO_MUNDO + TAMANHO_FRANGO) {
        f->y = Y_BAIXO_MUNDO + TAMANHO_FRANGO;
    }
    if (f->y > Y_CIMA_MUNDO - TAMANHO_FRANGO) {
        f->y = Y_CIMA_MUNDO - TAMANHO_FRANGO;
    }
#else
    if (f->x < -LIMITE_X_FRANGO) {
        f->x = -LIMITE_X_FRANGO;
    }
    if (f->x > LIMITE_X_FRANGO) {
        f->x = LIMITE_X_FRANGO;
    }
#endif
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
    desenharVisual(APARENCIA_FRANGO, f->x, f->y,
                   TAMANHO_FRANGO * 2.4f, TAMANHO_FRANGO * 3.0f,
                   f->corR, f->corG, f->corB);
}

void moverFrangoFrente(Frango *f)
{
    if (!f->estaVivo) {
        return;
    }
#if JOGO_LATERAL
    f->x += PASSO_FRANGO;
    f->faixaAtual++;
#elif JOGO_INVERTIDO
    if (f->faixaAtual > 0) {
        f->y -= PASSO_FRANGO;
        f->faixaAtual--;
    }
#else
    f->y += PASSO_FRANGO;
    f->faixaAtual++;
#endif
}

void moverFrangoTras(Frango *f)
{
    if (!f->estaVivo) {
        return;
    }
#if JOGO_LATERAL
    if (f->faixaAtual > 0) {
        f->x -= PASSO_FRANGO;
        f->faixaAtual--;
    }
#elif JOGO_INVERTIDO
    if (f->faixaAtual < META_VITORIA) {
        f->y += PASSO_FRANGO;
        f->faixaAtual++;
    }
#else
    if (f->faixaAtual > 0) {
        f->y -= PASSO_FRANGO;
        f->faixaAtual--;
    }
#endif
}

void moverFrangoEsquerda(Frango *f)
{
    if (!f->estaVivo) {
        return;
    }
#if JOGO_LATERAL
    f->y -= PASSO_FRANGO;
#else
    f->x -= PASSO_FRANGO;
#endif
    limitarFrangoNaTela(f);
}

void moverFrangoDireita(Frango *f)
{
    if (!f->estaVivo) {
        return;
    }
#if JOGO_LATERAL
    f->y += PASSO_FRANGO;
#else
    f->x += PASSO_FRANGO;
#endif
    limitarFrangoNaTela(f);
}

void resetarFrango(Frango *f)
{
    int faixaInicial = obterFaixaInicialFrango();

#if JOGO_LATERAL
    f->faixaAtual = faixaInicial;
    f->x = obterXFaixa(f->faixaAtual) + ALTURA_FAIXA * 0.5f;
    f->y = (Y_BAIXO_MUNDO + Y_CIMA_MUNDO) * 0.5f;
#else
    f->x = 0.0f;
    f->faixaAtual = faixaInicial;
    f->y = obterYFaixa(f->faixaAtual) + ALTURA_FAIXA * 0.5f;
#endif
    f->estaVivo = 1;
}
