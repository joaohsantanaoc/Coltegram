//Bibliotecas presentes no código
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//Sucesso para retornar 
#define SUCESSO 0 
//Constantes para delimitar o máximo de caracteres
#define NUM_MAX_CARACTERES_NOME_USUARIO (99+1)
#define NUM_MAX_CARACTERES_EMAIL (50+1)
#define NUM_MAX_CARACTERES_SENHA (50+1)

//Estrutura do perfil
typedef struct perfil_s{
    //Variáveis presentes no perfil
    int ID;
    char nome_usuario[NUM_MAX_CARACTERES_NOME_USUARIO];
    char email[NUM_MAX_CARACTERES_EMAIL];
    char senha[NUM_MAX_CARACTERES_SENHA];
    char senha_confirmada[NUM_MAX_CARACTERES_SENHA];

}perfil_t;

//Função para tirar quebra linha das strings
void util_removeQuebraLinhaFinal(char dados[]) {
    int tamanho;
    tamanho = strlen(dados);
    if ((tamanho > 0) && (dados[tamanho - 1] == '\n')) {
        dados[tamanho - 1] = '\0';
    }
}
//Função para cadastro
void cadastro_perfil(perfil_t ** ponteiro_perfil, int * num_perfis){
    perfil_t perfis;

    printf ("\t\tBem vindo ao cadastro do seu perfil!!!\t\t\n");
    printf ("Digite o ID do seu usuario:\n");
    scanf ("%d", &perfis.ID);
    getchar();
    printf ("Digite o nome do seu usuario:\n");
    fgets (perfis.nome_usuario, NUM_MAX_CARACTERES_NOME_USUARIO, stdin);
    util_removeQuebraLinhaFinal (perfis.nome_usuario);
    printf ("Agora digite o seu e-mail:\n");
    fgets (perfis.email, NUM_MAX_CARACTERES_EMAIL, stdin);
    util_removeQuebraLinhaFinal (perfis.email);
    printf ("Digite sua senha:\n");
    fgets (perfis.senha, NUM_MAX_CARACTERES_SENHA, stdin);
    util_removeQuebraLinhaFinal (perfis.senha);
    printf ("Agora confirme sua senha:\n");
    fgets (perfis.senha_confirmada, NUM_MAX_CARACTERES_SENHA, stdin);
    util_removeQuebraLinhaFinal (perfis.senha_confirmada);
    //Compara se a senha confirmada é igual a senha anteriormente digitada
    while (strcmp (perfis.senha,perfis.senha_confirmada) != 0){
        printf ("Senha incorreta!Tente novamente!\n");
        printf ("Digite sua senha:\n");
        fgets (perfis.senha, NUM_MAX_CARACTERES_SENHA, stdin);
        util_removeQuebraLinhaFinal (perfis.senha);
        printf ("Agora confirme sua senha:\n");
        fgets (perfis.senha_confirmada, NUM_MAX_CARACTERES_SENHA, stdin);
        util_removeQuebraLinhaFinal (perfis.senha_confirmada);
    }

    (*num_perfis)++;
    *ponteiro_perfil = realloc (*ponteiro_perfil,(*num_perfis) * sizeof (perfil_t));
    (*ponteiro_perfil)[*num_perfis -1] = perfis;
    printf ("Cadastro concluido!!!\n");


}

//Função principal
int main (int argc,char ** argv){
    int opcao;
    perfil_t *ponteiro_perfil = NULL;
    int num_perfis=0;

    printf ("Bem vindo ao Coltegram!\n");
    printf ("Instagram feito por:\nIcaro Cardoso Nascimento\nJoao Henrique Santana Oliveira Campos\nMatheus Fernandes de Oliveira Brandemburg\n");
    do{
        printf ("Bem vindo ao Coltegram!!!\n");
        printf ("Digite sua opcao:\n");
        printf ("<Cadastar> (1)\n<Sair> (0)\n");
        scanf ("%d", &opcao);

        switch (opcao){
            case 1:
            cadastro_perfil (&ponteiro_perfil, &num_perfis);
            break;
            case 0:
            printf ("Volte sempre!!!\n");
            break;
            default:
            printf ("Opcao invalida!!!\n");
        }

    }while (opcao != 0);

    free (ponteiro_perfil);

    //Se chegou até aqui é porque ocorreu tudo bem!!!
    return SUCESSO;
}