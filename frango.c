#include "frango.h"
#include "visual.h"

#define APARENCIA_FRANGO VISUAL_FRANGO

static void limitarFrangoNaTela(Frango *f)
{
    if (f->x < -LIMITE_X_FRANGO) {
        f->x = -LIMITE_X_FRANGO;
    }
    if (f->x > LIMITE_X_FRANGO) {
        f->x = LIMITE_X_FRANGO;
    }
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
