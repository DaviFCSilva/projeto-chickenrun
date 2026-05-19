CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -DGLUT_DISABLE_ATEXIT_HACK -DGLUT_NO_LIB_PRAGMA

ifeq ($(OS),Windows_NT)
TARGET = chicken_run.exe
CFLAGS += -I.
LIBS = -lfreeglut -lglu32 -lopengl32 -lopenal -lm
else
TARGET = chicken_run
LIBS = -lglut -lGLU -lGL -lopenal -lm
endif

SRCS = main.c jogo.c frango.c obstaculo.c cenario.c \
       colisao.c efeitos.c audio.c hud.c menu.c captura_tela.c visual.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	$(RM) $(TARGET)
