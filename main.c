#include "grooveshark.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <allegro.h>

// Headers --> passar para o .h depois
void init();
void deinit();
void le_info_mouse();
void gerencia_tela();
void carrega_imagens();
void destroi_imagens();
void tela_estatica();
void menu();
void cadastrar();
void buscar();
void filtrar();
void remover();

// Bitmaps
BITMAP *fundo = NULL, *buffer = NULL, *logo = NULL, *tela_inicio = NULL, *botao_cadastrar = NULL, 
*botao_remover = NULL, *botao_filtrar = NULL, *botao_buscar = NULL, *botao_voltar = NULL, 
*botao_buscar_por = NULL, *botao_buscar_por_banda = NULL, *botao_buscar_por_album = NULL, 
*botao_buscar_por_musica = NULL, *tela_buscar = NULL, *botao_filtrar_por = NULL, 
*botao_filtrar_por_decada_banda = NULL, *botao_filtrar_por_decada_album = NULL, 
*botao_filtrar_por_genero = NULL, *tela_filtrar = NULL, *botao_remover_por = NULL, 
*tela_remover = NULL, *botao_cadastrar_por = NULL, *botao_cadastrar_banda = NULL, 
*botao_cadastrar_album = NULL, *botao_cadastrar_musica = NULL, *botao_cadastrar_ano = NULL,
*botao_cadastrar_genero = NULL, *botao_cadastrar_nome = NULL, *botao_cadastrar_caminho = NULL,
*botao_cadastrar_descricao = NULL, *botao_cadastrar_duracao = NULL, *botao_cadastrar_por_banda = NULL,
*botao_cadastrar_por_album = NULL, *botao_cadastrar_por_musica = NULL, *tela_cadastrar_banda = NULL,
*tela_cadastrar_album = NULL, *tela_cadastrar_musica = NULL, *tela_cadastro_banda_existe = NULL,
*tela_cadastro_album_existe = NULL, *tela_cadastro_musica_existe = NULL, *tela_cadastro_sucesso = NULL,
*tela_remover_musica = NULL, *tela_musica_n_existe = NULL, *tela_remocao_sucesso = NULL,
*tela_remover_album = NULL, *tela_remover_banda = NULL, *tela_album_n_existe = NULL,
*tela_banda_n_existe = NULL, *tela_buscar_album = NULL, *tela_buscar_banda = NULL,
*tela_buscar_musica = NULL, *botao_play = NULL, *botao_stop = NULL;

// Musicas
MIDI *som;

// Fonte
extern FONT *font;

// Variaveis do mouse
extern volatile mouse_x; // Variavel com a posicao x do cursor
extern volatile mouse_y; // Variavel com a posicao y do cursor
extern volatile mouse_b; // Variavel que indica quando o botao esquerdo do mouse foi pressionado

// Demais variaveis
int cursor_x, cursor_y;
int tela = 0; // Maquina de estados

// Funcao que simula uma caixa de texto
char *getText(int x, int y, BITMAP *buff, BITMAP *tela_fundo, int cor, char edittext[128])
{
    // char    edittext[128];
     BITMAP *fundo = buff;
     BITMAP *buffer = tela_fundo; //create_bitmap(SCREEN_W, SCREEN_H);
     
     int     caret  = 0;
     
     int l = 0;

    // memset(edittext, 0, sizeof(edittext));
     
     while (l == 0)
     {
           //clear(buffer);          
           draw_sprite(buffer, fundo, x - 10, y - 20);
           vline(buffer, ( caret * 8 ) + x, y-2, y+8, cor);
           if(keypressed())
           {
                    int  newkey   = readkey();
                    char ASCII    = newkey & 0xff;
                    char scancode = newkey >> 8;

                    // Quando um caractere e pressionado ele entra na string, sai da string ou encerra a leitura
                    if(ASCII >= 32 && ASCII <= 126)
                    {
				             if(caret < 128 - 1)
				             {
				    	              edittext[caret] = ASCII;
					                  caret++;
					                  edittext[caret] = '\0';
                             }        
                    }
                    else if(scancode == KEY_BACKSPACE)
                    {
                         if (caret > 0) caret--;
                         edittext[caret] = '\0';
                    }
                    else if(scancode == KEY_ENTER)
                    {
				         l = 1;
                    }
             }
             
             textprintf_ex(buffer, font, x, y, cor, -1, "%s", edittext);
             blit(buffer, screen, 0, 0, 0, 0, 800, 600);
             
      }
      
      return edittext;
}

