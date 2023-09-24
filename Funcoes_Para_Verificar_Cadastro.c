#include<stdio.h> //Biblioteca básica

#include<stdlib.h> //Biblioteca alocação dinâmica

#include<string.h> //Biblioteca para strings

#include<locale.h> //Biblioteca para introduzir acentos

#include <stdbool.h> //variável de 2 estados 

#include <ctype.h>

#define SUCESSO 0

#define Tamanho_Maximo 50

#define Espaco_Em_Branco 1

#define Erro_No_Arroba 2

#define Email_Em_Uso 3

#define ID_Em_Uso 4

#define Nome_Em_Uso 5



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


int verificarEspacoEmBranco(char string[]){

    for (int i = 0; string[i] != '\0'; i++){
        if (string[i] == ' '){
                    return Espaco_Em_Branco;
                }
    }
    return SUCESSO;
}

int verificarArroba(char string[]){

    int contadorArroba = 0;

        for (int i = 0; string[i] != '\0'; i++) {
            contadorArroba ++;
        }

        if (contadorArroba != 1) {
            return Erro_No_Arroba;
        }
    return SUCESSO;
}

int verificarDisponibilidadeEmail(char email[], perfil **vetorDeDados, int numeroDePerfis){

    for (int i = 0; i < numeroDePerfis; i++){
        if (strcmp(vetorDeDados[i]->email, email) == 0){
            return Email_Em_Uso;
        }
    }
    return SUCESSO;
}

int verificarDisponibilidadeID(char ID[], perfil **vetorDeDados, int numeroDePerfis){

        for (int i = 0; i < numeroDePerfis; i++){
        if (strcmp(vetorDeDados[i]->ID, ID) == 0){
            return ID_Em_Uso;
        }
    }
    return SUCESSO;
}

int verificarNomeUsuario(char nomeUsuario[], perfil **vetorDeDados, int numeroDePerfis){

    for (int i = 0; i < numeroDePerfis; i++){
        if (strcmp(vetorDeDados[i]->nome, nomeUsuario == 0) ){
            return Nome_Em_Uso;
        }
    }

    return SUCESSO;
}



int verificarEmail(char email[], perfil **vetorDeDados, int numeroDePerfis){ //Função verifica se o email possui @, espaços em branco e se ja foi utilizado

    int contadorArroba = 0;

        for (int i = 0; email[i] != '\0'; i++){
        
            //Verifica espaços em branco

            if (email[i] == ' '){
                return Espaco_Em_Branco;
            }

            if (email[i] == '@'){
                contadorArroba ++;
            }

        }

        if (contadorArroba != 1){
            return Erro_No_Arroba;
        }

        for (int i = 0; i < numeroDePerfis; i++)
        {
            if(strcmp(vetorDeDados[i]->email, email) == 0){
                return Email_Em_Uso;
            }
        }
        

    return SUCESSO;
}