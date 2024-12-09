/*                                             COLTEGRAM
                                    FEITO A PARTIR DE PROGRAMAÇÂO EM C

LEMBRETES:
-Rodar sed -i 's/\r//g'  dadosColtegram.txt no terminal do linux para trnsferir informações do Windows para o Linux;
-Guardar o arquivo dadosColtegram.txt tanto na pasta main e na pasta output;
-As ferramentas ascii-image-converter.bin, ascii-image-converter.exe e ascii_art.txt devem estar presentes na pasta;
-Rodar chmod +x ascii-image-converter.bin para imprimir as imagens no linux
-------------------------------------------------------------------------------------------------------------------------------------------------------
*/

// Bibliotecas presentes no programa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

// Constantes presentes
#define SUCESSO 0
#define ERRO 1
#define Sem_Postagens 1
#define Tamanho_Maximo 100
#define NUM_MAX_CARACTERES_NOME_USUARIO (99 + 1)
#define NUM_MAX_CARACTERES_EMAIL (50 + 1)
#define NUM_MAX_CARACTERES_SENHA (50 + 1)
#define NUM_MAX_CARACTERES_ID (50 + 1)
#define MAX_IMAGENS (1000)
#define NUM_MAX_CARACTERES_LEGENDA (299 + 1)
#define NUM_MAX_CARACTERES_COMENTARIO (299 + 1)
// Tamanho do buffer para cada linha da imagem
#define BUFFER_TAMANHO 50000

// Tamanho máximo da linha de comando
#define LINHA_COMANDO 10000

// Limiar inferior que considera o download com sucesso
#define LIMIAR_INFERIOR_TAMANHO_IMAGEM 500

// Nome do executável da ferramenta de download e conversão da imagem
#ifdef __unix__
#define FERRAMENTA_IMAGEM "./ascii-image-converter.bin"
#else
#define FERRAMENTA_IMAGEM "ascii-image-converter.exe"
#endif

// Nome do arquivo de saída temporário da imagem
#define ARQUIVO_IMAGEM_TMP "ascii_art.txt"
// Falha ao carregar a imagem fornecida
#define ERRO_CARREGAR_IMAGEM 1

/* Constantes */

// Número de colunas da imagem
#define IMAGEM_NUMERO_COLUNAS 120

// Definição de imagem colorida
#define IMAGEM_COLORIDA true
// Definição de imagem preto/branco
#define IMAGEM_PRETO_BRANCO false
// Definição de imagem utilizada
#define MODO_IMAGEM IMAGEM_COLORIDA
// Define o tamanho máximo para a imagem
#define NUM_MAX_IMAGEM (999 + 1)
// Define o erro
#define USUARIO_INVALIDO -1
#define MAX_NUMERO_COMENTARIOS 100

/**
 *  \brief Função principal.
 *
 *  \param [in] argc Número de argumentos.
 *  \param [in] argv Valores dos argumentos.
 *  \return Código de erro indicando o que aconteceu com o programa.
 */
/**
 *  \brief Função que carrega uma imagem informada na URL.
 *
 *  \param [in] colorido Define se a imagem será colorida.
 *  \param [in] largura Define a largura da imagem gerada.
 *  \return Endereço da estrutura com a imagem.
 *          Caso a imagem não tenha sido carregada corretamente, a função
 *          retornará NULL.
 */

/// Estrutura que representa uma imagem em Ascii
struct asciiImg_s{
    uint8_t *bytes;
    int nBytes;
};
/// Tipo "Imagem ASCII"
typedef struct asciiImg_s asciiImg_t;
// Estrutura para o perfil
typedef struct perfil_s{
    char ID[NUM_MAX_CARACTERES_ID];
    char nome_usuario[NUM_MAX_CARACTERES_NOME_USUARIO];
    char email[NUM_MAX_CARACTERES_EMAIL];
    char senha[NUM_MAX_CARACTERES_SENHA];
    char senha_confirmada[NUM_MAX_CARACTERES_SENHA];
    int numeroDePostagens;
} perfil_t;
// Estrutura para login
typedef struct login_s{
    char ID_login[NUM_MAX_CARACTERES_ID];
    char nome_usuario_login[NUM_MAX_CARACTERES_NOME_USUARIO];
    char email_login[NUM_MAX_CARACTERES_EMAIL];
    char senha_login[NUM_MAX_CARACTERES_SENHA];
} login_t;

// Estrutura para comentarios
typedef struct comentario_s{
    char ** mensagem;
    int numero_comentarios;
    char ** perfil_que_comentou;
} comentario_t;

// Estrutura para curtidas
typedef struct curtida_s{
    char id_curtida[NUM_MAX_CARACTERES_ID];
    bool curtida;
    int num_curtidas;
} curtida_t;

// Estrutura para posts
typedef struct posts_s{
    char ID_post[NUM_MAX_CARACTERES_ID];
    asciiImg_t **img;
    char url[NUM_MAX_IMAGEM][MAX_IMAGENS];
    char legenda[NUM_MAX_CARACTERES_LEGENDA];
    comentario_t comentario;
    curtida_t curtidas;
    int num_imagens;
    int Numero_De_Fotos;
} posts_t;
// Função para tirar o '\n' das strings
void util_removeQuebraLinhaFinal(char dados[]){
    int tamanho;
    tamanho = strlen(dados);
    if ((tamanho > 0) && (dados[tamanho - 1] == '\n')){
        dados[tamanho - 1] = '\0';
    }
}