// Carrega os bitmaps usados em todo o programa
void carrega_imagens() 
{
     fundo = load_bitmap("bmp/cor_de_fundo.bmp", NULL); 
     buffer = load_bitmap("bmp/fundo_texto.bmp", NULL);
     botao_play = load_bitmap("bmp/botao_play.bmp", NULL);
     botao_stop = load_bitmap("bmp/botao_stop.bmp", NULL);          
     tela_inicio = load_bitmap("bmp/groovesharklogo.bmp", NULL);
     tela_buscar = load_bitmap("bmp/tela_buscar.bmp", NULL);
     tela_buscar_musica = load_bitmap("bmp/tela_buscar_musica.bmp", NULL);
     tela_buscar_album = load_bitmap("bmp/tela_buscar_album.bmp", NULL);
     tela_buscar_banda = load_bitmap("bmp/tela_buscar_banda.bmp", NULL);
     tela_filtrar = load_bitmap("bmp/tela_filtrar.bmp", NULL);
     tela_remover = load_bitmap("bmp/tela_remover.bmp", NULL);
     tela_remover_musica = load_bitmap("bmp/tela_remover_musica.bmp", NULL);
     tela_cadastrar_banda = load_bitmap("bmp/tela_cadastrar_banda.bmp", NULL);
     tela_cadastrar_album = load_bitmap("bmp/tela_cadastrar_album.bmp", NULL);
     tela_cadastrar_musica = load_bitmap("bmp/tela_cadastrar_musica.bmp", NULL);
     tela_cadastro_banda_existe = load_bitmap("bmp/tela_cadastro_banda_existe.bmp", NULL);
     tela_cadastro_album_existe = load_bitmap("bmp/tela_cadastro_album_existe.bmp", NULL);
     tela_cadastro_musica_existe = load_bitmap("bmp/tela_cadastro_musica_existe.bmp", NULL);
     tela_cadastro_sucesso = load_bitmap("bmp/tela_cadastro_sucesso.bmp", NULL);
     tela_musica_n_existe = load_bitmap("bmp/tela_musica_n_existe.bmp", NULL);
     tela_album_n_existe = load_bitmap("bmp/tela_album_n_existe.bmp", NULL);
     tela_banda_n_existe = load_bitmap("bmp/tela_banda_n_existe.bmp", NULL);
     tela_remocao_sucesso = load_bitmap("bmp/tela_remocao_sucesso.bmp", NULL);
     tela_remover_album = load_bitmap("bmp/tela_remover_album.bmp", NULL);
     tela_remover_banda = load_bitmap("bmp/tela_remover_banda.bmp", NULL);
     logo = load_bitmap("bmp/grooveshark_logo_horizontal.bmp", NULL);
     botao_cadastrar = load_bitmap("bmp/botao_cadastrar.bmp", NULL);
     botao_cadastrar_por = load_bitmap("bmp/botao_cadastrar_por.bmp", NULL);
     botao_cadastrar_banda = load_bitmap("bmp/botao_cadastrar_banda.bmp", NULL);
     botao_cadastrar_album = load_bitmap("bmp/botao_cadastrar_album.bmp", NULL);
     botao_cadastrar_musica = load_bitmap("bmp/botao_cadastrar_musica.bmp", NULL);
     botao_cadastrar_por_musica = load_bitmap("bmp/botao_cadastrar_por_musica.bmp", NULL);
     botao_cadastrar_por_album = load_bitmap("bmp/botao_cadastrar_por_album.bmp", NULL);
     botao_cadastrar_por_banda = load_bitmap("bmp/botao_cadastrar_por_banda.bmp", NULL);
     botao_cadastrar_nome = load_bitmap("bmp/botao_cadastrar_nome.bmp", NULL);
     botao_cadastrar_ano = load_bitmap("bmp/botao_cadastrar_ano.bmp", NULL);
     botao_cadastrar_genero = load_bitmap("bmp/botao_cadastrar_genero.bmp", NULL);
     botao_cadastrar_caminho = load_bitmap("bmp/botao_cadastrar_caminho.bmp", NULL);
     botao_cadastrar_duracao = load_bitmap("bmp/botao_cadastrar_duracao.bmp", NULL);
     botao_cadastrar_descricao = load_bitmap("bmp/botao_cadastrar_descricao.bmp", NULL);
     botao_remover = load_bitmap("bmp/botao_remover.bmp", NULL);
     botao_buscar = load_bitmap("bmp/botao_buscar.bmp", NULL);
     botao_filtrar = load_bitmap("bmp/botao_filtrar.bmp", NULL);
     botao_voltar = load_bitmap("bmp/botao_voltar.bmp", NULL);     
     botao_buscar_por = load_bitmap("bmp/botao_buscar_por.bmp", NULL);
     botao_buscar_por_banda = load_bitmap("bmp/botao_buscar_por_banda.bmp", NULL);      
     botao_buscar_por_album = load_bitmap("bmp/botao_buscar_por_album.bmp", NULL);
     botao_buscar_por_musica = load_bitmap("bmp/botao_buscar_por_musica.bmp", NULL);
     botao_filtrar_por = load_bitmap("bmp/botao_filtrar_por.bmp", NULL);
     botao_filtrar_por_decada_banda = load_bitmap("bmp/botao_filtrar_por_decada_banda.bmp", NULL);
     botao_filtrar_por_decada_album = load_bitmap("bmp/botao_filtrar_por_decada_album.bmp", NULL);
     botao_filtrar_por_genero = load_bitmap("bmp/botao_filtrar_por_genero.bmp", NULL);
     botao_remover_por = load_bitmap("bmp/botao_remover_por.bmp", NULL);
}

