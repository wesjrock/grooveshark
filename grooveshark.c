#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grooveshark.h"

/*------------------------------------- ARQUIVO -----------------------------------------*/

void carrega_base_dados (Lista_banda *L, BANDA *B, ALBUM *A, MUSICA *M, HASH *H, FILE *bandas, FILE *albuns, FILE *musicas)
{
    char linha_banda[1024], linha_album[1024], linha_musica[1024];
    char *separa, nomeBanda[TAM_NOME_BANDA], generoBanda[TAM_NOME_GENERO], descricaoBanda[TAM_NOME_DESCRICAO], nomeAlbum[TAM_NOME_ALBUM], nomeMusica[TAM_NOME_MUSICA], caminhoMusica[TAM_CAMINHO], duracaoMusica[TAM_DURACAO];
    int anoBanda, anoAlbum, anoMusica;
 
    banda:
    while(!feof(bandas))
    {
        fgets(linha_banda, sizeof(linha_banda), bandas); //le a linha bandas(bandas.txt)
         
        if(linha_banda[strlen(linha_banda)-1]=='\n')//retira o enter da linha
            linha_banda[strlen(linha_banda)-1]='\0';
         
        while(strcmp(linha_banda, ";"))
        {
            //printf("\nLinha: |%s|\n",linha_banda);
             
            separa = (char*)strtok(linha_banda, "/");
            strcpy(nomeBanda, separa); //salvo nome da banda
            separa = (char*)strtok(NULL, "/");
            strcpy(generoBanda, separa); //salvo genero da banda
            separa = (char*)strtok(NULL, "/");
            strcpy(descricaoBanda, separa); //salvo a descricao da banda
            separa = (char*)strtok(NULL, "/");
            anoBanda = atoi(separa); //salvo o ano da banda
             
            B = cria_banda(nomeBanda,generoBanda,descricaoBanda,anoBanda);
            cadastra_banda(L, B); //cadastro banda na lista
         
            //printf("\n\nBanda: %s\nGenero: %s\nDescricao: %s\nAno: %d",B->nome,B->genero,B->descricao,B->ano);
             
            album:
            while(!feof(albuns))
            {
                fgets(linha_album, sizeof(linha_album), albuns); //leio a linha de album(albuns.txt)
                 
                if(linha_album[strlen(linha_album)-1]=='\n')
                    linha_album[strlen(linha_album)-1]='\0';
                 
                if(strcmp(linha_album, ";"))
                {
                    while(strcmp(linha_album, ";"))
                    {
                        separa = (char*)strtok(linha_album, "/"); 
                        strcpy(nomeAlbum, separa); //salva o nome do album
                        separa = (char*)strtok(NULL, "/");
                        anoAlbum = atoi(separa); //salva o ano do album
 
                        A = cria_album(nomeAlbum,anoAlbum);
                        cadastra_album(B, A);//cadastro album
 
                        //printf("\n\n     Album: %s\n     Ano: %d\n     Pertencente a banda: %s",L->ini->ini->nome_album,L->ini->ini->ano_album,L->ini->ini->banda->nome);
                         
                        while(!feof(musicas))
                        {
                            fgets(linha_musica,sizeof(linha_musica),musicas);
                             
                            if(linha_musica[strlen(linha_musica)-1]=='\n')
                                linha_musica[strlen(linha_musica)-1]='\0';
                             
                            if((strcmp(linha_musica, "---"))&&(strcmp(linha_musica, ";"))) //se tiver musica
                            {
                                if(strcmp(linha_musica, "---")) //ainda nao acabaram as musicas do album atual
                                {
                                    while(strcmp(linha_musica, ";") && strcmp(linha_musica, "---"))
                                    {   
                                        separa = (char*)strtok(linha_musica, "/");
                                        strcpy(nomeMusica, separa); //salvo nome da musica
                                        separa = (char*)strtok(NULL, "/");
                                        strcpy(duracaoMusica, separa); //salvo duracao
                                        strcpy(caminhoMusica,"teste");//
                                        anoMusica = 1990;//
 
                                        M = cria_musica(nomeMusica,caminhoMusica,duracaoMusica,anoMusica);
                                        cadastra_musica(A,M,H); //cadastro musica
                                        M->album = A;
                                        M->prox = NULL;
                                         
                                        //printf("\n\n         Musica: %s\n         Duracao: %s\n         Pertencente a album: %s\n         Da banda : %s",M->nome_musica,M->duracao,M->album->nome_album,M->album->banda->nome);
                                         
                                        fgets(linha_musica,sizeof(linha_musica),musicas);
                                         
                                        if(linha_musica[strlen(linha_musica)-1]=='\n')//leio nova linha de musica
                                            linha_musica[strlen(linha_musica)-1]='\0';  
                                    }
                                }
                            }
                            if(!strcmp(linha_musica, "---")) 
                                goto banda;
                            if(!strcmp(linha_musica, ";"))
                                goto album;
                        }   
                         
                        fgets(linha_album, sizeof(linha_album), albuns);//leio nova linha de album
                         
                        if(linha_album[strlen(linha_album)-1]=='\n')
                            linha_album[strlen(linha_album)-1]='\0';                    
                    }
                }
                else goto album;
            }
             
            fgets(linha_banda, sizeof(linha_banda), bandas);//leio nova linha de banda
             
            if(linha_banda[strlen(linha_banda)-1]=='\n')
                linha_banda[strlen(linha_banda)-1]='\0'; 
        }
    }
}
 
 
void salvar_base_dados (Lista_banda *L, FILE *bandas, FILE *albuns, FILE *musicas)
{
    BANDA *auxB;
    ALBUM *auxA;
    MUSICA *auxM;   
     
    auxB = L->ini;
    while(auxB != NULL)
    {
        fprintf(bandas, "%s/", auxB->nome);
        fprintf(bandas, "%s/", auxB->genero);
        fprintf(bandas, "%s/", auxB->descricao);
        if(auxB->prox == NULL)
            fprintf(bandas, "%d", auxB->ano);
        else
            fprintf(bandas, "%d\n", auxB->ano);  
         
        auxA = auxB->ini;
        while(auxA != NULL)
        {
            fprintf(albuns, "%s/", auxA->nome_album);
            if(auxA->prox == NULL)
                fprintf(albuns, "%d", auxA->ano_album);
            else
                fprintf(albuns, "%d\n", auxA->ano_album);    
             
            auxM = auxA->ini;
            while(auxM != NULL)
            {
                fprintf(musicas, "%s/", auxM->nome_musica);
                if(auxM->prox == NULL)
                    fprintf(musicas, "%s", auxM->duracao);
                else
                    fprintf(musicas, "%s\n", auxM->duracao); 
                 
                auxM = auxM->prox;
            }
            if(auxA->prox != NULL)
                fprintf(musicas, "\n;\n");  
             
            auxA = auxA->prox;   
        }
         
        if(auxB->prox == NULL)
        {
            fprintf(albuns, "\n;");
            fprintf(musicas, "\n---");
        }
        else
        {
            fprintf(albuns, "\n;\n");
            fprintf(musicas, "\n---\n");
        }
         
        auxB = auxB->prox;
    }   
}
 