asciiImg_t *insta_carregaImagem(char url[], bool colorido, int largura){

    FILE *arquivo;
    char buffer[BUFFER_TAMANHO];
    int nBytes, nBytesTotal = 0;
    char linhaComando[LINHA_COMANDO];

    asciiImg_t *img;

    // Aloca espaço para uma imagem
    img = malloc(sizeof(asciiImg_t));
    if (img == NULL)
        return NULL;
    // Inicializa a estrutura
    img->bytes = NULL;
    img->nBytes = 0;

    // Monta a linha de comando
    (void)sprintf(linhaComando, "%s %s %s -W %d -c > %s", FERRAMENTA_IMAGEM, url, (colorido ? "-C" : ""), largura, ARQUIVO_IMAGEM_TMP);

    // Chama o programa para fazer o download da imagem
    (void)system(linhaComando);

    // Tenta abrir o arquivo recem criado
    arquivo = fopen(ARQUIVO_IMAGEM_TMP, "r");
    if (arquivo != NULL){

        while (!feof(arquivo)){

            // Limpa a linha
            (void)memset(buffer, 0, sizeof(buffer));

            // Tenta ler uma linha
            if (fgets(buffer, BUFFER_TAMANHO, arquivo) == NULL)
                continue;

            // Descobre o número de bytes da linha
            for (nBytes = 0; buffer[nBytes] != 0; nBytes++)
                ;

            // Aloca o espaço
            img->bytes = realloc(img->bytes, sizeof(unsigned char) * (nBytesTotal + nBytes));

            // Copia para o espaço alocado
            (void)memcpy(&(img->bytes[nBytesTotal]), buffer, nBytes);
            nBytesTotal += nBytes;
        }

        // Finaliza a imagem colocando o \0 final e o tamanho
        img->bytes = realloc(img->bytes, sizeof(unsigned char) * (nBytesTotal + 1));
        img->bytes[nBytesTotal++] = '\0';
        img->nBytes = nBytesTotal;

        // Fecha o arquivo
        fclose(arquivo);
    }

    // Verifica se a imagem é válida
    if (img->nBytes < LIMIAR_INFERIOR_TAMANHO_IMAGEM){
        // Libera todo o espaço alocado
        free(img->bytes);
        free(img);

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
void insta_imprimeImagem(asciiImg_t *img){
    if (img == NULL)
        return;
    printf("%s", img->bytes);
}

/**
 *  \brief Função que libera a memória alocada por uma imagem.
 *
 *  \param [in] img Endereço da estrutura com os dados da imagem a ser liberada.
 */
void insta_liberaImagem(asciiImg_t *img){
    free(img->bytes);
    free(img);
}
void comecar_usuario_com_NULL(posts_t ***ponteiro_postagem, int num_perfis){

    *ponteiro_postagem = (posts_t **)realloc(*ponteiro_postagem, num_perfis * sizeof(posts_t *));
    (*ponteiro_postagem)[num_perfis - 1] = NULL;
}

void alocarMatriz(posts_t ***ponteiro_postagem, int num_postagens, int posicao_usuario_logado, int numero_perfis){

    *ponteiro_postagem = (posts_t **)realloc(*ponteiro_postagem, numero_perfis * sizeof(posts_t *));
    (*ponteiro_postagem)[posicao_usuario_logado] = (posts_t *)realloc((*ponteiro_postagem)[posicao_usuario_logado], (num_postagens + 1) * sizeof(posts_t));
}

int lerPostagensArquivo(posts_t ***ponteiro_postagem, int numeroPerfil, int totalDePostagens){

    FILE *arquivoPostagem;

    char nome_Do_Arquivo[Tamanho_Maximo];

    posts_t *postagens_Perfil = NULL;

    int posicao_Da_Postagem = 0;
    int j;

    sprintf(nome_Do_Arquivo, "%d.txt", numeroPerfil);

    arquivoPostagem = fopen(nome_Do_Arquivo, "r");

    if (arquivoPostagem == NULL){
        printf("Erro ao abrir postagens ou postagens inexistentes!\n");
        return ERRO;
    }

    if (totalDePostagens == 0){
        return Sem_Postagens;
    }

    postagens_Perfil = (posts_t *)malloc(totalDePostagens * sizeof(posts_t));

    while (posicao_Da_Postagem <= totalDePostagens){  //Aqui a condiçao eh ler ate chegar no final do arquivo ex: 0.txt

        fgets(postagens_Perfil[posicao_Da_Postagem].ID_post, NUM_MAX_CARACTERES_ID, arquivoPostagem);  //Pegou o id da postagem
        util_removeQuebraLinhaFinal(postagens_Perfil[posicao_Da_Postagem].ID_post);

        if (fscanf(arquivoPostagem, "%d", &postagens_Perfil[posicao_Da_Postagem].Numero_De_Fotos) != true){ //Pegou o numero de fotos da postagem
            break;
        };
        fgetc(arquivoPostagem);
        
        if (postagens_Perfil[posicao_Da_Postagem].Numero_De_Fotos > 0) {
            postagens_Perfil[posicao_Da_Postagem].img = malloc(sizeof(asciiImg_t *) * postagens_Perfil[posicao_Da_Postagem].Numero_De_Fotos); //O problema deve estar aqui
        } else {
            postagens_Perfil[posicao_Da_Postagem].img = NULL;
        }


        for (j = 0; j < postagens_Perfil[posicao_Da_Postagem].Numero_De_Fotos; j++){  //Condição esta dizendo, repita essa parada ate o numero de fotos -1

            fgets(postagens_Perfil[posicao_Da_Postagem].url[j], MAX_IMAGENS, arquivoPostagem);      //Pega o link logo abaixo do numero de fotos e joga na posiçao j que começa com 0
            util_removeQuebraLinhaFinal(postagens_Perfil[posicao_Da_Postagem].url[j]);
            postagens_Perfil[posicao_Da_Postagem].img[j] = insta_carregaImagem(postagens_Perfil[posicao_Da_Postagem].url[j], MODO_IMAGEM, IMAGEM_NUMERO_COLUNAS);
        }
        
        fgets(postagens_Perfil[posicao_Da_Postagem].legenda, NUM_MAX_CARACTERES_LEGENDA, arquivoPostagem);
        util_removeQuebraLinhaFinal(postagens_Perfil[posicao_Da_Postagem].legenda);

        posicao_Da_Postagem++;
    }

    *ponteiro_postagem = (posts_t **)realloc(*ponteiro_postagem, (numeroPerfil + 1) * sizeof(posts_t *));

    (*ponteiro_postagem)[numeroPerfil] = (posts_t *)realloc((*ponteiro_postagem)[numeroPerfil], totalDePostagens * sizeof(posts_t));

    (*ponteiro_postagem)[numeroPerfil] = postagens_Perfil;

    fclose(arquivoPostagem);

    return SUCESSO;
}

int funcaoLerArquivo(perfil_t **ponteiro_perfil, int *num_perfis, posts_t ***ponteiro_postagem){

    perfil_t *VetorPerfil = NULL;

    FILE *arquivo;

    int numero_de_postagens = 0;

    arquivo = fopen("dadosColtegram.txt", "r+");

    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo");
        return ERRO;
    }

    // Lê os dados
    while (true){

        if (feof(arquivo)){
            break;
        }

        VetorPerfil = (perfil_t *)realloc(VetorPerfil, sizeof(perfil_t) * (*num_perfis + 1));

        if (fgets(VetorPerfil[*num_perfis].ID, NUM_MAX_CARACTERES_ID, arquivo) == NULL){
            break;
        };

        util_removeQuebraLinhaFinal(VetorPerfil[*num_perfis].ID);

        if (fgets(VetorPerfil[*num_perfis].nome_usuario, NUM_MAX_CARACTERES_NOME_USUARIO, arquivo) == NULL){
            break;
        };

        util_removeQuebraLinhaFinal(VetorPerfil[*num_perfis].nome_usuario);

        if (fgets(VetorPerfil[*num_perfis].email, NUM_MAX_CARACTERES_EMAIL, arquivo) == NULL){
            break;
        };
        util_removeQuebraLinhaFinal(VetorPerfil[*num_perfis].email);

        if (fgets(VetorPerfil[*num_perfis].senha, NUM_MAX_CARACTERES_SENHA, arquivo) == NULL){
            break;
        };
        util_removeQuebraLinhaFinal(VetorPerfil[*num_perfis].senha);

        if (fscanf(arquivo, "%d", &VetorPerfil[*num_perfis].numeroDePostagens) != true){
            break;
        };
        fgetc(arquivo);

        if (feof(arquivo)){
            break;
        }

        numero_de_postagens += VetorPerfil[*num_perfis].numeroDePostagens;

        (*num_perfis)++;
        comecar_usuario_com_NULL(ponteiro_postagem,*num_perfis);

        lerPostagensArquivo(ponteiro_postagem, (*num_perfis - 1), VetorPerfil[*num_perfis - 1].numeroDePostagens);
    }

    fclose(arquivo);

    (*ponteiro_perfil) = (perfil_t *)realloc(*ponteiro_perfil, sizeof(perfil_t) * (*num_perfis + 1));

    (*ponteiro_perfil) = VetorPerfil;

    return numero_de_postagens;
}

int funcaoEscrevePostagem(posts_t **ponteiro_postagem, int numeroPerfil, int totalDePostagens){

    FILE * arquivoPostagem;

    char nome_Arquivo[Tamanho_Maximo];
    int postagem = 0;
    int i;

    sprintf(nome_Arquivo, "%d.txt", numeroPerfil);

    arquivoPostagem = fopen(nome_Arquivo, "w");


    if (arquivoPostagem == NULL){
        printf("Erro ao abrir postagens ou postagens inexistentes!\n");
        return ERRO;
    }

for(postagem = 0; postagem < totalDePostagens; postagem++){

    util_removeQuebraLinhaFinal(ponteiro_postagem[numeroPerfil][postagem].ID_post);
    util_removeQuebraLinhaFinal(ponteiro_postagem[numeroPerfil][postagem].url[0]);
    util_removeQuebraLinhaFinal(ponteiro_postagem[numeroPerfil][postagem].legenda);

}

for(postagem = 0; postagem < totalDePostagens; postagem++){

    fprintf(arquivoPostagem, "%s\n", ponteiro_postagem[numeroPerfil][postagem].ID_post);
    fprintf(arquivoPostagem, "%d\n", ponteiro_postagem[numeroPerfil][postagem].Numero_De_Fotos);
    for(i = 0; i < ponteiro_postagem[numeroPerfil][postagem].Numero_De_Fotos; i++){
        fprintf(arquivoPostagem, "%s\n", ponteiro_postagem[numeroPerfil][postagem].url[i]);
    }
    fprintf(arquivoPostagem, "%s\n", ponteiro_postagem[numeroPerfil][postagem].legenda);
}


fclose(arquivoPostagem);

return SUCESSO;

}

int funcaoEscreveArquivo(perfil_t *dadosNaMemoria, int num_perfis, posts_t **ponteiro_postagem, int *vetorComNumeroPostagens){

    FILE *arquivo;
    int i;

    arquivo = fopen("dadosColtegram.txt", "w");

    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo");
        return ERRO;
    }

    for (i = 0; i < num_perfis; i++){

        util_removeQuebraLinhaFinal(dadosNaMemoria[i].ID);
        util_removeQuebraLinhaFinal(dadosNaMemoria[i].nome_usuario);
        util_removeQuebraLinhaFinal(dadosNaMemoria[i].email);
        util_removeQuebraLinhaFinal(dadosNaMemoria[i].senha);
    }

    for (i = 0; i < num_perfis; i++){

        fprintf(arquivo, "%s\n", dadosNaMemoria[i].ID);
        fprintf(arquivo, "%s\n", dadosNaMemoria[i].nome_usuario);
        fprintf(arquivo, "%s\n", dadosNaMemoria[i].email);
        fprintf(arquivo, "%s\n", dadosNaMemoria[i].senha);
        fprintf(arquivo, "%d\n", dadosNaMemoria[i].numeroDePostagens);

        funcaoEscrevePostagem(ponteiro_postagem, i, vetorComNumeroPostagens[i]);
    }

    fclose(arquivo);
    return SUCESSO;
}

