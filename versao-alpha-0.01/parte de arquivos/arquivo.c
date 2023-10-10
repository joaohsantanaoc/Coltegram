//Bibliotecas presentes no código
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Constantes presentes no código
#define SUCESSO 0
#define NUM_MAX_CARACTERES_STRING 30

//Estrutura para dados de uma pessoa
typedef struct pessoa_s{
    char nome[NUM_MAX_CARACTERES_STRING];
    int idade;
    char sexo; 
}pessoa_t;

//Função para tirar o '\n' das strings
void util_removeQuebraLinhaFinal(char dados[]) {
    int tamanho;
    tamanho = strlen(dados);
    if ((tamanho > 0) && (dados[tamanho - 1] == '\n')) {
        dados[tamanho - 1] = '\0';
    }
}

void cadastro(pessoa_t ** ponteiro_pessoa, int * num_pessoas){
    pessoa_t pessoa;

    printf ("Digite o nome da pessoa:\n");
    fgets (pessoa.nome, NUM_MAX_CARACTERES_STRING, stdin);
    util_removeQuebraLinhaFinal (pessoa.nome);
    printf ("Digite o sexo dessa pessoa:\n");
    printf ("(M ou m) Masculino\n(F ou f) Feminino\n");
    scanf ("%c", &pessoa.sexo);
    printf ("Agora digite a idade dessa pessoa:\n");
    scanf ("%d", &pessoa.idade);

    (*num_pessoas)++;
    *ponteiro_pessoa = realloc (*ponteiro_pessoa, (*num_pessoas) * sizeof (pessoa_t));
    (*ponteiro_pessoa)[*num_pessoas - 1] = pessoa;

    printf ("Cadastro concluido!!!\n");

}


//Função para listar todos os dados cadastrados
void listar_em_tabela(pessoa_t * ponteiro_pessoa, int num_pessoas){
    int i;

    if (num_pessoas < 1){
        printf ("Nao ha pessoas cadastradas!!!\n");
        return SUCESSO;
    }

    printf ("NUMERO  NOME DA PESSOA                  SEXO IDADE\n");
    for (i=0;i<num_pessoas;i++){
        printf ("%2d        %-30s %2c   %2d\n", i+1,ponteiro_pessoa[i].nome, ponteiro_pessoa[i].sexo ,ponteiro_pessoa[i].idade);
    }

}
//Função para salvar os dados cadastrados em um arquivo
void salvar_em_arquivo(pessoa_t * ponteiro_pessoa,int num_pessoas){
    FILE * arquivo;
    int i;

    arquivo = fopen("arquivo.txt", "w");
    if (!arquivo){
        printf ("Nao foi possivel abrir o arquivo");
        exit(1);
    }else {
        fprintf (arquivo,"%d\n", num_pessoas);
        for (i=0;i<num_pessoas;i++){
            fprintf (arquivo, "%2d %-30s %2c %2d\n", i+1 ,ponteiro_pessoa[i].nome,ponteiro_pessoa[i].sexo,ponteiro_pessoa[i].idade);
        }
    }
    fclose(arquivo);

}
void ler_arquivo(pessoa_t * ponteiro_pessoa,int num_pessoas){
    FILE * arquivo;
    int i;

    arquivo = fopen("arquivo.txt", "r");
    if (!arquivo){
        printf ("Nao foi possivel abrir o arquivo");
        exit(1);
    }else {
        fscanf (arquivo,"%d\n", &num_pessoas);
        for (i=0;i<num_pessoas;i++){
            fgets (ponteiro_pessoa[i].nome, NUM_MAX_CARACTERES_STRING, arquivo);
            fscanf(arquivo,"%c",&ponteiro_pessoa[i].sexo);
            fscanf(arquivo,"%d",&ponteiro_pessoa[i].idade);
            fprintf (arquivo, "%2d %-30s %2c %2d\n", i+1 ,ponteiro_pessoa[i].nome,ponteiro_pessoa[i].sexo,ponteiro_pessoa[i].idade);
        }
    }
    fclose(arquivo);

}
//Função principal
int main (int argc,char ** argv){
    pessoa_t *ponteiro_pessoa = NULL;
    int num_pessoas = 0;
    int opcao;


    do {
        printf ("Bem vindo ao nosso sistema!\n");
        printf ("O que voce deseja realizar:\n");
        printf ("(1) <CADASTRO>\n(2) <LISTAR EM TABELA>\n(0) <PARAR>\n");
        scanf ("%d", &opcao);
        getchar ();

        switch (opcao){
            case 1:{
                cadastro(&ponteiro_pessoa,&num_pessoas);
                salvar_em_arquivo(ponteiro_pessoa,num_pessoas);
                break;
            }
            case 2:{
                listar_em_tabela(ponteiro_pessoa,num_pessoas);
                ler_arquivo (ponteiro_pessoa,num_pessoas);
                break;
            }
            case 0:{
                printf ("Sistema finalizado!\n");
                break;
            }
            default:{
                printf ("Opcao invalida!\n");
            }
        }

    }while (opcao != 0);
   
    
    free (ponteiro_pessoa);
    //Se tudo correu bem é porque chegou aqui!!!
    return SUCESSO;
}