/*------------------------------------- <fim> ARQUIVO -----------------------------------------*/












/*----------------------------------------- BANDA ---------------------------------------------*/
 
BANDA *cria_banda(char *nome, char *genero, char *descricao, int ano)
{
    if(nome == NULL || genero == NULL || descricao == NULL || ano <= 0)
        return NULL;
    BANDA *banda = MALLOC(BANDA);
    if(banda == NULL)
        return NULL;
    strcpy(banda->nome, nome);
    strcpy(banda->genero, genero);
    strcpy(banda->descricao, descricao);
    banda->ano = ano;
    banda->prox = NULL;
    banda->ini = NULL;
    return banda;
}

int esta_vazio_banda(Lista_banda* L) {
    if (L->ini == NULL)
        return VERDADEIRO;
    else
        return FALSO;
}

 
int cadastra_banda(Lista_banda *L, BANDA *B) {
  
	if(busca_banda(L, B->nome) != NULL)
        return ERRO;                    
    else{ 
        if (esta_vazio_banda(L)){
			L->ini = B;
			B->ini = NULL;
		}
        else{
            BANDA *aux = L->ini;
            while (aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = B;
            B->prox = NULL;
            B->ini = NULL;
        }
    return SUCESSO;
    }
}
 
 
int remove_banda(Lista_banda *L, BANDA *B, HASH *H) {
    if(L == NULL || B == NULL || H == NULL)
        return ARGUMENTO_INVALIDO;
     
    BANDA *p = L->ini;
     
    if(L->ini == NULL)
        return LISTA_SEM_BANDA;
     
    /*remocao do inicio*/
    if (!strcmp(L->ini->nome, B->nome))
    {
         
        while(L->ini->ini != NULL){   //remove todos os albuns da banda(com suas musicas)
            remove_album(B, L->ini->ini, H);
        }
         
        L->ini = L->ini->prox;
         
        free(p);    
    }
         
    /*remocao do meio ou fim*/
    else
    {   
        while (p->prox != NULL && strcmp(p->prox->nome,B->nome) != 0) 
            p = p->prox;
         
        if(p->prox == NULL)
            return BANDA_NAO_EXISTE;
         
        ALBUM *aux = p->prox->ini;
        while(aux != NULL)  //remove todos os albuns da banda(com suas musicas)
            remove_album(B,aux,H);      
         
        p->prox = p->prox->prox;   
    }
    return SUCESSO;
}

 
BANDA* busca_banda(Lista_banda *L, void *nome)//retorna ponteiro para banda procurada, nulo se nao achar
{
    if(L == NULL || nome == NULL )
        return NULL;
    BANDA *paux = L->ini;
    while(paux != NULL)
    {
        if(!strcmp(paux->nome, nome))
            return paux;
        paux = paux->prox;
    }
    return NULL;
}
 
/*------------------------------------- <fim> BANDA -----------------------------------------*/ 











/*---------------------------------------- ALBUM --------------------------------------------*/

ALBUM *cria_album(char *nome, int ano)
{
    if(nome == NULL || ano <=0 )
        return NULL;
    ALBUM *album = MALLOC(ALBUM);
    strcpy(album->nome_album, nome);
    album->ano_album = ano;
    album->prox = NULL;
    album->ini = NULL;

	return album;    
}

int esta_vazio_album (BANDA *B){
 
    if (B == NULL)
        return ARGUMENTO_INVALIDO;
 
    if (B->ini == NULL)
        return VERDADEIRO;
 
    return FALSO;
}


int cadastra_album (BANDA *B, ALBUM *A){
 
    //-------------------------------------------------------------
    // Validade dos argumentos
    if (B == NULL || A == NULL)
        return ARGUMENTO_INVALIDO;
    //=============================================================
 
    //-------------------------------------------------------------
    // Cadastra o álbum logo de primeira se é o único da banda
    if (esta_vazio_album(B)){
        
		B->ini = A;
        A->banda = B;
        return SUCESSO;
    }
    //=============================================================
 
    //--------------------------------------------------------------
    // Verifica se o álbum já não existe
    Lista_banda * Lista;
    Lista = MALLOC(Lista_banda);
    Lista->ini = B;
    if ( busca_album(Lista, A->nome_album) != NULL )
        return ALBUM_JA_EXISTE;
    //==============================================================
 
    //--------------------------------------------------------------
    // Percorre até o último álbum da banda e insere logo após
    ALBUM * auxAlbum;
    auxAlbum = B->ini;
 
    while ( auxAlbum->prox != NULL )
        auxAlbum = auxAlbum->prox;
 
	A->banda = B; 
    auxAlbum->prox = A;
    //==============================================================
 
    return SUCESSO;
 
}

int remove_album (BANDA *B, ALBUM *A, HASH *H){
  
    // Validade dos argumentos
    if (B == NULL || A == NULL)
        return ARGUMENTO_INVALIDO;
 
    // Banda sem álbuns
    if (esta_vazio_album(B))
        return BANDA_SEM_ALBUM;
  
     // Verifica se o álbum realmente existe
    Lista_banda * Lista;
    Lista = MALLOC(Lista_banda);
    Lista->ini = B;
    if ( busca_album(Lista, A->nome_album) == NULL )
        return ALBUM_NAO_EXISTE;   
	
	// Se o álbum é o primeiro, remove-o duma vez
    if (B->ini == A){
        MUSICA *aux = A->ini;
		while(aux != NULL){
            remove_musica(H, aux->nome_musica, B->nome); //remove todas as musicas do album(no hashing tb)
            
            aux = aux->prox;
        }
        B->ini = NULL;
        return SUCESSO;
    }
  
    // Percorre a lista de albuns até encontrá-lo, e então o remove
    ALBUM *auxAlbum;
    auxAlbum = B->ini;
    while (auxAlbum->prox != NULL  &&  strcmp(auxAlbum->prox->nome_album,A->nome_album) != 0)
        auxAlbum = auxAlbum->prox;
  
    MUSICA *aux2 = auxAlbum->prox->ini;
	while(aux2 != NULL){
        remove_musica(H, aux2->nome_musica, B->nome); //remove todas as musicas do album(no hashing tb)
    aux2 = aux2->prox;
	}
    
	auxAlbum->prox = A->prox;
    free(A);
  
    return SUCESSO;
  
}

ALBUM* busca_album(Lista_banda *L, char *album)//retorna ponteiro pro album procurado, nulo se nao achar
{
    if(L == NULL || album == NULL)
        return NULL;
    
    if(esta_vazio_banda(L))
        return NULL;
        
    BANDA *pauxBanda = L->ini;
    ALBUM *pauxAlbum = pauxBanda->ini;
    while(pauxBanda != NULL)
    {   
        while( pauxAlbum != NULL )
        {
            if(!strcmp(pauxAlbum->nome_album, album))
                return pauxAlbum;
            pauxAlbum = pauxAlbum->prox;
        }
        pauxBanda = pauxBanda->prox;
        if(pauxBanda != NULL)
            pauxAlbum = pauxBanda->ini;
    }
    return NULL;
}

/*---------------------------------------- <fim >ALBUM --------------------------------------------*/











/*------------------------------------------- MUSICA-----------------------------------------------*/

MUSICA *cria_musica(char *nome, char *caminho, char *duracao, int ano)
{
    if( nome == NULL || duracao == NULL || ano <=0 )//ignora caminho como obrigatorio
        return NULL;
    MUSICA *musica = (MUSICA*)malloc(sizeof(MUSICA));
    if(musica == NULL)
        return NULL;
    strcpy(musica->nome_musica, nome);
    if(caminho != NULL)
        strcpy(musica->caminho, caminho);
    strcpy(musica->duracao, duracao);
    musica->ano_musica = ano;
    musica->album = NULL;
    musica->prox=  NULL;
    musica->ant = NULL;
    return musica;
}

int esta_vazio_musica (ALBUM *A){
 
    if (A->ini == NULL) return SUCESSO;
     
return FALSO;
}

int cadastra_musica (ALBUM *A, MUSICA *M, HASH *H){
 
    if (A == NULL || M == NULL || H == NULL)
    	return ARGUMENTO_INVALIDO;
    	
    MUSICA *aux = A->ini;
    int posicao; 
	
	//aux = MALLOC(MUSICA);
    posicao = calcula_hash(M->nome_musica); //calcula em que posição deve ser inserida na hash
    M->album = A;
    
	if(esta_vazio_musica(A)){
 
 	    A->ini = M;
	    M->album = A;
	    M->ant = A->ini;
  
        Cadastra_HASH(&H,&M,posicao);
 
    } 
   
	else{
		
		while(aux->prox != NULL && strcmp(M->nome_musica,aux->nome_musica) != 0){
			aux = aux->prox;
			
		}
		
		if(!strcmp(M->nome_musica,aux->nome_musica) && !strcmp(M->album->banda->nome,aux->album->banda->nome))//se musica for igual e da mesma banda
			return MUSICA_JA_EXISTE;
			
		aux->prox = M;
		M->ant = aux;
		M->album = A;
		M->prox = NULL;	
	
		Cadastra_HASH(&H,&M,posicao);
	}   
	
	return SUCESSO; 
}

int remove_musica (HASH *H, char *musica, char *banda){

	int posicao = calcula_hash(musica);
	MUSICA_HASH *aux, *aux2;
	MUSICA *M;
	aux = H[posicao].ini;
	
	laco:
	while(aux != NULL  &&  strcmp(aux->nome,musica) != 0)
		aux = aux->prox;
	
		if(aux == NULL)
			return(MUSICA_N_EXISTE);
		
		if(strcmp(aux->ptr->album->banda->nome,banda))//se saiu do laco, nao for null, ainda tem que checar se eh da mesma banda
			goto laco;
			
	M = aux->ptr;
	
	if(M->album->ini == M)//inicio da lista
	{	
		M->album->ini = M->prox;
		free(M);
		
		if(aux->prox == NULL)//primeiro do hashing
		{
			H[posicao].ini = NULL;
			free(aux);	
		}
		else
		{
			while(aux2->prox != aux)//meio ou fim do hashing
				aux2 = aux2->prox;
			
			aux2->prox = aux->prox;
			free(aux);	
		}		
		
		return(SUCESSO);
	}
	
	else if(M->prox == NULL)//fim da lista
	{
		M->ant->prox = NULL;
		free(M);
		
		if(aux->prox == NULL)//primeiro do hashing
		{
			H[posicao].ini = NULL;
			free(aux);	
		}
		else
		{
			while(aux2->prox != aux)//meio ou fim do hashing
				aux2 = aux2->prox;
			
			aux2->prox = aux->prox;
			free(aux);	
		}
	
		return(SUCESSO);	
	}
	
	else//meio da lista
	{
		M->ant->prox = M->prox;
		free(M);
		
		if(aux->prox == NULL)//primeiro do hashing
		{
			H[posicao].ini = NULL;
			free(aux);	
		}
		else
		{
			while(aux2->prox != aux)//meio ou fim do hashing
				aux2 = aux2->prox;
			
			aux2->prox = aux->prox;
			free(aux);	
		}
		
		return(SUCESSO);
	}
}

MUSICA** busca_musica(HASH *H, char *musica)
{
    int i=0;
    if( H == NULL || musica == NULL)
        return NULL;
    MUSICA **musicas = (MUSICA**)malloc(100*sizeof(MUSICA*));
    if(musicas == NULL)
        return NULL;
   MUSICA_HASH *paux = H[calcula_hash(musica)].ini;
    while(paux != NULL)
    {
        if(!strcmp(paux->nome, musica))
            musicas[i] = paux->ptr;	
		 paux = paux->prox;
		 i++;
    }
    musicas[i] = NULL;
    return musicas;
}

/*------------------------------------------- <fim>MUSICA-----------------------------------------------*/


















/*------------------------------------------- Restante... -----------------------------------------------*/

Lista_banda *cria_lista(void)
{
    Lista_banda *lista = MALLOC(Lista_banda);
    if(lista == NULL)
        return NULL;
    lista->ini = NULL;
    return lista;
}
 
// Retorna NULL ou o ponteiro para o vetor, *e erro ou se sucesso, tamanho do vetor
BANDA **filtra_decada_banda (Lista_banda *L, int *e, int *decada){
 
    /*****
     * REESCREVENDO ESTA FUNÇÂO -- MESMO ERRO DA FILTRA BANDA GENERO
     * 
     **/
 
    //-----------------------------------------------------------------
    // Validade dos argumentos
    if (L == NULL || decada == NULL){
        *e = ARGUMENTO_INVALIDO;
        return NULL;
    }
    //=================================================================
 
    //-----------------------------------------------------------------
    // Checa se a lista contém bandas
    if (esta_vazio_banda(L)){
        *e = LISTA_SEM_BANDA;
        return NULL;
    }
    //=================================================================
     
    //-----------------------------------------------------------------
    // Conta e armazena as bandas filtradas
    int i = 0, c = 0;
    BANDA * Banda;
    BANDA ** B;
     
     
    Banda = L->ini;
 
    while (Banda != NULL){
        if ( (Banda->ano >= *decada) && (Banda->ano < *decada + 10) ){
            c++;
        }
        Banda = Banda->prox;
    }
    B = MALLOC_SIZE(BANDA*,c);
    if(B == NULL){
        *e = ERRO_MEMORIA;
        return NULL;
    }
     
    Banda = L->ini;
    while (Banda != NULL){
        if ( (Banda->ano >= *decada) && (Banda->ano < *decada + 10) ){
            B[i] = Banda;
            i++;
        }
        Banda = Banda->prox;
    }
    //=================================================================
     
    QSort_B_Data(B, 0, c-1);
    *e = c;
    return B;
}
 
// Retorna NULL ou ponteiro para vetor, *e erro ou se sucesso, tamanho do vetor
ALBUM **filtra_decada_album (Lista_banda *L, int *e, int *decada){
 
    //--------------------------------------------
    // Validade dos argumentos
    if (L == NULL || decada == NULL){
        *e = ARGUMENTO_INVALIDO;
        return NULL;
    }
    //============================================
 
    //--------------------------------------------
    // Lista sem bandas
    if (esta_vazio_banda(L)){
        *e = LISTA_SEM_BANDA;
        return NULL;
    }
    //============================================
 
    //--------------------------------------------
    // Conta e Armazena os álbuns filtrados
    int i = 0, c = 0;
    BANDA * Banda;
    ALBUM * Album;
    ALBUM ** A;
    Banda = L->ini;
     
    //Conta Albuns
    while (Banda != NULL){
        if (!esta_vazio_album(Banda)){
            Album = Banda->ini;
            // Percorre cada álbum
            while (Album != NULL){
                if ( (Album->ano_album >= *decada) && (Album->ano_album < *decada + 10) ){
                    c++;
                }
                Album = Album->prox;
            }
        }
        Banda = Banda->prox;
    }
     
    //Aloca Memória para A
    A = MALLOC_SIZE(ALBUM*,c);
    if(A == NULL){
        *e = ERRO_MEMORIA;
        return NULL;
    }
     
    // Percorre cada Banda
    Banda = L->ini;
    while (Banda != NULL){
        if (!esta_vazio_album(Banda)){
            Album = Banda->ini;
            // Percorre cada álbum
            while (Album != NULL){
                if ( (Album->ano_album >= *decada) && (Album->ano_album < *decada + 10) ){
                    A[i] = Album;
                    i++;
                }
                Album = Album->prox;
            }
        }
        Banda = Banda->prox;
    }
    //==========================================
     
    QSort_A_Data(A, 0, c-1);
    *e = c;
    return A;
}
 
//Retorna NULL ou ponteiro para vetor, *e erro ou se sucesso, tamanho do vetor
BANDA **filtra_genero_banda (Lista_banda *L, int *e, char *genero){
    if(L == NULL){
        *e = ARGUMENTO_INVALIDO;
        return NULL;
    }
    int c = 0;
    int i = 0;
    BANDA *aux;
    BANDA **B;
     
    /* Determina número de bandas */
    aux = L->ini;
    while(aux != NULL){
 
        if(strcmp(aux->genero, genero) == 0)
            c++;
        aux = aux->prox;
    }
     
    /* Se não encontrou nenhuma banda do genero requisitado */
    if(c == 0){
        *e = ERRO;
        return NULL;
    }
    /* Aloca vetor de ponteiros */
    B = MALLOC_SIZE(BANDA*,c);
    if(B == NULL){
        *e = ERRO_MEMORIA;
        return NULL;
    }
    /* Carrega ponteiros no vetor */
    aux = L->ini;
    while(aux != NULL){
        if(strcmp(aux->genero, genero) == 0){
            B[i] = aux;
             
            printf("I %d B %s\n", i, B[i]->nome);
            i++;
        }
        aux = aux->prox;
    }
     
    QSort_B_Nome(B, 0, c-1);
    *e = c;
    return B;
}

int calcula_hash(char *musica){//ROGIEL
 
    int i=0,soma=0;
     
    while(musica[i]!='\0'){
         
        if(musica[i]==' '); // caso o usuario digite a mesma palamvra, com espaços diferente
        else soma+= (int)musica[i]; 
        i++;    
    }   
    if (soma < 0) soma= (-1)*soma; // trata os casos em que a palavra possui acento
     
return soma%MOD;  // retorna a posição da hash
}
 

void Cria_Hash(HASH **H){ //ROGIEL
      
     int i; 
     (*H) = MALLOC_SIZE(HASH,MOD);
      
    for(i=0; i<MOD; i++) (*H)[i].ini = NULL; 
}

int Cadastra_HASH(HASH **H, MUSICA **M, int posicao){
 
 	if ( H == NULL || M == NULL || posicao < 0 )
 		return ARGUMENTO_INVALIDO;
 		
	MUSICA_HASH *mu, *aux = (*H)[posicao].ini;
	mu = MALLOC(MUSICA_HASH); //bloquinho
	
	strcpy(mu->nome,(*M)->nome_musica);
	mu->ptr = (*M);
	
	if(aux == NULL)
	{
		(*H)[posicao].ini = mu;
		mu->prox = NULL;
	}
	
	else
	{
		while(aux->prox != NULL)
			aux = aux->prox;
			
		aux->prox = mu;
		mu->prox = NULL;		
	}

	return SUCESSO;
}

/*separa os sub-grupos e troca*/
int separa_Bnome(BANDA **V, int ini, int fim){
    int i, j;
    BANDA *aux;
     
    i = ini;
    for (j = ini+1; j <= fim; j++){
        if (strcmp((*V[j]).nome, (*V[ini]).nome) < 0){
            i++;
            aux = V[i];
            V[i] = V[j];
            V[j] = aux;
        }
    }
    aux = V[ini];
    V[ini] = V[i];
    V[i] = aux;
 
    return i;
}
 
void QSort_B_Nome(BANDA **V, int ini, int fim){
    int r;
 
    if(fim > ini){
        r = separa_Bnome(V, ini, fim);
        QSort_B_Nome(V, ini, r - 1);
        QSort_B_Nome(V, r + 1, fim);
    }
}
 

/*separa os sub-grupos e troca*/
int separa_data(BANDA **V, int ini, int fim){
    int i, j;
    BANDA *aux;
     
    i = ini;
    for (j = ini+1; j <= fim; j++){
        if ((*V[j]).ano < (*V[ini]).ano){
            i++;
            aux = V[i];
            V[i] = V[j];
            V[j] = aux;
        }
    }
    aux = V[ini];
    V[ini] = V[i];
    V[i] = aux;
 
    return i;
}
 
void QSort_B_Data(BANDA **V, int ini, int fim){
    int r;
 
    if(fim > ini){
        r = separa_data(V, ini, fim);
        QSort_B_Nome(V, ini, r - 1);
        QSort_B_Nome(V, r + 1, fim);
    }
}
 
 
/*separa os sub-grupos e troca*/
int separa_Adata(ALBUM **V, int ini, int fim){
    int i, j;
    ALBUM *aux;
     
    i = ini;
    for (j = ini+1; j <= fim; j++){
        if ((*V[j]).ano_album < (*V[ini]).ano_album){
            i++;
            aux = V[i];
            V[i] = V[j];
            V[j] = aux;
        }
    }
    aux = V[ini];
    V[ini] = V[i];
    V[i] = aux;
 
    return i;
}

int Destroy(Lista_banda *L, HASH *H){
    if (L == NULL || H == NULL)
        return ARGUMENTO_INVALIDO;
 
    while(L->ini != NULL){
        remove_banda(L, L->ini, H);
    }
 
    free(L);    
     
    return SUCESSO;
}
 
void QSort_A_Data(ALBUM **V, int ini, int fim){
    int r;
 
    if(fim > ini){
        r = separa_Adata(V, ini, fim);
        QSort_A_Data(V, ini, r - 1);
        QSort_A_Data(V, r + 1, fim);
    }
}


