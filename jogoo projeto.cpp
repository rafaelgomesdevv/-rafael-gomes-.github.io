#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUMERO_DADOS 5
#define MAXIMO_TURNOS 20
#define MAXIMO_LANCAMENTOS 3

// Estrutura do Jogador
typedef struct {
    char nome[50];
    int pontuacoes[20];  // Pontuações de 1 a 20
    int pontuacoes_usadas[20];  // posições de pontuação usadas
    int pontuacao_total;
} Jogador;

// Protótipos de funções
void exibirRegras();
void lancarDados(int dados[], int dados_disponiveis[], int num_dados);
void imprimirDados(int dados[], int num_dados);
int calcularPontuacao(int dados[], int num_dados);
void imprimirTabelaPontuacao(Jogador* jogador);
int escolherPontuacao(Jogador* jogador, int pontuacao_atual);
void jogarJogo();
void salvarJogo(Jogador* jogadores, int num_jogadores);
int carregarJogo(Jogador* jogadores);

int main() {
    srand(time_t(NULL));  // Inicializar gerador de números aleatórios

    while (1) {
        printf("\n--- Jogo de Dados ---\n");
        printf("1. Regras do Jogo\n");
        printf("2. Iniciar Novo Jogo\n");
        printf("3. Carregar Jogo\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");

        int escolha;
        scanf_s("%d", &escolha);

        switch (escolha) {
        case 1:
            exibirRegras();
            break;
        case 2:
            jogarJogo();
            break;
        case 3:
            // Carregar jogo (a implementar)
            printf("Função de carregar jogo ainda não implementada.\n");
            break;
        case 4:
            return 0;
        default:
            printf("Opção inválida!\n");
        }
    }
    return 0;
}

void exibirRegras() {
    printf("\n--- Regras do Jogo ---\n");
    printf("Objetivo: Atingir a maior pontuação possível.\n\n");
    printf("Regras básicas:\n");
    printf("1. Jogue 5 dados em cada turno.\n");
    printf("2. Máximo de 3 lançamentos por turno.\n");
    printf("3. A pontuação é calculada pela soma dos valores dos dados não usados.\n");
    printf("4. Deve preencher todas as pontuações de 1 a 20.\n");
    printf("5. Bônus de 50 pontos por completar cada grupo de valores:\n");
    printf("   - Grupo 1-5\n");
    printf("   - Grupo 6-15\n");
    printf("   - Grupo 16-20\n");
    printf("6. O jogo termina após 20 rodadas.\n");

    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n');  // Limpar buffer de entrada
    getchar();  // Aguardar Enter
}

void lancarDados(int dados[], int dados_disponiveis[], int num_dados) {
    for (int i = 0; i < num_dados; i++) {
        if (dados_disponiveis[i]) {
            dados[i] = rand() % 6 + 1;
        }
    }
}

void imprimirDados(int dados[], int num_dados) {
    printf("Dados: ");
    for (int i = 0; i < num_dados; i++) {
        printf("%d ", dados[i]);
    }
    printf("\n");
}

int calcularPontuacao(int dados[], int num_dados) {
    int soma_total = 0;
    int valores_usados[7] = { 0 };  // Rastrear valores de dados usados

    // Marcar dados usados
    for (int i = 0; i < num_dados; i++) {
        valores_usados[dados[i]] = 1;
    }

    // Somar valores não usados
    for (int i = 1; i <= 6; i++) {
        if (!valores_usados[i]) {
            soma_total += i;
        }
    }

    return soma_total;
}

void jogarJogo() {
    Jogador jogadores[2];
    int jogador_atual = 0;

    // Inicializar jogadores
    strcpy_s(jogadores[0].nome, "Jogador 1");
    strcpy_s(jogadores[1].nome, "Jogador 2");

    // Inicializar pontuações e marcações
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 20; i++) {
            jogadores[j].pontuacoes[i] = 0;
            jogadores[j].pontuacoes_usadas[i] = 0;
        }
        jogadores[j].pontuacao_total = 0;
    }

    for (int rodada = 0; rodada < MAXIMO_TURNOS; rodada++) {
        printf("\n--- Rodada %d ---\n", rodada + 1);
        printf("Turno de %s\n", jogadores[jogador_atual].nome);

        int dados[NUMERO_DADOS];
        int dados_disponiveis[NUMERO_DADOS];
        for (int i = 0; i < NUMERO_DADOS; i++) dados_disponiveis[i] = 1;

        int lancamentos_restantes = MAXIMO_LANCAMENTOS;
        int pontuacao_escolhida = 0;

        while (lancamentos_restantes > 0 && !pontuacao_escolhida) {
            lancarDados(dados, dados_disponiveis, NUMERO_DADOS);
            imprimirDados(dados, NUMERO_DADOS);

            int pontuacao_atual = calcularPontuacao(dados, NUMERO_DADOS);
            printf("Pontuação atual: %d\n", pontuacao_atual);

            // Lógica de escolha de pontuação
            if (lancamentos_restantes > 1) {
                char resposta;
                printf("Deseja relançar alguns dados? (s/n): ");
                scanf_s(" %c", &resposta);

                if (resposta == 's' || resposta == 'S') {
                    printf("Quais dados deseja manter? (1-5, 0 para terminar)\n");
                    int dado;
                    while (1) {
                        scanf_s("%d", &dado);
                        if (dado == 0) break;
                        if (dado >= 1 && dado <= 5) {
                            dados_disponiveis[dado - 1] = 0;
                        }
                    }
                }
                else {
                    pontuacao_escolhida = escolherPontuacao(&jogadores[jogador_atual], pontuacao_atual);
                    break;
                }
            }
            else {
                pontuacao_escolhida = escolherPontuacao(&jogadores[jogador_atual], pontuacao_atual);
            }

            lancamentos_restantes--;
        }

        // Trocar jogador
        jogador_atual = 1 - jogador_atual;
    }

    // Calcular e mostrar pontuação final
    printf("\n--- Pontuação Final ---\n");
    for (int j = 0; j < 2; j++) {
        printf("%s: %d pontos\n", jogadores[j].nome, jogadores[j].pontuacao_total);
    }
}

int escolherPontuacao(Jogador* jogador, int pontuacao_atual) {
    printf("Escolha uma pontuação para registrar:\n");

    // Mostrar pontuações disponíveis
    for (int i = 0; i < 20; i++) {
        if (!jogador->pontuacoes_usadas[i]) {
            printf("%d. Pontuação %d (Não utilizada)\n", i + 1, i + 1);
        }
    }

    int escolha;
    do {
        printf("Sua escolha (1-20): ");
        scanf_s("%d", &escolha);
    } while (escolha < 1 || escolha > 20 || jogador->pontuacoes_usadas[escolha - 1]);

    // Registrar pontuação
    jogador->pontuacoes_usadas[escolha - 1] = 1;
    jogador->pontuacoes[escolha - 1] = pontuacao_atual;
    jogador->pontuacao_total += pontuacao_atual;

    // Verificar bônus
    int bonus = 0;
    int grupos[3][2] = { {0,5}, {5,15}, {15,20} };

    for (int i = 0; i < 3; i++) {
        int grupo_completo = 1;
        for (int j = grupos[i][0]; j < grupos[i][1]; j++) {
            if (!jogador->pontuacoes_usadas[j]) {
                grupo_completo = 0;
                break;
            }
        }

        if (grupo_completo) {
            bonus += 50;
        }
    }

    jogador->pontuacao_total += bonus;

    return 1;
}