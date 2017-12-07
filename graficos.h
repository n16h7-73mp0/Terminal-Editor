#ifndef GRAFICOS_H
#define GRAFICOS_H

#include <ncurses.h>
#include "editor.h"
#include "arquivos.h"

typedef struct {
	int y;
	int x;
} tamanho_janela;

extern int limte_superior_tela;
extern int cursor_y, cursor_x;
extern WINDOW* janela_editor;
extern WINDOW* barra_titulo;
extern WINDOW* barra_status;
extern tamanho_janela tamanho_stdscr;

//Inicia o ncurses
void inicia_ncurses(void);
void imprime_string(CARACTERE* cabeca);
void renderiza_buffer(LINHA* buff);
//Move o cursor na tela, leva em consideração os limites
void move_cursor(size_t y, size_t x);
void atualiza_barra_status(DESCRITOR_ARQUIVO desc_arq, MODO modo);


#endif