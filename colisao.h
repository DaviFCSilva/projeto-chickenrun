#ifndef COLISAO_H
#define COLISAO_H

#include "frango.h"
#include "obstaculo.h"

#define COLISAO_NENHUMA 0
#define COLISAO_CARRO 1
#define COLISAO_TORA 2

typedef struct {
    float x, y;
    float largura, altura;
} RetanguloColisao;

int verificarColisaoAABB(RetanguloColisao a, RetanguloColisao b);
int verificarColisaoFrango(Frango *f, Obstaculo obstaculos[], int total);
int verificarVitoria(Frango *f);
int verificarDerrota(Frango *f);

#endif
