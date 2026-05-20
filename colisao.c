#include "colisao.h"
#include "configuracao.h"

static RetanguloColisao criarRetanguloFrango(Frango *f)
{
    RetanguloColisao r;

    r.x = f->x;
    r.y = f->y;
    r.largura = TAMANHO_FRANGO * 1.6f;
    r.altura = TAMANHO_FRANGO * 1.7f;
    return r;
}

static RetanguloColisao criarRetanguloObstaculo(Obstaculo *o)
{
    RetanguloColisao r;

    r.x = o->x;
    r.y = o->y;
    r.largura = o->largura;
    r.altura = o->altura;
    return r;
}

int verificarColisaoAABB(RetanguloColisao a, RetanguloColisao b)
{
    float esquerdaA = a.x - a.largura * 0.5f;
    float direitaA = a.x + a.largura * 0.5f;
    float baixoA = a.y - a.altura * 0.5f;
    float cimaA = a.y + a.altura * 0.5f;
    float esquerdaB = b.x - b.largura * 0.5f;
    float direitaB = b.x + b.largura * 0.5f;
    float baixoB = b.y - b.altura * 0.5f;
    float cimaB = b.y + b.altura * 0.5f;

    return esquerdaA < direitaB && direitaA > esquerdaB &&
           baixoA < cimaB && cimaA > baixoB;
}

int verificarColisaoFrango(Frango *f, Obstaculo lista[], int total)
{
    int i;
    RetanguloColisao areaFrango = criarRetanguloFrango(f);

    for (i = 0; i < total; i++) {
        RetanguloColisao areaObstaculo;

        if (!lista[i].ativo) {
            continue;
        }
        areaObstaculo = criarRetanguloObstaculo(&lista[i]);
        if (verificarColisaoAABB(areaFrango, areaObstaculo)) {
            if (lista[i].tipo == TIPO_CARRO) {
                return COLISAO_CARRO;
            }
            if (lista[i].tipo == TIPO_TORA) {
                return COLISAO_TORA;
            }
        }
    }
    return COLISAO_NENHUMA;
}

int verificarVitoria(Frango *f)
{
#if JOGO_INVERTIDO
    return f->faixaAtual <= 0;
#else
    return f->faixaAtual >= META_VITORIA;
#endif
}

int verificarDerrota(Frango *f)
{
    return f->vidas <= 0;
}
