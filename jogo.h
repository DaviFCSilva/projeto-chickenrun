#ifndef JOGO_H
#define JOGO_H

typedef enum {
    ESTADO_MENU,
    ESTADO_JOGANDO,
    ESTADO_PAUSADO,
    ESTADO_VITORIA,
    ESTADO_DERROTA
} EstadoJogo;

typedef struct {
    EstadoJogo estadoAtual;
    int pontuacao;
    int melhorPontuacao;
    int tempoInicio;
    int tempoDecorrido;
    int larguraJanela;
    int alturaJanela;
} JogoEstado;

extern JogoEstado jogo;

void iniciarJogo(int largura, int altura);
void iniciarPartida(void);
void atualizarTempoJogo(void);
void somarPontuacao(int pontos);
void salvarMelhorPontuacao(void);

#endif
