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
    int idEquipe;
    char nomeJogador[30];
    int idade;
    int numeroCamisa;
    char posicao[20];
    bool status ;
    
};
typedef struct JOGADOR jogador;
struct EQUIPE {
    int id;
    char nomeEquipe[30];
    char cidade[30];
    int dataFundacao;
    char nomeTreinador[30];
    int jogadores;
    
};

typedef struct EQUIPE equipe;
struct PARTIDAS {
    int id;
    int idEquipes[1000];
    int idCampeonato[1000];
    char equipeA[50];
    char equipeB[50];
    int dataPartida;
    int num_gools;
    char placar_final[20];
    
    
};
typedef struct PARTIDAS partidas;

/*      Variáveis Globais       */
int totalJogadores = 0;
equipe v_equipes[TAM];
jogador v_jogador[TAM];
int totalEquipes = 0;
FILE *fileEquipes;
FILE *fileJogadores;
/*--------------------------------*/

/*      TRABALHANDO COM FICHEIRO      */

// Criar Ficheiro
void criarFicheiro() {
    fileEquipes = fopen("BD_equipes.txt", "a");
    fileJogadores = fopen("BD_jogadores.txt", "a");
    if (fileEquipes != NULL) {
        printf("Database Equipes is open \n");
        if (fileJogadores != NULL)
        {
           printf("Database Equipes is open \n");
        }else {
            printf("Error Database Equipes");
            fclose(fileJogadores);
            exit(1);
        } 
    } else {
        printf("Error Database Equipes");
        fclose(fileEquipes);
        exit(1);
    }
}

// Escrever no ficheiro
void escreverFicheiro(){
    fileEquipes = fopen("BD_equipes.txt", "w");
    fileJogadores = fopen("BD_jogadores.txt", "w");
    if (fileEquipes != NULL) {
        printf("Database Equipes is open \n");
        if (fileJogadores != NULL)
        {
           printf("Database Equipes is open \n");
        }else {
            printf("Error Database Equipes");
            fclose(fileJogadores);
            exit(1);
        } 
    } else {
        printf("Error Database Equipes");
        fclose(fileEquipes);
        exit(1);
    }
    for (int i = 0; i < totalJogadores; i++)
    {
        jogador j = v_jogador[i];
        fprintf(fileJogadores, "%d;%d;%s;%d;%d;%s;%d\n",j.id, j.idEquipe, j.nomeJogador, j.idade, j.numeroCamisa, j.posicao, j.status);
    }
     fclose(fileJogadores);
        printf("Database dos jogadores foi escrita com sucesso\n");
    for (int i = 0; i < totalEquipes; i++)
    {
        equipe e = v_equipes[i];
        fprintf(fileEquipes, "%d;%s;%s;%d;%s;%d\n",e.id, e.nomeEquipe, e.cidade, e.dataFundacao, e.nomeTreinador, e.jogadores);   
    }
    fclose(fileEquipes);
    printf("Database das equipes foi escrita com sucesso\n");
}

// carregar o ficheiro
void carregarFicheiro(){
    jogador j;
    equipe e;
    fileEquipes = fopen("BD_equipes.txt", "r");
    fileJogadores = fopen("BD_jogadores.txt", "r");
    if (fileEquipes != NULL) {
        printf("Database Equipes is open \n");
        if (fileJogadores != NULL)
        {
           printf("Database Equipes is open \n");
        }else {
            printf("Aviso: Database equipes não encontrado. Criando novo...\n");
            return;
        } 
    } else {
        printf("Aviso: Database equipes não encontrado. Criando novo...\n");
            return;
    }
        char linhaJogador[1000]; // vetor que armazena temporariamente os valores do Database jogador em linha
        char linhaEquipe[1000]; // vetor que armazena temporariamente os valores do Database equipe em linha
    while (fgets(linhaJogador, sizeof(linhaJogador), fileJogadores) != NULL) {
    // reparte os valores da linhaJogador nas suas respectivas variaveis
    sscanf(linhaJogador, "%d;%d;%[^;];%d;%d;%[^;];%d\n", &j.id,&j.idEquipe, j.nomeJogador, &j.idade, &j.numeroCamisa, j.posicao, &j.status);
    //armazena tudo no vetor em causa
    v_jogador[totalJogadores++] = j; 
    }
        fclose(fileJogadores);
    while (fgets(linhaEquipe, sizeof(linhaEquipe), fileEquipes) != NULL) {
         // reparte os valores da linhaEquipe nas suas respectivas variaveis
    sscanf(linhaEquipe, "%d;%[^;];%[^;];%d;%[^;];%d\n",&e.id, e.nomeEquipe, &e.cidade, &e.dataFundacao, e.nomeTreinador, &e.jogadores);
    //armazena tudo no vetor em causa
    v_equipes[totalEquipes++] = e; 
    }
        fclose(fileEquipes);

}
/*----------------------------------------------*/

/*      TODAS AS FUNCOES        */
bool cadastrar_jogador(jogador j);
bool cadastrar_equipes(equipe e);
bool actualizarStatus(jogador j, equipe e, int indiceE, int indiceJ);
bool adicionarJogador(int idE, int idJ);
void listarJogadores();
void listarEquipes ();
/*----------------------------------------------*/

/*            BUSCAS           */
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

/*          SubMenus           */
void menuEquipes(){
    system("clear");
    equipe e;
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
                } else printf("Erro no cadastro\n");
            break;

            case 2:
            system("clear");
            listarEquipes();
            break;
            case 3:
                system("clear");
                int idE=0,idJ=0;
                printf("-----------PESQUISANDO EQUIPE---------\n\n");
                    printf("Digite o id da equipe em causa: \n");
                        scanf("%d", &idE); 
                        getchar();   
                printf("\n-----------PESQUISANDO JOGADOR---------\n");
                    printf("Digite o id do jogador em causa: \n");
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
void menuJogadores(){
    system("clear");
    jogador j;
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
            } else printf("Erro no cadastro\n");
            
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


/*void menuCampeonatos(){
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
}*/

/*=============================================*/
int main() {
    criarFicheiro();
    carregarFicheiro();
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
               menuJogadores();
                break;

            case 2:
                menuEquipes();
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

 escreverFicheiro();
    return 0;
}

/*          CADASTROS           */
bool cadastrar_jogador(jogador j) {
    j.id = totalJogadores + 1;
    j.idEquipe= 0;
    j.status = false;
    v_jogador[totalJogadores] = j;
    totalJogadores++;   
    return true;
}

bool cadastrar_equipes(equipe e){
  e.id = totalEquipes + 1;
  e.jogadores = 0;
    v_equipes[totalEquipes] = e;
    totalEquipes++; 
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
bool adicionarJogador(int idE, int idJ){
    int indiceE, indiceJ, op;
     indiceE = buscarEquipe(idE);
    equipe e = v_equipes[indiceE];
    if (e.id == idE && e.jogadores < 30){
        printf("Nome da Equipe: %s; \n N de vagas: %d vagas\n", e.nomeEquipe, (30 - e.jogadores));
        // sleep(2);
        // system("clear");
        indiceJ = buscarJogadores(idJ);
        jogador j = v_jogador[indiceJ];
        if(j.id == idJ && j.status != true){
             printf("Nome do Jogador: %s; \n Status: %d vagas\n\n", j.nomeJogador, j.status);
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

bool actualizarStatus(jogador j, equipe e, int indiceE, int indiceJ){
   e.jogadores = e.jogadores + 1;
   j.idEquipe = e.id;
   j.status=true;
   v_equipes[indiceE] = e;
    v_jogador[indiceJ] = j;
     return true;                
}


