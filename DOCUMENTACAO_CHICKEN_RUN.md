# Documentacao do Projeto Chicken Run

## 1. Objetivo do projeto

O Chicken Run e um jogo 2D feito em linguagem C usando GLUT/OpenGL para a parte grafica e OpenAL para sons. A ideia principal e controlar uma galinha que precisa atravessar faixas de grama, pistas e rios ate chegar na linha de chegada.

O jogador perde vida quando bate em carros ou cai no rio sem estar em cima de uma tora. O jogador vence quando alcanca a faixa final. O jogo tambem possui menu inicial, pausa, pontuacao, relogio, sons, efeitos visuais e interacao por teclado e mouse.

## 2. Como compilar e executar

No Windows com MSYS2/UCRT64:

```powershell
cd C:\Users\Davi\Documents\project\ChickenRun
$env:PATH="C:\msys64\ucrt64\bin;$env:PATH"
mingw32-make
.\chicken_run.exe
```

O projeto usa o arquivo `Makefile`. No Windows ele compila com:

```makefile
gcc -Wall -Wextra -std=c99 -o chicken_run.exe main.c jogo.c frango.c obstaculo.c cenario.c colisao.c efeitos.c audio.c hud.c menu.c -lfreeglut -lglu32 -lopengl32 -lopenal -lm
```

Bibliotecas usadas:

- `freeglut`: cria janela, recebe eventos de teclado/mouse e controla callbacks.
- `OpenGL`: desenha o jogo com primitivas graficas.
- `GLU`: biblioteca auxiliar grafica usada no link.
- `OpenAL`: carrega e toca sons externos em formato WAV.
- `math`: usada em circulos, elipses e particulas.

## 3. Organizacao dos modulos

### `main.c`

E o centro do jogo. Registra os callbacks do GLUT, controla o loop principal, chama renderizacao, atualizacao, teclado, mouse, pontuacao, vitoria, derrota e pausa.

Funcoes importantes:

- `main`: inicializa GLUT, janela, jogo, frango, obstaculos, efeitos e audio.
- `renderizar`: limpa a tela e desenha menu, cenario, frango, obstaculos, HUD e telas finais.
- `atualizarJogo`: atualiza tempo, obstaculos, colisoes, efeitos e agenda o proximo frame.
- `processarTeclado`: trata WASD, setas, pausa, reinicio e ESC.
- `processarMouse`: trata cliques em botoes do menu, pausa e telas finais.
- `finalizarComVitoria`: aplica bonus de tempo, salva melhor pontuacao e muda para tela de vitoria.
- `perderVida`: cria efeitos, toca som, tira vida e verifica derrota.

Constantes importantes:

- `PONTOS_POR_FAIXA`: pontos ganhos ao avancar uma nova faixa.
- `BONUS_VITORIA_BASE`: bonus inicial ao vencer.
- `PENALIDADE_BONUS_SEGUNDO`: quanto o bonus diminui por segundo.
- `INTERVALO_TIMER_MS`: intervalo de atualizacao do jogo.

### `jogo.c` e `jogo.h`

Guardam o estado geral da partida.

O estado global `jogo` contem:

- estado atual: menu, jogando, pausado, vitoria ou derrota;
- pontuacao;
- melhor pontuacao;
- tempo inicial;
- tempo decorrido;
- largura e altura da janela.

Funcoes importantes:

- `iniciarJogo`: prepara o jogo pela primeira vez e inicia no menu.
- `iniciarPartida`: zera pontuacao e tempo e muda para estado jogando.
- `atualizarTempoJogo`: calcula quanto tempo passou.
- `somarPontuacao`: adiciona pontos.
- `salvarMelhorPontuacao`: atualiza o recorde.

### `frango.c` e `frango.h`

Controlam a galinha do jogador.

O struct `Frango` guarda:

- posicao `x` e `y`;
- vidas;
- faixa atual;
- se esta vivo;
- cor.

Funcoes importantes:

