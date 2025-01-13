#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define TAM 1000

/*      Criando os Registos     */
struct JOGADOR {
    int id;
    char nomeJogador[30];
    int idade;
    int numeroCamisa;
    char posicao[20];
    int equipeActual;
};
typedef struct JOGADOR jogador;
struct EQUIPE {
    int id;
    char nomeEquipe[30];
    char cidade[30];
    int dataFundacao;
    char nomeTreinador[30];
    int totalJogadores;

};

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
    FILE *file;
    file = fopen("base_de_Dados.txt", "a");
    if (file == NULL) {
        printf("Erro na Abertura do file");
        fclose(file);
        exit(1);
    } else {
        printf("file aberto com sucesso");
    }
}
// Escrever no ficheiro
void escreverFicheiro(jogador j){
        FILE *file;
    file = fopen("base_de_Dados.txt", "a");

    if (file == NULL) {
    printf("Erro ao abrir o ficheiro\n");
    exit(1); // Encerra o programa em caso de erro
} 
    fprintf(file, "%d;%s;%d;%d;%s\n", j.id, j.nomeJogador, j.idade, j.numeroCamisa, j.posicao);
    fclose(file);
}

// carregar o ficheiro

void carregarFicheiro(){
    jogador j;
    FILE * file;
    file = fopen ("base_de_Dados.txt", "r");
    if (file == NULL) {
    printf("Aviso: Ficheiro não encontrado. Criando novo...\n");
        return;
    }
    char linha[1000]; //vetor que vai armazenar temporariamente os valores da linha lida
    while (fgets(linha, sizeof(linha), file) != NULL) {


    // reparte os valores da linha nas suas respectivas variaveis
    sscanf(linha, "%d;%[^;];%d;%d;%[^;\n]", &j.id, j.nomeJogador, &j.idade, &j.numeroCamisa, j.posicao);
    //armazena tudo no vetor em causa
    v_jogador[totalJogadores++] = j;
  
    }
fclose(file);

}
/*----------------------------------------------*/

/*      TODAS AS FUNCOES        */
bool cadastrar_jogador(jogador j);
void listarJogadores();
/*----------------------------------------------*/

/*            Buscas           */
int buscarEquipe(int id) {
    for (int i = 0; i < totalEquipes; i++) {
        equipe e = v_equipes[i];
        if (e.id == id) {
            printf("O id do time: %d\n", e.id);
            return e.id;
        }
    }
    return -1; // Retorna -1 apenas se não encontrar nenhuma equipe
}

/*          SubMenus           */
void menuEquipes(){
    int opcao;
    do {
        printf("\n 1- Cadastrar Equipes \n");
        printf(" 2- Listar Equipes \n");
        printf(" 3- voltar\n");
        printf(" Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
        system("clear");
           printf("cadastrando equipes");
            break;

            case 2:
            system("clear");
           printf("listando as equipes");
            break;

            case 3:
            system("clear");
           printf("voltando para o menu anterior");
            break;

        }
    }while(opcao != 3);
}

void menuJogadores(jogador j){
    system("clear");
    int opcao;
    do {
        printf("\n 1- Cadastrar jogadores \n");
        printf(" 2- Listar jogadores \n");
        printf(" 3- voltar\n");
        printf(" Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
        system("clear");
           printf("===============cadastrando jogadores===============\n\n");
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
                do {
                    printf("5. Equipe a que pertence: ");
                    scanf("%d", &j.equipeActual);
                    getchar(); // Limpa o buffer de entrada

                    int idTime = buscarEquipe(j.equipeActual);
                    if (idTime != -1) {
                        printf("Time encontrado e com vagas: %d\n", idTime);
                        break; // Sai do loop se o ID for válido
                    } else {
                        printf("Equipe não encontrada. Por favor, insira um ID válido.\n");
                    }
                } while (true); // Continua até que um ID válido seja encontrado

                
                if (cadastrar_jogador(j)) {
                    sleep(3);
                    system("clear");
                    printf("Cadastro feito com sucesso\n");
                        
                } else {
                    printf("Erro no cadastro\n");
                }
                
                
            break;

            case 2:
            system("clear");
            listarJogadores();
           printf("-----------------------------\n");
            break;

            case 3:
            system("clear");
            main();
            break;

        }
    }while(opcao != 3);
}

void menuCampeonatos(){
    int opcao;
    do {
        printf("\n 1- Cadastrar campeonatos \n");
        printf(" 2- listar campeonato \n");
        printf(" 3- finalizar campeonato\n");
        printf(" 4- voltar\n");
        printf(" Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
        system("clear");
           printf("cadastrando campeonato");
            break;

            case 2:
            system("clear");
           printf("listando os campeonatos");
            break;

            case 3:
            system("clear");
            printf("ter a funcao pesquisar campeonato pelo nome");
           printf("finalizar dizendo o 1,2,3 classificado");
            break;
            case 4:
            system("clear");
              
            break;
        }
    }while(opcao != 3);
}

/*=============================================*/
int main() {
    carregarFicheiro();
    jogador j;
    printf("\n SEJA BEM-VINDO AO GIRABOLA \n ");
    int opcao;
    do {
        printf("\n 1- Jogadores \n");
        printf(" 2- Equipes \n");
        // printf(" 3-  \n");
        // printf(" 4-  \n");
        // printf(" 5-  \n");
        // printf(" 6-  \n");
        // printf(" 7-  \n");
        // printf(" 8-   \n");
        printf(" 9- Terminar  \n");
        printf(" Opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o '\n' deixado pelo scanf

        switch (opcao) {
            case 1:
            system("clear");
               menuJogadores(j);
                break;

            case 2:

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
    escreverFicheiro(j);
   
    return true;
}

 //LISTAR CLIENTES
void listarJogadores (){
		jogador j;
	for (int i = 0; i < totalJogadores; i++) {
        jogador j = v_jogador[i];
        printf("________DADOS PESSOAIS_________________\n");
        printf("Codigo: %d \n", j.id);
        printf("Nome: %s \n", j.nomeJogador);
        printf("Idade: %d \n", j.idade);
        printf("Numero Camisa: %d \n", j.numeroCamisa);
        printf("Posicao: %s \n\n", j.posicao);
    }
}