#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define TAM 1000
#define MAX_EQUIPES 20
#define MAX_EQUIPES_PARTIDA 2  
#define MAX_JOGADORES 11   
#define MAX_GOLS 100    


/*      Criando os Registos     */
struct JOGADOR {
    int id;
    int idEquipe;
    char nomeJogador[30];
    int idade;
    int numeroCamisa;
    char posicao[20];
    bool status ;
    int gols;
    
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

struct PARTIDA{
      int id;
    int idCampeonato;
    int equipesSelecionadas[2];
    char data[20];
    bool status;
    int totalGolsA;
    int totalGolsB;
    int jogadoresGolA[11];
    int jogadoresGolB[11];
    char placar[20];
};
typedef struct PARTIDA partida;

struct CAMPEONATO {
    int id;
    char nome[50];
    int ano;
    int equipesParticipantes[MAX_EQUIPES];
    int totalEquipes;
    int campeao;
    int viceCampeao;
    int terceiroLugar;
    bool status;  
};

typedef struct CAMPEONATO campeonato;

/*=========================================================*/

/*      Variáveis Globais       */
int totalJogadores = 0;
int totalCampeonatos = 0;
int totalEquipes = 0;
int totalPartidas = 0;
equipe v_equipes[TAM];
jogador v_jogador[TAM];
partida v_partidas[TAM];
campeonato v_campeonatos[TAM];
FILE *fileEquipes;
FILE *fileJogadores;
FILE *fileCampeonatos;
FILE *filePartidas;
/*--------------------------------*/

/*      TRABALHANDO COM FICHEIRO      */

// Criar Ficheiro
void criarFicheiro() {
    fileEquipes = fopen("BD_equipes.txt", "a");
    fileJogadores = fopen("BD_jogadores.txt", "a");
    fileCampeonatos = fopen("BD_campeonatos.txt", "a");
    filePartidas = fopen("BD_partidas.txt", "a");  // Criação do arquivo de partidas

    if (fileEquipes != NULL) {
        printf("Database Equipes is open \n");
        if (fileJogadores != NULL) {
            printf("Database jogadores is open \n");
            if (fileCampeonatos != NULL) {
                printf("Database campeonatos is open \n");
                if (filePartidas != NULL) {
                    printf("Database partidas is open \n");  // Confirmação de abertura do arquivo de partidas
                } else {
                    printf("Error opening Database partidas\n");
                    fclose(filePartidas);
                    exit(1);
                }
            } else {
                printf("Error opening Database campeonatos\n");
                fclose(fileCampeonatos);
                exit(1);
            }
        } else {
            printf("Error opening Database jogadores\n");
            fclose(fileJogadores);
            exit(1);
        } 
    } else {
        printf("Error opening Database equipes\n");
        fclose(fileEquipes);
        exit(1);
    }
}

// Escrever no ficheiro
void escreverFicheiro() {
    fileEquipes = fopen("BD_equipes.txt", "w");
    fileJogadores = fopen("BD_jogadores.txt", "w");
    fileCampeonatos = fopen("BD_campeonatos.txt", "w");
    filePartidas = fopen("BD_partidas.txt", "w");  // Abertura do arquivo de partidas

    if (fileEquipes == NULL || fileJogadores == NULL || fileCampeonatos == NULL || filePartidas == NULL) {
        printf("Erro ao abrir um dos arquivos.\n");
        return;
    }

    // Escrever Jogadores
    for (int i = 0; i < totalJogadores; i++) {
        jogador j = v_jogador[i];
        fprintf(fileJogadores, "%d;%d;%s;%d;%d;%s;%d;%d\n",
                j.id, j.idEquipe, j.nomeJogador, j.idade, j.numeroCamisa, j.posicao, j.status ? 1 : 0, j.gols);
    }
    fclose(fileJogadores);

    // Escrever Equipes
    for (int i = 0; i < totalEquipes; i++) {
        equipe e = v_equipes[i];
        fprintf(fileEquipes, "%d;%s;%s;%d;%s;%d\n",
                e.id, e.nomeEquipe, e.cidade, e.dataFundacao, e.nomeTreinador, e.jogadores);
    }
    fclose(fileEquipes);

    // Escrever Campeonatos
    for (int i = 0; i < totalCampeonatos; i++) {
        campeonato c = v_campeonatos[i];

        fprintf(fileCampeonatos, "%d;%s;%d;%d;%d;%d;%d;%d;",
                c.id, c.nome, c.ano, c.status ? 1 : 0, c.totalEquipes,
                c.campeao > 0 ? c.campeao : 0, 
                c.viceCampeao > 0 ? c.viceCampeao : 0, 
                c.terceiroLugar > 0 ? c.terceiroLugar : 0);

        // Escreve os IDs das equipes participantes
        for (int j = 0; j < c.totalEquipes; j++) {
            fprintf(fileCampeonatos, "%d", c.equipesParticipantes[j]);
            if (j < c.totalEquipes - 1) fprintf(fileCampeonatos, ",");
        }
        fprintf(fileCampeonatos, "\n");
    }
    fclose(fileCampeonatos);
    
  // Escrever a partida no arquivo
for (int i = 0; i < totalPartidas; i++) {
    partida p = v_partidas[i];

    // Escrever os dados principais da partida
    fprintf(filePartidas, "%d;%d;%d;%d;%s;%d;%d;", p.id, p.idCampeonato, p.equipesSelecionadas[0], p.equipesSelecionadas[1], p.data, p.status ? 1 : 0);

    // Escrever os jogadores que marcaram para a equipe A
    for (int j = 0; j < p.totalGolsA; j++) {
        fprintf(filePartidas, "%d", p.jogadoresGolA[j]);
        if (j < p.totalGolsA - 1) {
            fprintf(filePartidas, ",");
        }
    }
    fprintf(filePartidas, ";");

    // Escrever os jogadores que marcaram para a equipe B
    for (int j = 0; j < p.totalGolsB; j++) {
        fprintf(filePartidas, "%d", p.jogadoresGolB[j]);
        if (j < p.totalGolsB - 1) {
            fprintf(filePartidas, ",");
        }
    }

    // Finalizar linha
    fprintf(filePartidas, "\n");
fclose(filePartidas);
}

}

// carregar o ficheiro
void carregarFicheiro() {
    jogador j;
    equipe e;
    campeonato c;
    partida p;
    fileEquipes = fopen("BD_equipes.txt", "r");
    fileJogadores = fopen("BD_jogadores.txt", "r");
    fileCampeonatos = fopen("BD_campeonatos.txt", "r");
    filePartidas = fopen("BD_partidas.txt", "r");  // Abertura do arquivo de partidas

    if (fileEquipes == NULL || fileJogadores == NULL || fileCampeonatos == NULL || filePartidas == NULL) {
        printf("Erro ao carregar um dos arquivos.\n");
        return;
    }

    char linhaJogador[1000], linhaEquipe[1000], linhaCampeonato[1000], linhaPartida[1000];

    // Carregar Jogadores
    while (fgets(linhaJogador, sizeof(linhaJogador), fileJogadores) != NULL) {
        sscanf(linhaJogador, "%d;%d;%[^;];%d;%d;%[^;];%d;%d\n", 
               &j.id, &j.idEquipe, j.nomeJogador, &j.idade, &j.numeroCamisa, j.posicao, &j.status, &j.gols);
        v_jogador[totalJogadores++] = j; 
    }
    fclose(fileJogadores);

    // Carregar Equipes
    while (fgets(linhaEquipe, sizeof(linhaEquipe), fileEquipes) != NULL) {
        sscanf(linhaEquipe, "%d;%[^;];%[^;];%d;%[^;];%d\n",
               &e.id, e.nomeEquipe, e.cidade, &e.dataFundacao, e.nomeTreinador, &e.jogadores);
        v_equipes[totalEquipes++] = e; 
    }
    fclose(fileEquipes);

    // Carregar Campeonatos
    while (fgets(linhaCampeonato, sizeof(linhaCampeonato), fileCampeonatos) != NULL) {
        char equipesStr[200];
        int statusTemp, campeaoTemp, viceTemp, terceiroTemp;

        sscanf(linhaCampeonato, "%d;%[^;];%d;%d;%d;%d;%d;%d;%[^;]\n",
               &c.id, c.nome, &c.ano, &statusTemp, &c.totalEquipes,
               &campeaoTemp, &viceTemp, &terceiroTemp, equipesStr);

        // Converter status para booleano
        c.status = (statusTemp == 1);

        // Garantir que os valores de classificação sejam válidos
        c.campeao = (campeaoTemp > 0) ? campeaoTemp : 0;
        c.viceCampeao = (viceTemp > 0) ? viceTemp : 0;
        c.terceiroLugar = (terceiroTemp > 0) ? terceiroTemp : 0;

        // Processar os IDs das equipes participantes
        char *token = strtok(equipesStr, ",");
        int i = 0;
        while (token != NULL && i < MAX_EQUIPES) {
            c.equipesParticipantes[i++] = atoi(token);
            token = strtok(NULL, ",");
        }
        c.totalEquipes = i;

        v_campeonatos[totalCampeonatos++] = c;
    }
    fclose(fileCampeonatos);

    while (fgets(linhaPartida, sizeof(linhaPartida), filePartidas) != NULL) {
        int statusTemp;
        char jogadoresA[100], jogadoresB[100];  // Strings temporárias para armazenar os IDs dos jogadores

        // Leitura dos dados da partida
        sscanf(linhaPartida, "%d;%d;%d;%d;%[^;];%d;%d;%d;%[^;];%[^\n]", 
               &p.id, &p.idCampeonato, &p.equipesSelecionadas[0], &p.equipesSelecionadas[1], 
               p.data, &statusTemp, &p.totalGolsA, &p.totalGolsB, jogadoresA, jogadoresB);

        p.status = (statusTemp == 1); 

        // Processar os jogadores que marcaram para a equipe A
        char *tokenA = strtok(jogadoresA, ",");
        int i = 0;
        while (tokenA != NULL && i < p.totalGolsA) {
            p.jogadoresGolA[i++] = atoi(tokenA);
            tokenA = strtok(NULL, ",");
        }

        // Processar os jogadores que marcaram para a equipe B
        char *tokenB = strtok(jogadoresB, ",");
        int j = 0;
        while (tokenB != NULL && j < p.totalGolsB) {
            p.jogadoresGolB[j++] = atoi(tokenB);
            tokenB = strtok(NULL, ",");
        }

        // Armazenar a partida carregada
        v_partidas[totalPartidas++] = p;
    }

    fclose(filePartidas);
}
/*----------------------------------------------*/

/*      PROTOTIPOS D TODAS AS FUNCOES        */
bool cadastrar_jogador(jogador j);
bool cadastrar_equipes(equipe e);
bool cadastrar_campeonatos(campeonato c);
bool cadastrar_partidas(partida p, campeonato c);
bool actualizarStatus(jogador j, equipe e, int indiceE, int indiceJ);
bool registrarGol(int idJogador, equipe e);
bool finalizarPartida(int idPartida, int golsEquipeA, int golsEquipeB, int* jogadoresGolA, int* jogadoresGolB);
bool adicionarJogador(int idE, int idJ);
bool adicionarEquipeCampeonato(int idCampeonato, int idEquipe);
void listarJogadores();
void listarEquipes ();
void listarCampeonatos();
void listarPartidas();
void listarEquipesDoCampeonato(int idCampeonato);
/*----------------------------------------------*/

/*            BUSCAS           */
int buscarEquipe(int id) {
    for (int i = 0; i < totalEquipes; i++) {
        equipe e = v_equipes[i];
        if (e.id == id) {
            return i;
        }
    }
    return -1; // Retorna -1 apenas se não encontrar nenhuma equipe
}
int buscarJogadores(int id) {
    for (int i = 0; i < totalJogadores; i++) {
        jogador j = v_jogador[i];
        if (j.id == id) {
            return i;
        }
    }
    return -1; // Retorna -1 apenas se não encontrar nenhum jogador
}
int buscarCampeonato(int id) {
    for (int i = 0; i < totalCampeonatos; i++) {
        campeonato c = v_campeonatos[i];
        if (c.id == id) {
            return i;
        }
    }
    return -1; // Retorna -1 apenas se não encontrar nenhum jogador
}
int buscarPartida(int id) {
    for (int i = 0; i < totalPartidas; i++) {
        partida p = v_partidas[i];
        if (p.id == id) {
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
void menuCampeonatos() {
    int opcao;
    campeonato c; // Variável para armazenar o campeonato que será cadastrado

    do {
        printf("\n 1- Cadastrar campeonatos \n");
        printf(" 2- Listar campeonatos \n");
        printf(" 3- Listar Equipes de um campeonato\n");
        printf(" 4- Adicionar Equipes ao campeonato\n");
        printf(" 5- Voltar\n");
        printf(" Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                system("clear");
                printf("\n========= Cadastrar Campeonato =========\n");
                printf("Nome do Campeonato: ");
                fgets(c.nome, sizeof(c.nome), stdin);
                c.nome[strcspn(c.nome, "\n")] = '\0'; // Remover \n do fgets

                do {
                    printf("Ano de Realização: ");
                    scanf("%d", &c.ano);
                    getchar(); // Limpar buffer do teclado
                    if (c.ano < 2025 || c.ano > 2030) {
                        printf("Ano inválido! Digite um valor entre 2025 e 2030.\n");
                    }
                } while (c.ano < 2025 || c.ano > 2030);

                if (cadastrar_campeonatos(c)) {
                    printf("Campeonato cadastrado com sucesso! (Status: INATIVO)\n");
                } else {
                    printf("Erro ao cadastrar o campeonato\n");
                }
                break;

            case 2:
                system("clear");
                printf("Listando os campeonatos...\n");
                listarCampeonatos();
                break;

            case 3:
                system("clear");
                int idC;
                printf("EQUIPES DE UM CAMPEONATO\n");
                printf("Digite o id do campeonato em questao? \n");
                    scanf("%d", &idC);
                    getchar();
                listarEquipesDoCampeonato(idC);
            break;

            case 4:
                system("clear");
                int idCampeonato, idEquipe;
            if (totalCampeonatos == 0 || totalEquipes == 0) {
                printf("Não há campeonatos ou equipes cadastradas.\n");
                break;
            }
                system("clear");
                printf("Digite o ID do campeonato ao qual deseja adicionar uma equipe: \n");
                    scanf("%d", &idCampeonato);
                    getchar(); // Limpa buffer
                    // Exibir equipes disponíveis
                printf("\nEquipes cadastradas:\n");
                for (int i = 0; i < totalEquipes; i++) {
                    printf("ID: %d | Nome: %s\n", v_equipes[i].id, v_equipes[i].nomeEquipe);
                }

                // Escolher a equipe
                printf("\nDigite o ID da equipe que deseja adicionar: \n");
                    scanf("%d", &idEquipe);
                    getchar();
                if(adicionarEquipeCampeonato( idCampeonato, idEquipe)){
                    printf("Equipe adicionada com sucesso ao campeonato!\n");
                }else {
                    printf("Nao foi possivel cadastrar a equipe ao Campeonato\n");
                }
            case 5:
                system("clear");
                break;
            break;
        }
    } while (opcao != 5);
}
void menuPartidas() {
    int opcao;
    
    do {
        printf("\n 1- Cadastrar partida \n");
        printf(" 2- Listar partidas \n");
        printf(" 3- Finalizar partida \n");
        printf(" 4- Voltar\n");
        printf(" Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                system("clear");
                int idC, indiceC;
                partida p;
                printf("\n========= Cadastrar Partida =========\n");
                printf("Digite o ID do campeonato: ");
                scanf("%d", &idC);
                getchar();  // Limpar buffer

                // Buscar o campeonato pelo ID
                indiceC = buscarCampeonato(idC);
                if (indiceC != -1) {
                    campeonato c = v_campeonatos[indiceC];
                    printf("\nEquipes do Campeonato %s:\n", c.nome);
                    
                    listarEquipesDoCampeonato(c.id);

                    printf("\nDigite o ID da equipe A: ");
                    scanf("%d", &p.equipesSelecionadas[0]);
                    getchar();  // Limpar buffer

                    printf("Digite o ID da equipe B: ");
                    scanf("%d", &p.equipesSelecionadas[1]);
                    getchar();  // Limpar buffer

                    printf("Digite a data da partida (DD/MM/AAAA): ");
                    fgets(p.data, sizeof(p.data), stdin);
                    p.data[strcspn(p.data, "\n")] = '\0'; // Remover \n do fgets

                    // Cadastrar a partida no campeonato
                    if (cadastrar_partidas(p, c)) {
                        printf("Partida cadastrada com sucesso!\n");
                    } else {
                        printf("Erro ao cadastrar a partida.\n");
                    }
                } else {
                    printf("Campeonato não encontrado!\n");
                }
            break;


            case 2:
                system("clear");
                 listarPartidas();
                break;

            case 3:
                system("clear");
                int idPartida, golsEquipeA, golsEquipeB;
                int jogadoresGolA[100], jogadoresGolB[100];

                printf("Digite o ID da partida a ser finalizada: ");
                scanf("%d", &idPartida);
                getchar();  

                // Solicitar o placar da equipe A e os jogadores que marcaram
                printf("Digite o placar da equipe A: ");
                scanf("%d", &golsEquipeA);
                getchar();
                for (int i = 0; i < golsEquipeA; i++) {
                    printf("Digite o ID do jogador que fez o gol da equipe A (%d/%d): ", i + 1, golsEquipeA);
                    scanf("%d", &jogadoresGolA[i]);
                    getchar();
                }

                // Solicitar o placar da equipe B e os jogadores que marcaram
                printf("Digite o placar da equipe B: ");
                scanf("%d", &golsEquipeB);
                getchar();
                for (int i = 0; i < golsEquipeB; i++) {
                    printf("Digite o ID do jogador que fez o gol da equipe B (%d/%d): ", i + 1, golsEquipeB);
                    scanf("%d", &jogadoresGolB[i]);
                    getchar();
                }

                // Finalizar a partida passando os parâmetros coletados
                if (finalizarPartida(idPartida, golsEquipeA, golsEquipeB, jogadoresGolA, jogadoresGolB)) {
                    printf("Partida finalizada com sucesso!\n");
                } else {
                    printf("Erro: a partida já foi finalizada ou ocorreu outro erro.\n");
                }
                
                break;

            case 4:
                system("clear");
                break;
        }
    } while (opcao != 4);
}

/*=============================================*/
int main() {
    criarFicheiro();
    carregarFicheiro();
    printf("\n SEJA BEM-VINDO AO GIRABOLA \n ");
    int opcao;
    do {
        printf("\n 1- Jogadores \n");
        printf(" 2- Equipes \n");
        printf(" 3- Campeonatos \n");
        printf(" 4- Partidas \n");
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
                menuCampeonatos();
                break;

            case 4:
                menuPartidas();
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
    j.gols=0;
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

bool cadastrar_campeonatos(campeonato c) {
    c.id = totalCampeonatos + 1;
    c.totalEquipes = 0;
    c.campeao = 0;
    c.viceCampeao = 0;
    c.terceiroLugar = 0;
    c.status = false;  // GARANTE QUE O STATUS COMEÇA COMO INATIVO (0)

    v_campeonatos[totalCampeonatos] = c;
    totalCampeonatos++;

    return true;
}

bool cadastrar_partidas(partida p, campeonato c) {
    p.id = totalPartidas + 1; // Atribuir ID único para a partida
    p.idCampeonato = c.id; // Associar a partida ao campeonato
    p.status = false; // Partida ainda não finalizada
    v_partidas[totalPartidas] = p; // Armazenar a partida no vetor
    totalPartidas++; // Incrementar o contador de partidas
    return true;
}

bool registrarGol(int idJogador, equipe e) {
    int indiceJ = buscarJogadores(idJogador);
    if (indiceJ != -1) {
        if (v_jogador[indiceJ].idEquipe == e.id) {
            v_jogador[indiceJ].gols++;  
            printf("Gol de %s registrado com sucesso!\n", v_jogador[indiceJ].nomeJogador);
        } else {
            printf("Jogador não pertence à equipe %s.\n", e.nomeEquipe);
            return false;
        }
    } else {
        printf("Jogador não encontrado.\n");
        return false;
    }
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

void listarCampeonatos() {
    if (totalCampeonatos == 0) {
        printf("Não há campeonatos cadastrados.\n");
        return;
    }

    for (int i = 0; i < totalCampeonatos; i++) {
        campeonato c = v_campeonatos[i];
        
        printf("=====================================\n");
        printf("ID do Campeonato: %d\n", c.id);
        printf("Nome do Campeonato: %s\n", c.nome);
        printf("Ano de Realização: %d\n", c.ano);
        printf("Status: %s\n", c.status ? "Ativo" : "Inativo");
        printf("Total de Equipes: %d\n", c.totalEquipes);
        
        if (c.status) {
            printf("Campeão: %d | Vice-Campeão: %d | 3º Lugar: %d\n", 
                   c.campeao, c.viceCampeao, c.terceiroLugar);
        }
        printf("=====================================\n");
    }
}

void listarPartidas() {
    printf("\n===== Listando Todas as Partidas =====\n");
    int encontrado = 0;

    for (int i = 0; i < totalPartidas; i++) {
        // Buscando o índice da equipe A e equipe B
        partida p = v_partidas[i];
        int indiceEquipeA = buscarEquipe(p.equipesSelecionadas[0]);
        int indiceEquipeB = buscarEquipe(p.equipesSelecionadas[1]);

        // Verificando se os índices são válidos
        if (indiceEquipeA != -1 && indiceEquipeB != -1) {
            equipe eA = v_equipes[indiceEquipeA];
            equipe eB = v_equipes[indiceEquipeB];
            printf("ID Partida: %d | Campeonato ID: %d | Equipe A: %s | Equipe B: %s | Data: %s | Status: %s\n",
                p.id, 
                p.idCampeonato,
                eA.nomeEquipe, 
                eB.nomeEquipe,
                p.data, 
                p.status ? "Finalizada" : "Não Finalizada");
            encontrado = 1;
        } else {
            printf("Erro ao buscar o nome das equipes para a partida %d\n", p.id);
        }
    }
    
    if (!encontrado) {
        printf("Nenhuma partida cadastrada.\n");
    }
}

void listarEquipesDoCampeonato(int idCampeonato) {
      int indiceC = -1;

    // Busca o campeonato pelo ID usando a função buscarCampeonato
    indiceC = buscarCampeonato(idCampeonato);

    if (indiceC != -1) {
        campeonato c = v_campeonatos[indiceC];
        printf("Equipes no Campeonato %s (%d):\n", c.nome, idCampeonato);
    
        // Exibe todas as equipes que fazem parte desse campeonato
        for (int i = 0; i < c.totalEquipes; i++) {
            int indiceE = c.equipesParticipantes[i] -1;
            equipe e = v_equipes[indiceE];
            printf("ID: %d | Nome: %s\n", e.id, e.nomeEquipe);
        }
    } else {
        printf("Campeonato com ID %d não encontrado.\n", idCampeonato);
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

bool adicionarEquipeCampeonato(int idCampeonato, int idEquipe) {
    int indiceC = buscarCampeonato(idCampeonato);
    int indiceE = buscarEquipe(idEquipe);

    if (indiceC == -1) {
        printf("Campeonato não encontrado!\n");
        return false;
    }

    if (indiceE == -1) {
        printf("Equipe não encontrada!\n");
        return false;
    }

    campeonato *c = &v_campeonatos[indiceC];

    if (c->totalEquipes >= MAX_EQUIPES) {
        printf("Este campeonato já atingiu o limite máximo de 20 equipes!\n");
        return false;
    }

    // Verificar se a equipe já está no campeonato
    for (int i = 0; i < c->totalEquipes; i++) {
        if (c->equipesParticipantes[i] == idEquipe) {
            printf("Esta equipe já está cadastrada neste campeonato!\n");
            return false;
        }
    }

    // Adicionar equipe
    c->equipesParticipantes[c->totalEquipes] = idEquipe;
    c->totalEquipes++;

    // Atualizar status do campeonato
    if (c->totalEquipes >= 4) {
        c->status = true;
        printf("O campeonato agora está ATIVO!\n");
    } else {
        printf("O campeonato ainda está INATIVO (mínimo 4 equipes).\n");
    }

    return true;
}

bool finalizarPartida(int idPartida, int golsEquipeA, int golsEquipeB, int* jogadoresGolA, int* jogadoresGolB) {
    int indiceP = buscarPartida(idPartida);
    if (indiceP == -1) {
        printf("Partida não encontrada!\n");
        return false;
    }

    partida *p = &v_partidas[indiceP]; // Obtém a referência para a partida

    if (p->status == true) {
        printf("Esta partida já foi finalizada!\n");
        return false;
    }

    // Atualizando o placar da partida
    p->totalGolsA = golsEquipeA;
    p->totalGolsB = golsEquipeB;
    p->status = true; // Marcar a partida como finalizada

    // Registrar os gols para a equipe A
    for (int i = 0; i < golsEquipeA; i++) {
        int indiceA = buscarEquipe(p->equipesSelecionadas[0]);
        equipe e = v_equipes[indiceA];
        if (!registrarGol(jogadoresGolA[i], e)) {
            return false;
        }
        p->jogadoresGolA[i] = jogadoresGolA[i]; // Salvar ID do jogador que marcou
    }

    // Registrar os gols para a equipe B
    for (int i = 0; i < golsEquipeB; i++) {
        int indiceB = buscarEquipe(p->equipesSelecionadas[1]);
        equipe e = v_equipes[indiceB];
        if (!registrarGol(jogadoresGolB[i], e)) {
            return false;
        }
        p->jogadoresGolB[i] = jogadoresGolB[i]; // Salvar ID do jogador que marcou
    }

    return true;
}