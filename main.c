#include <GL/glut.h>
#include <ctype.h>
#include <stdlib.h>

#include "audio.h"
#include "captura_tela.h"
#include "cenario.h"
#include "colisao.h"
#include "configuracao.h"
#include "efeitos.h"
#include "frango.h"
#include "hud.h"
#include "jogo.h"
#include "menu.h"
#include "obstaculo.h"

Frango frangoJogador;
Frango frangoExtra;

#define PONTOS_POR_FAIXA 12
#define BONUS_VITORIA_BASE 360
#define PENALIDADE_BONUS_SEGUNDO 6
#define TEMPO_MAX_FRAME 0.05f
#define INTERVALO_TIMER_MS 16

static float offsetCamera = 0.0f;
static int faixaRecorde = 0;
static int tempoAnterior = 0;
static int tempoInicioPausa = 0;

void renderizar(void);
void redimensionar(int w, int h);
void processarTeclado(unsigned char tecla, int x, int y);
void processarTeclaEspecial(int tecla, int x, int y);
void processarMouse(int botao, int estado, int x, int y);
void processarMovimentoMouse(int x, int y);
void atualizarJogo(int valor);

static void configurarOpenGL(void)
{
    glClearColor(0.04f, 0.09f, 0.11f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(3.0f);
}

static void adicionarObstaculosExtrasAoLadoDoFrango(void)
{
    if (LADO_CARRO_EXTRA != 0) {
        adicionarObstaculoAoLadoDoFrango(frangoJogador.x, frangoJogador.y,
                                         TIPO_CARRO, LADO_CARRO_EXTRA);
    }
    if (LADO_TORA_EXTRA != 0) {
        adicionarObstaculoAoLadoDoFrango(frangoJogador.x, frangoJogador.y,
                                         TIPO_TORA, LADO_TORA_EXTRA);
    }
}

static void prepararPartidaCompleta(void)
{
    iniciarPartida();
    iniciarFrango(&frangoJogador);
    iniciarObstaculos();
    adicionarObstaculosExtrasAoLadoDoFrango();
    limparEfeitos();
    faixaRecorde = 0;
    offsetCamera = 0.0f;
    tempoAnterior = glutGet(GLUT_ELAPSED_TIME);
    tocarMusicaFundo();
}

static void atualizarOffsetCamera(void)
{
#if JOGO_LATERAL
    offsetCamera = frangoJogador.x - 0.20f;
#else
    offsetCamera = frangoJogador.y - 0.42f;
#endif
    if (offsetCamera < 0.0f) {
        offsetCamera = 0.0f;
    }
}

static void atualizarPontuacaoPorFaixa(void)
{
    if (frangoJogador.faixaAtual > faixaRecorde) {
        int diferenca = frangoJogador.faixaAtual - faixaRecorde;
        somarPontuacao(diferenca * PONTOS_POR_FAIXA);
        tocarSom(SOM_PONTUACAO);
        faixaRecorde = frangoJogador.faixaAtual;
    }
}

static void finalizarComVitoria(void)
{
    int segundos = jogo.tempoDecorrido / 1000;
    int bonus = BONUS_VITORIA_BASE - segundos * PENALIDADE_BONUS_SEGUNDO;

    if (bonus < 0) {
        bonus = 0;
    }
    somarPontuacao(bonus);
    salvarPontuacaoClassificacao();
    jogo.estadoAtual = ESTADO_VITORIA;
    tocarSom(SOM_VITORIA);
}

static void perderVida(TipoSom som)
{
    criarExplosaoParticulas(frangoJogador.x, frangoJogador.y);
    ativarFlashTela();
    tocarSom(som);
    frangoJogador.vidas--;

    if (verificarDerrota(&frangoJogador)) {
        salvarPontuacaoClassificacao();
        jogo.estadoAtual = ESTADO_DERROTA;
        tocarSom(SOM_DERROTA);
        return;
    }
    resetarFrango(&frangoJogador);
    offsetCamera = 0.0f;
}

static int frangoEstaSobreTora(float deltaTempo)
{
    int i;
    RetanguloColisao areaFrango;

    areaFrango.x = frangoJogador.x;
    areaFrango.y = frangoJogador.y;
    areaFrango.largura = TAMANHO_FRANGO * 1.6f;
    areaFrango.altura = TAMANHO_FRANGO * 1.6f;

    for (i = 0; i < totalObstaculos; i++) {
        RetanguloColisao areaTora;

        if (!obstaculos[i].ativo || obstaculos[i].tipo != TIPO_TORA) {
            continue;
        }
        areaTora.x = obstaculos[i].x;
        areaTora.y = obstaculos[i].y;
        areaTora.largura = obstaculos[i].largura;
        areaTora.altura = obstaculos[i].altura;

        if (verificarColisaoAABB(areaFrango, areaTora)) {
#if JOGO_LATERAL
            frangoJogador.y += obstaculos[i].velocidade * deltaTempo;
#else
            frangoJogador.x += obstaculos[i].velocidade * deltaTempo;
#endif
            return 1;
        }
    }
    return 0;
}

static void verificarEventosDeColisao(float deltaTempo)
{
    int resultado = verificarColisaoFrango(&frangoJogador,
                                           obstaculos, totalObstaculos);
    TipoFaixa faixa = obterTipoFaixa(frangoJogador.faixaAtual);

    if (resultado == COLISAO_CARRO) {
        perderVida(SOM_COLISAO);
        return;
    }

    if (faixa == FAIXA_RIO) {
        if (!frangoEstaSobreTora(deltaTempo)) {
            perderVida(SOM_SPLASH);
#if JOGO_LATERAL
        } else if (frangoJogador.y < Y_BAIXO_MUNDO ||
                   frangoJogador.y > Y_CIMA_MUNDO) {
            perderVida(SOM_SPLASH);
#else
        } else if (frangoJogador.x < -1.05f || frangoJogador.x > 1.05f) {
            perderVida(SOM_SPLASH);
#endif
        }
    }
}

static void executarMovimento(void (*movimento)(Frango *))
{
    if (jogo.estadoAtual != ESTADO_JOGANDO) {
        return;
    }
    movimento(&frangoJogador);
    registrarPosicaoTrilha(frangoJogador.x, frangoJogador.y);
    tocarSom(SOM_PULO);
    atualizarPontuacaoPorFaixa();
}

static void alternarPausa(void)
{
    int agora = glutGet(GLUT_ELAPSED_TIME);

    if (jogo.estadoAtual == ESTADO_JOGANDO) {
        jogo.estadoAtual = ESTADO_PAUSADO;
        tempoInicioPausa = agora;
    } else if (jogo.estadoAtual == ESTADO_PAUSADO) {
        jogo.estadoAtual = ESTADO_JOGANDO;
        jogo.tempoInicio += agora - tempoInicioPausa;
    }
}

void renderizar(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (jogo.estadoAtual == ESTADO_MENU) {
        desenharMenuInicial();
        desenharBotaoCaptura();
        glutSwapBuffers();
        return;
    }

    atualizarOffsetCamera();
    glPushMatrix();
#if JOGO_LATERAL
    glTranslatef(-offsetCamera, 0.0f, 0.0f);
#else
    glTranslatef(0.0f, -offsetCamera, 0.0f);
#endif
    desenharCenario(offsetCamera);
    desenharTrilha();
    desenharObstaculos();
    desenharFrango(&frangoJogador);
    desenharParticulas();
    glPopMatrix();

    desenharHUD();
    if (jogo.estadoAtual == ESTADO_PAUSADO) {
        desenharTelaPausa();
    } else if (jogo.estadoAtual == ESTADO_VITORIA) {
        desenharTelaVitoria(jogo.pontuacao, jogo.tempoDecorrido);
    } else if (jogo.estadoAtual == ESTADO_DERROTA) {
        desenharTelaDerrota(jogo.pontuacao);
    }
    desenharFlashTela();
    desenharBotaoCaptura();
    glutSwapBuffers();
}

void redimensionar(int w, int h)
{
    if (h <= 0) {
        h = 1;
    }
    jogo.larguraJanela = w;
    jogo.alturaJanela = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -0.15, 1.15, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void processarTeclado(unsigned char tecla, int x, int y)
{
    unsigned char normalizada = (unsigned char)tolower((int)tecla);
    (void)x;
    (void)y;

    if (tecla == 27) {
        finalizarAudio();
        exit(0);
    }
    if (normalizada == 'p') {
        alternarPausa();
        return;
    }
    if (normalizada == 'q') {
        salvarCapturaTelaDownloads();
        return;
    }
    if (normalizada == 'r') {
        prepararPartidaCompleta();
        return;
    }

    if (normalizada == 'w') {
#if JOGO_LATERAL
        executarMovimento(moverFrangoDireita);
#else
        executarMovimento(moverFrangoFrente);
#endif
    } else if (normalizada == 's') {
#if JOGO_LATERAL
        executarMovimento(moverFrangoEsquerda);
#else
        executarMovimento(moverFrangoTras);
#endif
    } else if (normalizada == 'a') {
#if JOGO_LATERAL
        executarMovimento(moverFrangoTras);
#else
        executarMovimento(moverFrangoEsquerda);
#endif
    } else if (normalizada == 'd') {
#if JOGO_LATERAL
        executarMovimento(moverFrangoFrente);
#else
        executarMovimento(moverFrangoDireita);
#endif
    }
}

void processarTeclaEspecial(int tecla, int x, int y)
{
    (void)x;
    (void)y;

    if (tecla == GLUT_KEY_UP) {
#if JOGO_LATERAL
        executarMovimento(moverFrangoDireita);
#else
        executarMovimento(moverFrangoFrente);
#endif
    } else if (tecla == GLUT_KEY_DOWN) {
#if JOGO_LATERAL
        executarMovimento(moverFrangoEsquerda);
#else
        executarMovimento(moverFrangoTras);
#endif
    } else if (tecla == GLUT_KEY_LEFT) {
#if JOGO_LATERAL
        executarMovimento(moverFrangoTras);
#else
        executarMovimento(moverFrangoEsquerda);
#endif
    } else if (tecla == GLUT_KEY_RIGHT) {
#if JOGO_LATERAL
        executarMovimento(moverFrangoFrente);
#else
        executarMovimento(moverFrangoDireita);
#endif
    }
}

void processarMouse(int botao, int estado, int x, int y)
{
    EstadoJogo estadoAnterior;

    if (botao != GLUT_LEFT_BUTTON || estado != GLUT_DOWN) {
        return;
    }

    if (clicouNoBotaoCaptura(x, y)) {
        salvarCapturaTelaDownloads();
        glutPostRedisplay();
        return;
    }

    estadoAnterior = jogo.estadoAtual;
    processarCliqueMouse(x, y);

    if ((estadoAnterior == ESTADO_MENU ||
         estadoAnterior == ESTADO_VITORIA ||
         estadoAnterior == ESTADO_DERROTA) &&
        jogo.estadoAtual == ESTADO_JOGANDO) {
        prepararPartidaCompleta();
    }

    if (estadoAnterior == ESTADO_PAUSADO && jogo.estadoAtual == ESTADO_JOGANDO) {
        jogo.tempoInicio += glutGet(GLUT_ELAPSED_TIME) - tempoInicioPausa;
    }
    glutPostRedisplay();
}

void processarMovimentoMouse(int x, int y)
{
    atualizarDestaqueMenu(x, y);
    glutPostRedisplay();
}

void atualizarJogo(int valor)
{
    int agora = glutGet(GLUT_ELAPSED_TIME);
    float deltaTempo;
    (void)valor;

    if (tempoAnterior == 0) {
        tempoAnterior = agora;
    }
    deltaTempo = (float)(agora - tempoAnterior) / 1000.0f;
    tempoAnterior = agora;
    if (deltaTempo > TEMPO_MAX_FRAME) {
        deltaTempo = TEMPO_MAX_FRAME;
    }

    if (jogo.estadoAtual == ESTADO_JOGANDO) {
        atualizarTempoJogo();
        atualizarObstaculos(deltaTempo);
        atualizarPontuacaoPorFaixa();
        verificarEventosDeColisao(deltaTempo);
        if (jogo.estadoAtual == ESTADO_JOGANDO &&
            verificarVitoria(&frangoJogador)) {
            finalizarComVitoria();
        }
    }

    atualizarParticulas(deltaTempo);
    glutPostRedisplay();
    glutTimerFunc(INTERVALO_TIMER_MS, atualizarJogo, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(900, 700);
    glutCreateWindow("Chicken Run");

    configurarOpenGL();
    iniciarJogo(900, 700);
    iniciarFrango(&frangoJogador);
    iniciarFrango(&frangoExtra);
    iniciarObstaculos();
    limparEfeitos();
    iniciarAudio();
    tocarMusicaFundo();

    frangoExtra.x = 0.25f;


    glutDisplayFunc(renderizar);
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(processarTeclado);
    glutSpecialFunc(processarTeclaEspecial);
    glutMouseFunc(processarMouse);
    glutPassiveMotionFunc(processarMovimentoMouse);
    glutTimerFunc(INTERVALO_TIMER_MS, atualizarJogo, 0);

    tempoAnterior = glutGet(GLUT_ELAPSED_TIME);
    atexit(finalizarAudio);
    glutMainLoop();
    return 0;
}
