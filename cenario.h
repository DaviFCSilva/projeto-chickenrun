#ifndef CENARIO_H
#define CENARIO_H

#define LARGURA_MUNDO 2.0f
#define X_ESQUERDA_MUNDO -1.0f
#define X_DIREITA_MUNDO 1.0f
#define ALTURA_FAIXA 0.18f

typedef enum {
    FAIXA_GRAMA,
    FAIXA_PISTA,
    FAIXA_RIO
} TipoFaixa;

void desenharCenario(float offsetCamera);
void desenharGrama(float x, float y, float largura, float altura);
void desenharPista(float x, float y, float largura, float altura);
void desenharRio(float x, float y, float largura, float altura);
void desenharLinhasDePista(float y, float altura);
TipoFaixa obterTipoFaixa(int indiceFaixa);
float obterYFaixa(int indiceFaixa);

#endif
