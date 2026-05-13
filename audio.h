#ifndef AUDIO_H
#define AUDIO_H

#include <AL/al.h>
#include <AL/alc.h>

typedef enum {
    SOM_PULO,
    SOM_COLISAO,
    SOM_SPLASH,
    SOM_VITORIA,
    SOM_DERROTA,
    SOM_PONTUACAO,
    TOTAL_SONS
} TipoSom;

void iniciarAudio(void);
void finalizarAudio(void);
void tocarSom(TipoSom tipo);
void pararSom(TipoSom tipo);
void tocarMusicaFundo(void);
void pararMusicaFundo(void);

#endif