// Função para verificar se já existe um mesmo ID de usuário no programa
int usuario_existente(perfil_t *perfis, int num_perfis, char *nome){
    int i;
    for (i = 0; i < num_perfis; i++){
        if (strcmp(perfis[i].nome_usuario, nome) == 0){
            return 1;
        }
    }
    return 0;
}
// Função para verificar se já existe um mesmo nome de usuário no programa
int id_existente(perfil_t *perfis, int num_perfis, char *ID){
    int i;
    for (i = 0; i < num_perfis; i++){
        if (strcmp(perfis[i].ID, ID) == 0){
            return 1;
        }
    }
    return 0;
}
// Função para verificar se já existe um mesmo e-mail no programa
int email_existente(perfil_t *perfis, int num_perfis, char *email){
    int i;
    for (i = 0; i < num_perfis; i++){
        if (strcmp(perfis[i].email, email) == 0){
            return 1;
        }
    }
    return 0;
}
// Função para o cadastro do perfil
void cadastro_perfil(perfil_t **ponteiro_perfil, int *num_perfis, posts_t ***ponteiro_postagem){
    perfil_t perfis;
    perfis.numeroDePostagens = 0;
    int i;
    int contador_espaco = 0;
    bool contador_arroba = false, contador_ponto = false;
    printf("\t\tBem vindo ao cadastro do seu perfil!!!\t\t\n");
    do{
        printf("Digite o ID do seu usuario: (o @ do seu usuario)\n");
        fgets(perfis.ID, NUM_MAX_CARACTERES_ID, stdin);
        util_removeQuebraLinhaFinal(perfis.ID);
        contador_espaco = 0;
        for (i = 0; perfis.ID[i] != '\0'; i++){
            if (perfis.ID[i] == ' '){
                contador_espaco++;
            }
        }
        if (contador_espaco > 0){
            printf("O nome do usuario nao pode conter espacos!!!\n");
        }
        if (id_existente(*ponteiro_perfil, *num_perfis, perfis.ID)){
            printf("ID ja existe!!!\n");
        }
    } while (contador_espaco > 0 || id_existente(*ponteiro_perfil, *num_perfis, perfis.ID));
    printf("Digite o nome do seu usuario:\n");
    fgets(perfis.nome_usuario, NUM_MAX_CARACTERES_NOME_USUARIO, stdin);
    util_removeQuebraLinhaFinal(perfis.nome_usuario);
    if (usuario_existente(*ponteiro_perfil, *num_perfis, perfis.nome_usuario)){
        printf("Ja existe um usuario com o mesmo nome. Digite novamente o nome do seu usuario:\n");
        fgets(perfis.nome_usuario, NUM_MAX_CARACTERES_NOME_USUARIO, stdin);
        util_removeQuebraLinhaFinal(perfis.nome_usuario);
    }
    do{
        printf("Agora digite o seu e-mail:\n");
        fgets(perfis.email, NUM_MAX_CARACTERES_EMAIL, stdin);
        util_removeQuebraLinhaFinal(perfis.email);
        for (i = 0; perfis.email[i] != '\0'; i++){
            if (perfis.email[i] == '@'){
                contador_arroba = true;
            }
            if (perfis.email[i] == '.'){
                contador_ponto = true;
            }
        }
        if (contador_arroba == false){
            printf("E-mail invalido ou inexistente!!!\n");
            contador_ponto = false;
        }
        if (contador_ponto == false){
            printf("E-mail invalido ou inexistente!!!\n");
            contador_arroba = false;
        }
        if (email_existente(*ponteiro_perfil, *num_perfis, perfis.email)){
            printf("Email ja existe em outra conta:\n");
            printf("Digite um email valido:\n");
            fgets(perfis.email, NUM_MAX_CARACTERES_EMAIL, stdin);
            util_removeQuebraLinhaFinal(perfis.email);
        }
    } while ((contador_arroba == false) || (contador_ponto == false));
    printf("Digite sua senha:\n");
    fgets(perfis.senha, NUM_MAX_CARACTERES_SENHA, stdin);
    util_removeQuebraLinhaFinal(perfis.senha);
    printf("Agora confirme sua senha:\n");
    fgets(perfis.senha_confirmada, NUM_MAX_CARACTERES_SENHA, stdin);
    util_removeQuebraLinhaFinal(perfis.senha_confirmada);
    while (strcmp(perfis.senha, perfis.senha_confirmada) != 0){
        printf("Senha incorreta! Tente novamente!\n");
        printf("Digite sua senha:\n");
        fgets(perfis.senha, NUM_MAX_CARACTERES_SENHA, stdin);
        util_removeQuebraLinhaFinal(perfis.senha);
        printf("Agora confirme sua senha:\n");
        fgets(perfis.senha_confirmada, NUM_MAX_CARACTERES_SENHA, stdin);
        util_removeQuebraLinhaFinal(perfis.senha_confirmada);
    }
    (*num_perfis)++;
    *ponteiro_perfil = realloc(*ponteiro_perfil, (*num_perfis) * sizeof(perfil_t));
    (*ponteiro_perfil)[*num_perfis - 1] = perfis;
    comecar_usuario_com_NULL(ponteiro_postagem, *num_perfis);
    printf("Cadastro concluido!!!\n");
}
// Função para fazer login no sistema
int login(perfil_t *ponteiro_perfil, int num_perfis, login_t *ponteiro_login){
    int i, escolha;
    if (num_perfis < 1){
        printf("Nao ha perfis cadastrados nesse instagram!\n");
        return USUARIO_INVALIDO;
    }
    printf("\t\tLOGIN:\n");
    printf("ID do usuario:\n");
    for (i = 0; i < num_perfis; i++){
        printf("%d.%-51s\n", i + 1, ponteiro_perfil[i].ID);
    }
    printf("Qual perfil voce deseja fazer login (digite o numero corrrespondente):\n");
    scanf("%d%*c", &escolha);
    escolha--;
    if (escolha < 0 || escolha >= num_perfis){
        printf("Opcao invalida!\n");
        return USUARIO_INVALIDO;
    }
    printf("Digite seu email para esse perfil:\n");
    fgets(ponteiro_login->email_login, NUM_MAX_CARACTERES_EMAIL, stdin);
    util_removeQuebraLinhaFinal(ponteiro_login->email_login);
    printf("Agora digite sua senha para esse perfil:\n");
    fgets(ponteiro_login->senha_login, NUM_MAX_CARACTERES_SENHA, stdin);
    util_removeQuebraLinhaFinal(ponteiro_login->senha_login);
    if ((strcmp(ponteiro_perfil[escolha].senha, ponteiro_login->senha_login) == 0) &&
        (strcmp(ponteiro_perfil[escolha].email, ponteiro_login->email_login) == 0)){
        printf("Perfil Acessado!\n");
        printf("Login realizado!!!\n");
        printf("Bem vindo ao Coltegram %-51s\n", ponteiro_perfil[escolha].ID);
        return escolha;
    }
    else{
        printf("Perfil incorreto!!!\n");
        return USUARIO_INVALIDO;
    }
}
// Função bubble sort para ordenar as strings dos IDs
void listar_IDs_cadastrados(perfil_t *ponteiro_perfil, int num_perfis){
    int i, j;
    perfil_t tmp;
    // Listar Ids de forma ordenada
    for (i = 0; i < (num_perfis - 1); i++){
        for (j = 0; j < (num_perfis - 1); j++){
            // Se estiver fora de ordem...
            if (strcmp(ponteiro_perfil[j].ID, ponteiro_perfil[j + 1].ID) > 0){
                // ... troca de posicao
                tmp = ponteiro_perfil[j];
                ponteiro_perfil[j] = ponteiro_perfil[j + 1];
                ponteiro_perfil[j + 1] = tmp;
            }
        }
    }
}
// Função que imprime os IDs em ordem alfabética e formato de tabela
void imprimir_IDs_cadastrados(perfil_t *ponteiro_perfil, int num_perfis){
    int i;
    // Chamar a função bubble sort para ordenar os Ids
    listar_IDs_cadastrados(ponteiro_perfil, num_perfis);
    printf("Ids\n");
    for (i = 0; i < num_perfis; i++){
        printf("@%-51s\n", ponteiro_perfil[i].ID);
    }
}
// Função bubble sort que ordena os nomes de usuários cadastrados em ordem alfabética
void listar_nomes_cadastrados(perfil_t *ponteiro_perfil, int num_perfis){
    int i, j;
    perfil_t tmp;
    // Listar Ids de forma ordenada
    for (i = 0; i < (num_perfis - 1); i++){
        for (j = 0; j < (num_perfis - 1); j++){
            // Se estiver fora de ordem...
            if (strcmp(ponteiro_perfil[j].nome_usuario, ponteiro_perfil[j + 1].nome_usuario) > 0){
                // ... troca de posicao
                tmp = ponteiro_perfil[j];
                ponteiro_perfil[j] = ponteiro_perfil[j + 1];
                ponteiro_perfil[j + 1] = tmp;
            }
        }
    }
}
// Função que imprime os nomes de usuario em ordem alfabética e formato de tabela
void imprimir_nomes_cadastrados(perfil_t *ponteiro_perfil, int num_perfis){
    int i;
    // Chamar a função bubble sort para ordenar os nomes de usuario
    listar_nomes_cadastrados(ponteiro_perfil, num_perfis);
    printf("Nomes de Usuario\n");
    for (i = 0; i < num_perfis; i++){
        printf("%-51s\n", ponteiro_perfil[i].nome_usuario);
    }
}
// Função Bubble sort que ordena os e-mails cadastrados em ordem alfabetica
void listar_emails_cadastrados(perfil_t *ponteiro_perfil, int num_perfis){
    int i, j;
    perfil_t tmp;
    // Listar Ids de forma ordenada
    for (i = 0; i < (num_perfis - 1); i++){
        for (j = 0; j < (num_perfis - 1); j++){
            // Se estiver fora de ordem...
            if (strcmp(ponteiro_perfil[j].email, ponteiro_perfil[j + 1].email) > 0){
                // ... troca de posicao
                tmp = ponteiro_perfil[j];
                ponteiro_perfil[j] = ponteiro_perfil[j + 1];
                ponteiro_perfil[j + 1] = tmp;
            }
        }
    }
}
// Função que imprime os e-mails em ordem alfabética e formato de tabela
void imprimir_emails_cadastrados(perfil_t *ponteiro_perfil, int num_perfis){
    int i;
    // Chamar a função bubble sort para ordenar os e-mails
    listar_emails_cadastrados(ponteiro_perfil, num_perfis);
    printf("Emails\n");
    for (i = 0; i < num_perfis; i++){
        printf("%-51s\n", ponteiro_perfil[i].email);
    }
}
// Função que imprime tudo em ordem alfabética e formato de tabela
void imprimir_tudo_cadastrado(perfil_t *ponteiro_perfil, int num_perfis){
    int i;
    // Chamar a função bubble sort para ordenar os Ids
    listar_IDs_cadastrados(ponteiro_perfil, num_perfis);
    // Chamar a função bubble sort para ordenar os e-mails
    listar_emails_cadastrados(ponteiro_perfil, num_perfis);
    // Chamar a função bubble sort para ordenar os nomes dos usuários
    listar_nomes_cadastrados(ponteiro_perfil, num_perfis);
    printf("Ids                                              E-mails                                             Nomes de usuarios\n");
    for (i = 0; i < num_perfis; i++){
        printf("@%-51s %-51s %-51s\n", ponteiro_perfil[i].ID, ponteiro_perfil[i].email, ponteiro_perfil[i].nome_usuario);
    }
}


