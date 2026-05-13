#ifndef EFEITOS_H
#define EFEITOS_H

#define MAX_PARTICULAS 120

typedef struct {
    float x, y;
    float velX, velY;
    float vida;
    float corR, corG, corB;
    float tamanho;
    int ativa;
} Particula;

void criarExplosaoParticulas(float x, float y);
void atualizarParticulas(float deltaTempo);
void desenharParticulas(void);
void ativarFlashTela(void);
void desenharFlashTela(void);
void registrarPosicaoTrilha(float x, float y);
void desenharTrilha(void);
void limparEfeitos(void);

#endif
