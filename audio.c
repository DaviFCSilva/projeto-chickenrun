#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *arquivo;
    ALuint buffer;
    ALuint fonte;
} SomAudio;

static ALCdevice *dispositivoAudio = NULL;
static ALCcontext *contextoAudio = NULL;
static SomAudio sons[TOTAL_SONS] = {
    {"assets/audio/pulo.wav", 0, 0},
    {"assets/audio/colisao.wav", 0, 0},
    {"assets/audio/splash.wav", 0, 0},
    {"assets/audio/vitoria.wav", 0, 0},
    {"assets/audio/derrota.wav", 0, 0},
    {"assets/audio/pontuacao.wav", 0, 0}
};
static ALuint bufferMusica = 0;
static ALuint fonteMusica = 0;
static int audioAtivo = 0;

static unsigned int lerU32(FILE *arquivo)
{
    unsigned char b[4];

    if (fread(b, 1, 4, arquivo) != 4) {
        return 0;
    }
    return (unsigned int)b[0] |
           ((unsigned int)b[1] << 8) |
           ((unsigned int)b[2] << 16) |
           ((unsigned int)b[3] << 24);
}

static unsigned short lerU16(FILE *arquivo)
{
    unsigned char b[2];

    if (fread(b, 1, 2, arquivo) != 2) {
        return 0;
    }
    return (unsigned short)(b[0] | (b[1] << 8));
}

static int lerId(FILE *arquivo, char id[5])
{
    if (fread(id, 1, 4, arquivo) != 4) {
        return 0;
    }
    id[4] = '\0';
    return 1;
}

static int pularBytes(FILE *arquivo, unsigned int bytes)
{
    return fseek(arquivo, (long)(bytes + (bytes % 2)), SEEK_CUR) == 0;
}

static ALenum obterFormato(int canais, int bits)
{
    if (canais == 1 && bits == 8) {
        return AL_FORMAT_MONO8;
    }
    if (canais == 1 && bits == 16) {
        return AL_FORMAT_MONO16;
    }
    if (canais == 2 && bits == 8) {
        return AL_FORMAT_STEREO8;
    }
    if (canais == 2 && bits == 16) {
        return AL_FORMAT_STEREO16;
    }
    return 0;
}

static int carregarWav(const char *caminho, ALuint buffer)
{
    FILE *arquivo = fopen(caminho, "rb");
    char id[5];
    unsigned short formatoAudio = 0;
    unsigned short canais = 0;
    unsigned int taxa = 0;
    unsigned short bits = 0;
    unsigned int tamanhoDados = 0;
    unsigned char *dados = NULL;
    ALenum formatoOpenAL = 0;
    int encontrouFmt = 0;
    int encontrouData = 0;

    if (arquivo == NULL) {
        return 0;
    }

    if (!lerId(arquivo, id) || strcmp(id, "RIFF") != 0) {
        fclose(arquivo);
        return 0;
    }

    lerU32(arquivo);
    if (!lerId(arquivo, id) || strcmp(id, "WAVE") != 0) {
        fclose(arquivo);
        return 0;
    }

    while (lerId(arquivo, id)) {
        unsigned int tamanho = lerU32(arquivo);

        if (strcmp(id, "fmt ") == 0) {
            formatoAudio = lerU16(arquivo);
            canais = lerU16(arquivo);
            taxa = lerU32(arquivo);
            lerU32(arquivo);
            lerU16(arquivo);
            bits = lerU16(arquivo);
            if (tamanho > 16) {
                pularBytes(arquivo, tamanho - 16);
            }
            encontrouFmt = 1;
        } else if (strcmp(id, "data") == 0) {
            dados = (unsigned char *)malloc((size_t)tamanho);
            if (dados == NULL) {
                fclose(arquivo);
                return 0;
            }
            if (fread(dados, 1, tamanho, arquivo) != tamanho) {
                free(dados);
                fclose(arquivo);
                return 0;
            }
            tamanhoDados = tamanho;
            encontrouData = 1;
        } else if (!pularBytes(arquivo, tamanho)) {
            free(dados);
            fclose(arquivo);
            return 0;
        }

        if (encontrouFmt && encontrouData) {
            break;
        }
    }

    fclose(arquivo);

    if (!encontrouFmt || !encontrouData || formatoAudio != 1) {
        free(dados);
        return 0;
    }

    formatoOpenAL = obterFormato(canais, bits);
    if (formatoOpenAL == 0) {
        free(dados);
        return 0;
    }

    alBufferData(buffer, formatoOpenAL, dados, (ALsizei)tamanhoDados, (ALsizei)taxa);
    free(dados);
    return alGetError() == AL_NO_ERROR;
}