// Função para cadastro de uma postagem
int cadastro_postagem(posts_t ***ponteiro_postagem, int *num_postagens, int posicao_usuario_logado, int numero_perfis){
    posts_t postagens;
    int i;

    printf("\t\tPOSTAGEM\t\t\n");
    printf("Digite o nome de seu post:\n");
    fgets(postagens.ID_post, NUM_MAX_CARACTERES_ID, stdin);
    util_removeQuebraLinhaFinal(postagens.ID_post);

    printf("Digite quantas imagens voce deseja colocar em seu post:\n");
    scanf("%d%*c", &postagens.Numero_De_Fotos);

    // Aloca espaço para as imagens
    postagens.img = malloc(sizeof(asciiImg_t *) * postagens.Numero_De_Fotos);

    printf("Agora de upload na imagem de seu post:\n");
    printf("Para isso digite o url de sua imagem com o jpg no final\n");
    printf("Exemplo: https://img.freepik.com/fotos-premium/fundo-de-rosas-bonitas_534373-220.jpg\n");
    // https://static.todamateria.com.br/upload/ba/sq/basquetebol-og.jpg
    //https://www.coltec.ufmg.br/coltec-ufmg/wp-content/uploads/2018/06/leandro.jpg
    //https://profrancis.com.br/wp-content/uploads/2021/10/WhatsApp-Image-2021-09-06-at-11.51.44-1-1000x1000.jpeg
    //https://pbs.twimg.com/profile_images/1070325082143248385/_bFgPMyu_400x400.jpg
    //https://i1.rgstatic.net/ii/profile.image/278000563179523-1443291907950_Q512/Dener-Brandao.jpg
    //https://devday.github.io/2018/assets/img/speakers/virginia-mota.jpg
    //https://www.ufmg.br/boletim/bol1649/img/5_marcio_fantini.jpg
    //https://ufmg.br/thumbor/qFWA3ci0G6Kos3IqvoW0epIDyeM=/42x0:2007x3010/352x540/https://ufmg.br/storage/8/6/f/9/86f9e52e4836a3631e1fadfbc281530a_15362519523862_572083660.jpg


    for (i = 0; i < postagens.Numero_De_Fotos; i++){
        printf("URL: ");
        fgets(postagens.url[i], NUM_MAX_IMAGEM, stdin);
        util_removeQuebraLinhaFinal(postagens.url[i]);

        postagens.img[i] = insta_carregaImagem(postagens.url[i], MODO_IMAGEM, IMAGEM_NUMERO_COLUNAS);
        if (postagens.img[i] == NULL){
            // Falha ao carregar a imagem
            fprintf(stderr, "Falha ao carregar a imagem da URL %s\n", postagens.url[i]);
            return ERRO_CARREGAR_IMAGEM;
        }
        // Mostra a imagem, o número de bytes e libera a memória
        insta_imprimeImagem(postagens.img[i]);
    }

    printf("Digite uma legenda para seu post: (MAX 300 caracteres)\n");
    fgets(postagens.legenda, NUM_MAX_CARACTERES_LEGENDA, stdin);
    util_removeQuebraLinhaFinal(postagens.legenda);

    (*num_postagens)++;
    alocarMatriz(ponteiro_postagem, *num_postagens, posicao_usuario_logado, numero_perfis);
    (*ponteiro_postagem)[posicao_usuario_logado][*num_postagens - 1] = postagens;

    (*ponteiro_postagem)[posicao_usuario_logado][*num_postagens - 1].comentario.numero_comentarios = 0;
    (*ponteiro_postagem)[posicao_usuario_logado][*num_postagens - 1].comentario.mensagem = NULL;
    (*ponteiro_postagem)[posicao_usuario_logado][*num_postagens - 1].comentario.perfil_que_comentou = NULL;

    return SUCESSO;
}

// Função para imprimir informações de posts
int imprime_posts_do_usuario_logado(posts_t **ponteiro_postagem, int * vetor_com_numero_de_postagens_do_usuario, int posicao_usuario_logado){
    int i, j;

    if (vetor_com_numero_de_postagens_do_usuario[posicao_usuario_logado] < 1){
        printf("Voce nao postou posts ainda!\n");
        return ERRO;
    }

    printf("SEUS POSTS\n");

    for (i = 0; i < vetor_com_numero_de_postagens_do_usuario[posicao_usuario_logado]; i++){

        if(ponteiro_postagem[posicao_usuario_logado][i].Numero_De_Fotos == 0){

            ponteiro_postagem[posicao_usuario_logado][i].Numero_De_Fotos = 1;

        }

        printf("Titulo\n");
        printf("%s\n", ponteiro_postagem[posicao_usuario_logado][i].ID_post);
        printf("IMAGEM:\n");
        
        for (j = 0; j < ponteiro_postagem[posicao_usuario_logado][i].Numero_De_Fotos; j++){
            // Mostra a imagem, o número de bytes e libera a memória
            insta_imprimeImagem((ponteiro_postagem)[posicao_usuario_logado][i].img[j]);
            printf ("\n");
        }


        printf("\nLegenda:\n");
        printf("%s\n", ponteiro_postagem[posicao_usuario_logado][i].legenda);
    }
    return SUCESSO;
}

