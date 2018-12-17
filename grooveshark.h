/************************************************************************
 *            Aluno: Aulos Marino                      7986409          * 
 *            Aluno: Bruno Pinto Ferraz Fabbri         4154844          *
 *            Aluno: Frederico de Oliveira Sampaio     8922100          * 
 *            Aluno: Lucas Marques Gasparino           8936892          *
 *            Aluno: Rogiel dos Santos                 8061793          *
 *            Aluno: Tiago Leite                       7595289          * 
 *            Aluno: Wesley Tiozzo                     8077925          *
 *                                                                      *
 *                  Trabalho final: GrooveShark                         *
 *            Sistema utilizado para desenvolvimento: Windows           *
 *                          Disciplina SCC0201                          *
 *************************************************************************/
  
  
#ifndef GROOVESHARK
#define GROOVESHARK
 
#include <stdio.h>
  
/* ----------------Defines---------------- */
#define TAM_NOME_BANDA      64
#define TAM_NOME_GENERO     32
#define TAM_NOME_DESCRICAO  2048
#define TAM_NOME_ALBUM      64
#define TAM_NOME_MUSICA     64
#define TAM_CAMINHO         128
#define TAM_DURACAO         24
  
#define MOD 17

#define COR 0x000000
  
#define SUCESSO 1
#define ERRO 0
#define VERDADEIRO 1
#define FALSO 0
  
#define ARGUMENTO_INVALIDO -1
#define ERRO_MEMORIA -2
  
#define ALBUM_JA_EXISTE    -9
#define BANDA_SEM_ALBUM    -11
#define ALBUM_NAO_EXISTE   -13
#define BANDA_NAO_EXISTE   -21
 
#define LISTA_SEM_BANDA    -15
#define MUSICA_JA_EXISTE   -16
#define MUSICA_N_EXISTE    -17
  
#define MALLOC(a) (a*)malloc(sizeof(a))
#define MALLOC_SIZE(a, b) (a*)malloc(b*sizeof(a))
/* ----------------Fim Defines---------------- */
  
/* ----------------Estruturas de Dados---------------- */
typedef struct banda {
    char nome[TAM_NOME_BANDA];
    char genero[TAM_NOME_GENERO];
    char descricao[TAM_NOME_DESCRICAO];
    int ano;
    struct banda *prox;
    struct album *ini;
} BANDA;
  
typedef struct {
    BANDA *ini;
} Lista_banda;
  
typedef struct album {
    char nome_album[TAM_NOME_ALBUM];
    int ano_album;
    char caminho[TAM_CAMINHO];
    struct banda *banda;
    struct album *prox;
    struct musica *ini;
} ALBUM;
  
typedef struct musica {
    char nome_musica[TAM_NOME_MUSICA];
    char caminho[TAM_CAMINHO];
    char duracao[TAM_DURACAO];
    int ano_musica;
    struct album *album;
    struct musica *prox;
    struct musica *ant;
} MUSICA;
  
typedef struct musica_hash {
    char nome[TAM_NOME_MUSICA];
    MUSICA *ptr;
    struct musica_hash *prox;
} MUSICA_HASH;
  
typedef struct {
    MUSICA_HASH *ini;
} HASH;
  
 /* ----------------Fim Estruturas de Dados---------------- */
  
/* --------------------Protótipos--------------------- */
  
/* Lê os dados armazenados no arquivo e carrega-os na memória */
void carrega_base_dados (Lista_banda *L, BANDA *B, ALBUM *A, MUSICA *M, HASH *H, FILE *bandas, FILE *albuns, FILE *musicas);
  
 /********************************************************************/
/* Verifica se a lista de bandas está vazia se estiver retorna VERDADEIRO caso contra retorna FALSO*/
int esta_vazio_banda (Lista_banda *L);
 
/********************************************************************/
/* Verifica se a lista de álbums está vazia, se estiver retorna VERDADEIRO, se o  não estiver retorna FALSO e se o bloco B for NULL retorna ARGUMENTO_INVALIDO*/
 
int esta_vazio_album (BANDA *B);
 
/********************************************************************/
 
/* Verifica se a lista de músicas está vazia, se estiver retorna SUCESSO, caso contrario retorna FALSO  */
int esta_vazio_musica (ALBUM *A);
  
/********************************************************************/
  
/* Insere uma banda na lista de bandas, se a banda já existir retorna ERRO caso contrario retorna SUCESSO */
int cadastra_banda (Lista_banda *L, BANDA *B);
 
/********************************************************************/ 
/* Insere um álbum na lista de álbums da banda B, se alguns do argumento não forem válidos retorna ARGUMENTO_INVALIDO, caso contrario retorna SUCESSO*/
int cadastra_album (BANDA *B, ALBUM *A); 
 
