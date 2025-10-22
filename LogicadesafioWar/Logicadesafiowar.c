#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TERRITORIOS 10
#define MAX_MISSOES 5
#define MAX_JOGADORES 2

// Estrutura para representar um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Exibe a missão de um jogador (apenas no início)
void exibirMissao(char* missao) {
    printf("Sua missão: %s\n\n", missao);
}

// Atribui uma missão aleatória ao jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Simula um ataque entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Não é possível atacar territórios da mesma cor.\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("%s (A) rolou %d | %s (D) rolou %d\n", atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("O atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
    } else {
        printf("O defensor venceu!\n");
        atacante->tropas--;
    }
}

// Verifica se a missão do jogador foi cumprida
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    // Exemplo 1: "Conquistar 3 territorios seguidos"
    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0) {
        int cont = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                cont++;
                if (cont >= 3)
                    return 1;
            } else {
                cont = 0;
            }
        }
    }

    // Exemplo 2: "Eliminar todas as tropas da cor vermelha"
    if (strcmp(missao, "Eliminar todas as tropas da cor vermelha") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0)
                return 0;
        }
        return 1;
    }

    // Outras missões podem ser implementadas aqui com lógica adicional
    return 0;
}

// Libera memória alocada dinamicamente
void liberarMemoria(char** missoesJogadores, int total) {
    for (int i = 0; i < total; i++) {
        free(missoesJogadores[i]);
    }
    free(missoesJogadores);
}

int main() {
    srand(time(NULL));

    // Definindo missões
    char* missoes[MAX_MISSOES] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar pelo menos 5 territorios",
        "Manter 2 territorios com mais de 10 tropas",
        "Conquistar o territorio Alpha"
    };

    // Mapa simplificado
    Territorio mapa[MAX_TERRITORIOS] = {
        {"Alpha", "vermelho", 5},
        {"Bravo", "azul", 3},
        {"Charlie", "vermelho", 4},
        {"Delta", "azul", 6},
        {"Echo", "azul", 2},
        {"Foxtrot", "vermelho", 7},
        {"Golf", "azul", 5},
        {"Hotel", "vermelho", 3},
        {"India", "azul", 4},
        {"Juliet", "vermelho", 6}
    };

    // Aloca missões dos jogadores dinamicamente
    char** missoesJogadores = (char**)malloc(sizeof(char*) * MAX_JOGADORES);
    for (int i = 0; i < MAX_JOGADORES; i++) {
        missoesJogadores[i] = (char*)malloc(sizeof(char) * 100);
        atribuirMissao(missoesJogadores[i], missoes, MAX_MISSOES);
    }

    // Cores dos jogadores
    char* coresJogadores[MAX_JOGADORES] = {"azul", "vermelho"};

    // Exibe as missões uma única vez
    for (int i = 0; i < MAX_JOGADORES; i++) {
        printf("Jogador %d (%s):\n", i + 1, coresJogadores[i]);
        exibirMissao(missoesJogadores[i]);
    }

    int turno = 0;
    int vencedor = -1;

    // Loop do jogo (simplificado)
    while (vencedor == -1) {
        printf("\n--- Turno %d ---\n", turno + 1);

        int jogadorAtual = turno % MAX_JOGADORES;
        printf("Jogador %d (%s) está atacando...\n", jogadorAtual + 1, coresJogadores[jogadorAtual]);

        // Ataque aleatório de território (exemplo simples)
        int origem = rand() % MAX_TERRITORIOS;
        int destino = rand() % MAX_TERRITORIOS;

        printf("Tentando atacar de %s para %s...\n", mapa[origem].nome, mapa[destino].nome);
        atacar(&mapa[origem], &mapa[destino]);

        // Verifica se o jogador venceu
        if (verificarMissao(missoesJogadores[jogadorAtual], mapa, MAX_TERRITORIOS, coresJogadores[jogadorAtual])) {
            vencedor = jogadorAtual;
            printf("\n=== Jogador %d (%s) venceu cumprindo sua missão: %s ===\n", vencedor + 1, coresJogadores[vencedor], missoesJogadores[vencedor]);
        }

        turno++;
        if (turno > 100) break; // Evita loop infinito em testes
    }

    liberarMemoria(missoesJogadores, MAX_JOGADORES);
    return 0;
}