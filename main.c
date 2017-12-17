#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <locale.h>

#include "arquivos.h"
#include "editor.h"
#include "graficos.h"

#define KEY_NOVA_LINHA 10
#define KEY_INSERE 23
#define KEY_APAGA 127

//Variaveis de graficos.h
int limte_superior_tela;
int cursor_y, cursor_x;
WINDOW* janela_editor;
WINDOW* barra_titulo;
WINDOW* barra_status;
tamanho_janela tamanho_stdscr;

MODO modo;

PROPRIEDADES_EDITOR editor_prop;

DESCRITOR_ARQUIVO desc_arq;
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");
	
	//Verifica os argumentos
	//Se somente 1 arg (nome do executavel)
	if(argc == 1)
	{
		printf("Você precisa passar o nome do arquivo como argumento.\n");
		return(-1);
	}
	//Se mais de dois args (nome do exec + nome do arquivo)
	else if(argc > 2)
	{
		printf("Você passou muitos argumentos. Passe somente o nome do arquivo.\n");
		return(-1);
	}else
	{
		//Ponteiro para o primeiro item da lista que armazena as linhas
		LINHA* cabeca = NULL;
		//bem intuitivo
		strcpy(desc_arq.nome, argv[1]);
		
		//Se conseguir, com sucesso passar o arquivo para a memória
		if(cria_buffer_arquivo(&cabeca))
		{
			//Inicia o ncruses, cria as janelas
			inicia_ncurses();
			//Inicia no modo OCIOSO
			modo = OCIOSO;
			
			//Imprime o nome do arquivo no meio da barra de titulos
			mvwprintw(barra_titulo, 1, (tamanho_stdscr.x - strlen(desc_arq.nome))/2, "%s", desc_arq.nome);
			wrefresh(barra_titulo);
			//renderiza o buffer na tela.
			renderiza_buffer(cabeca);
			wmove(janela_editor,0,0);

			//Some com o cursor
			curs_set(0);
			wrefresh(janela_editor);
		}
		else
		{
			printf("Houve um erro ao passar o arquivo para o buffer.\n");
			return(-1);
		}
			

		//Loop principal
		for(;;)
		{
			//pega a posicao do cursor na janela de edição
			getyx(janela_editor, cursor_y, cursor_x);
			//variavel que vai armazenar a tecla pressionada pelo usuário
			wchar_t ch = getch();

			//Sequência de if's para processar o que o usuário digitou
			//teclas direcionais
			if(ch == KEY_DOWN && modo == INSERCAO)
				if(ultima_linha(cabeca, cursor_y))
					continue;
				else
				{
					//int chrs_atual = tamanho_linha(cabeca, cursor_y);
					int chrs_prox = tamanho_linha(cabeca, cursor_y + 1);
					if(cursor_x > chrs_prox)
						move_cursor(cursor_y + 1, cursor_x - (cursor_x - chrs_prox));
					else
						move_cursor(cursor_y + 1, cursor_x);
				}
					
			else if(ch == KEY_UP && modo == INSERCAO)
				move_cursor(cursor_y - 1, cursor_x);
			else if(ch == KEY_LEFT && modo == INSERCAO)
				move_cursor(cursor_y, cursor_x - 1);
			else if(ch == KEY_RIGHT && modo == INSERCAO)
				if(ultimo_caractere(cabeca, cursor_y, cursor_x))
					continue;
				else
					move_cursor(cursor_y, cursor_x + 1);
			//Tecla INSERT
			else if(ch == KEY_IC)
				percorre_modo(&modo);
			//BACKSPACE
			else if(ch == KEY_APAGA && modo == INSERCAO)
			{
				//Remove caractere e atualiza a janela
				remove_caractere(&cabeca, cursor_y, cursor_x);
				renderiza_buffer(cabeca);
				//Se o cursor estava na lateral esquerda deleta o '\n' da linha anterior
				//e coloca a linha atual no final da anterior, então o cursor precisa subir
				if(cursor_x == 0)
					move_cursor(cursor_y-1, cursor_x);
				//Senão o cursor só precisa ir para trás
				else
					move_cursor(cursor_y, cursor_x - 1);	
			}
			//CTRL + W
			else if(ch == KEY_INSERE)
				//Salva o buffer no arquivo
				salva_buffer_arquivo(&cabeca);	
			//ENTER
			else if(modo == INSERCAO && ch == KEY_NOVA_LINHA)
			{
				quebra_linha(&cabeca,cursor_y, cursor_x);
				move_cursor(cursor_y + 1, 0);
				renderiza_buffer(cabeca);
			}
			//Qualquer outra tecla
			else if(modo == INSERCAO)
			//Se não é nenhum dos caracteres de controle, assume que é um caractere que vai fazer
			//parte do texto
			{
				insere_caractere_meio(&cabeca, cursor_y, cursor_x, ch);
				move_cursor(cursor_y, cursor_x + 1);
				renderiza_buffer(cabeca);
			}
			
			//Atualiza algumas janelas
			atualiza_barra_status(desc_arq, modo);
			wrefresh(janela_editor);
		}

		//Limpa as coisas
		refresh();
		getch();
		delwin(barra_titulo);
		delwin(barra_status);
		endwin();	
	}
	return(0);
}