- `iniciarFrango`: define vidas, cor e posicao inicial.
- `desenharFrango`: desenha a galinha com elipses, circulos, triangulos, pontos e linhas.
- `moverFrangoFrente`, `moverFrangoTras`, `moverFrangoEsquerda`, `moverFrangoDireita`: movimentacao.
- `resetarFrango`: volta a galinha para o inicio sem reiniciar a partida inteira.

Constantes importantes:

- `MAX_VIDAS`: quantidade de vidas.
- `TAMANHO_FRANGO`: tamanho visual e base da colisao.
- `PASSO_FRANGO`: distancia de cada movimento.
- `LIMITE_X_FRANGO`: limite horizontal.
- `Y_INICIAL_FRANGO`: posicao inicial no eixo Y.

### `cenario.c` e `cenario.h`

Desenham o mapa e definem o tipo de cada faixa.

Tipos de faixa:

- `FAIXA_GRAMA`: local seguro.
- `FAIXA_PISTA`: local com carros.
- `FAIXA_RIO`: local com toras; se a galinha nao estiver em uma tora, perde vida.

Funcoes importantes:

- `obterTipoFaixa`: decide se a faixa e grama, pista ou rio.
- `obterYFaixa`: converte indice de faixa em coordenada Y.
- `desenharCenario`: desenha as faixas visiveis.
- `desenharGrama`, `desenharPista`, `desenharRio`: desenham cada tipo de faixa.
- `desenharLinhasDePista`: desenha as linhas amarelas da pista.

Melhoria visual feita:

- pista ganhou bordas;
- rio ganhou faixa escura;
- grama ganhou detalhe inferior;
- faixa final ganhou linha de chegada xadrez.

### `obstaculo.c` e `obstaculo.h`

Controlam carros e toras.

O struct `Obstaculo` guarda:

- posicao;
- largura e altura;
- velocidade;
- tipo;
- cor;
- se esta ativo.

Tipos:

- `TIPO_CARRO`: causa derrota de vida por colisao.
- `TIPO_TORA`: permite atravessar o rio e carrega a galinha horizontalmente.

Funcoes importantes:

- `iniciarObstaculos`: limpa e cria os obstaculos iniciais.
- `gerarFaixa`: cria carros ou toras conforme o tipo da faixa.
- `atualizarObstaculos`: move os obstaculos e faz eles reaparecerem no outro lado.
- `desenharObstaculos`: desenha carros e toras.

Constantes importantes:

- `MAX_OBSTACULOS`: limite do vetor de obstaculos.
- `META_VITORIA`: faixa que o jogador precisa atingir para vencer.
- `CARROS_POR_FAIXA`: quantidade de carros por pista.
- `TORAS_POR_FAIXA`: quantidade de toras por rio.
- `VELOCIDADE_PISTA`: velocidade base dos carros.
- `VELOCIDADE_RIO`: velocidade base das toras.
- `VARIACAO_VELOCIDADE`: variacao conforme a faixa.

Melhoria visual feita:

- carros ganharam farois, janela e detalhe central;
- toras ganharam brilho e linhas internas.

### `colisao.c` e `colisao.h`

Detectam colisao e condicoes finais.

Funcoes importantes:

- `verificarColisaoAABB`: testa colisao entre dois retangulos.
- `verificarColisaoFrango`: verifica se a galinha bateu em carro ou esta em tora.
- `verificarVitoria`: retorna verdadeiro quando a faixa atual chega na meta.
- `verificarDerrota`: retorna verdadeiro quando as vidas chegam a zero.

A tecnica usada e AABB, que significa colisao por caixas alinhadas aos eixos. Ela compara esquerda, direita, baixo e cima de dois retangulos. E simples, rapida e suficiente para um jogo 2D com objetos retangulares.

### `efeitos.c` e `efeitos.h`

Controlam os efeitos visuais.

Efeitos implementados:

- particulas quando a galinha perde vida;
- flash vermelho na tela quando ocorre dano;
- trilha amarela nos pulos/movimentos.

Funcoes importantes:

