#include "audio.h"
#include <math.h>
#include <stdlib.h>

#define TAXA_AUDIO 22050
#define PI_AUDIO 3.1415926535

static ALCdevice *dispositivoAudio = NULL;
static ALCcontext *contextoAudio = NULL;
static ALuint buffers[TOTAL_SONS];
static ALuint fontes[TOTAL_SONS];
static ALuint bufferMusica = 0;
static ALuint fonteMusica = 0;
static int audioAtivo = 0;

static void gerarTom(ALuint buffer, float frequencia, float duracao, float volume)
{
    int i;
    int totalAmostras = (int)((float)TAXA_AUDIO * duracao);
    short *amostras = (short *)malloc(sizeof(short) * (size_t)totalAmostras);

    if (amostras == NULL) {
        return;
    }

    for (i = 0; i < totalAmostras; i++) {
        double tempo = (double)i / (double)TAXA_AUDIO;
        double envelope = 1.0 - ((double)i / (double)totalAmostras);
        double onda = sin(2.0 * PI_AUDIO * (double)frequencia * tempo) * 0.78;
        onda += sin(4.0 * PI_AUDIO * (double)frequencia * tempo) * 0.22;
        amostras[i] = (short)(onda * envelope * (double)volume * 32767.0);
    }

    alBufferData(buffer, AL_FORMAT_MONO16, amostras,
                 (ALsizei)(sizeof(short) * (size_t)totalAmostras), TAXA_AUDIO);
    free(amostras);
}

static void gerarMusica(ALuint buffer)
{
    int i;
    int totalAmostras = TAXA_AUDIO * 2;
    short *amostras = (short *)malloc(sizeof(short) * (size_t)totalAmostras);
    float notas[4] = {261.63f, 329.63f, 392.00f, 523.25f};

    if (amostras == NULL) {
        return;
    }

    for (i = 0; i < totalAmostras; i++) {
        int nota = (i / (TAXA_AUDIO / 4)) % 4;
        double tempo = (double)i / (double)TAXA_AUDIO;
        double onda = sin(2.0 * PI_AUDIO * (double)notas[nota] * tempo);
        amostras[i] = (short)(onda * 0.14 * 32767.0);
    }

    alBufferData(buffer, AL_FORMAT_MONO16, amostras,
                 (ALsizei)(sizeof(short) * (size_t)totalAmostras), TAXA_AUDIO);
    free(amostras);
}

static void configurarFonte(ALuint fonte, ALuint buffer, int repetir, float ganho)
{
    alSourcei(fonte, AL_BUFFER, (ALint)buffer);
    alSourcei(fonte, AL_LOOPING, repetir ? AL_TRUE : AL_FALSE);
    alSourcef(fonte, AL_GAIN, ganho);
}

void iniciarAudio(void)
{
    dispositivoAudio = alcOpenDevice(NULL);
    if (dispositivoAudio == NULL) {
        audioAtivo = 0;
        return;
    }

    contextoAudio = alcCreateContext(dispositivoAudio, NULL);
    if (contextoAudio == NULL) {
        alcCloseDevice(dispositivoAudio);
        dispositivoAudio = NULL;
        audioAtivo = 0;
        return;
    }

    alcMakeContextCurrent(contextoAudio);
    alGenBuffers(TOTAL_SONS, buffers);
    alGenSources(TOTAL_SONS, fontes);
    alGenBuffers(1, &bufferMusica);
    alGenSources(1, &fonteMusica);

    gerarTom(buffers[SOM_PULO], 660.0f, 0.10f, 0.35f);
    gerarTom(buffers[SOM_COLISAO], 120.0f, 0.28f, 0.55f);
    gerarTom(buffers[SOM_SPLASH], 220.0f, 0.22f, 0.45f);
    gerarTom(buffers[SOM_VITORIA], 880.0f, 0.45f, 0.45f);
    gerarTom(buffers[SOM_DERROTA], 90.0f, 0.60f, 0.50f);
    gerarTom(buffers[SOM_PONTUACAO], 760.0f, 0.08f, 0.30f);
    gerarMusica(bufferMusica);

    configurarFonte(fontes[SOM_PULO], buffers[SOM_PULO], 0, 0.70f);
    configurarFonte(fontes[SOM_COLISAO], buffers[SOM_COLISAO], 0, 0.90f);
    configurarFonte(fontes[SOM_SPLASH], buffers[SOM_SPLASH], 0, 0.85f);
    configurarFonte(fontes[SOM_VITORIA], buffers[SOM_VITORIA], 0, 0.85f);
    configurarFonte(fontes[SOM_DERROTA], buffers[SOM_DERROTA], 0, 0.85f);
    configurarFonte(fontes[SOM_PONTUACAO], buffers[SOM_PONTUACAO], 0, 0.60f);
    configurarFonte(fonteMusica, bufferMusica, 1, 0.18f);

    audioAtivo = 1;
}

void finalizarAudio(void)
{
    if (!audioAtivo) {
        return;
    }

    alSourceStop(fonteMusica);
    alDeleteSources(TOTAL_SONS, fontes);
    alDeleteSources(1, &fonteMusica);
    alDeleteBuffers(TOTAL_SONS, buffers);
    alDeleteBuffers(1, &bufferMusica);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(contextoAudio);
    alcCloseDevice(dispositivoAudio);
    audioAtivo = 0;
}

void tocarSom(TipoSom tipo)
{
    if (!audioAtivo || tipo < 0 || tipo >= TOTAL_SONS) {
        return;
    }
    alSourceStop(fontes[tipo]);
    alSourcePlay(fontes[tipo]);
}

void pararSom(TipoSom tipo)
{
    if (!audioAtivo || tipo < 0 || tipo >= TOTAL_SONS) {
        return;
    }
    alSourceStop(fontes[tipo]);
}

void tocarMusicaFundo(void)
{
    if (audioAtivo) {
        alSourcePlay(fonteMusica);
    }
}

void pararMusicaFundo(void)
{
    if (audioAtivo) {
        alSourceStop(fonteMusica);
    }
}
