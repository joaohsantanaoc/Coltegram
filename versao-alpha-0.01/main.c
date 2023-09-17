//A ideia é pegar todos os outros arquivos .c funcionando e fazer funcionar aqui!
//Sugiro que esse main seja mudado apenas sobre supervisão de todo grupo 

//Mantenha o código limpo pelo amor de deus!!!


#include<stdio.h> //Biblioteca básica

#include<stdlib.h> //Biblioteca alocação dinâmica

#include<string.h> //Biblioteca para strings

#include<locale.h> //Biblioteca para introduzir acentos

#include <stdbool.h> //variável de 2 estados 

#include <ctype.h>

#define SUCESSO 0

#define Tamanho_Maximo 50

typedef struct curtida
{
    bool curtida;
}curtida;

typedef struct comentario
{
    char mensagem[Tamanho_Maximo];
}comentario;

typedef struct postagem
{
    curtida numeroCurtida [Tamanho_Maximo];
    comentario numeroComentario [Tamanho_Maximo];
    char descricao[Tamanho_Maximo];
    //imagem
}postagem;

typedef struct perfil
{
    int ID;
    char nome[Tamanho_Maximo];
    char email [Tamanho_Maximo];
    char senha [Tamanho_Maximo];
    char senha_confirmada[Tamanho_Maximo];
    postagem dadosPostagem[Tamanho_Maximo];
}perfil;

    //Função para tirar quebra linha das strings
void util_removeQuebraLinhaFinal(char dados[]) {
    int tamanho;
    tamanho = strlen(dados);
    if ((tamanho > 0) && (dados[tamanho - 1] == '\n')) {
        dados[tamanho - 1] = '\0';
    }
}

    //Função que verifica se já existe um usuário com o mesmo nome
int usuario_existente(perfil * perfis, int num_perfis, char * nome){
    int i;
    for (i = 0; i < num_perfis; i++){
        if (strcmp (perfis[i].nome, nome) == 0){
            return 1;   //usuario com o mesmo nome foi encontrado
        }
    }
    return 0;   //usuario com o mesmo nome não foi encontrado
}

    //Função que verifica se existe um mesmo ID
int id_existente(perfil * perfis, int num_perfis, int ID){
    int i;
    for (i = 0;i < num_perfis; i++){
        if (perfis[i].ID  == ID){
            return 1;   //ID já existente
        }
    }
    return 0;   //ID não existe
}

    //Função que verifica se já existe um usuário com o mesmo email
int email_existente(perfil *perfis, int num_perfis, char * email){
    int i;
    for (i = 0; i < num_perfis; i++){
        if (strcmp (perfis[i].email, email) == 0){
            return 1;   //Email já existe
        }
    }
    return 0;   //Email não cadastrado ainda 
}

    //Função que verifica se o formato do e-mail é válido, No classroom está dizendo que o @ precisa estar seguido de um domínio, alguém tem alguma dica sobre o que é isso?
int verificar_Email(perfil *perfis){ 

int contagemArroba = 0;

       for (int i = 0; i < Tamanho_Maximo; i++){

            if (perfis->email[i] == "@"){

                contagemArroba++;
            }
        }
            if (contagemArroba != 1){
                return -1; //Retorna email inválido
            }
            else{
                return SUCESSO; //Retorna email Válido
            }
}
/*Itens a serem reavaliados:

    1) É realmente necessário a função "id_existente"?
    No meu pensamento o ID poderia ser mantido simples e ser apenas uma variável do tipo int que mostra qual a posição o perfil está salvo no vetor.

    2) É possível tornar a função cadastrar_usuario mais simples. Ela está enorme e muito confusa, principalmente a passagem dos ponteiros e parâmetros.
*/

int main(int argc, char**argv){


    return SUCESSO;
}