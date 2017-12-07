#ifndef GRAFICOS_H
#define GRAFICOS_H

#include <ncurses.h>
#include "editor.h"

typedef struct {
	int y;
	int x;
} tamanho_janela;

//Inicia o ncurses
void inicia_ncurses(void);
void imprime_string(CARACTERE* cabeca);
void renderiza_buffer(LINHA* buff);


#endif