- `limparEfeitos`: zera particulas, trilha e flash.
- `criarExplosaoParticulas`: gera particulas ao redor da galinha.
- `atualizarParticulas`: move particulas, reduz vida delas e atualiza flash/trilha.
- `desenharParticulas`: desenha particulas.
- `ativarFlashTela`: liga o flash vermelho.
- `desenharFlashTela`: desenha a camada vermelha.
- `registrarPosicaoTrilha`: salva pontos da trilha.
- `desenharTrilha`: desenha os quadrados transparentes da trilha.

### `hud.c` e `hud.h`

Desenham informacoes na tela.

Elementos do HUD:

- pontos;
- tempo;
- vidas;
- melhor pontuacao;
- barra de progresso ate a chegada.

Funcoes importantes:

- `desenharTexto`: texto pequeno.
- `desenharTextoGrande`: texto maior.
- `desenharPontuacao`: mostra pontos.
- `desenharTimer`: mostra relogio no formato minuto:segundo.
- `desenharVidas`: mostra vidas.
- `desenharHUD`: desenha todos os elementos.

### `menu.c` e `menu.h`

Controlam o menu inicial, pausa, tela de vitoria e tela de derrota.

Funcoes importantes:

- `desenharMenuInicial`: tela inicial com botoes.
- `desenharTelaPausa`: overlay de pausa.
- `desenharTelaVitoria`: mostra vitoria, pontos e tempo.
- `desenharTelaDerrota`: mostra derrota e pontos.
- `verificarCliqueEmBotao`: confere se o mouse clicou em um botao.
- `processarCliqueMouse`: decide acao de cada botao conforme estado atual.
- `atualizarDestaqueMenu`: destaca botao quando o mouse passa por cima.

### `audio.c` e `audio.h`

Controlam sons e musica de fundo.

O audio agora usa arquivos externos em formato WAV. Isso deixa a explicacao mais simples: o codigo abre os arquivos da pasta `assets/audio`, carrega os dados no OpenAL e depois apenas toca cada som quando o jogo precisa.

Sons implementados:

- `assets/audio/pulo.wav`;
- `assets/audio/colisao.wav`;
- `assets/audio/splash.wav`;
- `assets/audio/vitoria.wav`;
- `assets/audio/derrota.wav`;
- `assets/audio/pontuacao.wav`;
- `assets/audio/musica.wav`.

Funcoes importantes:

- `iniciarAudio`: abre dispositivo OpenAL, cria buffers e fontes.
- `carregarWav`: le um arquivo WAV simples e coloca no buffer do OpenAL.
- `configurarFonte`: liga um buffer a uma fonte de som.
- `tocarSom`: toca um efeito sonoro.
- `tocarMusicaFundo`: toca musica em loop.
- `finalizarAudio`: libera recursos.

## 4. O que foi feito na limpeza e melhoria do projeto

### Remocao de comentarios do codigo

Todos os comentarios dos arquivos `.c` e `.h` foram removidos para atender ao pedido. A explicacao do projeto ficou concentrada neste arquivo de documentacao.

### Codigo mais limpo

Foram removidos itens desnecessarios, como valores de enum e constantes que nao eram usados. Tambem foram criadas constantes para parametros importantes, evitando numeros soltos em partes centrais do jogo.

Exemplos:

- pontuacao por faixa em `main.c`;
- bonus de vitoria em `main.c`;
- velocidade dos carros e toras em `obstaculo.c`;
- quantidade de particulas em `efeitos.h`.

### Melhorias visuais

As melhorias foram feitas sem criar uma arquitetura nova:

- a galinha usa formas mais variadas;
- carros e toras ganharam detalhes;
- o cenario ganhou mais contraste;
- o menu ficou mais apresentado;
- o HUD ganhou fundo e barra de progresso;
- os efeitos ficaram mais visiveis.

### Ajustes de gameplay

Foram ajustados:

- pontos por faixa: `12`;
- bonus de vitoria: `360` menos `6` por segundo;
- velocidade base dos carros: `0.34`;
- velocidade base das toras: `0.24`;
- variacao de velocidade: `0.035`;
- particulas: `120`;
- trilha: `28` pontos.

## 5. Como cada requisito foi atendido

### 1. Desenhar com a biblioteca no minimo 5 figuras primarias

O jogo usa primitivas do OpenGL em varios arquivos:

- `GL_QUADS`: retangulos do cenario, HUD, menu, carros, toras, particulas e trilha.
- `GL_TRIANGLES`: bico, crista, pe da galinha, detalhes do menu e janela dos carros.
- `GL_TRIANGLE_FAN`: circulos e elipses da galinha.
- `GL_LINES`: pernas da galinha, linhas da pista, ondas do rio e detalhes das toras.
- `GL_POINTS`: olhos, rodas e detalhes da grama.

Com isso o requisito e atendido com mais de 5 usos distribuidos no projeto.

### 2. No minimo 3 efeitos

Os efeitos estao no modulo `efeitos.c`:

- explosao de particulas ao perder vida;
- flash vermelho ao tomar dano;
- trilha amarela no caminho da galinha.

Eles sao ativados em `main.c`. Quando a galinha perde vida, `perderVida` chama `criarExplosaoParticulas` e `ativarFlashTela`. Quando ela se move, `executarMovimento` chama `registrarPosicaoTrilha`.

### 3. Relogio contador

O relogio e controlado pelo estado do jogo:

- `jogo.tempoInicio` guarda o momento em que a partida comecou.
- `atualizarTempoJogo` calcula `tempoDecorrido`.
- `desenharTimer` mostra o tempo no HUD.

Quando o jogo pausa, o tempo de pausa e compensado para nao contar indevidamente.

### 4. Sistema de pontuacao

A pontuacao fica em `jogo.pontuacao`.

Ela aumenta de duas formas:

- ao avancar faixas novas: `PONTOS_POR_FAIXA`;
- ao vencer: bonus baseado no tempo.

O jogo tambem guarda `melhorPontuacao`.

### 5. Interatividade

O jogador interage com:

- teclado para mover, pausar, reiniciar e sair;
- mouse para clicar em botoes;
- movimento do mouse para destacar botoes.

As principais funcoes sao `processarTeclado`, `processarTeclaEspecial`, `processarMouse` e `processarMovimentoMouse`.

### 6. Acao com teclado e acao com mouse

Teclado:

- `W` ou seta para cima: anda para frente.
- `S` ou seta para baixo: anda para tras.
- `A` ou seta para esquerda: anda para esquerda.
- `D` ou seta para direita: anda para direita.
- `P`: pausa ou continua.
- `R`: reinicia partida.
- `ESC`: sai do jogo.

Mouse:

- clique em `INICIAR`;
- clique em `SAIR`;
- clique em `CONTINUAR`;
- clique em `MENU`;
- clique em `REINICIAR`;
- destaque visual ao passar por cima dos botoes.

### 7. Uso de linguagem C e biblioteca GLUT

Todo o projeto esta escrito em C. O GLUT e usado para:

- criar a janela;
- registrar funcao de desenho;
- receber teclado;
- receber mouse;
- controlar timer;
- desenhar texto bitmap.

Exemplos:

- `glutInit`;
- `glutCreateWindow`;
- `glutDisplayFunc`;
- `glutKeyboardFunc`;
- `glutMouseFunc`;
- `glutTimerFunc`;
- `glutBitmapCharacter`.

### 8. Condicao de vitoria e derrota

Vitoria:

- acontece quando `frangoJogador.faixaAtual >= META_VITORIA`.
- a verificacao fica em `verificarVitoria`.
- a conclusao fica em `finalizarComVitoria`.

Derrota:

- acontece quando `frangoJogador.vidas <= 0`.
- a verificacao fica em `verificarDerrota`.
- a reducao de vida acontece em `perderVida`.

### 9. Menu inicial

O jogo comeca em `ESTADO_MENU`, definido em `iniciarJogo`.

O menu e desenhado em `desenharMenuInicial` e os cliques sao tratados em `processarCliqueMouse`.

### 10. Pausa

A pausa fica no estado `ESTADO_PAUSADO`.

Ela pode ser acionada pelo teclado com `P` ou pelo fluxo de menu. A tela e desenhada em `desenharTelaPausa`.

Quando o jogador continua, o tempo parado e descontado de `jogo.tempoInicio`, evitando que o relogio avance durante a pausa.