int imprime_posts_da_sua_escolha(posts_t **ponteiro_postagem, int numero_do_usuario, int numero_da_postagem){
    int i;

    printf("Titulo\n");
    printf("%s\n", ponteiro_postagem[numero_do_usuario][numero_da_postagem].ID_post);
    printf("IMAGEM:\n");

    for (i = 0; i < ponteiro_postagem[numero_do_usuario][numero_da_postagem].Numero_De_Fotos; i++){
        // Mostra a imagem, o número de bytes e libera a memória
        insta_imprimeImagem(ponteiro_postagem[numero_do_usuario][numero_da_postagem].img[i]);
        printf ("\n");
    }

    printf("Legenda:\n");
    printf("%s\n", ponteiro_postagem[numero_do_usuario][numero_da_postagem].legenda);
    return SUCESSO;
}
int editar_posts(posts_t **ponteiro_postagem, int * vetor_com_numero_de_postagens_do_usuario, int posicao_usuario_logado){
    int i;
    int opcao, index;

    if (vetor_com_numero_de_postagens_do_usuario[posicao_usuario_logado] < 1){
        printf("Voce nao postou posts ainda!\n");
        return ERRO;
    }
    printf("\t\tSEUS POSTS:\n");

    for (i = 0; i < vetor_com_numero_de_postagens_do_usuario[posicao_usuario_logado]; i++){
        printf("%d. %s\n", i + 1, ponteiro_postagem[posicao_usuario_logado][i].ID_post);
    }
    do{
        printf("O que voce deseja editar no seu post?\n");
        printf("(1) <TITULO DO MEU POST>\n(2) <IMAGEM DO POST>\n(3) <LEGENDA DO POST>\n(0) <SAIR>\n");
        printf("Digite o numero correspondente a sua opcao:\n");
        scanf("%d%*c", &opcao);
        switch (opcao){
            case 1:{
                printf("Qual titulo voce deseja mudar?\n");
                for (i = 0; i < vetor_com_numero_de_postagens_do_usuario[posicao_usuario_logado]; i++){
                    printf("%d. %s\n", i + 1, ponteiro_postagem[posicao_usuario_logado][i].ID_post);
                }
                printf("Digite o numero do titulo correspondente ao post que voce quer alterar:\n");
                scanf("%d%*c", &index);
                index--;
                if (index < 0 || index >= vetor_com_numero_de_postagens_do_usuario[posicao_usuario_logado]){
                    printf("Opcao invalida!\n");
                    return ERRO;
                }
                printf("Titulo do seu post:\n");
                printf("%s\n", ponteiro_postagem[posicao_usuario_logado][index].ID_post);
                printf("Digite o que voce quer mudar no titulo:\n");
                fgets(ponteiro_postagem[posicao_usuario_logado][index].ID_post, NUM_MAX_CARACTERES_ID, stdin);
                util_removeQuebraLinhaFinal(ponteiro_postagem[posicao_usuario_logado][index].ID_post);
                printf("Seu novo titulo do post:\n");
                printf("%-30s\n", ponteiro_postagem[posicao_usuario_logado][index].ID_post);
                break;
            }
            case 2:{
                printf("Digite o post que voce deseja acessar:\n");
                for (i = 0; i < vetor_com_numero_de_postagens_do_usuario[posicao_usuario_logado]; i++){
                    printf("%d. %s\n", i + 1, ponteiro_postagem[posicao_usuario_logado][i].ID_post);
                }
                scanf("%d%*c", &index);
                index--;
                if (index < 0 || index >= vetor_com_numero_de_postagens_do_usuario[posicao_usuario_logado]){
                    printf("Opcao invalida!\n");
                    return ERRO;
                }
                printf("Imagem do seu post:\n");
                for (i = 0; i < ponteiro_postagem[posicao_usuario_logado][index].Numero_De_Fotos; i++){
                    ponteiro_postagem[posicao_usuario_logado][index].img[i] = insta_carregaImagem(ponteiro_postagem[posicao_usuario_logado][index].url[i], MODO_IMAGEM, IMAGEM_NUMERO_COLUNAS);
                    if (ponteiro_postagem[posicao_usuario_logado][index].img[i] == NULL){
                        // Falha ao carregar a imagem
                        fprintf(stderr, "Falha ao carregar a imagem da URL %s\n", ponteiro_postagem[posicao_usuario_logado][index].url[i]);
                        return ERRO_CARREGAR_IMAGEM;
                    }
                    // Mostra a imagem, o número de bytes e libera a memória
                    insta_imprimeImagem(ponteiro_postagem[posicao_usuario_logado][index].img[i]);
                }
                printf("Digite quantas imagens voce vai querer inserir:\n");
                scanf("%d%*c", &ponteiro_postagem[posicao_usuario_logado][index].Numero_De_Fotos);

                for (i = 0; i < ponteiro_postagem[posicao_usuario_logado][index].Numero_De_Fotos; i++){
                    printf("Digite a nova url:\n");
                    fgets(ponteiro_postagem[posicao_usuario_logado][index].url[i], NUM_MAX_IMAGEM, stdin);
                    util_removeQuebraLinhaFinal(ponteiro_postagem[posicao_usuario_logado][index].url[i]);

                    ponteiro_postagem[posicao_usuario_logado][index].img[i] = insta_carregaImagem(ponteiro_postagem[posicao_usuario_logado][index].url[i], MODO_IMAGEM, IMAGEM_NUMERO_COLUNAS);
                    if (ponteiro_postagem[posicao_usuario_logado][index].img[i] == NULL){
                        // Falha ao carregar a imagem
                        fprintf(stderr, "Falha ao carregar a imagem da URL %s\n", ponteiro_postagem[posicao_usuario_logado][index].url[i]);
                        return ERRO_CARREGAR_IMAGEM;
                    }
                    insta_imprimeImagem(ponteiro_postagem[posicao_usuario_logado][index].img[i]);
                }
                break;
            }
            case 3:{
                printf("Digite o post que voce deseja acessar:\n");
                for (i = 0; i < vetor_com_numero_de_postagens_do_usuario[posicao_usuario_logado]; i++){
                    printf("%d. %s\n", i + 1, ponteiro_postagem[posicao_usuario_logado][i].ID_post);
                }
                scanf("%d%*c", &index);
                index--;
                if (index < 0 || index >= vetor_com_numero_de_postagens_do_usuario[posicao_usuario_logado]){
                    printf("Opcao invalida!\n");
                    return ERRO;
                }
                printf("Legenda do seu post:\n");
                printf("LEGENDA:\n");
                printf("%s\n", ponteiro_postagem[posicao_usuario_logado][index].legenda);
                printf("Digite o que voce quer mudar na legenda:\n");
                fgets(ponteiro_postagem[posicao_usuario_logado][index].legenda, NUM_MAX_CARACTERES_LEGENDA, stdin);
                util_removeQuebraLinhaFinal(ponteiro_postagem[posicao_usuario_logado][index].legenda);
                printf("Sua nova legenda:\n");
                printf("%s\n", ponteiro_postagem[posicao_usuario_logado][index].legenda);
                break;
            }
            case 0:{
                printf ("Saindo...\n");
                break;
            }
            default:{
                printf("Opcao invalida!!!\n");
            }
        }

    } while (opcao != 0);

    return SUCESSO;
}
/*
void excluir_posts(posts_t * ponteiro_postagem,int num_postagens){
    int i, index;

     if (num_postagens < 1){
        printf ("Voce nao postou posts ainda!\n");
        return;
    }
    printf ("\t\tSEUS POSTS:\n");
    for (i=0;i<num_postagens;i++){
        printf ("%d. %-30s\n", i+1, ponteiro_postagem[i].ID_post);
    }
    printf ("Digite o numero da postagem correspondente ao post que voce quer excluir:\n");
    scanf ("%d", &index);
    getchar();
    index--;
    //Excluir o post
    if (index >=0 && index < num_postagens){
        for (i=index;i< num_postagens -1;i++){
            ponteiro_postagem[i] = ponteiro_postagem[i + 1];
        }
        //Atualiza o numero de postagens
        num_postagens--;
        printf ("O post foi excluido com sucesso!\n");
    }else {
        printf ("Opcao invalida!\n");
    }
}
*/
int comentar_em_seu_propio_post(posts_t **ponteiro_postagem, int * vetor_com_numero_de_postagens,  int posicao_usuario_logado, perfil_t * ponteiro_perfil){
    int i, index;
    if (vetor_com_numero_de_postagens[posicao_usuario_logado] < 1){
        printf("Voce nao postou posts!\n");
        return ERRO;
    }

    for (i = 0; i < vetor_com_numero_de_postagens[posicao_usuario_logado]; i++){
        printf("%d.%s\n", i + 1, ponteiro_postagem[posicao_usuario_logado][i].ID_post);
    }
    printf("Digite qual post voce deseja acessar e comentar:\n");
    scanf("%d%*c", &index);
    index--;
    if (index < 0 || index >= vetor_com_numero_de_postagens[posicao_usuario_logado]){
        printf("Opcao invalida!\n");
        return ERRO;
    }

    posts_t * postagemAtual = &(ponteiro_postagem[posicao_usuario_logado][index]);

    postagemAtual->comentario.mensagem = realloc(postagemAtual->comentario.mensagem, sizeof(char *) * (postagemAtual->comentario.numero_comentarios + 1));
    postagemAtual->comentario.mensagem[postagemAtual->comentario.numero_comentarios] = malloc(sizeof(char) * NUM_MAX_CARACTERES_COMENTARIO);

    postagemAtual->comentario.perfil_que_comentou = realloc(postagemAtual->comentario.perfil_que_comentou, sizeof (char *) * (postagemAtual->comentario.numero_comentarios + 1));
    postagemAtual->comentario.perfil_que_comentou[postagemAtual->comentario.numero_comentarios] = malloc (sizeof (char *) * NUM_MAX_CARACTERES_ID);

    strcpy (postagemAtual->comentario.perfil_que_comentou[postagemAtual->comentario.numero_comentarios],ponteiro_perfil[posicao_usuario_logado].ID);

    printf("O que voce deseja comentar no post %s?\n", postagemAtual->ID_post);
    fgets(postagemAtual->comentario.mensagem[postagemAtual->comentario.numero_comentarios], NUM_MAX_CARACTERES_COMENTARIO, stdin);
    util_removeQuebraLinhaFinal(postagemAtual->comentario.mensagem[postagemAtual->comentario.numero_comentarios]);

    printf("Comentario feito!\n");
    postagemAtual->comentario.numero_comentarios++;


    return SUCESSO;
}
int comentar_no_post_dos_outros(posts_t **ponteiro_postagem, int num_postagens, perfil_t *ponteiro_perfil, int num_perfis,int posicao_usuario_logado){
    int i;
    int escolha_perfil, escolha_postagem;
    if (num_postagens < 1){
        printf("Nao ha posts no sistema!\n");
        return ERRO;
    }

    printf("Qual perfil voce deseja acessar?\n");
    for (i = 0; i < num_perfis; i++){
        printf("%d.%s\n", i + 1, ponteiro_perfil[i].ID);
    }

    printf("Sua opcao: ");
    scanf("%d%*c", &escolha_perfil);
    escolha_perfil--;

    if (escolha_perfil < 0 || escolha_perfil >= num_perfis){
        printf("Opcao invalida!\n");
        return ERRO;
    }

    printf("Em qual post desse perfil voce quer acessar?\n");
    for (i = 0; i < num_postagens; i++){
        printf("%d.%s\n", i + 1, ponteiro_postagem[escolha_perfil][i].ID_post);
    }
    printf("Sua opcao: ");
    scanf("%d%*c", &escolha_postagem);
    escolha_postagem--;
    if (escolha_postagem < 0 || escolha_postagem >= num_postagens){
        printf("Opcao invalida!\n");
        return ERRO;
    }

    imprime_posts_da_sua_escolha(ponteiro_postagem, escolha_perfil, escolha_postagem);

    posts_t * postagemAtual = &(ponteiro_postagem[escolha_perfil][escolha_postagem]);

    postagemAtual->comentario.mensagem = realloc(postagemAtual->comentario.mensagem, sizeof(char *) * (postagemAtual->comentario.numero_comentarios + 1));
    postagemAtual->comentario.mensagem[postagemAtual->comentario.numero_comentarios] = malloc(sizeof(char) * NUM_MAX_CARACTERES_COMENTARIO);

    postagemAtual->comentario.perfil_que_comentou = realloc(postagemAtual->comentario.perfil_que_comentou, sizeof (char *) * (postagemAtual->comentario.numero_comentarios + 1));
    postagemAtual->comentario.perfil_que_comentou[postagemAtual->comentario.numero_comentarios] = malloc (sizeof (char *) * NUM_MAX_CARACTERES_ID);
    strcpy (postagemAtual->comentario.perfil_que_comentou[postagemAtual->comentario.numero_comentarios],ponteiro_perfil[posicao_usuario_logado].ID);

    printf("O que voce deseja comentar no post %s?\n", postagemAtual->ID_post);
    fgets(postagemAtual->comentario.mensagem[postagemAtual->comentario.numero_comentarios], NUM_MAX_CARACTERES_COMENTARIO, stdin);
    util_removeQuebraLinhaFinal(postagemAtual->comentario.mensagem[postagemAtual->comentario.numero_comentarios]);
    
    postagemAtual->comentario.numero_comentarios++;
    printf("Comentario feito!\n");

    return SUCESSO;
}
int listar_comentario(posts_t **ponteiro_postagem, int num_postagens, perfil_t *ponteiro_perfil, int num_perfis,int posicao_usuario_logado){
    int i;
    int escolha_perfil;
    int escolha_postagem;
    if (num_postagens < 1){
        printf("Voce nao postou posts!\n");
        return ERRO;
    }
    printf("Qual perfil voce deseja acessar?\n");
    for (i = 0; i < num_perfis; i++){
        printf("%d.%s\n", i + 1, ponteiro_perfil[i].ID);
    }
    printf("Sua opcao: ");
    scanf("%d%*c", &escolha_perfil);
    escolha_perfil--;
    if (escolha_perfil < 0 || escolha_perfil >= num_perfis){
        printf("Opcao invalida!\n");
        return ERRO;
    }
    printf("Qual postagem voce deseja acessar do perfil %s?\n", ponteiro_perfil[escolha_perfil].ID);
    for (i = 0; i < num_postagens; i++){
        printf("%d.%s\n", i + 1, ponteiro_postagem[escolha_perfil][i].ID_post);
    }
    printf("SUA ESCOLHA: ");
    scanf("%d%*c", &escolha_postagem);
    escolha_postagem--;
    if (escolha_postagem < 0 || escolha_postagem >= num_postagens){
        printf("Opcao invalida!\n");
        return ERRO;
    }

    imprime_posts_da_sua_escolha(ponteiro_postagem, escolha_perfil, escolha_postagem);
    printf("Comentarios:\n");
    printf("Numero de comentarios: %d\n", ponteiro_postagem[escolha_perfil][escolha_postagem].comentario.numero_comentarios);
    for (i = 0; i < ponteiro_postagem[escolha_perfil][escolha_postagem].comentario.numero_comentarios; i++){
        printf("%d.%s: %s\n", i + 1, ponteiro_postagem[escolha_perfil][escolha_postagem].comentario.perfil_que_comentou[ponteiro_postagem[escolha_perfil][escolha_postagem].comentario.numero_comentarios], ponteiro_postagem[escolha_perfil][escolha_postagem].comentario.mensagem[i]);
    }

    return SUCESSO;
}
        // essa funcao vai permitir que o usuario logado curta postagens de outros usuarios!

