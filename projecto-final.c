#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define TAM 1000

/*      Criando os Registos     */
struct JOGADOR {
    char type;
    int id;
    int idEquipe;
    char nomeJogador[30];
    int idade;
    int numeroCamisa;
    char posicao[20];
    bool status ;
    
};
typedef struct JOGADOR jogador;
struct EQUIPE {
    char type;
    int id;
    char nomeEquipe[30];
    char cidade[30];
    int dataFundacao;
    char nomeTreinador[30];
    int jogadores;
    
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
void escreverFicheiroJogador(jogador j){
        FILE *file;
    file = fopen("base_de_Dados.txt", "a");

    if (file == NULL) {
    printf("Erro ao abrir o ficheiro\n");
    exit(1); // Encerra o programa em caso de erro
} 
    fprintf(file, "%c;%d;%d;%s;%d;%d;%s;%d\n",j.type, j.id, j.idEquipe, j.nomeJogador, j.idade, j.numeroCamisa, j.posicao, j.status);
    fclose(file);
}
void escreverFicheiroEquipes(equipe e){
        FILE *file;
    file = fopen("base_de_Dados.txt", "a");

    if (file == NULL) {
    printf("Erro ao abrir o ficheiro\n");
    exit(1); // Encerra o programa em caso de erro
} 
    fprintf(file, "%c;%d;%s;%s;%d;%s\n",e.type, e.id, e.nomeEquipe, e.cidade, e.dataFundacao, e.nomeTreinador);
    fclose(file);
}
// carregar o ficheiro

void carregarFicheiro(){
    jogador j;
    equipe e;
    FILE * file;
    file = fopen ("base_de_Dados.txt", "r");
    if (file == NULL) {
    printf("Aviso: Ficheiro não encontrado. Criando novo...\n");
        return;
    }
    char linha[1000]; //vetor que vai armazenar temporariamente os valores da linha lida
    while (fgets(linha, sizeof(linha), file) != NULL) {
    
    
        printf("%c", linha[0]);
     if (linha[0] == 'J'){
    // reparte os valores da linha nas suas respectivas variaveis
    sscanf(linha, "%c;%d;%d;%[^;];%d;%d;%[^;];%d\n", &j.type, &j.id,&j.idEquipe, j.nomeJogador, &j.idade, &j.numeroCamisa, j.posicao, &j.status);
    //armazena tudo no vetor em causa
    v_jogador[totalJogadores++] = j;
      }else if(linha[0] == 'E'){
         // reparte os valores da linha nas suas respectivas variaveis
    sscanf(linha, "%c;%d;%[^;];%[^;];%d;%[^;\n]", &e.type, &e.id, e.nomeEquipe, &e.cidade, &e.dataFundacao, e.nomeTreinador);
    //armazena tudo no vetor em causa
    v_equipes[totalEquipes++] = e;
      }  
    }
fclose(file);

}
/*----------------------------------------------*/

/*      TODAS AS FUNCOES        */
bool cadastrar_jogador(jogador j);
bool cadastrar_equipes(equipe e);
bool actualizarStatus(jogador j, equipe e, int indiceE, int indiceJ);
bool actualizarEquipe(equipe e, int indice);
void listarJogadores();
void listarEquipes ();
/*----------------------------------------------*/

/*            Buscas           */
int buscarEquipe(int id) {
    for (int i = 0; i < totalEquipes; i++) {
        equipe e = v_equipes[i];
        if (e.id == id) {
            printf("O id do time: %d\n", e.id);
            return i;
        }
    }
    return -1; // Retorna -1 apenas se não encontrar nenhuma equipe
}
int buscarJogadores(int id) {
    for (int i = 0; i < totalJogadores; i++) {
        jogador j = v_jogador[i];
        if (j.id == id) {
            printf("O id do jogador: %d\n", j.id);
            return i;
        }
    }
    return -1; // Retorna -1 apenas se não encontrar nenhum jogador
}

/*---------------------------------------------*/

bool adicionarJogador(int idE, int idJ){
    int indiceE, indiceJ, op;
     indiceE = buscarEquipe(idE);
    equipe e = v_equipes[indiceE];
    if (e.id == idE && e.jogadores < 30){
        printf("equipe encontrada e ainda tem vagas\n");
        sleep(2);
        system("clear");
        indiceJ = buscarJogadores(idJ);
        jogador j = v_jogador[indiceJ];
        if(j.id == idJ && j.status != true){
            printf("jogador encontrado e nao esta em nenhum time\n\n");
            printf("deseja cadastrar este jogador no time? \n 1-Sim\n2-Nao\n") ;
                scanf("%d", &op);
            switch (op){
                case 1:
                   if(actualizarStatus(j, e, indiceE, indiceJ)){
                    printf("good");
                   }
                    return true;
                break;
                default:
                
                break;
            }
        }
    }
                    return false;
}


/*          SubMenus           */
void menuEquipes(equipe e){
    int opcao;
    do {
        printf("\n 1- Cadastrar Equipes \n");
        printf(" 2- Listar Equipes \n");
        printf(" 3- Adicionar jogadores\n");
        printf(" 4- voltar\n");
        printf(" Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
        system("clear");
           printf("Nome da Equipa: ");
            fgets(e.nomeEquipe, 30, stdin);
            e.nomeEquipe[strcspn(e.nomeEquipe, "\n")] = '\0'; 
            
            printf("Cidade da equipa: ");
            fgets(e.cidade, 30, stdin);
            e.cidade[strcspn(e.cidade, "\n")] = '\0'; 

            printf("Ano de Fundacao ");
            scanf("%d", &e.dataFundacao);
            getchar();
            
            printf("Nome do treinador: ");
            fgets(e.nomeTreinador, 30, stdin);
            e.nomeTreinador[strcspn(e.nomeTreinador, "\n")] = '\0'; 

            if (cadastrar_equipes(e)) {
                    sleep(3);
                    system("clear");
                    printf("Cadastro feito com sucesso\n");
                        
                } else {
                    printf("Erro no cadastro\n");
                }
            break;

            case 2:
            system("clear");
            listarEquipes();
            break;
            case 3:
                system("clear");
                int idE=0,idJ=0;
                printf("-----------PESQUISANDO EQUIPE---------\n\n");
                    printf("Digite o id da equipe em causa\n");
                        scanf("%d", &idE); 
                        getchar();   
                 
                    printf("-----------PESQUISANDO JOGADOR---------\n");
                     printf("Digite o id do jogador em causa\n");
                        scanf("%d", &idJ);  
                        getchar();
                        
                  if(adicionarJogador(idE, idJ)){
                    printf("cadastrado comsucesso");
                  }else printf("deu pau");
                
            break;
            case 4:
            system("clear");
           printf("voltando para o menu anterior");
            break;

        }
    }while(opcao != 4);
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
    equipe e;
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
                menuEquipes(e);
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

/*          CADASTROS           */
bool cadastrar_jogador(jogador j) {
    j.id = totalJogadores + 1;
    j.type = 'J';
    j.status = false;
    v_jogador[totalJogadores] = j;
    totalJogadores++; 
    escreverFicheiroJogador(j);
   
    return true;
}

bool cadastrar_equipes(equipe e){
  e.id = totalEquipes + 1;
  e.type = 'E';
    v_equipes[totalEquipes] = e;
    totalEquipes++; 
   escreverFicheiroEquipes(e);
    return true;
}

/*--------------------------------------------*/
/*           LISTAS              */
 //JOGADORES
void listarJogadores (){

	for (int i = 0; i < totalJogadores; i++) {
        jogador j = v_jogador[i];
        printf("________DADOS PESSOAIS_________________\n");
        printf("Codigo: %d \n", j.id);
        printf("Nome: %s \n", j.nomeJogador);
        printf("Idade: %d \n", j.idade);
        printf("Numero Camisa: %d \n", j.numeroCamisa);
        printf("Posicao: %s \n", j.posicao);
        if(j.idEquipe != 0) printf("equipe actual: %d \n", j.idEquipe);
        printf("Status : %d \n\n", j.status);
    }
}

//EQUIPES
void listarEquipes (){

	for (int i = 0; i < totalEquipes; i++) {
        equipe e = v_equipes[i];
        printf("________DADOS PESSOAIS DE EQUIPES_________________\n");
        printf("Codigo: %d \n", e.id);
        printf("Nome da equipe: %s \n", e.nomeEquipe);
        printf("cidade: %s \n", e.cidade);
        printf("ano da fundacao: %d \n", e.dataFundacao);
        printf("nome do tecnico: %s \n", e.nomeTreinador);
        if(e.jogadores != 0) printf("total de jogadores actualmente: %d \n", e.jogadores);

    }
}

/*-----------------------------------------------*/

/*          ACTUALIZACOES           */
bool actualizarStatus(jogador j, equipe e, int indiceE, int indiceJ){
   e.jogadores = e.jogadores + 1;
   j.idEquipe = e.id;
   j.status=true;
   v_equipes[indiceE] = e;
    v_jogador[indiceJ] = j;
     escreverFicheiroJogador(j);
     escreverFicheiroEquipes(e);
     return true;                
}
