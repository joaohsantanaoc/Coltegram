/*Trabalho coltecgram
Trio:
Matheus Fernandes de Oliveira Brandemburg
João Henrique Santana
Icaro Cardoso
104-B

Tarefa:
Fazer uma versão de terminal das postagens do Instagram.

Requisitos:
Seu programa deve implementar as funcionalidades básicas do sistema
Instagram contendo os seguintes módulos:
• Perfil
• Postagens
• Comentários
• Curtidas

*********************************************************************************************************************

Struct do perfil deve conter: TODOS OS DADOS DESSA STRUCT SERÃO GUARDADOS EM VETOR DE CHAR

• ID: Identificador único do perfil. Esse identificador deverá ser
utilizado para fazer todas as ligações entre um perfil e qualquer
outro elemento do sistema.

• Nome: Nome do usuário

• E-mail: E-mail do usuário. Deve ser um e-mail válido no sentido de
que deve possuir um @ seguido de um domínio. O e-mail que é deverá
ser utilizado para logar no sistema. Não pode existir mais de um
e-mail igual cadastrado no sistema.

• Senha: Senha utilizada para logar no sistema.

*********************************************************************************************************************

Struct das postagens deve conter:

• ID: Identificador único fornecido pelo sistema para identificar uma 
postagem.


• Imagens: Lista de imagens em ASCII.


• Descrição: Texto descritivo a ser impresso logo após a(s)
imagem(ns).


• Comentários: Lista de comentários feitos por outros usuários na
referida postagem.


• Curtidas: Lista dos usuários que curtiram a postagem.
Struct{
    matriz de char = Para representar as pessoas que curtiram
    
}
Matriz de char?
*/

#include<stdio.h> //Biblioteca básica

#include<stdlib.h> //Biblioteca alocação dinâmica

#include<string.h> //Biblioteca para strings

#include<locale.h> //Biblioteca para introduzir acentos

#include <stdbool.h> //variável de 2 estados 

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
    postagem dadosPostagem[Tamanho_Maximo];
}perfil;


int main(int argc, char**argv){

    setlocale(LC_ALL, "Portuguese");//Permite que acentos e caracteres especiais sejam mostrados no terminal. Não está funcionando no VScode.

    //Espaço para variáveis:
    perfil perfisSalvos[Tamanho_Maximo];

    //Fim do espaço.
    printf("Olá, Digite a opção desejada:\n");
    printf("(1)- Login\n");
    printf("(2)- Listar usuários\n");
    printf("(3)- Cadastrar\n");



    return SUCESSO;
}