int curtirPostagem(posts_t **ponteiro_postagem, int escolha_perfil, int escolha_postagem, perfil_t * ponteiro_perfil, int num_perfis, int num_postagens, int posicao_usuario_logado) {
    int i, j;
    char c;
    int num_curtidas = 0;

    // repete o processo para curtir a postagem do outro usuario

    printf("qual perfil voce deseja acessar?\n");
    for (i = 0;i < num_perfis;i++){
        printf ("%d.%s\n", i + 1,ponteiro_perfil[i].ID);
    }
    printf("Sua escolha:\n");
    scanf("%d%*c", &escolha_perfil);
    escolha_perfil--;

    printf ("Qual postagem desse perfil voce deseja acessar?\n");
    for (j = 0;j < num_postagens;j++){
        printf ("%d.%s\n", j + 1, ponteiro_postagem[escolha_perfil][j].ID_post);
    }
    printf("Qual post voce deseja acessar?\n");
    scanf ("%d%*c", &escolha_postagem);
    escolha_postagem--;

     if(escolha_postagem < 0 || escolha_postagem > num_postagens){
        printf("Voce nao pode acessar as postagens!\n");
        return ERRO;
    }


    printf ("Voce deseja curtir essa postagem?\n");
    scanf ("%c%*c", &c);

    // checa se o usuario logado ja curtiu a postagem do outro usuario
    if (ponteiro_postagem[posicao_usuario_logado][escolha_postagem].curtidas.curtida == true) {
        printf("IMPOSSIVEL CURTIR DUAS VEZES!\n");
        return ERRO;
    }
    
    //se for curtir, vai contar mais uma curtida
        if(c == 's' || c == 'S'){
        ponteiro_postagem[escolha_perfil][escolha_postagem].curtidas.curtida = true;
        ponteiro_postagem[escolha_perfil][escolha_postagem].curtidas.num_curtidas++;
        num_curtidas = ponteiro_postagem[posicao_usuario_logado][escolha_postagem].curtidas.num_curtidas;
        printf("Curtido por %s\n", ponteiro_perfil[posicao_usuario_logado].ID);
        printf("Total de curtidas: %d\n", num_curtidas);
    }else{
         ponteiro_postagem[escolha_perfil][escolha_postagem].curtidas.curtida = false;
    }
    return SUCESSO;
}

        //funcao das curtidas!