/********************************************************************/
 
/********************************************************************/
/* Cria e inicializa vetor para a tabela hash */
void Cria_Hash(HASH **H);
 
/********************************************************************/ 
/* Cálcula o hash da música  e retorna a soma dos caracteres já calculado o mod*/
int calcula_hash (char *musica);
 
/********************************************************************/
/*Insere um ponteiro para a música na tabela hash, os alguns dos argumentos forem invalidos retorna ARGUMENTO_INVALIDO, caso contrario retorna SUCESSO */
int Cadastra_HASH(HASH **H, MUSICA **M, int posicao);
 
/********************************************************************/
/* Insere uma música na lista de músicas e tabela hash, se os argumentos forem invalidos retorna ARGUMENTOS_INVALIDO, se a musica ja existir retorna MUSICA_JA_EXISTE, caso contrario retorna SUCESSO*/
int cadastra_musica (ALBUM *A, MUSICA *M, HASH *H);
 
/********************************************************************/
/* Remove uma música da lista e tabela hash, se a musica não existir retorna MUSICA_N_EXISTE, caso contrario retorna SUCESSO */
int remove_musica (HASH *H, char *musica, char *banda); 
 
/********************************************************************/ 
/*Remove um álbum e todas as músicas contidas nele, se o argumento for invalido retorna ARGUMENTO_INVALIDO, caso contrario retorna SUCESSO*/
int remove_album (BANDA *B, ALBUM *A, HASH *H);
 
/********************************************************************/
/* Remove uma banda e todos os álbuns e músicas contidas nela, se a banda não existir retorna BANDA_NAO_EXISTE, caso contrario retorna SUCESSO*/
int remove_banda(Lista_banda *L, BANDA *B, HASH *H); 
 
 /********************************************************************/
/* Retorna um vetor de ponteiros para as bandas do gênero escolhido, se o argumento for inválido retorna NULL, caso contrario retorna o bloco B*/
BANDA **filtra_genero_banda (Lista_banda *L, int *e, char *genero);
 
/********************************************************************/
/* Retorna um vetor de ponteiros para as bandas da década escolhida */
BANDA** filtra_decada_banda (Lista_banda *L, int *e, int *decada);//LUCAS
/********************************************************************/
/* Retorna um vetor de ponteiros para os álbuns da década escolhida */
ALBUM **filtra_decada_album (Lista_banda *L, int *e, int *decada);//LUCAS
/********************************************************************/ 
/* Busca uma banda por nome */
BANDA* busca_banda (Lista_banda *L, void *nome); //TIAGO
/********************************************************************/ 
/* Busca um álbum por nome */
ALBUM* busca_album (Lista_banda *L, char *album); //TIAGO
/********************************************************************/ 
/* Busca uma música por nome */
MUSICA** busca_musica (HASH *H, char *musica);
 
/*********************************************************************/
/* Salva a memória atual no arquivo */
void salvar_base_dados (Lista_banda *L, FILE *bandas, FILE *albuns, FILE *musicas);
 
/********************************************************************/
/* Cria e inicializa uma lista de bandas */
Lista_banda *cria_lista(void);
/********************************************************************/ 
/* Cria e inicializa um nó do tipo banda, se algum dado for inválido retorna NULL */
BANDA *cria_banda(char *nome, char *genero, char *descricao, int ano);
/********************************************************************/
 
/* Cria e inicializa um nó do tipo álbum se algum dos dados forem inválido retorna NULL */
ALBUM *cria_album(char *nome, int ano);
 
/********************************************************************/
/* Retorna o pivo para usar no quickort */
int separa_Bnome(BANDA **V, int ini, int fim);
 
/*Para ordena os nomes da banda */
void QSort_B_Nome(BANDA **V, int ini, int fim);
 
/* Ordena bandas por data */
int separa_data(BANDA **V, int ini, int fim);
/********************************************************************/
 
/*ordena por ano as bandas*/
void QSort_B_Data(BANDA **V, int ini, int fim);
 
/********************************************************************/
/* Retorna o pivo para usar no quickort */
int separa_Adata(ALBUM **V, int ini, int fim);
 
/********************************************************************/
/*ordena por ano os albuns*/
void QSort_A_Data(ALBUM **V, int ini, int fim);
 
/********************************************************************/
/* Cria e inicialiaza um nó tipo álbum */
MUSICA *cria_musica(char *nome, char *caminho, char *duracao, int ano);

/* Retorna ARGUMETO_IVALIDO se L ou H forem NULL e SUCESSO se bem sucedido */
int Destroy(Lista_banda *L, HASH *H);
  
/* --------------------Fim Protótipos--------------------- */
  
#endif /* GROOVESHARK */
