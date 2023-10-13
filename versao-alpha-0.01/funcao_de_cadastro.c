//Bibliotecas presentes no programa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

//Constantes presntes
#define SUCESSO 0
#define NUM_MAX_CARACTERES_NOME_USUARIO (99 + 1)
#define NUM_MAX_CARACTERES_EMAIL (50 + 1)
#define NUM_MAX_CARACTERES_SENHA (50 + 1)
#define NUM_MAX_CARACTERES_ID (50 + 1)

#define NUM_MAX_CARACTERES_LEGENDA (299+1)
#define NUM_MAX_CARACTERES_COMENTARIO (299+1)
// Tamanho do buffer para cada linha da imagem
#define BUFFER_TAMANHO  50000

// Tamanho máximo da linha de comando
#define LINHA_COMANDO   10000

// Limiar inferior que considera o download com sucesso
#define LIMIAR_INFERIOR_TAMANHO_IMAGEM  500

// Nome do executável da ferramenta de download e conversão da imagem
#ifdef __unix__ 
#define FERRAMENTA_IMAGEM   "./ascii-image-converter.bin"
#else
#define FERRAMENTA_IMAGEM   "ascii-image-converter.exe"
#endif

// Nome do arquivo de saída temporário da imagem
#define ARQUIVO_IMAGEM_TMP  "ascii_art.txt"
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
struct asciiImg_s {
  uint8_t * bytes;
  int nBytes;
};
/// Tipo "Imagem ASCII"
typedef struct asciiImg_s asciiImg_t;
//Estrutura para o perfil
typedef struct perfil_s {
    char ID[NUM_MAX_CARACTERES_ID];
    char nome_usuario[NUM_MAX_CARACTERES_NOME_USUARIO];
    char email[NUM_MAX_CARACTERES_EMAIL];
    char senha[NUM_MAX_CARACTERES_SENHA];
    char senha_confirmada[NUM_MAX_CARACTERES_SENHA];
} perfil_t;
//Estrutura para login
typedef struct login_s {
    char ID_login[NUM_MAX_CARACTERES_ID];
    char nome_usuario_login[NUM_MAX_CARACTERES_NOME_USUARIO];
    char email_login[NUM_MAX_CARACTERES_EMAIL];
    char senha_login[NUM_MAX_CARACTERES_SENHA];
} login_t;

//Estrutura para comentarios
typedef struct comentario_s{
    char id_comentario[NUM_MAX_CARACTERES_ID];
    char perfil_que_comentou[NUM_MAX_CARACTERES_ID];
    char mensagem[NUM_MAX_CARACTERES_COMENTARIO];
}comentario_t;

//Estrutura para curtidas
typedef struct curtida_s{
    char id_curtida[NUM_MAX_CARACTERES_ID];
    bool curtida;
}curtida_t;

//Estrutura para posts
typedef struct posts_s{
    char ID_post[NUM_MAX_CARACTERES_ID];
    asciiImg_t *img;
    char * url;
    char legenda[NUM_MAX_CARACTERES_LEGENDA];
    comentario_t comentario;
    curtida_t curtidas;
}posts_t;

