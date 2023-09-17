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
//Função que verifica se já existe um usuário com o mesmo nome
int usuario_existente(perfil_t * perfis, int num_perfis,char * nome){
    int i;
    for (i=0;i<num_perfis;i++){
        if (strcmp(perfis[i].nome_usuario, nome) == 0){
            return 1;//usuario com o mesmo nome foi encontrado
        }
    }
    return 0;//usuario com o mesmo nome não foi encontrado
}
//Função que verifica se existe um mesmo ID
int id_existente(perfil_t * perfis, int num_perfis, int ID){
    int i;
    for (i=0;i<num_perfis;i++){
        if (perfis[i].ID  == ID){
            return 1;//ID já existente
        }
    }
    return 0;//ID não existe

}
//Função que verifica se já existe um usuário com o mesmo email
int email_existente(perfil_t *perfis, int num_perfis, char * email){
    int i;
    for (i=0;i<num_perfis;i++){
        if (strcmp(perfis[i].email, email) == 0){
            return 1;//Email já existe
        }
    }
    return 0;//Email não cadastrado ainda 

}
//Função para cadastro
void cadastro_perfil(perfil_t ** ponteiro_perfil, int * num_perfis){
    perfil_t perfis;
    int i;
    int contador_arroba = 0, contador_espaco = 0;

    printf ("\t\tBem vindo ao cadastro do seu perfil!!!\t\t\n");
    printf ("Digite o ID do seu usuario: (somente em numeros)\n");
    scanf ("%d", &perfis.ID);
    //Verifica se existe um mesmo ID em outra conta
    if (id_existente(*ponteiro_perfil,*num_perfis,perfis.ID)){
        printf ("ID já existe!!!\n");
        printf ("Digite um ID valido:\n");
        scanf ("%d", &perfis.ID);
    }
    getchar();
    do{
        printf ("Digite o nome do seu usuario:\n");
        fgets (perfis.nome_usuario, NUM_MAX_CARACTERES_NOME_USUARIO, stdin);
        util_removeQuebraLinhaFinal (perfis.nome_usuario);
        //Verifica se existe um usuario com o mesmo nome:
        if (usuario_existente(*ponteiro_perfil, *num_perfis, perfis.nome_usuario)){
            printf ("Ja existe um usuario com o mesmo nome. Digite novamente o nome do seu usuario:\n");
            fgets (perfis.nome_usuario, NUM_MAX_CARACTERES_NOME_USUARIO, stdin);
            util_removeQuebraLinhaFinal (perfis.nome_usuario);
        }
        //Se o nome do usuario possuir espaços em branco, o programa pedirá para digitar novamente
        for (i=0;perfis.nome_usuario[i] != '\0';i++){
            if (perfis.nome_usuario[i] == ' '){
                contador_espaco++;
            }
        }
        if (contador_espaco > 0){
            printf ("O nome do usuario nao pode conter espacos!!!\n");
        }
    }while (contador_espaco > 0);
    do{
        printf ("Agora digite o seu e-mail:\n");
        fgets (perfis.email, NUM_MAX_CARACTERES_EMAIL, stdin);
        util_removeQuebraLinhaFinal (perfis.email);
        //Se o email nao possuir @, pedirá para digita-lo novamente
        for (i=0;perfis.email[i] != '\0';i++){
            if (perfis.email[i] == '@'){
                contador_arroba++;
            }
        }
        if (contador_arroba < 1){
            printf ("E-mail invalido ou inexistente!!!\n");
        }
        //Verifica se existe um mesmo endereço de email existente 
        if (email_existente(*ponteiro_perfil,*num_perfis,perfis.email)){
            printf ("Email ja existe em outra conta:\n");
            printf ("Digite um email valido:\n");
            fgets (perfis.email, NUM_MAX_CARACTERES_EMAIL, stdin);
            util_removeQuebraLinhaFinal (perfis.email);
        }
    }while (contador_arroba < 1);
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