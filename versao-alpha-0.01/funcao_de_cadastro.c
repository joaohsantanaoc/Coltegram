//Bibliotecas presentes no programa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//Constantes presntes
#define SUCESSO 0
#define NUM_MAX_CARACTERES_NOME_USUARIO (99 + 1)
#define NUM_MAX_CARACTERES_EMAIL (50 + 1)
#define NUM_MAX_CARACTERES_SENHA (50 + 1)
#define NUM_MAX_CARACTERES_ID (50 + 1)

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
    char * tmp; 

    //Listar Ids de forma ordenada
    for (i = 0; i < (num_perfis - 1); i++) {
        for (j = 0; j < (num_perfis - 1); j++) {
        // Se estiver fora de ordem...
           if (strcmp(ponteiro_perfil->ID[j], ponteiro_perfil->ID[j + 1]) > 0) {
               // ... troca de posicao
               tmp = ponteiro_perfil->ID[j];
               ponteiro_perfil->ID[j] = ponteiro_perfil->ID[j + 1];
               ponteiro_perfil->ID[j + 1] = tmp;
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

//Função principal
int main(int argc, char **argv) {
    int opcao, escolha, escolha2,escolha3;
    perfil_t *ponteiro_perfil = NULL;
    login_t login_info;
    int num_perfis = 0;
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
            case 1:
                cadastro_perfil(&ponteiro_perfil, &num_perfis);
                break;
            case 2:
                login(ponteiro_perfil, num_perfis, &login_info, &logado);
                while (logado) {
                    printf("Quais acoes voce deseja executar:\n");
                    printf("(1) <Buscar perfis>\n(2) <Visitar perfis>\n(3) <Listar perfis cadastrados>\n(4) <Acoes do usuario>\n(0) <Sair>\n");
                    scanf("%d", &escolha);
                    getchar();

                    switch (escolha) {
                        case 1:
                        //Buscar perfis
                        break;
                        case 2:
                        //Visitar perfis
                        break;
                        case 3:
                        //Listar perfis cadastrados
                        do {
                            printf ("\t\tLISTAR PERFIS CADASTRADOS NO SISTEMA:\n");
                            printf ("(1) <Listar IDs>\n(2) <Listar e-mails>\n(3) <Listar nome dos usuarios>\n(0) <Parar>\n");
                            printf ("O que voce deseja realizar:\n");
                            scanf ("%d", &escolha3);
                            getchar();

                            switch (escolha3){
                                case 1:
                                //Listar Ids em ordem alfabetica
                                imprimir_IDs_cadastrados (ponteiro_perfil, num_perfis);
                                break;
                                case 2:
                                //Listar e-mails em ordem alfabetica
                                break;
                                case 3:
                                //Listar nome dos usuarios em ordem alfabetica
                                break;
                                case 0:
                                printf ("Saindo...\n");
                                break;
                                default:
                                printf ("Opcao invalida!!!\n");
                            }
                        } while (escolha3 != 0);
                        break;
                        case 4:
                        do {
                            printf("Acoes do usuario:\n");
                            printf("(1) <Postar Posts>\n(2) <Editar Posts>\n(3) <Listar Posts>\n(4) <Detalhar Posts>\n(5) <Apagar posts>\n(6) <Deslogar>\n(0) <Sair>\n");
                            printf("O que voce deseja fazer?\n");
                            scanf("%d", &escolha2);
                            getchar();

                            switch (escolha2) {
                                case 1:
                                //Postar posts
                                break;
                                case 2:
                                //Editar posts
                                break;
                                case 3:
                                //Listar posts
                                break;
                                case 4:
                                //Detalhar posts
                                break;
                                case 5:
                                //Apagar posts
                                printf("Piroca");
                                break;
                                case 6:
                                printf("Saindo do perfil...\n");
                                logado = false;
                                break;
                                case 0:
                                printf("Voltando ao menu principal!!!\n");
                                break;
                                default:
                                printf("Opcao invalida!!!\n");
                            }
                        } while (escolha2 != 0 && logado);
                        break;
                        case 0:
                        printf ("Saindo...\n");
                        break;
                        default:
                        printf("Opcao invalida!\n");
                }
            }
            break;
            case 0:
            printf("Volte sempre!!!\n");
            break;
            default:
            printf("Opcao invalida!!!\n");
        }

    } while (opcao != 0);

    free(ponteiro_perfil);
    return SUCESSO;
}