### 11. Uso de funcoes e bibliotecas graficas

O projeto usa funcoes graficas do OpenGL e GLUT.

OpenGL:

- `glBegin`;
- `glEnd`;
- `glVertex2f`;
- `glColor3f`;
- `glColor4f`;
- `glClear`;
- `glOrtho`;
- `glTranslatef`;
- `glPushMatrix`;
- `glPopMatrix`.

GLUT:

- janela;
- callbacks;
- timer;
- teclado;
- mouse;
- textos bitmap.

Alem disso, OpenAL foi usado como biblioteca extra para audio.

### 12. Audios e sons

O modulo `audio.c` usa OpenAL para carregar arquivos WAV externos da pasta `assets/audio`.

Sons:

- pulo;
- colisao;
- splash;
- vitoria;
- derrota;
- pontuacao;
- musica de fundo.

Essa versao e mais facil de explicar porque os sons ficam separados do codigo. Para trocar um som, basta substituir o arquivo WAV correspondente mantendo o mesmo nome.

### 13. Deteccao de colisao

A colisao usa AABB.

Fluxo:

1. O frango vira um retangulo de colisao.
2. Cada obstaculo vira outro retangulo.
3. `verificarColisaoAABB` compara as bordas.
4. Se encostar em carro, perde vida.
5. Se estiver no rio sem tora, perde vida.
6. Se estiver em uma tora, a tora carrega a galinha.

## 6. Fluxo principal do jogo

1. `main` inicia o GLUT, a janela e os sistemas do jogo.
2. O jogo comeca no menu.
3. O jogador clica em iniciar.
4. `prepararPartidaCompleta` reinicia partida, frango, obstaculos e efeitos.
5. O timer chama `atualizarJogo` a cada 16 ms.
6. `atualizarJogo` move obstaculos, atualiza tempo, verifica colisao e vitoria.
7. `renderizar` desenha o estado atual.
8. Teclado e mouse alteram o estado conforme as acoes do jogador.

## 7. Perguntas que a professora pode fazer

### 1. Qual e o objetivo do jogo?

Controlar a galinha para atravessar pistas e rios ate a linha de chegada. O jogador deve evitar carros e usar toras para atravessar rios.

### 2. Quais bibliotecas foram usadas?

Foram usadas GLUT/freeglut e OpenGL para graficos, OpenAL para audio e `math` para calculos de ondas, circulos e particulas.

### 3. Onde esta o loop principal do jogo?

O loop e controlado pelo GLUT. A funcao `glutMainLoop` inicia o loop. A atualizacao acontece em `atualizarJogo`, chamada repetidamente por `glutTimerFunc`.

### 4. Como o jogo sabe quando redesenhar a tela?

`atualizarJogo` chama `glutPostRedisplay`, pedindo ao GLUT para executar `renderizar`.

### 5. Como a galinha se move?

O teclado chama funcoes de movimento em `frango.c`: frente, tras, esquerda e direita. Cada movimento altera `x`, `y` ou `faixaAtual`.

### 6. Como funciona a pontuacao?

Quando a galinha chega a uma faixa mais avancada que o recorde da partida, ganha `PONTOS_POR_FAIXA`. Ao vencer, ganha um bonus baseado no tempo.

### 7. Como funciona o relogio?

O jogo salva o tempo inicial com `glutGet(GLUT_ELAPSED_TIME)` e calcula a diferenca em `atualizarTempoJogo`.

### 8. Como a pausa evita que o tempo continue contando?

Quando pausa, o jogo salva o momento da pausa. Ao continuar, soma o tempo pausado em `jogo.tempoInicio`, compensando a parada.

### 9. Como a colisao e detectada?

Com AABB. O frango e o obstaculo sao tratados como retangulos e o codigo verifica se eles se sobrepoem.

### 10. Por que usar AABB?

Porque e simples, rapido e suficiente para esse jogo 2D, onde carros, toras e galinha podem ser aproximados por retangulos.

### 11. Como o rio funciona?

Se a faixa atual for rio, o jogo verifica se a galinha esta sobre uma tora. Se nao estiver, perde vida. Se estiver, a tora move a galinha junto.

