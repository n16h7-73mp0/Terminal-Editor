#ifndef GRAFICOS_H
#define GRAFICOS_H

#include <ncurses.h>
#include "editor.h"
#include "arquivos.h"

typedef struct {
	int y;
	int x;
} tamanho_janela;

//Variaveis globais
extern int cursor_y, cursor_x;
extern WINDOW* janela_editor;
extern WINDOW* barra_titulo;
extern WINDOW* barra_status;
extern tamanho_janela tamanho_stdscr;

//Inicia o ncurses
void inicia_ncurses(void);
//Renderiza a lista de linhas
void renderiza_buffer(Cabeca cabeca);
//Move o cursor na tela
void move_cursor(size_t y, size_t x);
//Atualiza a janela de status
void atualiza_barra_status(MODO modo);
//Finaliza o ncurses
void termina_ncurses(void);

#endif