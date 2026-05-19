#ifndef VISUAL_H
#define VISUAL_H

typedef enum {
    VISUAL_FRANGO,
    VISUAL_CARRO,
    VISUAL_TABUA
} TipoVisual;

void desenharVisual(TipoVisual visual, float x, float y,
                    float largura, float altura,
                    float corR, float corG, float corB);

#endif