### 12. Onde estao os efeitos?

No modulo `efeitos.c`. Ele controla particulas, flash vermelho e trilha da galinha.

### 13. Como os sons sao carregados?

Eles ficam em arquivos WAV na pasta `assets/audio`. O `audio.c` abre cada arquivo, le o formato WAV e entrega os dados para o OpenAL tocar.

### 14. Como o menu funciona?

O menu desenha botoes e converte a posicao do mouse para coordenadas do jogo. Depois verifica se o clique caiu dentro de algum botao.

### 15. Como o jogo separa responsabilidades?

Cada modulo cuida de uma parte: frango, cenario, obstaculos, colisao, efeitos, audio, HUD, menu e estado geral. O `main.c` apenas coordena tudo.

## 8. Alteracoes que a professora pode pedir e como resolver

### 1. Mudar o formato da galinha

Arquivo principal: `frango.c`

Funcao: `desenharFrango`

Como resolver:

- Para deixar a galinha quadrada, substitua o corpo desenhado com `desenharElipse` por um bloco `GL_QUADS`.
- Para deixar mais redonda, aumente `raioX` e `raioY` na chamada de `desenharElipse`.
- Para deixar triangular, use `GL_TRIANGLES` no corpo.

Exemplo de corpo quadrado:

```c
glBegin(GL_QUADS);
glVertex2f(f->x - t, f->y - t);
glVertex2f(f->x + t, f->y - t);
glVertex2f(f->x + t, f->y + t);
glVertex2f(f->x - t, f->y + t);
glEnd();
```

### 2. Aumentar ou diminuir a quantidade de vidas

Arquivo: `frango.h`

Constante:

```c
#define MAX_VIDAS 3
```

Como resolver:

- Para 5 vidas, trocar para:

```c
#define MAX_VIDAS 5
```

Depois recompilar.

### 3. Deixar os carros mais rapidos

Arquivo: `obstaculo.c`

Constantes:

```c
#define VELOCIDADE_PISTA 0.34f
#define VARIACAO_VELOCIDADE 0.035f
```

Como resolver:

- Aumentar `VELOCIDADE_PISTA` para algo como `0.45f`.
- Se quiser que as faixas mais avancadas escalem mais, aumentar `VARIACAO_VELOCIDADE`.

### 4. Deixar o jogo mais facil no rio

Arquivo: `obstaculo.c`

Opcoes:

- aumentar `TORAS_POR_FAIXA`;
- aumentar `o.largura` quando `faixa == FAIXA_RIO`;
- diminuir `VELOCIDADE_RIO`.

Exemplo:

```c
#define VELOCIDADE_RIO 0.18f
```

Ou:

```c
o.largura = 0.42f;
```

### 5. Mudar a pontuacao por faixa

Arquivo: `main.c`

Constante:

```c
#define PONTOS_POR_FAIXA 12
```

Como resolver:

- Para ganhar 20 pontos por faixa:

```c
#define PONTOS_POR_FAIXA 20
```

### 6. Mudar a meta de vitoria

Arquivo: `obstaculo.h`

Constante:

```c
#define META_VITORIA 10
```

Como resolver:

- Para vencer na faixa 15:

```c
#define META_VITORIA 15
```

Importante: como o cenario e obstaculos usam essa constante, a mudanca se espalha corretamente para o mapa, a linha de chegada e a verificacao de vitoria.

### 7. Mudar as teclas de movimento

Arquivo: `main.c`

Funcao: `processarTeclado`

Como resolver:

- Trocar as letras nos `if`.
- Exemplo: se quiser usar `i`, `j`, `k`, `l`, trocar:

```c
if (normalizada == 'w')
```

por:

```c
if (normalizada == 'i')
```

E repetir para as outras direcoes.

### 8. Mudar texto ou botoes do menu

Arquivo: `menu.c`

Textos dos botoes:

```c
static BotaoMenu botaoIniciar = {0.0f, 0.42f, 0.58f, 0.14f, "INICIAR", 0};
```

Como resolver:

