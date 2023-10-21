
// ----------------------------------------------------------------------------------------------------------------------------
// Inicio do código fornecido pelo professor
// Não adicione ou remova nada.

// Se estiver no Windows e não conseguir ver a imagem colorida, pode ser que seu terminal não suporte Ascii Escape Colors.
// Para habilitá-la, execute o comando abaixo no terminal com permissão de Administrador.
// Após executado, o terminado deve ser fechado e aberto novamente para ter o suporte das cores habilitado.
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

// Nome do arquivo de saída temporório da imagem
#define ARQUIVO_IMAGEM_TMP  "ascii_art.txt"

/**
 *  \brief Função que carrega uma imagem informada na URL.
 *  
 *  \param [in] colorido Define se a imagem será colorida.
 *  \param [in] largura Define a largura da imagem gerada.
 *  \return Endereço da estrutura com a imagem. 
 *          Caso a imagem não tenha sido carregada corretamente, a função
 *          retornará NULL.
 */
asciiImg_t ** insta_carregaImagem(char url[], bool colorido, int largura,int num_imagens) {
  
  FILE * arquivo;
  char buffer[BUFFER_TAMANHO];
  int nBytes, nBytesTotal = 0;
  char linhaComando[LINHA_COMANDO];
  int i;

  asciiImg_t ** img;
  
  // Aloca espaço para uma imagem
  for (i=0;i<num_imagens;i++){
    img[i] = malloc(sizeof(asciiImg_t) * num_imagens);
    if (img[i] == NULL) return NULL;
  
    // Inicializa a estrutura
    img[i]->bytes = NULL;
    img[i]->nBytes = 0;
  }
  

  
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
      
      // Descobre o número de bytes da linha
      for(nBytes = 0; buffer[nBytes] != 0; nBytes++);
      for (i=0;i<num_imagens;i++){
        // Aloca o espaço
        img[i]->bytes = realloc(img[i]->bytes, sizeof(unsigned char) * (nBytesTotal + nBytes));
      
        // Copia para o espaço alocado
        (void)memcpy(&(img[i]->bytes[nBytesTotal]), buffer, nBytes);
        nBytesTotal+=nBytes;
            // Finaliza a imagem colocando o \0 final e o tamanho
        img[i]->bytes = realloc(img[i]->bytes, sizeof(unsigned char) * (nBytesTotal + 1));
        img[i]->bytes[nBytesTotal++] = '\0';
        img[i]->nBytes = nBytesTotal;
      }



    }
      
    
    // Fecha o arquivo
    fclose(arquivo);
  }
  
    for (i=0;i<num_imagens;i++){
      // Verifica se a imagem é válida
      if (img[i]->nBytes < LIMIAR_INFERIOR_TAMANHO_IMAGEM) {
        // Libera todo o espaço alocado
        free(img[i]->bytes);
        free(img[i]);
      }
    
    return NULL;
  }
  
  // Retorna a imagem carregada
  return img;
}

/**
 *  \brief Função que imprime uma Imagem ASCII.
 *  
 *  \param [in] img Endereço da estrutura com os dados da imagem.
 */
void insta_imprimeImagem(asciiImg_t * img) {
  printf("%s", img->bytes);
}

/**
 *  \brief Função que libera a memória alocada por uma imagem.
 *  
 *  \param [in] img Endereço da estrutura com os dados da imagem a ser liberada.
 */
void insta_liberaImagem(asciiImg_t ** img,int num_imagens) {
  int i;
  for (i=0;i<num_imagens;i++){
    free(img[i]->bytes);
    free(img[i]);

  }
  
}

// Fim do código fornecido pelo professor
// ----------------------------------------------------------------------------------------------------------------------------

/* Inclusões */
#include <stdio.h>

/* Erros */

// Constante que associa o código de erro 0 a execução esperada.
#define SUCESSO 0

// Falha ao carregar a imagem fornecida
#define ERRO_CARREGAR_IMAGEM 1

/* Constantes */

// Número de colunas da imagem
#define IMAGEM_NUMERO_COLUNAS     120

// Definição de imagem colorida
#define IMAGEM_COLORIDA           true
// Definição de imagem preto/branco
#define IMAGEM_PRETO_BRANCO      false
// Definição de imagem utilizada
#define MODO_IMAGEM               IMAGEM_COLORIDA

/**
 *  \brief Função principal.
 *  
 *  \param [in] argc Número de argumentos.
 *  \param [in] argv Valores dos argumentos.
 *  \return Código de erro indicando o que aconteceu com o programa.
 */
//Número de caracteres da url
#define URL 1000
//Função para tirar o '\n' das strings
void util_removeQuebraLinhaFinal(char dados[]) {
    int tamanho;
    tamanho = strlen(dados);
    if ((tamanho > 0) && (dados[tamanho - 1] == '\n')) {
        dados[tamanho - 1] = '\0';
    }
}
int main(int argc, char ** argv) {
  
  asciiImg_t ** img;
  char  url[URL];
  int num_imagens = 0;
  int i;
  

  printf ("Digite o numero de imagens que voce quer imprimir:\n");
  scanf ("%d", &num_imagens);
  getchar();
  for (i=0;i<num_imagens;i++){
    fgets(url, URL, stdin);
    util_removeQuebraLinhaFinal(url);
    img[i] = insta_carregaImagem(url, MODO_IMAGEM, IMAGEM_NUMERO_COLUNAS, num_imagens);
    if (img[i] == NULL) {
      // Falha ao carregar a imagem
      fprintf(stderr, "Falha ao carregar a imagem da URL %s\n", url);
      return ERRO_CARREGAR_IMAGEM;
  }
  
  // Mostra a imagem, o número de bytes e libera a memória
  insta_imprimeImagem(img[i]);
  printf("N.Bytes Imagem: %d\n", img[i]->nBytes);
  insta_liberaImagem(img[i],num_imagens);

  }

  
  // Se chegou até aqui é porque deu tudo certo
  return SUCESSO;
}