int curtida_ID(posts_t ** ponteiro_postagem, int num_postagens, int posicao_usuario_logado,int num_perfis,perfil_t * ponteiro_perfil){
    int i, j;
    int escolha_perfil, escolha_postagem;
    char c, s;
    int num_curtidas = 0;


    if(num_postagens < 1){
        printf("voce nao postou posts ainda!\n");
        return ERRO;
    }
    printf ("Qual perfil voce quer acessar:\n");
    for (i = 0;i < num_perfis;i++){
        printf ("%d.%s\n", i + 1,ponteiro_perfil[i].ID);
    }

    printf("Sua escolha:\n");
    scanf("%d%*c", &escolha_perfil);
    escolha_perfil--;
    
    if(escolha_perfil < 0 || escolha_perfil > num_perfis){
        printf("Voce nao pode acessar as postagens!\n");
        return ERRO;
    }
    printf ("Qual postagem desse perfil voce deseja acessar?\n");
    for (j = 0;j < num_postagens;j++){
        printf ("%d.%s\n", j + 1, ponteiro_postagem[escolha_perfil][j].ID_post);
    }
    printf("Qual post voce deseja acessar?\n");
    scanf ("%d%*c", &escolha_postagem);
    escolha_postagem--;

    if(escolha_postagem < 0 || escolha_postagem > num_postagens){
        printf("Voce nao pode acessar as postagens!\n");
        return ERRO;
    }
    if (ponteiro_postagem[escolha_perfil][escolha_postagem].curtidas.curtida == true) {
        printf("Impossivel curtir duas vezes.\n");
        return ERRO;
    }
    printf ("Voce deseja curtir essa postagem?\n");
    scanf ("%c%*c", &c);

    if (c == 's' || c == 'S'){
        ponteiro_postagem[escolha_perfil][escolha_postagem].curtidas.curtida = true;
        ponteiro_postagem[escolha_perfil][escolha_postagem].curtidas.num_curtidas++;
        printf("curtido por %s\n", ponteiro_perfil[escolha_perfil].ID);
        num_curtidas = ponteiro_postagem[posicao_usuario_logado][escolha_postagem].curtidas.num_curtidas;
        printf("Total de curtidas: %d\n", num_curtidas);
    }else {
        ponteiro_postagem[escolha_perfil][escolha_postagem].curtidas.curtida = false;
    }
    printf("voce deseja curtir posts de outro perfil?");
    scanf("%c%*c", &s);
    if(s == 's' || s == 'S'){

        //chamada da funcao "curtirPostagem", para curtir posts de outros usuarios

        curtirPostagem(ponteiro_postagem,escolha_perfil,escolha_postagem,ponteiro_perfil, num_perfis, num_postagens, posicao_usuario_logado);
    }
    return SUCESSO;
}
int buscar_perfis_ID(perfil_t *ponteiro_perfil, int num_perfis, char busca[]){
    int i;

    printf("Resultados para a busca %s:\n", busca);

    for (i = 0; i < num_perfis; i++){
        if (strstr(ponteiro_perfil[i].ID, busca)){
            printf("Perfis: %s\n", ponteiro_perfil[i].ID);
        }
    }

    return SUCESSO;
}
int buscar_perfis_email(perfil_t *ponteiro_perfil, int num_perfis, char busca[]){
    int i;

    printf("Resultados para a busca %s:\n", busca);

    for (i = 0; i < num_perfis; i++){
        if (strstr(ponteiro_perfil[i].email, busca)){
            printf("Perfis: %s\n", ponteiro_perfil[i].email);
        }
    }

    return SUCESSO;
}
int buscar_perfis_ID_ordenado(perfil_t *ponteiro_perfil, int num_perfis, char busca[]){
    int i, j;
    perfil_t tmp;
    // Listar Ids de forma ordenada
    printf("Resultados para a busca %s:\n", busca);
    for (i = 0; i < (num_perfis - 1); i++){
        if (strstr(ponteiro_perfil[i].ID, busca)){
            for (j = 0; j < (num_perfis - 1); j++){
                // Se estiver fora de ordem...
                if (strcmp(ponteiro_perfil[j].ID, ponteiro_perfil[j + 1].ID) > 0){
                    // ... troca de posicao
                    tmp = ponteiro_perfil[j];
                    ponteiro_perfil[j] = ponteiro_perfil[j + 1];
                    ponteiro_perfil[j + 1] = tmp;
                }
            }
        }
    }
    return SUCESSO;
}

int buscar_perfis_email_ordenado(perfil_t *ponteiro_perfil, int num_perfis, char busca[]){
    int i, j;
    perfil_t tmp;

    printf("Resultados para a busca %s\n", busca);
    for (i = 0; i < (num_perfis - 1); i++){
        if (strstr(ponteiro_perfil[i].email, busca)){
            for (j = 0; j < (num_perfis - 1); j++){
                // Se estiver fora de ordem...
                if (strcmp(ponteiro_perfil[j].email, ponteiro_perfil[j + 1].email) > 0){
                    // ... troca de posicao
                    tmp = ponteiro_perfil[j];
                    ponteiro_perfil[j] = ponteiro_perfil[j + 1];
                    ponteiro_perfil[j + 1] = tmp;
                }
            }
        }
    }            
    return SUCESSO;
}
int listar_emails_ordenado(perfil_t * ponteiro_perfil,int num_perfis,char busca[]){
    int i;

    buscar_perfis_email_ordenado(ponteiro_perfil,num_perfis,busca);
    for (i = 0;i < num_perfis;i++){
        printf ("Emails: %s\n", ponteiro_perfil[i].email);
    }

    return SUCESSO;
}
int listar_ids_ordenado(perfil_t * ponteiro_perfil,int num_perfis,char busca[]){
    int i;

    buscar_perfis_ID_ordenado(ponteiro_perfil,num_perfis,busca);
    for (i = 0;i < num_perfis;i++){
        printf ("Ids: %s\n", ponteiro_perfil[i].ID);
    }
    
    return SUCESSO;
}