- Alterar o texto entre aspas.
- Se o texto ficar maior, aumentar a largura do botao.

Exemplo:

```c
static BotaoMenu botaoIniciar = {0.0f, 0.42f, 0.70f, 0.14f, "JOGAR AGORA", 0};
```

### 9. Mudar sons

Pasta: `assets/audio`

Arquivos:

- `pulo.wav`;
- `colisao.wav`;
- `splash.wav`;
- `vitoria.wav`;
- `derrota.wav`;
- `pontuacao.wav`;
- `musica.wav`.

Como resolver:

- Grave ou exporte um novo arquivo WAV.
- Coloque na pasta `assets/audio`.
- Mantenha exatamente o mesmo nome do arquivo que voce quer substituir.
- Recompile se tambem tiver alterado codigo; se apenas trocou o WAV, basta abrir o jogo novamente.

Exemplo: para mudar o som do pulo, substitua `assets/audio/pulo.wav` por outro arquivo chamado `pulo.wav`.

Se quiser mudar o volume de um som, altere o ultimo parametro da chamada `configurarFonte` em `audio.c`.

### 10. Mudar intensidade dos efeitos

Arquivos: `efeitos.h` e `efeitos.c`

Opcoes:

- aumentar `MAX_PARTICULAS` para mais particulas;
- alterar `tempoFlash` para flash mais forte;
- alterar `MAX_TRILHA` para trilha mais longa;
- alterar a queda de `vida` das particulas para elas sumirem mais rapido ou mais devagar.

Exemplo:

```c
#define MAX_PARTICULAS 160
```

Ou, para particulas durarem mais:

```c
particulas[i].vida -= deltaTempo * 1.0f;
```

## 9. Como lidar com alteracoes em qualquer modulo

### Passo 1. Identificar o tipo de alteracao

Antes de mudar codigo, classifique a alteracao:

- visual da galinha: `frango.c`;
- visual do cenario: `cenario.c`;
- obstaculos, velocidade e dificuldade: `obstaculo.c`;
- colisao: `colisao.c`;
- pontos, pausa, vitoria, derrota e loop: `main.c` e `jogo.c`;
- HUD: `hud.c`;
- menu e botoes: `menu.c`;
- efeitos: `efeitos.c`;
- sons: `audio.c`.

### Passo 2. Procurar constantes antes de mexer em logica

Muitas alteracoes podem ser feitas trocando constantes.

Exemplos:

- vidas: `MAX_VIDAS`;
- tamanho da galinha: `TAMANHO_FRANGO`;
- passo de movimento: `PASSO_FRANGO`;
- meta de vitoria: `META_VITORIA`;
- velocidade dos carros: `VELOCIDADE_PISTA`;
- velocidade das toras: `VELOCIDADE_RIO`;
- pontos por faixa: `PONTOS_POR_FAIXA`.

Essa e a forma mais segura durante apresentacao.

### Passo 3. Se mexer em uma funcao publica, atualizar o `.h`

Se voce criar uma funcao nova que sera usada por outro arquivo, precisa declarar no `.h`.

Exemplo:

- criou funcao em `frango.c`;
- vai chamar em `main.c`;
- entao declare em `frango.h`.

Se a funcao so sera usada dentro do proprio arquivo, deixe `static` e nao coloque no `.h`.

### Passo 4. Manter a separacao dos modulos

Evite colocar tudo em `main.c`.

Regra pratica:

- desenho da galinha fica em `frango.c`;
- desenho do mundo fica em `cenario.c`;
- regras de colisao ficam em `colisao.c`;
- botoes ficam em `menu.c`;
- sons ficam em `audio.c`.

Isso facilita explicar para a professora e reduz erro ao alterar algo.

### Passo 5. Compilar logo apos cada alteracao

Use:

```powershell
$env:PATH="C:\msys64\ucrt64\bin;$env:PATH"
mingw32-make
```

Se aparecer erro, leia:

- nome do arquivo;
- numero da linha;
- mensagem principal.

Corrija uma coisa por vez.

### Passo 6. Testar pelo checklist rapido

Depois de qualquer alteracao, teste:

- abre o jogo;
- menu responde ao mouse;
- galinha anda com teclado;
- pausa funciona;
- carros colidem;
- rio causa splash se nao estiver em tora;
- pontos sobem;
- tempo conta;
- vitoria aparece;
- derrota aparece;
- som toca.

### Passo 7. Evitar mudancas grandes durante a apresentacao

Durante apresentacao, prefira mudancas em:

- constantes;
- cores;
- tamanhos;
- textos;
- velocidades;
- quantidades.

Evite alterar:

- assinatura de funcoes;
- estrutura dos structs;
- sistema de estados;
- callbacks do GLUT;
- inicializacao do OpenAL.

## 10. Mapa rapido: pedido da professora x onde mexer

| Pedido | Arquivo | Onde mexer |
|---|---|---|
| Mudar forma da galinha | `frango.c` | `desenharFrango` |
| Mudar tamanho da galinha | `frango.h` | `TAMANHO_FRANGO` |
| Mudar quantidade de vidas | `frango.h` | `MAX_VIDAS` |
| Mudar velocidade dos carros | `obstaculo.c` | `VELOCIDADE_PISTA` |
| Mudar velocidade das toras | `obstaculo.c` | `VELOCIDADE_RIO` |
| Mudar quantidade de carros | `obstaculo.c` | `CARROS_POR_FAIXA` |
| Mudar quantidade de toras | `obstaculo.c` | `TORAS_POR_FAIXA` |
| Mudar pontos | `main.c` | `PONTOS_POR_FAIXA` |
| Mudar bonus | `main.c` | `BONUS_VITORIA_BASE` |
| Mudar meta final | `obstaculo.h` | `META_VITORIA` |
| Mudar controles | `main.c` | `processarTeclado` |
| Mudar menu | `menu.c` | botoes e funcoes de desenho |
| Mudar HUD | `hud.c` | `desenharHUD` |
| Mudar colisao | `colisao.c` | `criarRetanguloFrango`, `criarRetanguloObstaculo` |
| Mudar sons | `assets/audio` | substituir arquivos `.wav` |
| Mudar particulas | `efeitos.c` e `efeitos.h` | `MAX_PARTICULAS`, `criarExplosaoParticulas` |

## 11. Explicacao curta para apresentar

Este jogo foi feito em C com GLUT e OpenGL. O GLUT cria a janela e chama funcoes de teclado, mouse, desenho e timer. O OpenGL desenha o cenario usando primitivas como quadrados, triangulos, linhas, pontos e circulos aproximados por `GL_TRIANGLE_FAN`. O jogo tem estados: menu, jogando, pausado, vitoria e derrota. A galinha se move por faixas; cada faixa avancada aumenta a pontuacao. Carros e toras sao obstaculos que se movem horizontalmente. A colisao usa AABB, comparando retangulos. Se a galinha bate em carro ou cai no rio sem tora, perde vida. Se chega na meta, vence. O jogo tambem tem HUD, relogio, sons em OpenAL e efeitos visuais como particulas, flash e trilha.

## 12. Ordem recomendada para demonstrar o jogo

1. Abrir o jogo pelo executavel.
2. Mostrar o menu inicial.
3. Clicar em iniciar com o mouse.
4. Mover com WASD ou setas.
5. Mostrar a pontuacao aumentando ao avancar.
6. Mostrar o relogio no HUD.
7. Pressionar `P` para pausar.
8. Continuar pelo menu de pausa.
9. Bater em um carro para mostrar colisao, som, flash e particulas.
10. Cair no rio sem tora para mostrar splash.
11. Subir em uma tora para mostrar que ela carrega a galinha.
12. Chegar na linha final para mostrar vitoria.
13. Reiniciar e demonstrar derrota perdendo todas as vidas.

## 13. Cuidados antes da apresentacao

- Verificar se `C:\msys64\ucrt64\bin` esta no `PATH`.
- Compilar com `mingw32-make`.
- Testar se a janela abre.
- Testar se o som toca.
- Deixar o terminal aberto na pasta do projeto.
- Saber onde estao as constantes principais.
- Fazer alteracoes pequenas e recompilar depois de cada uma.
