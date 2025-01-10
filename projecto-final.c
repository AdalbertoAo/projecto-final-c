#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include<string.h>

#define TAM 1000

/*      Criando os Registos     */
struct EQUIPE {
    int id;
    char nomeEquipe[30];
    char cidade[30];
    int dataFundacao;
    char nomeTreinador[30];
    int totalJogadores;
};

struct JOGADOR {
    int id;
    char nomeJogador[30];
    int idade;
    int numeroCamisa;
    char posicao[20];
};

typedef struct JOGADOR jogador;
typedef struct EQUIPE equipe;


/*      Variáveis Globais       */
int totalJogadores = 0;
equipe v_equipes[TAM];
jogador v_jogador[TAM];
int totalEquipes = 0;
/*--------------------------------*/

/*      TRABALHANDO COM FICHEIRO      */

// Criar FIcheiro
void criarFicheiro(){
FILE *ficheiro;
ficheiro = fopen("base_de_Dados.txt","w");
if (ficheiro == NULL){
    printf("Erro na Abertura do ficheiro");
    fclose(ficheiro);
    exit(1);
}else printf("Ficheiro aberto com sucesso");

}
/*----------------------------------------------*/



int main(){
    criarFicheiro();
    return 0;
}