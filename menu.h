#ifndef MENU_H
#define MENU_H

typedef struct {
    float x, y;
    float largura, altura;
    const char *rotulo;
    int destacado;
} BotaoMenu;

void desenharMenuInicial(void);
void desenharTelaPausa(void);
void desenharTelaVitoria(int pontuacao, int tempo);
void desenharTelaDerrota(int pontuacao);
int verificarCliqueEmBotao(BotaoMenu *botao, int mouseX, int mouseY);
void processarCliqueMouse(int mouseX, int mouseY);
void atualizarDestaqueMenu(int mouseX, int mouseY);

#endif
