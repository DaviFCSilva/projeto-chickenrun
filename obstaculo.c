#include "obstaculo.h"
#include "cenario.h"
#include "configuracao.h"
#include "visual.h"
#include <stdlib.h>
#include <time.h>

Obstaculo obstaculos[MAX_OBSTACULOS];
int totalObstaculos = 0;

#define CARROS_POR_FAIXA 3
#define TORAS_POR_FAIXA 3
#define VELOCIDADE_PISTA 0.34f
#define VELOCIDADE_RIO 0.24f
#define VARIACAO_VELOCIDADE 0.035f
#define APARENCIA_CARRO VISUAL_CARRO
#define APARENCIA_TORA VISUAL_TABUA

static void adicionarObstaculo(Obstaculo novo)
{
    if (totalObstaculos >= MAX_OBSTACULOS) {
        return;
    }
    obstaculos[totalObstaculos] = novo;
    totalObstaculos++;
}

void iniciarObstaculos(void)
{
    int i;

    totalObstaculos = 0;
    srand((unsigned int)time(NULL));
    for (i = 1; i <= META_VITORIA + 2; i++) {
        gerarFaixa(i);
    }
}

void gerarFaixa(int indiceFaixa)
{
    int i;
    int quantidade;
    TipoFaixa faixa = obterTipoFaixa(indiceFaixa);

    if (faixa == FAIXA_GRAMA) {
        return;
    }

    quantidade = (faixa == FAIXA_RIO) ? TORAS_POR_FAIXA : CARROS_POR_FAIXA;
    for (i = 0; i < quantidade; i++) {
        Obstaculo o;
        float direcao = (indiceFaixa % 2 == 0) ? 1.0f : -1.0f;
        float deslocamento = (float)((indiceFaixa * 17 + i * 13) % 20) / 100.0f;
        float velocidadeBase = (faixa == FAIXA_RIO) ? VELOCIDADE_RIO : VELOCIDADE_PISTA;

        if (indiceFaixa == 3) {
        velocidadeBase = 0.30f;
    }

#if JOGO_LATERAL
        o.x = obterXFaixa(indiceFaixa) + ALTURA_FAIXA * 0.5f;
        o.y = Y_BAIXO_MUNDO + 0.14f + (float)i * 0.46f + deslocamento;
#else
        o.x = -0.85f + (float)i * 0.78f + deslocamento;
        o.y = obterYFaixa(indiceFaixa) + ALTURA_FAIXA * 0.5f;
        o.altura = ALTURA_FAIXA * 0.48f;
#endif
        o.velocidade = direcao *
                       (velocidadeBase +
                        VARIACAO_VELOCIDADE * (float)(indiceFaixa % 5));
        
        o.ativo = 1;

        if (faixa == FAIXA_RIO) {
            o.tipo = TIPO_TORA;
#if JOGO_LATERAL
            o.largura = ALTURA_FAIXA * 0.50f;
            o.altura = 0.34f;
#else
            o.largura = 0.34f;
#endif
            o.corR = 0.52f;
            o.corG = 0.28f;
            o.corB = 0.10f;
        } else {
            o.tipo = TIPO_CARRO;
#if JOGO_LATERAL
            o.largura = ALTURA_FAIXA * 0.48f;
            o.altura = 0.22f;
#else
            o.largura = 0.22f;
#endif
            o.corR = 0.85f - 0.10f * (float)i;
            o.corG = 0.12f + 0.20f * (float)(indiceFaixa % 3);
            o.corB = 0.18f + 0.15f * (float)i;
        }
        adicionarObstaculo(o);
    }
}

void atualizarObstaculos(float deltaTempo)
{
    int i;

    for (i = 0; i < totalObstaculos; i++) {
#if JOGO_LATERAL
        float limiteCima = Y_CIMA_MUNDO + 0.25f + obstaculos[i].altura;
        float limiteBaixo = Y_BAIXO_MUNDO - 0.25f - obstaculos[i].altura;
#else
        float limite = 1.35f + obstaculos[i].largura;
#endif

        if (!obstaculos[i].ativo) {
            continue;
        }
#if JOGO_LATERAL
        obstaculos[i].y += obstaculos[i].velocidade * deltaTempo;
        if (obstaculos[i].y > limiteCima) {
            obstaculos[i].y = limiteBaixo;
        }
        if (obstaculos[i].y < limiteBaixo) {
            obstaculos[i].y = limiteCima;
        }
#else
        obstaculos[i].x += obstaculos[i].velocidade * deltaTempo;
        if (obstaculos[i].x > limite) {
            obstaculos[i].x = -limite;
        }
        if (obstaculos[i].x < -limite) {
            obstaculos[i].x = limite;
        }
#endif
    }
}

void desenharObstaculos(void)
{
    int i;

    for (i = 0; i < totalObstaculos; i++) {
        if (!obstaculos[i].ativo) {
            continue;
        }
        if (obstaculos[i].tipo == TIPO_CARRO) {
            desenharVisual(APARENCIA_CARRO,
                           obstaculos[i].x, obstaculos[i].y,
                           obstaculos[i].largura, obstaculos[i].altura,
                           obstaculos[i].corR, obstaculos[i].corG,
                           obstaculos[i].corB);
        } else if (obstaculos[i].tipo == TIPO_TORA) {
            desenharVisual(APARENCIA_TORA,
                           obstaculos[i].x, obstaculos[i].y,
                           obstaculos[i].largura, obstaculos[i].altura,
                           obstaculos[i].corR, obstaculos[i].corG,
                           obstaculos[i].corB);
        }
    }
}
