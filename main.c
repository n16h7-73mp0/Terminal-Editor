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
#define KEY_SAI 00

int cursor_y, cursor_x;
WINDOW* janela_editor;
WINDOW* barra_titulo;
WINDOW* barra_status;
tamanho_janela tamanho_stdscr;
MODO modo;
char* nome_arquivo;

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
		Cabeca cabeca = NULL;
		//bem intuitivo
		nome_arquivo = (char*)malloc(255);
		strcpy(nome_arquivo, argv[1]);
		
		//Se conseguir, com sucesso passar o arquivo para a memória
		if(cria_buffer_arquivo(&cabeca))
		{
			//O arquivo começa, obviamente, sem modificações
			modificado = 0;
			//Inicia o ncruses, cria as janelas
			inicia_ncurses();
			//Inicia no modo OCIOSO
			modo = OCIOSO;
			
			//Imprime o nome do arquivo no meio da barra de titulos
			mvwprintw(barra_titulo, 1, (tamanho_stdscr.x - strlen(nome_arquivo))/2, "%s", nome_arquivo);
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
				//Se for a última linha, não desce
				if(ultima_linha(cabeca, cursor_y))
					continue;
				else
				{
					//Quando for descer, a posição do cursor for maior que 
					//a quantidade de caracteres da linha de baixo
					//desce pro ultimo caractere da linha
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
				//Se for o ultimo caractere não vai para direita
				if(ultimo_caractere(cabeca, cursor_y, cursor_x))
					continue;
				else
					move_cursor(cursor_y, cursor_x + 1);
			//Tecla INSERT
			else if(ch == KEY_IC)
				//Altera o modo do editor
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
			{
				//Salva o buffer no arquivo
				if(salva_buffer_arquivo(&cabeca))
				{
					modificado = 0;
				}else
				{
					termina_ncurses();
					printf("Não foi possível salvar o arquivo.\n");
				}

			}
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
			atualiza_barra_status(modo);
			wrefresh(janela_editor);
		}

		//Limpa as coisas
		refresh();
		termina_ncurses();
	}
	return(0);
}