#ifndef FRANGO_H
#define FRANGO_H

#define MAX_VIDAS 3
#define TAMANHO_FRANGO 0.05f
#define PASSO_FRANGO 0.18f
#define LIMITE_X_FRANGO 0.95f
#define Y_INICIAL_FRANGO 0.09f

typedef struct {
    float x, y;
    int vidas;
    int faixaAtual;
    int estaVivo;
    float corR, corG, corB;
} Frango;

void iniciarFrango(Frango *f);
void desenharFrango(Frango *f);
void moverFrangoFrente(Frango *f);
void moverFrangoTras(Frango *f);
void moverFrangoEsquerda(Frango *f);
void moverFrangoDireita(Frango *f);
void resetarFrango(Frango *f);

#endif