// Funcao para registrar posicao do mouse quando botao esquerdo e pressionado
void le_info_mouse () 
{
     if (mouse_b & 1) 
     {
        cursor_x = mouse_x;
        cursor_y = mouse_y;
     }
}

// Funcao padrao do allegro para inicializacao
void init() 
{
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	if (res != 0) 
    {
		allegro_message(allegro_error);
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	/* add other initializations here */
}

void deinit() 
{
	clear_keybuf();
	allegro_exit();
	/* add other deinitializations here */
}

// Funcao que gerencia a maquina de estados
void gerencia_tela(char string[128], Lista_banda *L, HASH *H, BANDA *B, ALBUM *A, MUSICA *M, MUSICA** LM, MIDI *som)
{
     char banda_nome[TAM_NOME_BANDA];
     char banda_genero[TAM_NOME_GENERO];
     char banda_descricao[TAM_NOME_DESCRICAO];
     int banda_ano;
     
     char album_nome[TAM_NOME_ALBUM];
     int album_ano;
     
     char musica_nome[TAM_NOME_MUSICA];
     char musica_duracao[TAM_DURACAO];
     int musica_ano;
     
     int teste;
     
     // Se esta na tela de inicio, entra nesse if
     if (tela == 0) 
     {       
         if ((cursor_x > 0 ) && (cursor_y > 0) && (cursor_x < 800) && (cursor_y < 600)) 
         {
            menu();
            tela = 1;
         }
     }
     
     // Se esta no menu entra nesse if
     else if (tela == 1) 
     {   
         // Caso para quando o usuário clica no botao CADASTRAR
         if ((cursor_x > 100 ) && (cursor_y > 300) && (cursor_x < 306) && (cursor_y < 367)) 
         {
            cadastrar();
            tela = 2;
         }
         // Caso para quando o usuário clica no botao REMOVER
         if ((cursor_x > 100 ) && (cursor_y > 450) && (cursor_x < 306) && (cursor_y < 517)) 
         {
            remover();
            tela = 3;
         }
         // Caso para quando o usuário clica no botao FILTRAR
         if ((cursor_x > 500 ) && (cursor_y > 300) && (cursor_x < 706) && (cursor_y < 367)) 
         {
            filtrar();
            tela = 4;
         }
         // Caso para quando o usuário clica no botao BUSCAR
         if ((cursor_x > 500 ) && (cursor_y > 450) && (cursor_x < 706) && (cursor_y < 517)) 
         {
            buscar();
            tela = 5;
         }
     }
     
     // Se esta na tela CADASTRAR, entra nesse if
     else if (tela == 2) 
     {       
         // Botao cadastrar por BANDA  
         if ((cursor_x > 50 ) && (cursor_y > 280) && (cursor_x < 200) && (cursor_y < 328)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_cadastrar_banda = load_bitmap("bmp/tela_cadastrar_banda.bmp", NULL);
            
            strcpy(string, getText(210, 300, buffer, tela_cadastrar_banda, COR, string));
            strcpy(banda_nome, string);
            string[0] = '\0';
            strcpy(string, getText(210, 360, buffer, tela_cadastrar_banda, COR, string));
            strcpy(banda_genero, string);
            string[0] = '\0';
            strcpy(string, getText(210, 420, buffer, tela_cadastrar_banda, COR, string));
            strcpy(banda_descricao, string);
            string[0] = '\0';
            strcpy(string, getText(210, 480, buffer, tela_cadastrar_banda, COR, string));
            banda_ano = atoi(string);
            
            B = cria_banda(banda_nome, banda_genero, banda_descricao, banda_ano);
            teste = cadastra_banda(L,B);
            
            tela = 0;
            if(teste == SUCESSO)
               tela_estatica(tela_cadastro_sucesso);        
            else
                tela_estatica(tela_cadastro_banda_existe);        
            
         }
           // Botao cadastrar por ALBUM  
         if ((cursor_x > 50 ) && (cursor_y > 340) && (cursor_x < 200) && (cursor_y < 388)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_cadastrar_album = load_bitmap("bmp/tela_cadastrar_album.bmp", NULL);
            
            strcpy(string, getText(210, 300, buffer, tela_cadastrar_album, COR, string));
            strcpy(album_nome, string);
            string[0] = '\0';
            strcpy(string, getText(210, 360, buffer, tela_cadastrar_album, COR, string));
            album_ano = atoi(string);
            string[0] = '\0';
            
            string[0] = '\0';
            fflush(stdin);
            tela_cadastrar_banda = load_bitmap("bmp/tela_cadastrar_banda.bmp", NULL);
            
            strcpy(string, getText(210, 300, buffer, tela_cadastrar_banda, COR, string));
            strcpy(banda_nome, string);
            string[0] = '\0';
            strcpy(string, getText(210, 360, buffer, tela_cadastrar_banda, COR, string));
            strcpy(banda_genero, string);
            string[0] = '\0';
            strcpy(string, getText(210, 420, buffer, tela_cadastrar_banda, COR, string));
            strcpy(banda_descricao, string);
            string[0] = '\0';
            strcpy(string, getText(210, 480, buffer, tela_cadastrar_banda, COR, string));
            banda_ano = atoi(string);
            
            B = busca_banda(L, banda_nome);            
            if(B == NULL)
                 B = cria_banda(banda_nome, banda_genero, banda_descricao, banda_ano);           
            cadastra_banda(L,B);
                 
            A = cria_album(album_nome, album_ano);
            teste = cadastra_album(B, A);
            
            tela = 0;
            if(teste == SUCESSO)
               tela_estatica(tela_cadastro_sucesso);        
            else
                tela_estatica(tela_cadastro_album_existe); 
         }  
           // Botao cadastrar por MUSICA  
         if ((cursor_x > 50 ) && (cursor_y > 400) && (cursor_x < 200) && (cursor_y < 448)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_cadastrar_musica = load_bitmap("bmp/tela_cadastrar_musica.bmp", NULL);
            
            strcpy(string, getText(210, 300, buffer, tela_cadastrar_musica, COR, string));
            strcpy(musica_nome, string);
            string[0] = '\0';
            strcpy(string, getText(210, 360, buffer, tela_cadastrar_musica, COR, string));
            musica_ano = atoi(string);
            string[0] = '\0';
            strcpy(string, getText(210, 420, buffer, tela_cadastrar_musica, COR, string));
            strcpy(musica_duracao, string);
            
            string[0] = '\0';
            fflush(stdin);
            tela_cadastrar_album = load_bitmap("bmp/tela_cadastrar_album.bmp", NULL);
            
            strcpy(string, getText(210, 300, buffer, tela_cadastrar_album, COR, string));
            strcpy(album_nome, string);
            string[0] = '\0';
            strcpy(string, getText(210, 360, buffer, tela_cadastrar_album, COR, string));
            album_ano = atoi(string);
            
            string[0] = '\0';
            fflush(stdin);
            tela_cadastrar_banda = load_bitmap("bmp/tela_cadastrar_banda.bmp", NULL);
            
            strcpy(string, getText(210, 300, buffer, tela_cadastrar_banda, COR, string));
            strcpy(banda_nome, string);
            string[0] = '\0';
            strcpy(string, getText(210, 360, buffer, tela_cadastrar_banda, COR, string));
            strcpy(banda_genero, string);
            string[0] = '\0';
            strcpy(string, getText(210, 420, buffer, tela_cadastrar_banda, COR, string));
            strcpy(banda_descricao, string);
            string[0] = '\0';
            strcpy(string, getText(210, 480, buffer, tela_cadastrar_banda, COR, string));
            banda_ano = atoi(string);
            
            B = busca_banda(L, banda_nome);
            if(B == NULL)
                 B = cria_banda(banda_nome, banda_genero, banda_descricao, banda_ano);    
            cadastra_banda(L,B);
                 
            A = busca_album(L, album_nome);  
            if(A == NULL)
                 A = cria_album(album_nome, album_ano);                
            cadastra_album(B, A);
                 
            M = cria_musica(musica_nome, "caminho", musica_duracao, musica_ano);
            teste = cadastra_musica(A, M, H);
            
            tela = 0;
            if(teste == SUCESSO)
               tela_estatica(tela_cadastro_sucesso);        
            else
                tela_estatica(tela_cadastro_musica_existe);
         } 
          // Botao VOLTAR
         if ((cursor_x > 600 ) && (cursor_y > 500) && (cursor_x < 747) && (cursor_y < 548)) 
         {
            menu();
            tela = 1;
         }
     }
     
     // Se esta na tela REMOVER, entra nesse if
     else if (tela == 3) 
     {     
            // Botao remover por BANDA  
         if ((cursor_x > 50 ) && (cursor_y > 280) && (cursor_x < 200) && (cursor_y < 328)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_remover = load_bitmap("bmp/tela_remover_banda.bmp", NULL);
            
            strcpy(string, getText(310, 300, buffer, tela_remover, COR, string));
            strcpy(banda_nome, string);
            B = busca_banda(L, banda_nome);
            string[0] = '\0';
            
            teste = remove_banda(L, B, H);
            
            tela = 0;
            if(teste == SUCESSO)
                tela_estatica(tela_remocao_sucesso);        
            else
                tela_estatica(tela_banda_n_existe);
         }
         
           // Botao remover por ALBUM  
         if ((cursor_x > 50 ) && (cursor_y > 340) && (cursor_x < 200) && (cursor_y < 388)) 
         {
            string[0] = '\0';
            fflush(stdin);            
            tela_remover = load_bitmap("bmp/tela_remover_album.bmp", NULL);
            
            strcpy(string, getText(310, 300, buffer, tela_remover, COR, string));
            strcpy(album_nome, string);
            A = busca_album(L, album_nome);
            string[0] = '\0';
            strcpy(string, getText(310, 360, buffer, tela_remover, COR, string));
            strcpy(banda_nome, string);
            B = busca_banda(L, banda_nome);
            string[0] = '\0';
            
            teste = remove_album(B, A, H);
            
            tela = 0;
            if(teste == SUCESSO)
                tela_estatica(tela_remocao_sucesso);        
            else
                tela_estatica(tela_album_n_existe);
         }  
           // Botao remover por MUSICA  
         if ((cursor_x > 50 ) && (cursor_y > 400) && (cursor_x < 200) && (cursor_y < 448)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_remover = load_bitmap("bmp/tela_remover_musica.bmp", NULL);
            
            strcpy(string, getText(310, 300, buffer, tela_remover, COR, string));
            strcpy(musica_nome, string);
            string[0] = '\0';
            strcpy(string, getText(310, 360, buffer, tela_remover, COR, string));
            strcpy(banda_nome, string);
            string[0] = '\0';
            
            teste = remove_musica(H, musica_nome, banda_nome);
            
            tela = 0;
            if(teste == SUCESSO)
                tela_estatica(tela_remocao_sucesso);        
            else
                tela_estatica(tela_musica_n_existe);
         }  
          // Botao VOLTAR
         if ((cursor_x > 600 ) && (cursor_y > 500) && (cursor_x < 747) && (cursor_y < 548)) 
         {
            menu();
            tela = 1;
         }
     }
     
     // Se esta na tela FILTRAR, entra nesse if
     else if (tela == 4) 
     {  
             // Botao filtrar por DECADA BANDA  
         if ((cursor_x > 50 ) && (cursor_y > 280) && (cursor_x < 300) && (cursor_y < 328)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_filtrar = load_bitmap("bmp/tela_filtrar.bmp", NULL);
            
            strcpy(string, getText(310, 300, buffer, tela_filtrar, COR, string));
            menu();
            tela = 1;
         }
           // Botao filtrar por DECADA ALBUM  
         if ((cursor_x > 50 ) && (cursor_y > 340) && (cursor_x < 300) && (cursor_y < 388)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_filtrar = load_bitmap("bmp/tela_filtrar.bmp", NULL);
            
            strcpy(string, getText(310, 360, buffer, tela_filtrar, COR, string));
            menu();
            tela = 1;
         }  
           // Botao filtrar por GENERO
         if ((cursor_x > 50 ) && (cursor_y > 400) && (cursor_x < 300) && (cursor_y < 448)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_filtrar = load_bitmap("bmp/tela_filtrar.bmp", NULL);
            
            strcpy(string, getText(310, 420, buffer, tela_filtrar, COR, string));
            menu();
            tela = 1;
         }    
          // Botao VOLTAR 
         if ((cursor_x > 600 ) && (cursor_y > 500) && (cursor_x < 747) && (cursor_y < 548)) 
         {
            menu();
            tela = 1;
         }
     }
     
     // Se esta na tela BUSCAR, entra nesse if
     else if (tela == 5) 
     {   
           // Botao buscar por BANDA  
         if ((cursor_x > 50 ) && (cursor_y > 280) && (cursor_x < 200) && (cursor_y < 328)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_buscar = load_bitmap("bmp/tela_buscar_banda.bmp", NULL);
            
            strcpy(string, getText(310, 300, buffer, tela_buscar, COR, string));
            strcpy(banda_nome, string);
            string[0] = '\0';
            
            B = busca_banda(L, banda_nome);
            
            tela = 0;
            if(B != NULL)
            {
                tela_cadastrar_banda = load_bitmap("bmp/tela_cadastrar_banda.bmp", NULL);
                tela_estatica(tela_cadastrar_banda);        
                textprintf_ex(screen, font, 210, 300, COR, -1, "%s", B->nome);                
                textprintf_ex(screen, font, 210, 360, COR, -1, "%s", B->genero);                
                textprintf_ex(screen, font, 210, 420, COR, -1, "%s", B->descricao);               
                textprintf_ex(screen, font, 210, 480, COR, -1, "%d", B->ano);                                                                 
            }        
            else
                tela_estatica(tela_banda_n_existe);

         }
           // Botao buscar por ALBUM  
         if ((cursor_x > 50 ) && (cursor_y > 340) && (cursor_x < 200) && (cursor_y < 388)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_buscar = load_bitmap("bmp/tela_buscar_album.bmp", NULL);
            
            strcpy(string, getText(310, 300, buffer, tela_buscar, COR, string));
            strcpy(album_nome, string);
            string[0] = '\0';
            
            A = busca_album(L, album_nome);
            
            tela = 0;
            if(A != NULL)
            {
                tela_cadastrar_album = load_bitmap("bmp/tela_cadastrar_album.bmp", NULL);
                tela_estatica(tela_cadastrar_album);        
                textprintf_ex(screen, font, 210, 300, COR, -1, "%s", A->nome_album);                
                textprintf_ex(screen, font, 210, 360, COR, -1, "%d", A->ano_album);                  
            }
            else
                tela_estatica(tela_album_n_existe);

         }  
           // Botao buscar por MUSICA  
         if ((cursor_x > 50 ) && (cursor_y > 400) && (cursor_x < 200) && (cursor_y < 448)) 
         {
            string[0] = '\0';
            fflush(stdin);
            tela_buscar = load_bitmap("bmp/tela_buscar_musica.bmp", NULL);
            
            strcpy(string, getText(310, 300, buffer, tela_buscar, COR, string));
            strcpy(musica_nome, string);
            string[0] = '\0';
            
            LM = busca_musica(H, musica_nome);
            
            tela = 0;
            if(LM[0] != NULL)
            {
                tela_cadastrar_musica = load_bitmap("bmp/tela_cadastrar_musica.bmp", NULL);
                tela_estatica(tela_cadastrar_musica);        
                textprintf_ex(screen, font, 210, 300, COR, -1, "%s", LM[0]->nome_musica);
                textprintf_ex(screen, font, 210, 360, COR, -1, "%s", LM[0]->duracao);
                textprintf_ex(screen, font, 210, 420, COR, -1, "%d", LM[0]->ano_musica);                                
                blit(botao_play, screen, 0, 0, 340, 500, 800, 600);
                blit(botao_stop, screen, 0, 0, 440, 500, 800, 600);
            }
            else
                tela_estatica(tela_musica_n_existe);
 
         }
          // Botao VOLTAR  
         if ((cursor_x > 600 ) && (cursor_y > 500) && (cursor_x < 747) && (cursor_y < 548)) 
         {
            menu();
            tela = 1;
         }
     }
     
     cursor_x = 0;
     cursor_y = 0;
}

void tela_estatica(BITMAP *tela) 
{
     // Esconde o mouse para desenhar o gráfico
     show_mouse(NULL);

     // Imprime botoes com opçoes do menu
     blit(tela, screen, 0, 0, 0, 0, 800, 600);
     
     // Mostra o mouse novamente
     show_mouse(screen);

}

void menu() 
{
     // Esconde o mouse para desenhar o gráfico
     show_mouse(NULL);

     // Imprime botoes com opçoes do menu
     blit(fundo, screen, 0, 0, 0, 0, 800, 600);
     blit(logo, screen, 0, 0, 50, 50, 800, 600);
     blit(botao_cadastrar, screen, 0, 0, 100, 300, 800, 600);
     blit(botao_remover, screen, 0, 0, 100, 450, 800, 600);
     blit(botao_filtrar, screen, 0, 0, 500, 300, 800, 600);
     blit(botao_buscar, screen, 0, 0, 500, 450, 800, 600);               

     // Mostra o mouse novamente
     show_mouse(screen);
     sleep(200);

}

void cadastrar() 
{
     // Esconde o mouse para desenhar o gráfico
     show_mouse(NULL);

     // Imprime botoes com opçoes do menu
     blit(fundo, screen, 0, 0, 0, 0, 800, 600);
     blit(logo, screen, 0, 0, 50, 50, 800, 600);
     blit(botao_voltar, screen, 0, 0, 600, 500, 800, 600);
     blit(botao_cadastrar_por, screen, 0, 0, 0, 200, 800, 600); 
     blit(botao_cadastrar_banda, screen, 0, 0, 50, 280, 800, 600);
     blit(botao_cadastrar_album, screen, 0, 0, 50, 340, 800, 600);
     blit(botao_cadastrar_musica, screen, 0, 0, 50, 400, 800, 600); 
     
     // Mostra o mouse novamente
     show_mouse(screen);
     sleep(200);
}

void remover()
{
     // Esconde o mouse para desenhar o gráfico
     show_mouse(NULL);

     // Imprime botoes com opçoes do menu
     blit(fundo, screen, 0, 0, 0, 0, 800, 600);
     blit(logo, screen, 0, 0, 50, 50, 800, 600);
     blit(botao_voltar, screen, 0, 0, 600, 500, 800, 600);
     blit(botao_remover_por, screen, 0, 0, 0, 200, 800, 600);     
     blit(botao_cadastrar_banda, screen, 0, 0, 50, 280, 800, 600);
     blit(botao_cadastrar_album, screen, 0, 0, 50, 340, 800, 600);
     blit(botao_cadastrar_musica, screen, 0, 0, 50, 400, 800, 600);
     
     // Mostra o mouse novamente
     show_mouse(screen);
     sleep(200);
}

void filtrar() 
{
     // Esconde o mouse para desenhar o gráfico
     show_mouse(NULL);

     // Imprime botoes com opçoes do menu
     blit(fundo, screen, 0, 0, 0, 0, 800, 600);
     blit(logo, screen, 0, 0, 50, 50, 800, 600);
     blit(botao_voltar, screen, 0, 0, 600, 500, 800, 600);
     blit(botao_filtrar_por, screen, 0, 0, 0, 200, 800, 600);     
     blit(botao_filtrar_por_decada_banda, screen, 0, 0, 50, 280, 800, 600);
     blit(botao_filtrar_por_decada_album, screen, 0, 0, 50, 340, 800, 600);
     blit(botao_filtrar_por_genero, screen, 0, 0, 50, 400, 800, 600);
     
     // Mostra o mouse novamente
     show_mouse(screen);
     sleep(200);
}

void buscar() 
{
     // Esconde o mouse para desenhar o gráfico
     show_mouse(NULL);

     // Imprime botoes com opçoes do menu
     blit(fundo, screen, 0, 0, 0, 0, 800, 600);
     blit(logo, screen, 0, 0, 50, 50, 800, 600);
     blit(botao_voltar, screen, 0, 0, 600, 500, 800, 600);
     blit(botao_buscar_por, screen, 0, 0, 0, 200, 800, 600);     
     blit(botao_cadastrar_banda, screen, 0, 0, 50, 280, 800, 600);
     blit(botao_cadastrar_album, screen, 0, 0, 50, 340, 800, 600);
     blit(botao_cadastrar_musica, screen, 0, 0, 50, 400, 800, 600);
     
     // Mostra o mouse novamente
     show_mouse(screen);
     sleep(200);
}


// Destrói os bitmaps que foram carregados na memória
void destroi_imagens() 
{
     destroy_bitmap(fundo);
     destroy_bitmap(buffer);
     destroy_bitmap(tela_inicio);
     destroy_bitmap(tela_buscar);
     destroy_bitmap(tela_filtrar);
     destroy_bitmap(tela_remover);
     destroy_bitmap(logo);
     destroy_bitmap(botao_cadastrar);
     destroy_bitmap(botao_remover);
     destroy_bitmap(botao_buscar);
     destroy_bitmap(botao_filtrar);
     destroy_bitmap(botao_voltar);
     destroy_bitmap(botao_buscar_por);
     destroy_bitmap(botao_buscar_por_banda);
     destroy_bitmap(botao_buscar_por_album);
     destroy_bitmap(botao_buscar_por_musica);
     destroy_bitmap(botao_filtrar_por);
     destroy_bitmap(botao_filtrar_por_decada_banda);
     destroy_bitmap(botao_filtrar_por_decada_album);
     destroy_bitmap(botao_filtrar_por_genero);
     destroy_bitmap(botao_remover_por);
}

int main() 
{
	init();
	carrega_imagens();
	tela_estatica(tela_inicio);
	
	
	som = load_midi("musicas/Video_Game_Themes_-_Super_Mario_All_Stars.mid");
	//play_midi(som, TRUE);
	
	char string[128];
    memset(string, 0, sizeof(string));
    
    Lista_banda *L;
    L = cria_lista();
    
    HASH *H;
    Cria_Hash(&H);
    
    BANDA *B = MALLOC(BANDA);
    B->prox = NULL;
    B->ini = NULL;
    
    ALBUM *A = MALLOC(ALBUM);
    A->prox = NULL;
    A->ini = NULL;
    
    MUSICA *M = MALLOC(MUSICA);
    M->album = NULL;
    M->prox = NULL;
    M->ant = NULL;
    
    MUSICA** LM;
    
    FILE *bandas = fopen("arquivos/bandas.txt", "r");
    FILE *albuns = fopen("arquivos/albuns.txt", "r");
    FILE *musicas = fopen("arquivos/musicas.txt", "r");

    carrega_base_dados (L, B, A, M, H, bandas, albuns, musicas);
    
    fclose(bandas);
    fclose(albuns); 
    fclose(musicas);
    
	while (!key[KEY_ESC]) 
    {
          le_info_mouse();
          gerencia_tela(string, L, H, B, A, M, LM, som);
	}
	
	bandas = fopen("arquivo/bandas.txt", "w");
    albuns = fopen("arquivo/albuns.txt", "w");
    musicas = fopen("arquivo/musicas.txt", "w");
	
	salvar_base_dados (L, bandas, albuns, musicas);
	
	fclose(bandas);
    fclose(albuns); 
    fclose(musicas);
	
    destroi_imagens();
	deinit();
	return 0;
}
END_OF_MAIN();
