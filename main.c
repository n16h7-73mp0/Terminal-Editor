#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <locale.h>

#include "arquivos.h"
#include "editor.h"
#include "graficos.h"

#define KEY_WRITE 23

//Variaveis de graficos.h
int limte_superior_tela;
int cursor_y, cursor_x;
WINDOW* janela_editor;
WINDOW* barra_titulo;
WINDOW* barra_status;
tamanho_janela tamanho_stdscr;

MODO modo;

PROPRIEDADES_EDITOR editor_prop;
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");
	inicia_ncurses();

	DESCRITOR_ARQUIVO desc_arq;
	LINHA* cabeca = NULL;
	
	if(cria_buffer_arquivo(&cabeca, "teste.txt", &desc_arq))
	{
		modo = COMANDO;
		
		mvwprintw(barra_titulo, 1, (tamanho_stdscr.x - strlen(desc_arq.nome))/2, "%s", desc_arq.nome);
		mvwprintw(barra_titulo, 1, 1, "%d", editor_prop.linhas);
		wrefresh(barra_titulo);
		renderiza_buffer(cabeca);
		wmove(janela_editor,0,0);
		curs_set(0);
		wrefresh(janela_editor);
	}
	else
		printw("Não funcionou");


	//Loop principal
	for(;;)
	{
		getyx(janela_editor, cursor_y, cursor_x);
		wchar_t ch = getch();

		if(ch == KEY_DOWN && (modo == INSERCAO || modo == SOBRESCRICAO) && !ultima_linha(cabeca, cursor_y))
			move_cursor(cursor_y + 1, cursor_x);
		else if(ch == KEY_UP && (modo == INSERCAO || modo == SOBRESCRICAO))
			move_cursor(cursor_y - 1, cursor_x);
		else if(ch == KEY_LEFT && (modo == INSERCAO || modo == SOBRESCRICAO))
			move_cursor(cursor_y, cursor_x - 1);
		else if(ch == KEY_RIGHT && (modo == INSERCAO || modo == SOBRESCRICAO))
			move_cursor(cursor_y, cursor_x + 1);
		else if(ch == KEY_IC)
			percorre_modo(&modo);
		else if(ch == KEY_BACKSPACE && modo == INSERCAO)
		{
			if(cursor_x == 0)
			{
				concatena_linha_anterior(&cabeca, cursor_y);
				renderiza_buffer(cabeca);
				if(cursor_y != 0)
					move_cursor(cursor_y - 1, cursor_x);
			}else
			{
				remove_caractere(&cabeca, cursor_y, cursor_x);
				renderiza_buffer(cabeca);
				move_cursor(cursor_y, cursor_x - 1);	
			}
		}
		else if(ch == KEY_WRITE)
		{
			salva_buffer_arquivo(&cabeca, "teste.txt");	
		}
		else //Se não é nenhum dos caracteres de controle, assume que é um caractere que vai fazer
		//parte do texto
		{
			insere_caractere_meio(&cabeca, cursor_y, cursor_x, ch);
			renderiza_buffer(cabeca);
		}
		//mvwprintw(janela_editor,30,0, "%d", ch);

		atualiza_barra_status(desc_arq, modo);
		wrefresh(janela_editor);
	}

	refresh();
	getch();
	delwin(barra_titulo);
	delwin(barra_status);
	endwin();
	//fclose(arquivo);

	return(0);
}