static void configurarFonte(ALuint fonte, ALuint buffer, int repetir, float ganho)
{
    alSourcei(fonte, AL_BUFFER, (ALint)buffer);
    alSourcei(fonte, AL_LOOPING, repetir ? AL_TRUE : AL_FALSE);
    alSourcef(fonte, AL_GAIN, ganho);
}

void iniciarAudio(void)
{
    int i;

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

    for (i = 0; i < TOTAL_SONS; i++) {
        alGenBuffers(1, &sons[i].buffer);
        alGenSources(1, &sons[i].fonte);
        if (!carregarWav(sons[i].arquivo, sons[i].buffer)) {
            audioAtivo = 0;
            finalizarAudio();
            return;
        }
    }

    alGenBuffers(1, &bufferMusica);
    alGenSources(1, &fonteMusica);
    if (!carregarWav("assets/audio/musica.wav", bufferMusica)) {
        audioAtivo = 0;
        finalizarAudio();
        return;
    }

    configurarFonte(sons[SOM_PULO].fonte, sons[SOM_PULO].buffer, 0, 0.70f);
    configurarFonte(sons[SOM_COLISAO].fonte, sons[SOM_COLISAO].buffer, 0, 0.90f);
    configurarFonte(sons[SOM_SPLASH].fonte, sons[SOM_SPLASH].buffer, 0, 0.85f);
    configurarFonte(sons[SOM_VITORIA].fonte, sons[SOM_VITORIA].buffer, 0, 0.85f);
    configurarFonte(sons[SOM_DERROTA].fonte, sons[SOM_DERROTA].buffer, 0, 0.85f);
    configurarFonte(sons[SOM_PONTUACAO].fonte, sons[SOM_PONTUACAO].buffer, 0, 0.60f);
    configurarFonte(fonteMusica, bufferMusica, 1, 0.18f);

    audioAtivo = 1;
}

void finalizarAudio(void)
{
    int i;

    if (contextoAudio != NULL) {
        alSourceStop(fonteMusica);
        for (i = 0; i < TOTAL_SONS; i++) {
            if (sons[i].fonte != 0) {
                alDeleteSources(1, &sons[i].fonte);
                sons[i].fonte = 0;
            }
            if (sons[i].buffer != 0) {
                alDeleteBuffers(1, &sons[i].buffer);
                sons[i].buffer = 0;
            }
        }
        if (fonteMusica != 0) {
            alDeleteSources(1, &fonteMusica);
            fonteMusica = 0;
        }
        if (bufferMusica != 0) {
            alDeleteBuffers(1, &bufferMusica);
            bufferMusica = 0;
        }
        alcMakeContextCurrent(NULL);
        alcDestroyContext(contextoAudio);
        contextoAudio = NULL;
    }

    if (dispositivoAudio != NULL) {
        alcCloseDevice(dispositivoAudio);
        dispositivoAudio = NULL;
    }
    audioAtivo = 0;
}

void tocarSom(TipoSom tipo)
{
    if (!audioAtivo || tipo < 0 || tipo >= TOTAL_SONS) {
        return;
    }
    alSourceStop(sons[tipo].fonte);
    alSourcePlay(sons[tipo].fonte);
}

void pararSom(TipoSom tipo)
{
    if (!audioAtivo || tipo < 0 || tipo >= TOTAL_SONS) {
        return;
    }
    alSourceStop(sons[tipo].fonte);
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
