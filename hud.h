#ifndef HUD_H
#define HUD_H

void desenharHUD(void);
void desenharPontuacao(int pontos);
void desenharTimer(int segundos);
void desenharVidas(int vidas);
void desenharTexto(float x, float y, const char *texto);
void desenharTextoGrande(float x, float y, const char *texto);

#endif