// Função principal
int main(int argc, char **argv){
    int opcao, opcao2, opcao3, escolha1, escolha2, escolha3, escolha_buscar;
    perfil_t *ponteiro_perfil = NULL;
    posts_t **ponteiro_postagem = NULL;
    login_t login_info;
    int num_perfis = 0;
    int num_postagens = 0;
    int num_total_postagens = 0;  //Variavel para alguem utilizar na funçao que lista todas as postagens
    int posicao_usuario_logado = USUARIO_INVALIDO;
    int * vetor_Numero_Postagens_Usuarios = NULL;  //minha ideia aqui é ter um vetor que cada posição dele tem um numero representando a quantidade de postagens que um perfil tem.
    char busca[NUM_MAX_CARACTERES_ID];
    int i;
 
    funcaoLerArquivo(&ponteiro_perfil, &num_perfis, &ponteiro_postagem);

    vetor_Numero_Postagens_Usuarios = (int *)calloc(num_perfis, sizeof(int));

    for (i = 0; i < num_perfis; i++){

        vetor_Numero_Postagens_Usuarios[i] += ponteiro_perfil[i].numeroDePostagens;
    }

    printf("Bem vindo ao Coltegram!\n");
    printf("Instagram feito por:\nIcaro Cardoso Nascimento\nJoao Henrique Santana Oliveira Campos\nMatheus Fernandes de Oliveira Brandemburg\n");
    do{
        printf("Bem vindo ao Coltegram!!!\n");
        printf("Digite sua opcao:\n");
        printf("<Cadastar> (1)\n<Login> (2)\n<Sair> (0)\n");
        scanf("%d%*c", &opcao);
        switch (opcao){
            case 1:{
                cadastro_perfil(&ponteiro_perfil, &num_perfis, &ponteiro_postagem);

                vetor_Numero_Postagens_Usuarios = (int *)realloc(vetor_Numero_Postagens_Usuarios, sizeof(int) * num_perfis);
                vetor_Numero_Postagens_Usuarios[num_perfis - 1] = 0;
                break;
            }
            case 2:{
                posicao_usuario_logado = login(ponteiro_perfil, num_perfis, &login_info); // Aqui a variavel posicao_usuario_logado recebe o valor escolha, que deverá ser a posiçao do perfil.
                num_postagens = vetor_Numero_Postagens_Usuarios[posicao_usuario_logado];

                while (posicao_usuario_logado != USUARIO_INVALIDO){
                    printf("Quais acoes voce deseja executar:\n");
                    printf("(1) <Buscar perfis>\n(2) <Visitar perfis>\n(3) <Listar perfis cadastrados>\n(4) <Acoes do usuario>\n");
                    scanf("%d%*c", &escolha1);
                    switch (escolha1){
                        case 1:{
                            // Buscar perfis
                            do{
                                printf("\t\tBUSCAR PERFIS\n");
                                printf(".....................................................................\n\n");
                                printf("O que voce deseja buscar:\n");
                                printf("(1) <PERFIS>\n(2) <E-MAILS>\n(3) <PERFIS ORDENADOS>\n(4) <E-MAILS ORDENADOS>\n(0) <SAIR>\n");
                                printf("Sua opcao: ");
                                scanf("%d%*c", &escolha_buscar);

                                switch (escolha_buscar){
                                    case 1:{
                                        printf("Digite o ID:\n");
                                        fgets(busca, NUM_MAX_CARACTERES_ID, stdin);
                                        util_removeQuebraLinhaFinal(busca);
                                        buscar_perfis_ID(ponteiro_perfil, num_perfis, busca);
                                        break;
                                    }
                                    case 2:{
                                        printf("Digite o e-mail:\n");
                                        fgets(busca, NUM_MAX_CARACTERES_ID, stdin);
                                        util_removeQuebraLinhaFinal(busca);
                                        buscar_perfis_email(ponteiro_perfil, num_perfis, busca);
                                        break;
                                    }
                                    case 3:{
                                        printf("Digite o ID:\n");
                                        fgets(busca, NUM_MAX_CARACTERES_ID, stdin);
                                        util_removeQuebraLinhaFinal(busca);
                                        listar_ids_ordenado(ponteiro_perfil,num_perfis,busca);
                                        break;
                                    }
                                    case 4:{
                                        printf("Digite o email:\n");
                                        fgets(busca, NUM_MAX_CARACTERES_ID, stdin);
                                        util_removeQuebraLinhaFinal(busca);
                                        listar_emails_ordenado(ponteiro_perfil,num_perfis,busca);
                                        break;
                                    }
                                    case 0:{
                                        printf("Saindo...\n");
                                        break;
                                    }
                                    default:{
                                        printf("Opcao invalida!\n");
                                    }
                                }

                            } while (escolha_buscar != 0);
                            break;
                        }
                        case 2:{
                            // Visitar perfis
                            break;
                        }
                        case 3:{
                            // Listar perfis cadastrados
                            do{
                                printf("\t\tLISTAR PERFIS CADASTRADOS NO SISTEMA:\n");
                                printf("(1) <Listar IDs>\n(2) <Listar e-mails>\n(3) <Listar nome dos usuarios>\n(4) <Listar todas as informacoes>\n(0) <Parar>\n");
                                printf("O que voce deseja realizar:\n");
                                scanf("%d%*c", &escolha3);
                                switch (escolha3){
                                    case 1:{
                                        // Listar Ids em ordem alfabetica
                                        imprimir_IDs_cadastrados(ponteiro_perfil, num_perfis);
                                        break;
                                    }
                                    case 2:{
                                        // Listar e-mails em ordem alfabetica
                                        imprimir_emails_cadastrados(ponteiro_perfil, num_perfis);
                                        break;
                                    }
                                    case 3:{
                                        // Listar nome dos usuarios em ordem alfabetica
                                        imprimir_nomes_cadastrados(ponteiro_perfil, num_perfis);
                                        break;
                                    }
                                    case 4:{
                                        // Imprime todas as informações juntas
                                        imprimir_tudo_cadastrado(ponteiro_perfil, num_perfis);
                                        break;
                                    }

                                    default:{
                                        printf("Opcao invalida!!!\n");
                                    }
                                }
                            } while (escolha3 != 0);
                            break;
                        }
                        case 4:{
                            do{
                                printf("Acoes do usuario:\n");
                                printf("(1) <POSTAR POSTS>\n(2) <EDITAR POSTS>\n(3) <LISTAR POSTS>\n(4) <DETALHAR POSTS>\n(5) <APAGAR POSTS>\n(6) <COMENTARIOS>\n(7) <CURTIR>\n(8) <DESLOGAR>\n(0) <Sair>\n");
                                printf("O que voce deseja fazer?\n");
                                scanf("%d%*c", &escolha2);
                                switch (escolha2){
                                    case 1:{
                                        // Postar posts
                                        cadastro_postagem(&ponteiro_postagem, &num_postagens, posicao_usuario_logado, num_perfis); // Aqui você vai incluir a variavel posicao_usuario_logado como parametro
                                        vetor_Numero_Postagens_Usuarios[posicao_usuario_logado] ++;
                                        ponteiro_perfil[posicao_usuario_logado].numeroDePostagens ++;
                                        num_total_postagens ++;
                                        break;
                                    }
                                    case 2:{
                                        // Editar posts
                                        editar_posts(ponteiro_postagem,vetor_Numero_Postagens_Usuarios, posicao_usuario_logado);
                                        break;
                                    }
                                    case 3:{
                                        // Listar posts
                                        imprime_posts_do_usuario_logado(ponteiro_postagem, vetor_Numero_Postagens_Usuarios, posicao_usuario_logado);
                                        break;
                                    }
                                    case 4:{
                                        // Detalhar posts
                                        break;
                                    }
                                    case 5:{
                                        // Apagar posts
                                        /*
                                        excluir_posts(ponteiro_postagem,num_postagens);
                                        */
                                        break;
                                    }
                                    case 6:{
                                        do{
                                            printf("\t\tCOMENTARIOS:\n");
                                            printf("O que voce deseja fazer?\n");
                                            printf("(1) <COMENTAR EM SEU PROPIO POST>\n(2) <COMENTAR_POST_DE_OUTROS_PERFIS>\n(3) <LISTAR COMENTARIOS>\n(0) <SAIR>\n");
                                            printf("Sua opcao: ");
                                            scanf("%d%*c", &opcao2);

                                            switch (opcao2){
                                                case 1:{
                                                    comentar_em_seu_propio_post(ponteiro_postagem,vetor_Numero_Postagens_Usuarios,  posicao_usuario_logado,ponteiro_perfil);
                                                    break;
                                                }
                                                case 2:{
                                                    comentar_no_post_dos_outros(ponteiro_postagem, num_postagens,  ponteiro_perfil, num_perfis,posicao_usuario_logado);
                                                    break;
                                                }
                                                case 3:{
                                                    listar_comentario(ponteiro_postagem, num_postagens, ponteiro_perfil, num_perfis, posicao_usuario_logado);
                                                    break;
                                                }
                                                case 0:{
                                                    printf("Saindo...\n");
                                                    break;
                                                }
                                                default:{
                                                    printf("Opcao invalida!\n");
                                                }
                                            }
                                        } while (opcao2 != 0);
                                        break;
                                    }
                                    case 7:{
                                        do{
                                            printf("\t\tCURTIDAS\n");
                                            printf("Oque voce deseja fazer?:\n");
                                            printf("(1) <CURTIR>\n(0) <SAIR>\n");
                                            printf("Digite sua opcao: ");
                                            scanf("%d%*c", &opcao3);
                                            switch(opcao3){
                                                case 1:{
                                                    curtida_ID(ponteiro_postagem,num_postagens,posicao_usuario_logado,num_perfis,ponteiro_perfil);
                                                    break;
                                                }
                                                    
                                            }
                                        }while(opcao3 != 0);
                                        break;
                                    }
                                    case 8:{
                                        printf("Saindo do perfil...\n");
                                        posicao_usuario_logado = USUARIO_INVALIDO;
                                        break;
                                    }
                                    case 0:{
                                        printf("Voltando ao menu principal!!!\n");
                                        break;
                                    }
                                    default:{
                                        printf("Opcao invalida!!!\n");
                                    }
                                }
                            } while (escolha2 != 0 && posicao_usuario_logado != USUARIO_INVALIDO);
                            break;
                        }
                        case 0:{
                            printf("Saindo...\n");
                            break;
                        }
                        default:{
                            printf("Opcao invalida!\n");
                        }
                    }
                }
                break;
            }
            case 0:{
                printf("Volte sempre!!!\n");
                break;
            }
            default:{
                printf("Opcao invalida!!!\n");
            }
        }
    } while (opcao != 0);

    funcaoEscreveArquivo(ponteiro_perfil, num_perfis, ponteiro_postagem, vetor_Numero_Postagens_Usuarios);

    // Libera a memória alocada
    free(ponteiro_perfil);
    free(ponteiro_postagem);

    // Se chegou ate aqui é porque correu tudo bem
    return SUCESSO;
}