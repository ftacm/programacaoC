#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


#define TAM_NOME 30
#define TAM_COR  10


typedef struct {
    char nome[TAM_NOME]; /* nome do território */
    char cor[TAM_COR];   /* cor */
    int  tropas;         /* quantidade de tropas no território */
} Territorio;


static void removeNovaLinha(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

static void limpaEntrada(void) { 
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {  }
}

/* ---- Exibição do mapa ---- */
void exibirMapa(const Territorio *mapa, int n) {
    printf("\n=== MAPA ATUAL ===\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] %-28s | Cor: %-9s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("==================\n");
}


void cadastrarTerritorios(Territorio *mapa, int n) {
    printf("\n=== Cadastro de Territorios (%d) ===\n", n);
    for (int i = 0; i < n; i++) {
        Territorio *t = &mapa[i];

        printf("\n-- Territorio %d --\n", i);

        printf("Nome: ");
        if (fgets(t->nome, TAM_NOME, stdin) == NULL) {
            t->nome[0] = '\0';
        } else {
            removeNovaLinha(t->nome);
        }

        printf("Cor do exercito (ex.: azul, vermelho): ");
        if (fgets(t->cor, TAM_COR, stdin) == NULL) {
            t->cor[0] = '\0';
        } else {
            removeNovaLinha(t->cor);
        }

        printf("Tropas (inteiro >= 1): ");
        while (scanf("%d", &t->tropas) != 1 || t->tropas < 1) {
            limpaEntrada();
            printf("Valor invalido. Digite um inteiro >= 1: ");
        }
        limpaEntrada(); 
    }
}


void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("\nRolagens -> %s (A): %d  x  %s (D): %d\n",
           atacante->nome, dadoA, defensor->nome, dadoD);

    if (dadoA > dadoD) {
        
        int mover = atacante->tropas / 2; 
        if (mover < 1) mover = 1;         
        if (mover >= atacante->tropas) mover = atacante->tropas - 1; 

        printf("Resultado: %s venceu! Capturando %s.\n", atacante->nome, defensor->nome);
        printf("Ocupacao: movendo %d tropas para o territorio capturado.\n", mover);

        strncpy(defensor->cor, atacante->cor, TAM_COR - 1);
        defensor->cor[TAM_COR - 1] = '\0';
        defensor->tropas = mover;
        atacante->tropas -= mover;
    } else {
        
        printf("Resultado: defesa de %s resistiu. %s perde 1 tropa.\n",
               defensor->nome, atacante->nome);
        if (atacante->tropas > 1) {
            atacante->tropas -= 1;
        } else {
            
            atacante->tropas = 1;
        }
    }
}


bool validarSelecao(const Territorio *mapa, int n, int ia, int id) {
    if (ia < 0 || ia >= n || id < 0 || id >= n) {
        printf("Indices invalidos. Use valores entre 0 e %d.\n", n - 1);
        return false;
    }
    if (ia == id) {
        printf("Um territorio nao pode atacar a si mesmo.\n");
        return false;
    }
    if (strncmp(mapa[ia].cor, mapa[id].cor, TAM_COR) == 0) {
        printf("Ataque invalido: nao pode atacar um territorio da MESMA cor.\n");
        return false;
    }
    if (mapa[ia].tropas < 2) {
        printf("Ataque invalido: o atacante precisa ter pelo menos 2 tropas.\n");
        return false;
    }
    if (mapa[id].tropas < 1) {
        printf("Ataque invalido: o defensor precisa ter ao menos 1 tropa.\n");
        return false;
    }
    return true;
}


void liberarMemoria(Territorio *mapa) {
    free(mapa);
}


int main(void) {
    srand((unsigned)time(NULL));

    int n;
    printf("Quantos territorios terao no mapa? ");
    while (scanf("%d", &n) != 1 || n < 2) {
        limpaEntrada();
        printf("Valor invalido. Digite um inteiro >= 2: ");
    }
    limpaEntrada();


    Territorio *mapa = (Territorio *)calloc((size_t)n, sizeof(Territorio));
    if (!mapa) {
        perror("Falha ao alocar memoria");
        return 1;
    }

    cadastrarTerritorios(mapa, n);
    exibirMapa(mapa, n);


    while (1) {
        int opcao;
        printf("\n--- Menu ---\n");
        printf("1 - Atacar\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            limpaEntrada();
            printf("Entrada invalida.\n");
            continue;
        }
        limpaEntrada();

        if (opcao == 0) {
            break;
        } else if (opcao == 1) {
            exibirMapa(mapa, n);
            printf("Selecione indice do ATACANTE: ");
            int ia, id;
            if (scanf("%d", &ia) != 1) { limpaEntrada(); puts("Entrada invalida."); continue; }
            limpaEntrada();

            printf("Selecione indice do DEFENSOR: ");
            if (scanf("%d", &id) != 1) { limpaEntrada(); puts("Entrada invalida."); continue; }
            limpaEntrada();

            if (!validarSelecao(mapa, n, ia, id)) {
                continue;
            }

            atacar(&mapa[ia], &mapa[id]);
            exibirMapa(mapa, n);
        } else {
            printf("Opcao desconhecida.\n");
        }
    }

    liberarMemoria(mapa);
    printf("Encerrado. Memoria liberada.\n");
    return 0;
}
