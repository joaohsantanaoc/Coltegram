#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCESSO 0 
#define CARACTERES_MAX 51

typedef struct cardapio_s{
    char prato[CARACTERES_MAX];
    char ingrediente[CARACTERES_MAX];
    float preco;


}cardapio_t;

void util_removeQuebraLinhaFinal(char dados[]) {
    int tamanho;
    tamanho = strlen(dados);
    if ((tamanho > 0) && (dados[tamanho - 1] == '\n')) {
        dados[tamanho - 1] = '\0';
    }
}

void cadastrar_pratos(cardapio_t ** ponteiro_cardapio, int * num_pratos){
    cardapio_t novo_cardapio;
    int i;
    int contador_virgulas = 0;

    printf ("Digite o nome do prato:\n");
    fgets (novo_cardapio.prato, CARACTERES_MAX, stdin);
    util_removeQuebraLinhaFinal(novo_cardapio.prato);
    printf ("Digite os nomes do ingrediente do prato: (MIN 3)\n");
    printf ("Digitar os ingredientes separados por virgula!!!\n");
    fgets (novo_cardapio.ingrediente, CARACTERES_MAX,stdin);
    util_removeQuebraLinhaFinal(novo_cardapio.ingrediente);
    for (i=0; novo_cardapio.ingrediente[i] != '\0';i++){
        if (novo_cardapio.ingrediente[i] == ','){
            contador_virgulas++;
        }
    }
    if (contador_virgulas < 2){
        printf ("Eh necessario digitar tres ingredientes!\n");
        return; 
    }
    printf ("Digite o preco do prato:\n");
    scanf ("%f", &novo_cardapio.preco);

    (*num_pratos)++;
    *ponteiro_cardapio = realloc (*ponteiro_cardapio, (*num_pratos) * sizeof (cardapio_t));
    (*ponteiro_cardapio)[*num_pratos - 1] = novo_cardapio;
    printf ("Cadastro concluido!\n");

}
void listar_unico_prato(cardapio_t * ponteiro_cardapio, int num_pratos, int index){
    if (index >= 0 && index < num_pratos){
        printf ("%-51s %-51s %7.2f\n", ponteiro_cardapio[index].prato, ponteiro_cardapio[index].ingrediente, ponteiro_cardapio[index].preco );
        printf ("......................................................................................................................................\n");

    }

}
void listar_pratos(cardapio_t * ponteiro_cardapio, int num_pratos){
    int i;
    if (num_pratos == 0){
        printf ("Cardapio vazio!\n");
        return;

    }
    printf ("NOME DO PRATO:                                       INGREDIENTES:                                          PRECO:\n");
    for (i=0;i<num_pratos;i++){
        listar_unico_prato (ponteiro_cardapio, num_pratos, i);   
    }

}

int main (int argc,char ** argv){
    int opcao;
    int num_pratos = 0;
    cardapio_t * ponteiro_cardapio = NULL;
    int index;

    do {
        printf ("Aqui esta o nosso cardapio!\n");
        printf ("Digite o numero correspondente a cada funcao e digite 0 para sair:\n");
        printf ("<CADASTRAR> (1)\n<LISTAR UNICO PRATO> (2)\n<IMPRIMIR TABELA> (3)\n<PARAR> (0)\n");
        scanf ("%d", &opcao);
        getchar();

        switch (opcao){
            case 1:
            cadastrar_pratos (&ponteiro_cardapio, &num_pratos);
            break;
            case 2:
            if (num_pratos > 0){
                printf ("Digite o indice do prato:\n");
                scanf ("%d", &index);
                printf ("NOME DO PRATO:                                       INGREDIENTES:                                          PRECO:\n");
                listar_unico_prato(ponteiro_cardapio,num_pratos,index);

            }else {
                printf ("Cardapio vazio!\n");
            }
            
            break;
            case 3:
            listar_pratos(ponteiro_cardapio, num_pratos);
            break;
            case 0:
            printf ("Cardapio acabou!\n");
            break; 
            default:
            printf ("Opcao Invalida!\n");
        }

    }while (opcao != 0);
    free (ponteiro_cardapio);

    return SUCESSO; 
}