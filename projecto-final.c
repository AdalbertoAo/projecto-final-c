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
    filePartidas = fopen("BD_partidas.txt", "a"); 

    if (fileEquipes != NULL) {
        printf("Database Equipes is open \n");
        if (fileJogadores != NULL) {
            printf("Database jogadores is open \n");
            if (fileCampeonatos != NULL) {
                printf("Database campeonatos is open \n");
                if (filePartidas != NULL) {
                    printf("Database partidas is open \n"); 
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
    filePartidas = fopen("BD_partidas.txt", "w");

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
    
  // Escrever partida
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

    fprintf(filePartidas, "\n");

}
fclose(filePartidas);

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
    filePartidas = fopen("BD_partidas.txt", "r");

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
        char jogadoresA[100], jogadoresB[100];

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
        v_partidas[totalPartidas++] = p;
    }

    fclose(filePartidas);
}
/*----------------------------------------------*/

/*      PROTOTIPOS DE TODAS AS FUNCOES        */
bool cadastrar_jogador(jogador j);
bool cadastrar_equipes(equipe e);
bool cadastrar_campeonatos(campeonato c);
bool cadastrar_partidas(partida p, campeonato c);
bool actualizarStatus(jogador j, equipe e, int indiceE, int indiceJ);
bool registrarGol(int idJogador, equipe e);
bool finalizarPartida(int idPartida, int golsEquipeA, int golsEquipeB, int* jogadoresGolA, int* jogadoresGolB);
bool adicionarJogador(int idE, int idJ);
bool adicionarEquipeCampeonato(int idCampeonato, int idEquipe);
bool registrarCampeoes(int indiceC, int campeao, int viceCampeao, int terceiroLugar);
void listarJogadores();
void listarEquipes ();
void listarCampeonatos();
void listarPartidas();
void listarEquipesDoCampeonato(int idCampeonato);
void listarPartidasDeUmCampeonato(int idCampeonato);
void listarJogadoresDeUmaEquipe(int idE);
void listarCampeonatosComVencedores();
void calcularMediaIdadePorEquipe();
void listarArtilheirosPorCampeonato();
void consultarGolsPorJogador();
void listarTodosOsDados();

/*----------------------------------------------*/

/*            BUSCAS           */
int buscarEquipe(int id) {
    for (int i = 0; i < totalEquipes; i++) {
        equipe e = v_equipes[i];
        if (e.id == id) {
            return i;
        }
    }
    return -1;
}
int buscarJogadores(int id) {
    for (int i = 0; i < totalJogadores; i++) {
        jogador j = v_jogador[i];
        if (j.id == id) {
            return i;
        }
    }
    return -1;
}
int buscarCampeonato(int id) {
    for (int i = 0; i < totalCampeonatos; i++) {
        campeonato c = v_campeonatos[i];
        if (c.id == id) {
            return i;
        }
    }
    return -1;
}
int buscarPartida(int id) {
    for (int i = 0; i < totalPartidas; i++) {
        partida p = v_partidas[i];
        if (p.id == id) {
            return i;
        }
    }
    return -1;
}

/*---------------------------------------------*/


/*      VALIDAR DATAS       */
bool validarData(const char *data) {
    int dia, mes, ano;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) return false;
    // Adicione validações específicas (ex: meses com 31 dias)
    return true;
}
/*          SubMenus           */
void menuEquipes(){
    system("clear");
    equipe e;
    int opcao;
    do {
        printf("\n 1- Cadastrar Equipes \n");
        printf(" 2- Listar Equipes \n");
        printf(" 3- Adicionar jogadores\n");
        printf(" 4-Listar jogaddores de uma euipe\n");
        printf(" 5- voltar\n");
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
            int idE;
            system("clear");
           printf("digite o id da equipe em questao: \n");
           scanf("%d", &idE);
           system("clear");
           listarJogadoresDeUmaEquipe(idE);
            break;
            case 5:
            system("clear");
            break;
        }
    }while(opcao != 5);
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
                getchar();
            printf("4. Posição: ");
                fgets(j.posicao, sizeof(j.posicao), stdin);
                j.posicao[strcspn(j.posicao, "\n")] = '\0';
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
    campeonato c;

    do {
        printf("\n 1- Cadastrar campeonatos \n");
        printf(" 2- Listar campeonatos \n");
        printf(" 3- Listar Equipes de um campeonato\n");
        printf(" 4- Adicionar Equipes ao campeonato\n");
        printf(" 5- Finalizar Campeonato\n");
        printf(" 6- Listar Partidas de um campeonato\n");
        printf(" 7- Voltar\n");
        printf(" Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                system("clear");
                printf("\n========= Cadastrar Campeonato =========\n");
                printf("Nome do Campeonato: ");
                fgets(c.nome, sizeof(c.nome), stdin);
                c.nome[strcspn(c.nome, "\n")] = '\0';

                do {
                    printf("Ano de Realização: ");
                    scanf("%d", &c.ano);
                    getchar();
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
                    getchar();
                printf("\nEquipes cadastradas:\n");
                for (int i = 0; i < totalEquipes; i++) {
                    printf("ID: %d | Nome: %s\n", v_equipes[i].id, v_equipes[i].nomeEquipe);
                }

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
                int indC, indiceC, campeao, viceCampeao, terceiroLugar;

                listarCampeonatos();
                printf("Digite o ID do campeonato que deseja finalizar: ");
                scanf("%d", &indC);

                indiceC = buscarCampeonato(indC);
                if (indiceC == -1) {
                    printf("Campeonato não encontrado!\n");
                    break;
                }

                printf("Digite o ID da equipe campeã: ");
                scanf("%d", &campeao);
                printf("Digite o ID da equipe vice-campeã: ");
                scanf("%d", &viceCampeao);
                printf("Digite o ID da equipe em terceiro lugar: ");
                scanf("%d", &terceiroLugar);

                if (!registrarCampeoes(indC, campeao, viceCampeao, terceiroLugar)) {
                    printf("Erro ao finalizar o campeonato\n");
                }

            break;
            case 6:
            system("clear");
            int idC;
            printf("Digite o Id do campeonato em questao:\n");
            scanf("%d", &idC);
            system("clear");
            listarPartidasDeUmCampeonato(idC);
            break;
        }
    } while (opcao != 7);
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
                getchar(); 
                indiceC = buscarCampeonato(idC);
                if (indiceC != -1) {
                    campeonato c = v_campeonatos[indiceC];
                    printf("\nEquipes do Campeonato %s:\n", c.nome);
                    
                    listarEquipesDoCampeonato(c.id);

                    printf("\nDigite o ID da equipe A: ");
                    scanf("%d", &p.equipesSelecionadas[0]);
                    getchar(); 

                    printf("Digite o ID da equipe B: ");
                    scanf("%d", &p.equipesSelecionadas[1]);
                    getchar(); 

                    do {
                        printf("Data (DD/MM/AAAA): ");
                        fgets(p.data, sizeof(p.data), stdin);
                        p.data[strcspn(p.data, "\n")] = '\0';
                    } while (!validarData(p.data));

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

                printf("Digite o placar da equipe A: ");
                scanf("%d", &golsEquipeA);
                getchar();
                for (int i = 0; i < golsEquipeA; i++) {
                    printf("Digite o ID do jogador que fez o gol da equipe A (%d/%d): ", i + 1, golsEquipeA);
                    scanf("%d", &jogadoresGolA[i]);
                    getchar();
                }

                printf("Digite o placar da equipe B: ");
                scanf("%d", &golsEquipeB);
                getchar();
                for (int i = 0; i < golsEquipeB; i++) {
                    printf("Digite o ID do jogador que fez o gol da equipe B (%d/%d): ", i + 1, golsEquipeB);
                    scanf("%d", &jogadoresGolB[i]);
                    getchar();
                }

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
void menuEstatisticas(){
     int opcaoEstatisticas;
                do {
                    printf("\n 1- Listar campeonatos e vencedores \n");
                    printf(" 2- Média de idade por equipe \n");
                    printf(" 3- Listar artilheiros por campeonato \n");
                    printf(" 4- Consultar gols por jogador \n");
                    printf(" 5- Balanço geral \n");
                    printf(" 6- Voltar \n");
                    printf(" Opcao: ");
                    scanf("%d", &opcaoEstatisticas);
                    getchar();

                    switch (opcaoEstatisticas) {
                        case 1:
                         system("clear");
                            listarCampeonatosComVencedores();
                            break;

                        case 2:
                         system("clear");
                            calcularMediaIdadePorEquipe();
                            break;

                        case 3:
                         system("clear");
                            listarArtilheirosPorCampeonato();
                            break;

                        case 4:
                         system("clear");
                            consultarGolsPorJogador();
                            break;

                        case 5:
                            system("clear");
                            listarTodosOsDados();
                            break;
                        
                        case 6:
                        system("clear");
                        break;
                    }
                } while (opcaoEstatisticas != 6);
}
/*=============================================*/
int main() {
    setlocale(LC_ALL, "Portuguese");
    criarFicheiro();
    carregarFicheiro();
    printf("\n SEJA BEM-VINDO AO GIRABOLA \n ");
    int opcao;
    do {
        printf("\n 1- Jogadores \n");
        printf(" 2- Equipes \n");
        printf(" 3- Campeonatos \n");
        printf(" 4- Partidas \n");
        printf(" 5- Estatísticas \n");
        printf(" 6- Terminar  \n");
        printf(" Opcao: ");
        scanf("%d", &opcao);
        getchar();

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
               menuEstatisticas();
                break;

        }
    } while (opcao != 6);

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
    c.status = false;

    v_campeonatos[totalCampeonatos] = c;
    totalCampeonatos++;

    return true;
}

bool cadastrar_partidas(partida p, campeonato c) {
    p.id = totalPartidas + 1;
    p.idCampeonato = c.id;
    p.status = false;
    v_partidas[totalPartidas] = p;
    totalPartidas++; 
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
void listarJogadores() {
    if (totalJogadores == 0) {
        printf("\n╔══════════════════════════════╗");
        printf("\n║  Nenhum jogador cadastrado!  ║");
        printf("\n╚══════════════════════════════╝\n");
        return;
    }

    printf("\n══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                                          LISTA DE JOGADORES                                         \n");
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf(" ID │ Nome%-25s │ Idade │ Camisa │ Posição%-15s │ Equipe%-20s │ Status  \n", "", "", "");
    printf("────┼───────────────────────────────┼───────┼────────┼─────────────────────┼───────────────────────┼─────────\n");

    for (int i = 0; i < totalJogadores; i++) {
        jogador j = v_jogador[i];
        char nomeEquipe[30] = "Nenhuma";
        if (j.idEquipe != 0) {
            int indiceEquipe = buscarEquipe(j.idEquipe);
            if (indiceEquipe != -1) {
                strcpy(nomeEquipe, v_equipes[indiceEquipe].nomeEquipe);
            }
        }

        printf(" %-2d │ %-30s│ %-5d │ %-6d │ %-20s│ %-22s│ %s\n",
               j.id, j.nomeJogador, j.idade, j.numeroCamisa, j.posicao, 
               nomeEquipe, j.status ? "Ativo " : "Inativo");
    }
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
}
//EQUIPES
void listarEquipes() {
    if (totalEquipes == 0) {
        printf("\n╔══════════════════════════════╗");
        printf("\n║   Nenhuma equipe cadastrada! ║");
        printf("\n╚══════════════════════════════╝\n");
        return;
    }

    printf("\n══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                                           LISTA DE EQUIPES                                          \n");
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf(" ID │ Nome%-25s │ Cidade%-20s │ Fundação │ Treinador%-20s │ Jogadores \n", "", "", "");
    printf("────┼───────────────────────────────┼───────────────────────┼──────────┼───────────────────────┼──────────\n");

    for (int i = 0; i < totalEquipes; i++) {
        equipe e = v_equipes[i];
        printf(" %-2d │ %-30s│ %-22s│ %-8d │ %-22s│ %-9d\n",
               e.id, e.nomeEquipe, e.cidade, e.dataFundacao, e.nomeTreinador, e.jogadores);
    }
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
}

//CAMPEONATOS
void listarCampeonatos() {
    if (totalCampeonatos == 0) {
        printf("\n╔════════════════════════════════════╗");
        printf("\n║      Nenhum campeonato cadastrado! ║");
        printf("\n╚════════════════════════════════════╝\n");
        return;
    }

    printf("\n══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                                                      LISTA DE CAMPEONATOS                                                     \n");
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf(" ID │ Nome%-25s │ Ano  │ Status%-8s │ Equipes │ Campeão%-20s │ Vice%-20s │ 3º Lugar%-20s \n", "", "", "", "", "");
    printf("────┼───────────────────────────────┼──────┼───────────┼─────────┼───────────────────────┼───────────────────────┼───────────────────────\n");

    for (int i = 0; i < totalCampeonatos; i++) {
        campeonato c = v_campeonatos[i];
        char status[10];
        strcpy(status, c.status ? "Ativo" : "Finalizado");
        
        char campeao[30] = "N/A";
        char vice[30] = "N/A";
        char terceiro[30] = "N/A";

        if (!c.status) {
            int indice = buscarEquipe(c.campeao);
            if (indice != -1) strcpy(campeao, v_equipes[indice].nomeEquipe);
            
            indice = buscarEquipe(c.viceCampeao);
            if (indice != -1) strcpy(vice, v_equipes[indice].nomeEquipe);
            
            indice = buscarEquipe(c.terceiroLugar);
            if (indice != -1) strcpy(terceiro, v_equipes[indice].nomeEquipe);
        }

        printf(" %-2d │ %-30s│ %-4d │ %-9s │ %-7d │ %-22s│ %-22s│ %-22s\n",
               c.id, c.nome, c.ano, status, c.totalEquipes, campeao, vice, terceiro);
    }
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
}

//PARTIDAS
void listarPartidas() {
    if (totalPartidas == 0) {
        printf("\n╔══════════════════════════════╗");
        printf("\n║   Nenhuma partida cadastrada!║");
        printf("\n╚══════════════════════════════╝\n");
        return;
    }

    printf("\n══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                                           LISTA DE PARTIDAS                                          \n");
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf(" ID │ Campeonato%-20s │ Equipe A%-20s │ Equipe B%-20s │ Data%-12s │ Placar%-8s │ Status%-10s \n", "", "", "", "", "");
    printf("────┼───────────────────────────────┼───────────────────────┼───────────────────────┼────────────┼──────────┼────────────\n");

    for (int i = 0; i < totalPartidas; i++) {
        partida p = v_partidas[i];
        campeonato c = v_campeonatos[buscarCampeonato(p.idCampeonato)];
        equipe eA = v_equipes[buscarEquipe(p.equipesSelecionadas[0])];
        equipe eB = v_equipes[buscarEquipe(p.equipesSelecionadas[1])];
        
        char placar[20];
        if (p.status) {
            sprintf(placar, "%d x %d", p.totalGolsA, p.totalGolsB);
        } else {
            strcpy(placar, " - ");
        }

        printf(" %-2d │ %-25s│ %-22s│ %-22s│ %-11s│ %-9s│ %s\n",
               p.id, c.nome, eA.nomeEquipe, eB.nomeEquipe, p.data, placar,
               p.status ? "Finalizada" : "Pendente");
    }
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
}

//EQUIPES DE UM CAMPEONATO
void listarEquipesDoCampeonato(int idCampeonato) {
    int indiceC = buscarCampeonato(idCampeonato);
    if (indiceC == -1) {
        printf("\n╔══════════════════════════════════════════╗");
        printf("\n║  Campeonato não encontrado! (ID: %-4d)  ║", idCampeonato);
        printf("\n╚══════════════════════════════════════════╝\n");
        return;
    }

    campeonato c = v_campeonatos[indiceC];
    printf("\n══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                     EQUIPES PARTICIPANTES DO CAMPEONATO: %-30s                     \n", c.nome);
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf(" ID │ Nome%-25s │ Cidade%-20s │ Fundação │ Treinador%-20s \n", "", "", "");
    printf("────┼───────────────────────────────┼───────────────────────┼──────────┼───────────────────────\n");

    for (int i = 0; i < c.totalEquipes; i++) {
        int indiceE = buscarEquipe(c.equipesParticipantes[i]);
        if (indiceE != -1) {
            equipe e = v_equipes[indiceE];
            printf(" %-2d │ %-30s│ %-22s│ %-8d │ %-22s\n",
                   e.id, e.nomeEquipe, e.cidade, e.dataFundacao, e.nomeTreinador);
        }
    }
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
}
/*-----------------------------------------------*/

/*          ACTUALIZACOES           */
bool adicionarJogador(int idE, int idJ){
    int indiceE, indiceJ, op;
     indiceE = buscarEquipe(idE);
    equipe e = v_equipes[indiceE];
    if (e.id == idE && e.jogadores < 30){
        printf("Nome da Equipe: %s; \n N de vagas: %d vagas\n", e.nomeEquipe, (30 - e.jogadores));
        sleep(2);
        system("clear");
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

    for (int i = 0; i < c->totalEquipes; i++) {
        if (c->equipesParticipantes[i] == idEquipe) {
            printf("Esta equipe já está cadastrada neste campeonato!\n");
            return false;
        }
    }

            equipe *e = &v_equipes[indiceE];
        if (e->jogadores < 11) {
            printf("Equipe precisa ter 11 jogadores!\n");
            return false;
        }

    c->equipesParticipantes[c->totalEquipes] = idEquipe;
    c->totalEquipes++;

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

    partida *p = &v_partidas[indiceP];

    if (p->status == true) {
        printf("Esta partida já foi finalizada!\n");
        return false;
    }
    for (int i = 0; i < golsEquipeA; i++) {
        int indiceJogador = buscarJogadores(jogadoresGolA[i]);
        if (indiceJogador == -1 || v_jogador[indiceJogador].idEquipe != p->equipesSelecionadas[0]) {
            printf("Jogador %d inválido!\n", jogadoresGolA[i]);
            return false;
        }
    }

    for (int i = 0; i < golsEquipeA; i++) {
        int indiceJogador = buscarJogadores(jogadoresGolB[i]);
        if (indiceJogador == -1 || v_jogador[indiceJogador].idEquipe != p->equipesSelecionadas[1]) {
            printf("Jogador %d inválido!\n", jogadoresGolB[i]);
            return false;
        }
    }
    p->totalGolsA = golsEquipeA;
    p->totalGolsB = golsEquipeB;
    p->status = true;

    for (int i = 0; i < golsEquipeA; i++) {
        int indiceA = buscarEquipe(p->equipesSelecionadas[0]);
        equipe e = v_equipes[indiceA];
        if (!registrarGol(jogadoresGolA[i], e)) {
            return false;
        }
        p->jogadoresGolA[i] = jogadoresGolA[i];
    }

    for (int i = 0; i < golsEquipeB; i++) {
        int indiceB = buscarEquipe(p->equipesSelecionadas[1]);
        equipe e = v_equipes[indiceB];
        if (!registrarGol(jogadoresGolB[i], e)) {
            return false;
        }
        p->jogadoresGolB[i] = jogadoresGolB[i];
    }

    return true;
}


bool registrarCampeoes(int indiceC, int campeao, int viceCampeao, int terceiroLugar) {
    if (indiceC == -1) {
        printf("Índice do campeonato inválido!\n");
        return false;
    }

    if (buscarEquipe(campeao) == -1) {
        printf("ID do campeão não existe!\n");
        return false;
    }

    if (buscarEquipe(viceCampeao) == -1) {
        printf("ID do vice-campeão não existe!\n");
        return false;
    }

    if (buscarEquipe(terceiroLugar) == -1) {
        printf("ID do terceiro lugar não existe!\n");
        return false;
    }

        bool equipeValida = false;
        for (int i = 0; i < c->totalEquipes; i++) {
            if (c->equipesParticipantes[i] == campeao) equipeValida = true;
        }
        if (!equipeValida) {
            printf("Equipe campeã não está no campeonato!\n");
            return false;
        }

    v_campeonatos[indiceC].campeao = campeao;
    v_campeonatos[indiceC].viceCampeao = viceCampeao;
    v_campeonatos[indiceC].terceiroLugar = terceiroLugar;
    v_campeonatos[indiceC].status = false;

    printf("Campeões registrados com sucesso!\n");
    return true;
}


/*     FUNCOES DE ESTATISTICAS       */
void listarCampeonatosComVencedores() {
    if (totalCampeonatos == 0) {
        printf("\n╔════════════════════════════════════╗");
        printf("\n║      Nenhum campeonato cadastrado! ║");
        printf("\n╚════════════════════════════════════╝\n");
        return;
    }

    printf("\n══════════════════════════════════════════════════════════════════════════════════\n");
    printf("                                CAMPEONATOS E VENCEDORES                          \n");
    printf("══════════════════════════════════════════════════════════════════════════════════\n");
    printf(" ID │ Nome%-25s │ Ano  │ Status    │ Equipes │ %-20s %-20s %-20s\n", "", "Campeão", "Vice", "3º Lugar");
    printf("────┼───────────────────────────────┼──────┼───────────┼─────────┼────────────────────────────────────────────────────\n");

    for (int i = 0; i < totalCampeonatos; i++) {
        campeonato c = v_campeonatos[i];
        char status[10];
        strcpy(status, c.status ? "Ativo" : "Finalizado");
        
        char campeao[30] = "N/A";
        char vice[30] = "N/A";
        char terceiro[30] = "N/A";

        if (!c.status) { // Se finalizado
            int indice = buscarEquipe(c.campeao);
            if (indice != -1) strcpy(campeao, v_equipes[indice].nomeEquipe);
            
            indice = buscarEquipe(c.viceCampeao);
            if (indice != -1) strcpy(vice, v_equipes[indice].nomeEquipe);
            
            indice = buscarEquipe(c.terceiroLugar);
            if (indice != -1) strcpy(terceiro, v_equipes[indice].nomeEquipe);
        }

        printf(" %-2d │ %-30s│ %-4d │ %-9s │ %-7d │ %-20s %-20s %-20s\n", 
               c.id, c.nome, c.ano, status, c.totalEquipes, campeao, vice, terceiro);
    }
    printf("══════════════════════════════════════════════════════════════════════════════════\n");
}
void calcularMediaIdadePorEquipe() {
    if (totalEquipes == 0) {
        printf("\n╔══════════════════════════════╗");
        printf("\n║   Nenhuma equipe cadastrada! ║");
        printf("\n╚══════════════════════════════╝\n");
        return;
    }

    printf("\n══════════════════════════════════════════════════════════\n");
    printf("                MÉDIA DE IDADE POR EQUIPE                \n");
    printf("══════════════════════════════════════════════════════════\n");
    printf(" Equipe%-25s │ Jogadores │ Média de Idade \n", "");
    printf("───────────────────────────────────────────┼───────────┼────────────────\n");

    for (int i = 0; i < totalEquipes; i++) {
        equipe e = v_equipes[i];
        int soma = 0, total = 0;
        
        for (int j = 0; j < totalJogadores; j++) {
            if (v_jogador[j].idEquipe == e.id) {
                soma += v_jogador[j].idade;
                total++;
            }
        }

        if (total > 0) {
            printf(" %-34s│ %-9d │ %15.2f anos\n", e.nomeEquipe, total, (float)soma/total);
        } else {
            printf(" %-34s│ %-9s │ %15s\n", e.nomeEquipe, "0", "Sem jogadores");
        }
    }
    printf("══════════════════════════════════════════════════════════\n");
}
void listarArtilheirosPorCampeonato() {
    if (totalCampeonatos == 0) {
        printf("\n╔════════════════════════════════════╗");
        printf("\n║      Nenhum campeonato cadastrado! ║");
        printf("\n╚════════════════════════════════════╝\n");
        return;
    }

    printf("\n══════════════════════════════════════════════════════════════════════\n");
    printf("                         ARTILHEIROS POR CAMPEONATO                  \n");
    printf("══════════════════════════════════════════════════════════════════════\n");

    for (int i = 0; i < totalCampeonatos; i++) {
        campeonato c = v_campeonatos[i];
        printf("\n► Campeonato: %s (%d)\n", c.nome, c.ano);
        printf("-----------------------------------------------\n");
        
        int max_gols = -1;
        // Encontra a maior quantidade de gols
        for (int j = 0; j < totalJogadores; j++) {
            if (v_jogador[j].gols > max_gols) {
                max_gols = v_jogador[j].gols;
            }
        }

        if (max_gols <= 0) {
            printf(" Nenhum gol registrado neste campeonato\n");
            continue;
        }

        printf(" Jogador%-25s │ Equipe%-20s │ Gols \n", "", "");
        printf("───────────────────────────────────────────┼──────────────────────────┼───────\n");
        
        for (int j = 0; j < totalJogadores; j++) {
            if (v_jogador[j].gols == max_gols) {
                int indiceEquipe = buscarEquipe(v_jogador[j].idEquipe);
                char nomeEquipe[30] = "N/A";
                if (indiceEquipe != -1) {
                    strcpy(nomeEquipe, v_equipes[indiceEquipe].nomeEquipe);
                }
                printf(" %-34s│ %-25s│ %-5d\n", 
                       v_jogador[j].nomeJogador, nomeEquipe, v_jogador[j].gols);
            }
        }
    }
    printf("══════════════════════════════════════════════════════════════════════\n");
}
void consultarGolsPorJogador() {
    if (totalJogadores == 0) {
        printf("\n╔══════════════════════════════╗");
        printf("\n║  Nenhum jogador cadastrado!  ║");
        printf("\n╚══════════════════════════════╝\n");
        return;
    }

    int id;
    printf("\nDigite o ID do jogador: ");
    scanf("%d", &id);
    getchar();

    int indice = buscarJogadores(id);
    if (indice == -1) {
        printf("\n╔══════════════════════════════════╗");
        printf("\n║   Jogador não encontrado! %-4d  ║", id);
        printf("\n╚══════════════════════════════════╝\n");
        return;
    }

    jogador j = v_jogador[indice];
    printf("\n══════════════════════════════════════════════\n");
    printf("               ESTATÍSTICAS DO JOGADOR       \n");
    printf("══════════════════════════════════════════════\n");
    printf(" Nome: %-30s │ ID: %-5d \n", j.nomeJogador, j.id);
    printf("──────────────────────────────────────────────\n");
    printf(" Equipe: %-25s │ Gols: %-3d \n", 
           v_equipes[buscarEquipe(j.idEquipe)].nomeEquipe, j.gols);
    printf("══════════════════════════════════════════════\n");
}


void listarJogadoresDeUmaEquipe(int idEquipe) {
    int indiceE = buscarEquipe(idEquipe);
    if (indiceE == -1) {
        printf("\n╔══════════════════════════════════════════╗");
        printf("\n║     Equipe não encontrada! (ID: %-4d)    ║", idEquipe);
        printf("\n╚══════════════════════════════════════════╝\n");
        return;
    }

    equipe e = v_equipes[indiceE];
    printf("\n══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                          JOGADORES DA EQUIPE: %-30s                          \n", e.nomeEquipe);
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf(" ID │ Nome%-25s │ Idade │ Camisa │ Posição%-15s │ Gols \n", "", "");
    printf("────┼───────────────────────────────┼───────┼────────┼─────────────────────┼───────\n");

    int encontrados = 0;
    for (int i = 0; i < totalJogadores; i++) {
        if (v_jogador[i].idEquipe == idEquipe) {
            jogador j = v_jogador[i];
            printf(" %-2d │ %-30s│ %-5d │ %-6d │ %-20s│ %-5d\n",
                   j.id, j.nomeJogador, j.idade, j.numeroCamisa, j.posicao, j.gols);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf(" Nenhum jogador cadastrado nesta equipe.\n");
    }
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
}

void listarPartidasDeUmCampeonato(int idCampeonato) {
    int indiceC = buscarCampeonato(idCampeonato);
    if (indiceC == -1) {
        printf("\n╔══════════════════════════════════════════╗");
        printf("\n║  Campeonato não encontrado! (ID: %-4d)  ║", idCampeonato);
        printf("\n╚══════════════════════════════════════════╝\n");
        return;
    }

    campeonato c = v_campeonatos[indiceC];
    printf("\n══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                         PARTIDAS DO CAMPEONATO: %-30s                         \n", c.nome);
   printf("══════════════════════════════════════════════════════════════════\n");
    printf(" ID │ Equipe A%-20s │ Equipe B%-20s │ Data%-12s │ Placar \n", "", "", "");
    printf("────┼───────────────────────┼───────────────────────┼────────────┼────────\n");

    int encontradas = 0;
    for (int i = 0; i < totalPartidas; i++) {
        if (v_partidas[i].idCampeonato == idCampeonato) {
            partida p = v_partidas[i];
            equipe eA = v_equipes[buscarEquipe(p.equipesSelecionadas[0])];
            equipe eB = v_equipes[buscarEquipe(p.equipesSelecionadas[1])];
            char placar[20];
            sprintf(placar, "%d x %d", p.totalGolsA, p.totalGolsB);
            
            printf(" %-2d │ %-22s│ %-22s│ %-11s│ %-9s\n",
                   p.id, eA.nomeEquipe, eB.nomeEquipe, p.data, placar);
            encontradas++;
        }
    }

    if (encontradas == 0) {
        printf(" Nenhuma partida cadastrada neste campeonato.\n");
    }
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
}

void listarTodosOsDados() {
    printf("\n══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                                        RELATÓRIO COMPLETO DO SISTEMA                                  \n");
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    
    // Lista Equipes
    listarEquipes();
    
    // Lista Jogadores
    listarJogadores();
    
    // Lista Campeonatos
    listarCampeonatos();
    
    // Lista Partidas
    listarPartidas();
    
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("                                         FIM DO RELATÓRIO                                            \n");
    printf("══════════════════════════════════════════════════════════════════════════════════════════════════════\n");
}