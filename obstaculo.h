#ifndef OBSTACULO_H
#define OBSTACULO_H

#define MAX_OBSTACULOS 50
#define NUM_FAIXAS 10
#define META_VITORIA 10

typedef enum {
    TIPO_CARRO,
    TIPO_TORA,
    TIPO_RIO_VAZIO
} TipoObstaculo;

typedef struct {
    float x, y;
    float largura, altura;
    float velocidade;
    TipoObstaculo tipo;
    float corR, corG, corB;
    int ativo;
} Obstaculo;

extern Obstaculo obstaculos[MAX_OBSTACULOS];
extern int totalObstaculos;

void iniciarObstaculos(void);
void atualizarObstaculos(float deltaTempo);
void desenharObstaculos(void);
void gerarFaixa(int indiceFaixa);

#endif