//Função para tirar o '\n' das strings
void util_removeQuebraLinhaFinal(char dados[]) {
    int tamanho;
    tamanho = strlen(dados);
    if ((tamanho > 0) && (dados[tamanho - 1] == '\n')) {
        dados[tamanho - 1] = '\0';
    }
}
//Função para verificar se já existe um mesmo ID de usuário no programa
int usuario_existente(perfil_t *perfis, int num_perfis, char *nome) {
    int i;
    for (i = 0; i < num_perfis; i++) {
        if (strcmp(perfis[i].nome_usuario, nome) == 0) {
            return 1;
        }
    }
    return 0;
}
//Função para verificar se já existe um mesmo nome de usuário no programa
int id_existente(perfil_t *perfis, int num_perfis, char *ID) {
    int i;
    for (i = 0; i < num_perfis; i++) {
        if (strcmp(perfis[i].ID, ID) == 0) {
            return 1;
        }
    }
    return 0;
}
//Função para verificar se já existe um mesmo e-mail no programa
int email_existente(perfil_t *perfis, int num_perfis, char *email) {
    int i;
    for (i = 0; i < num_perfis; i++) {
        if (strcmp(perfis[i].email, email) == 0) {
            return 1;
        }
    }
    return 0;
}
//Função para o cadastro do perfil
void cadastro_perfil(perfil_t **ponteiro_perfil, int *num_perfis) {
    perfil_t perfis;
    int i;
    int contador_espaco = 0;
    bool contador_arroba = false, contador_ponto = false;
    printf("\t\tBem vindo ao cadastro do seu perfil!!!\t\t\n");
    do {
        printf("Digite o ID do seu usuario: (o @ do seu usuario)\n");
        fgets(perfis.ID, NUM_MAX_CARACTERES_ID, stdin);
        util_removeQuebraLinhaFinal(perfis.ID);
        contador_espaco = 0;
        for (i = 0; perfis.ID[i] != '\0'; i++) {
            if (perfis.ID[i] == ' ') {
                contador_espaco++;
            }
        }
        if (contador_espaco > 0) {
            printf("O nome do usuario nao pode conter espacos!!!\n");
        }
        if (id_existente(*ponteiro_perfil, *num_perfis, perfis.ID)) {
            printf("ID ja existe!!!\n");
        }
    } while (contador_espaco > 0 || id_existente(*ponteiro_perfil, *num_perfis, perfis.ID));
    printf("Digite o nome do seu usuario:\n");
    fgets(perfis.nome_usuario, NUM_MAX_CARACTERES_NOME_USUARIO, stdin);
    util_removeQuebraLinhaFinal(perfis.nome_usuario);
    if (usuario_existente(*ponteiro_perfil, *num_perfis, perfis.nome_usuario)) {
        printf("Ja existe um usuario com o mesmo nome. Digite novamente o nome do seu usuario:\n");
        fgets(perfis.nome_usuario, NUM_MAX_CARACTERES_NOME_USUARIO, stdin);
        util_removeQuebraLinhaFinal(perfis.nome_usuario);
    }
    do {
        printf("Agora digite o seu e-mail:\n");
        fgets(perfis.email, NUM_MAX_CARACTERES_EMAIL, stdin);
        util_removeQuebraLinhaFinal(perfis.email);
        for (i = 0; perfis.email[i] != '\0'; i++) {
            if (perfis.email[i] == '@') {
                contador_arroba = true;
            }
            if (perfis.email[i] == '.') {
                contador_ponto = true;
            }
        }
        if (contador_arroba == false) {
            printf("E-mail invalido ou inexistente!!!\n");
            contador_ponto = false;
        }
        if (contador_ponto == false){
            printf("E-mail invalido ou inexistente!!!\n");
            contador_arroba = false;
        }
        if (email_existente(*ponteiro_perfil, *num_perfis, perfis.email)) {
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
    while (strcmp(perfis.senha, perfis.senha_confirmada) != 0) {
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
    printf("Cadastro concluido!!!\n");
}
//Função para fazer login no sistema
void login(perfil_t *ponteiro_perfil, int num_perfis, login_t *ponteiro_login, bool *logado) {
    int i, escolha;
    if (num_perfis < 1) {
        printf("Nao ha perfis cadastrados nesse instagram!\n");
        return;
    }
    printf("\t\tLOGIN:\n");
    printf("ID do usuario:\n");
    for (i = 0; i < num_perfis; i++) {
        printf("%d.%-51s\n", i + 1, ponteiro_perfil[i].ID);
    }
    printf("Qual perfil voce deseja fazer login (digite o numero corrrespondente):\n");
    scanf("%d", &escolha);
    getchar();
    escolha--;
    if (escolha < 0 || escolha >= num_perfis) {
        printf("Opcao invalida!\n");
        return;
    }
    printf("Digite seu email para esse perfil:\n");
    fgets(ponteiro_login->email_login, NUM_MAX_CARACTERES_EMAIL, stdin);
    util_removeQuebraLinhaFinal(ponteiro_login->email_login);
    printf("Agora digite sua senha para esse perfil:\n");
    fgets(ponteiro_login->senha_login, NUM_MAX_CARACTERES_SENHA, stdin);
    util_removeQuebraLinhaFinal(ponteiro_login->senha_login);
    if ((strcmp(ponteiro_perfil[escolha].senha, ponteiro_login->senha_login) == 0) &&
        (strcmp(ponteiro_perfil[escolha].email, ponteiro_login->email_login) == 0)) {
        printf("Perfil Acessado!\n");
        printf("Login realizado!!!\n");
        printf("Bem vindo ao Coltegram %-51s\n", ponteiro_perfil[escolha].ID);
        *logado = true;
    } else {
        printf("Perfil incorreto!!!\n");
    }
}
//Função bubble sort para ordenar as strings dos IDs
void listar_IDs_cadastrados(perfil_t * ponteiro_perfil, int num_perfis){
    int i, j;
    perfil_t tmp; 
    //Listar Ids de forma ordenada
    for (i = 0; i < (num_perfis - 1); i++) {
        for (j = 0; j < (num_perfis - 1); j++) {
        // Se estiver fora de ordem...
           if (strcmp(ponteiro_perfil[j].ID, ponteiro_perfil[j + 1].ID) > 0) {
               // ... troca de posicao
               tmp = ponteiro_perfil[j];
               ponteiro_perfil[j] = ponteiro_perfil[j + 1];
               ponteiro_perfil[j + 1] = tmp;
            }
        }
    }
}
//Função que imprime os IDs em ordem alfabética e formato de tabela
void imprimir_IDs_cadastrados(perfil_t * ponteiro_perfil, int num_perfis){
    int i;
    //Chamar a função bubble sort para ordenar os Ids
    listar_IDs_cadastrados(ponteiro_perfil, num_perfis);
    printf ("Ids\n");
    for (i=0;i<num_perfis;i++){
        printf ("@%-51s\n", ponteiro_perfil[i].ID);
    }
}
//Função bubble sort que ordena os nomes de usuários cadastrados em ordem alfabética
void listar_nomes_cadastrados(perfil_t * ponteiro_perfil, int num_perfis){
    int i, j;
    perfil_t tmp; 
    //Listar Ids de forma ordenada
    for (i = 0; i < (num_perfis - 1); i++) {
        for (j = 0; j < (num_perfis - 1); j++) {
        // Se estiver fora de ordem...
           if (strcmp(ponteiro_perfil[j].nome_usuario, ponteiro_perfil[j + 1].nome_usuario) > 0) {
               // ... troca de posicao
               tmp = ponteiro_perfil[j];
               ponteiro_perfil[j] = ponteiro_perfil[j + 1];
               ponteiro_perfil[j + 1] = tmp;
            }
        }
    }
}
//Função que imprime os nomes de usuario em ordem alfabética e formato de tabela
void imprimir_nomes_cadastrados(perfil_t * ponteiro_perfil, int num_perfis){
    int i;
    //Chamar a função bubble sort para ordenar os nomes de usuario
    listar_nomes_cadastrados(ponteiro_perfil, num_perfis);
    printf ("Nomes de Usuario\n");
    for (i=0;i<num_perfis;i++){
        printf ("%-51s\n", ponteiro_perfil[i].nome_usuario);
    }
}
//Função Bubble sort que ordena os e-mails cadastrados em ordem alfabetica
void listar_emails_cadastrados(perfil_t * ponteiro_perfil, int num_perfis){
    int i, j;
    perfil_t tmp; 
    //Listar Ids de forma ordenada
    for (i = 0; i < (num_perfis - 1); i++) {
        for (j = 0; j < (num_perfis - 1); j++) {
        // Se estiver fora de ordem...
           if (strcmp(ponteiro_perfil[j].email, ponteiro_perfil[j + 1].email) > 0) {
               // ... troca de posicao
               tmp = ponteiro_perfil[j];
               ponteiro_perfil[j] = ponteiro_perfil[j + 1];
               ponteiro_perfil[j + 1] = tmp;
            }
        }
    }
}
//Função que imprime os e-mails em ordem alfabética e formato de tabela
void imprimir_emails_cadastrados(perfil_t * ponteiro_perfil, int num_perfis){
    int i;
    //Chamar a função bubble sort para ordenar os e-mails
    listar_emails_cadastrados(ponteiro_perfil, num_perfis);
    printf ("Emails\n");
    for (i=0;i<num_perfis;i++){
        printf ("%-51s\n", ponteiro_perfil[i].email);
    }
}
//Função que imprime tudo em ordem alfabética e formato de tabela
void imprimir_tudo_cadastrado(perfil_t * ponteiro_perfil, int num_perfis){
    int i;
    //Chamar a função bubble sort para ordenar os Ids
    listar_IDs_cadastrados(ponteiro_perfil, num_perfis);
    //Chamar a função bubble sort para ordenar os e-mails
    listar_emails_cadastrados(ponteiro_perfil,num_perfis);
    //Chamar a função bubble sort para ordenar os nomes dos usuários
    listar_nomes_cadastrados(ponteiro_perfil,num_perfis);
    printf ("Ids                                              E-mails                                             Nomes de usuarios\n");
    for (i=0;i<num_perfis;i++){
        printf ("@%-51s %-51s %-51s\n", ponteiro_perfil[i].ID, ponteiro_perfil[i].email, ponteiro_perfil[i].nome_usuario);
    }

}

asciiImg_t * insta_carregaImagem(char url[], bool colorido, int largura) {

  FILE * arquivo;
  char buffer[BUFFER_TAMANHO];
  int nBytes, nBytesTotal = 0;
  char linhaComando[LINHA_COMANDO];

  asciiImg_t * img;

  // Aloca espaço para uma imagem
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

      // Descobre o número de bytes da linha
      for(nBytes = 0; buffer[nBytes] != 0; nBytes++);

      // Aloca o espaço
      img->bytes = realloc(img->bytes, sizeof(unsigned char) * (nBytesTotal + nBytes));

      // Copia para o espaço alocado
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

  // Verifica se a imagem é válida
  if (img->nBytes < LIMIAR_INFERIOR_TAMANHO_IMAGEM) {
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
void insta_imprimeImagem(asciiImg_t * img) {
  printf("%s", img->bytes);
}

/**
 *  \brief Função que libera a memória alocada por uma imagem.
 *  
 *  \param [in] img Endereço da estrutura com os dados da imagem a ser liberada.
 */
void insta_liberaImagem(asciiImg_t * img) {
  free(img->bytes);
  free(img);
}
//Função para cadastro de uma postagem
void cadastro_postagem(posts_t **ponteiro_postagem,int *num_postagens){
    posts_t postagens;

    printf ("\t\tPOSTAGEM\t\t\n");
    printf ("Digite o nome de seu post:\n");
    fgets (postagens.ID_post, NUM_MAX_CARACTERES_ID, stdin);
    util_removeQuebraLinhaFinal(postagens.ID_post);
    /*
    printf ("Agora de upload na imagem de seu post:\n");
    printf ("Para isso digite o url de sua imagem com o jpg no final\n");
    printf ("Exemplo: https://img.freepik.com/fotos-premium/fundo-de-rosas-bonitas_534373-220.jpg\n");
    fgets (postagens.url, BUFFER_TAMANHO, stdin);
    util_removeQuebraLinhaFinal(postagens.url);
    postagens.img = insta_carregaImagem(postagens.url, MODO_IMAGEM, IMAGEM_NUMERO_COLUNAS);
    if (postagens.img == NULL) {
        // Falha ao carregar a imagem
        fprintf(stderr, "Falha ao carregar a imagem da URL %s\n", postagens.url);
        return ERRO_CARREGAR_IMAGEM;
    }
    */
    printf ("Digite uma legenda para seu post: (MAX 300 caracteres)\n");
    fgets(postagens.legenda, NUM_MAX_CARACTERES_LEGENDA ,stdin);
    util_removeQuebraLinhaFinal(postagens.legenda);

    (*num_postagens)++;
    *ponteiro_postagem = realloc(*ponteiro_postagem,*num_postagens * sizeof(posts_t));
    (*ponteiro_postagem)[*num_postagens-1] = postagens;
}
//Função para imprimir informações de posts
void imprime_posts(posts_t * ponteiro_postagem,int num_postagens){
    int i;

    if (num_postagens < 1){
        printf ("Voce nao postou posts ainda!\n");
        return;
    }

    printf ("SEUS POSTS\n");
    for (i=0;i<num_postagens;i++){
        printf ("NUMERO DO POST: %d\n", i+1);
        printf ("Titulo\n");
        printf ("%-30s\nLegenda\n %-300s\n", ponteiro_postagem[i].ID_post,ponteiro_postagem[i].legenda);
    }
    /*
    // Mostra a imagem, o número de bytes e libera a memória
    insta_imprimeImagem(ponteiro_postagem->img);
    printf("N.Bytes Imagem: %d\n", ponteiro_postagem->img->nBytes);
    insta_liberaImagem(ponteiro_postagem->img);
    */
}
void editar_posts(posts_t * ponteiro_postagem,int num_postagens){
    int i;
    int opcao, index;

    if (num_postagens < 1){
        printf ("Voce nao postou posts ainda!\n");
        return;
    }
    printf ("\t\tSEUS POSTS:\n");

    for (i=0;i<num_postagens;i++){
        printf ("%d. %-30s\n", i+1, ponteiro_postagem[i].ID_post);
    }
    do {
        printf ("O que voce deseja editar no seu post?\n");
        printf ("(1) <TITULO DO MEU POST>\n(2) <IMAGEM DO POST>\n(3) <LEGENDA DO POST>\n(0) <SAIR>\n");
        printf ("Digite o numero correspondente a sua opcao:\n");
        scanf ("%d", &opcao);
        getchar();
        switch (opcao){
            case 1:{
                printf ("Qual titulo voce deseja mudar?\n");
                for (i=0;i<num_postagens;i++){
                    printf ("%d. %-30s\n", i+1, ponteiro_postagem[i].ID_post);
                }
                printf ("Digite o numero do titulo correspondente ao post que voce quer alterar:\n");
                scanf ("%d", &index);
                getchar();
                index--;
                if (index < 0 || index >= num_postagens) {
                    printf("Opcao invalida!\n");
                    return;
                }
                printf ("Titulo do seu post:\n");
                printf ("%-30s\n", ponteiro_postagem[index].ID_post);
                printf ("Digite o que voce quer mudar no titulo:\n");
                fgets(ponteiro_postagem[index].ID_post,NUM_MAX_CARACTERES_ID,stdin);
                util_removeQuebraLinhaFinal(ponteiro_postagem[index].ID_post);
                printf ("Seu novo titulo do post:\n");
                printf ("%-30s\n", ponteiro_postagem[index].ID_post);
                break;
            }
            case 2:{
                break;
            }
            case 3:{
                printf ("Digite o post que voce deseja acessar:\n");
                for (i=0;i<num_postagens;i++){
                    printf ("%d. %-30s\n", i+1, ponteiro_postagem[i].ID_post);
                }
                scanf ("%d", &index);
                getchar();
                index--;
                if (index < 0 || index >= num_postagens) {
                    printf("Opcao invalida!\n");
                    return;
                }
                printf ("Legenda do seu post:\n");
                printf ("LEGENDA:\n");
                printf ("%-300s\n", ponteiro_postagem[index].legenda);
                printf ("Digite o que voce quer mudar na legenda:\n");
                fgets (ponteiro_postagem[index].legenda, NUM_MAX_CARACTERES_LEGENDA, stdin);
                util_removeQuebraLinhaFinal(ponteiro_postagem[index].legenda);
                printf ("Sua nova legenda:\n");
                printf ("%-300s\n", ponteiro_postagem[index].legenda);
                break;
            }
            case 0:{
                break;
            }
            default:{
                printf ("Opcao invalida!!!\n");
            }
        }    

    }while (opcao != 0);

}
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
//Função principal
int main(int argc, char **argv) {
    int opcao, escolha, escolha2,escolha3;
    perfil_t *ponteiro_perfil = NULL;
    posts_t *ponteiro_postagem = NULL;
    login_t login_info;
    int num_perfis = 0;
    int num_postagens = 0;
    bool logado = false;

    printf("Bem vindo ao Coltegram!\n");
    printf("Instagram feito por:\nIcaro Cardoso Nascimento\nJoao Henrique Santana Oliveira Campos\nMatheus Fernandes de Oliveira Brandemburg\n");
    do {
        printf("Bem vindo ao Coltegram!!!\n");
        printf("Digite sua opcao:\n");
        printf("<Cadastar> (1)\n<Login> (2)\n<Sair> (0)\n");
        scanf("%d", &opcao);
        getchar();
        switch (opcao) {
            case 1:{
                cadastro_perfil(&ponteiro_perfil, &num_perfis);
                break;
            }    
            case 2:{
                login(ponteiro_perfil, num_perfis, &login_info, &logado);
                while (logado && escolha != 0) {
                    printf("Quais acoes voce deseja executar:\n");
                    printf("(1) <Buscar perfis>\n(2) <Visitar perfis>\n(3) <Listar perfis cadastrados>\n(4) <Acoes do usuario>\n(0) <Sair>\n");
                    scanf("%d", &escolha);
                    getchar();
                    switch (escolha) {
                        case 1:{
                            //Buscar perfis
                            break;
                        }
                        case 2:{
                            //Visitar perfis
                            break;
                        }
                        case 3:{
                            //Listar perfis cadastrados
                            do {
                                printf ("\t\tLISTAR PERFIS CADASTRADOS NO SISTEMA:\n");
                                printf ("(1) <Listar IDs>\n(2) <Listar e-mails>\n(3) <Listar nome dos usuarios>\n(4) <Listar todas as informacoes>\n(0) <Parar>\n");
                                printf ("O que voce deseja realizar:\n");
                                scanf ("%d", &escolha3);
                                getchar();
                                switch (escolha3){
                                    case 1:{
                                        //Listar Ids em ordem alfabetica
                                        imprimir_IDs_cadastrados (ponteiro_perfil, num_perfis);
                                        break;
                                    }
                                    case 2:{
                                        //Listar e-mails em ordem alfabetica
                                        imprimir_emails_cadastrados(ponteiro_perfil,num_perfis);
                                        break;
                                    }
                                    case 3:{
                                        //Listar nome dos usuarios em ordem alfabetica
                                        imprimir_nomes_cadastrados(ponteiro_perfil,num_perfis);
                                        break;
                                    }
                                    case 4:{
                                        //Imprime todas as informações juntas
                                        imprimir_tudo_cadastrado(ponteiro_perfil,num_perfis);
                                        break;
                                    }
                                    case 0:{
                                        printf ("Saindo...\n");
                                        break;
                                    }
                                    default:{
                                        printf ("Opcao invalida!!!\n");
                                    }
                                }
                            } while (escolha3 != 0);
                            break;
                        }
                        case 4:{
                            do {
                                printf("Acoes do usuario:\n");
                                printf("(1) <Postar Posts>\n(2) <Editar Posts>\n(3) <Listar Posts>\n(4) <Detalhar Posts>\n(5) <Apagar posts>\n(6) <Deslogar>\n(0) <Sair>\n");
                                printf("O que voce deseja fazer?\n");
                                scanf("%d", &escolha2);
                                getchar();
                                switch (escolha2) {
                                    case 1:{
                                        //Postar posts
                                        cadastro_postagem(&ponteiro_postagem,&num_postagens);
                                        imprime_posts(ponteiro_postagem,num_postagens);
                                        break;
                                    }
                                    case 2:{
                                        //Editar posts
                                        editar_posts(ponteiro_postagem,num_postagens);
                                        break;
                                    }
                                    case 3:{
                                        //Listar posts
                                        imprime_posts(ponteiro_postagem,num_postagens);
                                        break;
                                    }
                                    case 4:{
                                        //Detalhar posts
                                        break;
                                    }
                                    case 5:{
                                        //Apagar posts
                                        excluir_posts(ponteiro_postagem,num_postagens);
                                        break;
                                    }
                                    case 6:{
                                        printf("Saindo do perfil...\n");
                                        logado = false;
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
                            } while (escolha2 != 0 && logado);
                            break;
                        }
                        case 0:{
                            printf ("Saindo...\n");
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
    
    //Libera a memória alocada
    free (ponteiro_perfil);
    free (ponteiro_postagem);
    //Se chegou ate aqui é porque correu tudo bem
    return SUCESSO;
}