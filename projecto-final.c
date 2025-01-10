#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>

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
// Criar Ficheiro
void criarFicheiro() {
    FILE *ficheiro;
    ficheiro = fopen("base_de_Dados.txt", "w");
    if (ficheiro == NULL) {
        printf("Erro na Abertura do ficheiro");
        fclose(ficheiro);
        exit(1);
    } else {
        printf("Ficheiro aberto com sucesso");
    }
}

/*----------------------------------------------*/

/*      TODAS AS FUNCOES        */
bool cadastrar_jogador(jogador j);

int main() {
    jogador j;
    printf("\n SEJA BEM-VINDO AO GIRABOLA \n ");
    int opcao;
    do {
        printf("\n 1- Cadastrar Equipes \n");
        printf(" 2- Listar Equipes \n");
        printf(" 3-  \n");
        printf(" 4-  \n");
        printf(" 5-  \n");
        printf(" 6-  \n");
        printf(" 7-  \n");
        printf(" 8-   \n");
        printf(" 9- Terminar  \n");
        printf(" Opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o '\n' deixado pelo scanf

        switch (opcao) {
            case 1:
                printf("1. Nome Completo: ");
                fgets(j.nomeJogador, sizeof(j.nomeJogador), stdin);
                j.nomeJogador[strcspn(j.nomeJogador, "\n")] = '\0'; // Remove o '\n' capturado pelo fgets

                do {
                    printf("2. Idade: ");
                    scanf("%d", &j.idade);
                    if (j.idade < 16 || j.idade > 40)
                        printf("Apenas jogadores com idades entre 16 e 40 podem ser cadastrados\n");
                } while (j.idade < 16 || j.idade > 40);

                printf("3. Numero da camisa: ");
                scanf("%d", &j.numeroCamisa);
                getchar(); // Limpa o '\n' deixado pelo scanf

                printf("4. Posição: ");
                fgets(j.posicao, sizeof(j.posicao), stdin);
                j.posicao[strcspn(j.posicao, "\n")] = '\0'; // Remove o '\n'

                if (cadastrar_jogador(j)) {
                    printf("Cadastro feito com sucesso\n");
                } else {
                    printf("Erro no cadastro\n");
                }
                break;

            case 2:
                // Listar equipes
                break;

            case 3:
                // Outro caso
                break;

            case 4:
                // Outro caso
                break;

            case 5:
                // Outro caso
                break;

            case 6:
                // Outro caso
                break;

            case 7:
                // Outro caso
                break;

            case 8:
                // Outro caso
                break;
        }
    } while (opcao != 9);

    return 0;
}

bool cadastrar_jogador(jogador j) {
    j.id = totalJogadores + 1;
    v_jogador[totalJogadores] = j;
    totalJogadores++;
    return true;
}