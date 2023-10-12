
// ----------------------------------------------------------------------------------------------------------------------------
// In�cio do c�digo fornecido pelo professor
// N�o adicione ou remova nada.

// Se estiver no Windows e n�o conseguir ver a imagem colorida, pode ser que seu terminal n�o suporte Ascii Escape Colors.
// Para habilit�-la, execute o comando abaixo no terminal com permiss�o de Administrador.
// Ap�s executado, o terminado deve ser fechado e aberto novamente para ter o suporte �s cores habilitado.
// Comando:
// reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/// Estrutura que representa uma imagem em Ascii
struct asciiImg_s {
  uint8_t * bytes;
  int nBytes;
};

/// Tipo "Imagem ASCII"
typedef struct asciiImg_s asciiImg_t;

// Tamanho do buffer para cada linha da imagem
#define BUFFER_TAMANHO  50000

// Tamanho m�ximo da linha de comando
#define LINHA_COMANDO   10000

// Limiar inferior que considera o download com sucesso
#define LIMIAR_INFERIOR_TAMANHO_IMAGEM  500

// Nome do execut�vel da ferramenta de download e convers�o da imagem
#ifdef __unix__ 
#define FERRAMENTA_IMAGEM   "./ascii-image-converter.bin"
#else
#define FERRAMENTA_IMAGEM   "ascii-image-converter.exe"
#endif

// Nome do arquivo de sa�da tempor�rio da imagem
#define ARQUIVO_IMAGEM_TMP  "ascii_art.txt"

/**
 *  \brief Fun��o que carrega uma imagem informada na URL.
 *  
 *  \param [in] colorido Define se a imagem ser� colorida.
 *  \param [in] largura Define a largura da imagem gerada.
 *  \return Endere�o da estrutura com a imagem. 
 *          Caso a imagem n�o tenha sido carregada corretamente, a fun��o
 *          retornar� NULL.
 */
asciiImg_t * insta_carregaImagem(char url[], bool colorido, int largura) {
  
  FILE * arquivo;
  char buffer[BUFFER_TAMANHO];
  int nBytes, nBytesTotal = 0;
  char linhaComando[LINHA_COMANDO];

  asciiImg_t * img;
  
  // Aloca espa�o para uma imagem
  img = malloc(sizeof(asciiImg_t));
  if (img == NULL) return NULL;
  
  // Inicializa a estrutura
  img->bytes = NULL;
  img->nBytes = 0;
  
  // Monta a linha de comando
  (void)sprintf(linhaComando, "%s %s %s -W %d -c > %s", FERRAMENTA_IMAGEM, url, (colorido ? "-C" : ""), largura, ARQUIVO_IMAGEM_TMP);
  
  // Chama o programa para fazer o download da imagem
  (void)system(linhaComando);

  // Tenta abrir o arquivo recem criado
  arquivo = fopen(ARQUIVO_IMAGEM_TMP, "r");
  if (arquivo != NULL) {
    
    while(!feof(arquivo)) {
      
      // Limpa a linha
      (void)memset(buffer, 0, sizeof(buffer));
      
      // Tenta ler uma linha
      if (fgets(buffer, BUFFER_TAMANHO, arquivo) == NULL) continue;
      
      // Descobre o n�mero de bytes da linha
      for(nBytes = 0; buffer[nBytes] != 0; nBytes++);
      
      // Aloca o espa�o
      img->bytes = realloc(img->bytes, sizeof(unsigned char) * (nBytesTotal + nBytes));
      
      // Copia para o espa�o alocado
      (void)memcpy(&(img->bytes[nBytesTotal]), buffer, nBytes);
      nBytesTotal+=nBytes;
    }

    // Finaliza a imagem colocando o \0 final e o tamanho
    img->bytes = realloc(img->bytes, sizeof(unsigned char) * (nBytesTotal + 1));
    img->bytes[nBytesTotal++] = '\0';
    img->nBytes = nBytesTotal;
    
    // Fecha o arquivo
    fclose(arquivo);
  }
  
  // Verifica se a imagem � v�lida
  if (img->nBytes < LIMIAR_INFERIOR_TAMANHO_IMAGEM) {
    // Libera todo o espa�o alocado
    free(img->bytes);
    free(img);
    
    return NULL;
  }
  
  // Retorna a imagem carregada
  return img;
}

/**
 *  \brief Fun��o que imprime uma Imagem ASCII.
 *  
 *  \param [in] img Endere�o da estrutura com os dados da imagem.
 */
void insta_imprimeImagem(asciiImg_t * img) {
  printf("%s", img->bytes);
}

/**
 *  \brief Fun��o que libera a mem�ria alocada por uma imagem.
 *  
 *  \param [in] img Endere�o da estrutura com os dados da imagem a ser liberada.
 */
void insta_liberaImagem(asciiImg_t * img) {
  free(img->bytes);
  free(img);
}

// Fim do c�digo fornecido pelo professor
// ----------------------------------------------------------------------------------------------------------------------------

/* Inclus�es */
#include <stdio.h>

/* Erros */

// Constante que associa o c�digo de erro 0 a execu��o esperada.
#define SUCESSO 0

// Falha ao carregar a imagem fornecida
#define ERRO_CARREGAR_IMAGEM 1

/* Constantes */

// N�mero de colunas da imagem
#define IMAGEM_NUMERO_COLUNAS     120

// Defini��o de imagem colorida
#define IMAGEM_COLORIDA           true
// Defini��o de imagem preto/branco
#define IMAGEM_PRETO_BRANCO      false
// Defini��o de imagem utilizada
#define MODO_IMAGEM               IMAGEM_COLORIDA

/**
 *  \brief Fun��o principal.
 *  
 *  \param [in] argc N�mero de argumentos.
 *  \param [in] argv Valores dos argumentos.
 *  \return C�digo de erro indicando o que aconteceu com o programa.
 */
int main(int argc, char ** argv) {
  
  asciiImg_t * img;
  char * url;
  
  if (argc < 2) {
    printf("Modo de Uso:\n"\
           "%s URL_IMAGEM\n",
           argv[0]
    );
    return SUCESSO;
  }
  
  // Carrega a imagem
  url = argv[1];
  img = insta_carregaImagem(url, MODO_IMAGEM, IMAGEM_NUMERO_COLUNAS);
  if (img == NULL) {
    // Falha ao carregar a imagem
    fprintf(stderr, "Falha ao carregar a imagem da URL %s\n", url);
    return ERRO_CARREGAR_IMAGEM;
  }
  
  // Mostra a imagem, o n�mero de bytes e libera a mem�ria
  insta_imprimeImagem(img);
  printf("N.Bytes Imagem: %d\n", img->nBytes);
  insta_liberaImagem(img);
  
  // Se chegou at� aqui � porque deu tudo certo
  return SUCESSO;
}