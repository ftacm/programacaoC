

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CAP_FILA   5
#define CAP_PILHA  3

typedef struct {
    char nome; /* 'I', 'O', 'T', 'L' */
    int  id;
} Peca;


typedef struct {
    Peca dados[CAP_FILA];
    int head;
    int size;
} Fila;


typedef struct {
    Peca dados[CAP_PILHA];
    int topo; /* -1 = vazia */
} Pilha;


void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
Peca gerarPeca(void);

int enqueue(Fila *f, Peca x);
int dequeue(Fila *f, Peca *out);

int push(Pilha *p, Peca x);
int pop(Pilha *p, Peca *out);

void exibirEstado(const Fila *f, const Pilha *p);
void exibirMenu(void);

/* ----------------------------
   Geracao de pecas
   ---------------------------- */
Peca gerarPeca(void) {
    static int proximoId = 0;
    const char tipos[4] = {'I','O','T','L'};
    int idx = rand() % 4;
    Peca p;
    p.nome = tipos[idx];
    p.id   = proximoId++;
    return p;
}

/* ----------------------------
   Fila circular
   ---------------------------- */
void inicializarFila(Fila *f) {
    f->head = 0;
    f->size = 0;
}

int enqueue(Fila *f, Peca x) {
    int tailIndex;
    if (f->size == CAP_FILA) return 0;
    tailIndex = (f->head + f->size) % CAP_FILA;
    f->dados[tailIndex] = x;
    f->size++;
    return 1;
}

int dequeue(Fila *f, Peca *out) {
    if (f->size == 0) return 0;
    if (out != NULL) *out = f->dados[f->head];
    f->head = (f->head + 1) % CAP_FILA;
    f->size--;
    return 1;
}

/* ----------------------------
   Pilha linear
   ---------------------------- */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int push(Pilha *p, Peca x) {
    if (p->topo >= CAP_PILHA - 1) return 0;
    p->topo++;
    p->dados[p->topo] = x;
    return 1;
}

int pop(Pilha *p, Peca *out) {
    if (p->topo < 0) return 0;
    if (out != NULL) *out = p->dados[p->topo];
    p->topo--;
    return 1;
}


void exibirEstado(const Fila *f, const Pilha *p) {
    int i, idx;

    printf("\nEstado atual:\n");

    /* Fila */
    printf("Fila de pecas\t");
    if (f->size == 0) {
        printf("(vazia)\n");
    } else {
        for (i = 0; i < f->size; i++) {
            idx = (f->head + i) % CAP_FILA;
            printf("[%c %d] ", f->dados[idx].nome, f->dados[idx].id);
        }
        printf("\n");
    }

    /* Pilha */
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (p->topo < 0) {
        printf("(vazia)");
    } else {
        for (i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->dados[i].nome, p->dados[i].id);
        }
    }
    printf("\n");
}

void exibirMenu(void) {
    printf("\nOpcoes de Acao:\n\n");
    printf("Codigo\tAcao\n");
    printf("1\tJogar peca\n");
    printf("2\tReservar peca\n");
    printf("3\tUsar peca reservada\n");
    printf("0\tSair\n");
    printf("Opcao: ");
}


int main(void) {
    Fila fila;
    Pilha pilha;
    int i, opcao;
    Peca tmp, gerada;

    srand((unsigned int)time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Preenche fila inicial */
    for (i = 0; i < CAP_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    exibirEstado(&fila, &pilha);

    while (1) {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }

        if (opcao == 0) {
            printf("\nEncerrando...\n");
            break;
        }
        else if (opcao == 1) {
            if (dequeue(&fila, &tmp)) {
                printf("\nVoce jogou a peca [%c %d].\n", tmp.nome, tmp.id);
            } else {
                printf("\nFila vazia (erro logico).\n");
            }
            gerada = gerarPeca();
            enqueue(&fila, gerada);
            exibirEstado(&fila, &pilha);
        }
        else if (opcao == 2) {
            if (pilha.topo >= CAP_PILHA - 1) {
                printf("\nReserva cheia. Nao e possivel reservar mais.\n");
            } else {
                if (dequeue(&fila, &tmp)) {
                    if (push(&pilha, tmp)) {
                        printf("\nPeca [%c %d] movida para a reserva.\n", tmp.nome, tmp.id);
                    } else {
                        printf("\nFalha ao empilhar. Peca retirada nao volta ao jogo.\n");
                    }
                } else {
                    printf("\nNao foi possivel reservar: fila vazia.\n");
                }
            }
            gerada = gerarPeca();
            enqueue(&fila, gerada);
            exibirEstado(&fila, &pilha);
        }
        else if (opcao == 3) {
            if (pop(&pilha, &tmp)) {
                printf("\nVoce usou a peca reservada [%c %d].\n", tmp.nome, tmp.id);
            } else {
                printf("\nReserva vazia. Nao ha peca para usar.\n");
            }
            gerada = gerarPeca();
            enqueue(&fila, gerada);
            exibirEstado(&fila, &pilha);
        }
        else {
            printf("\nOpcao desconhecida. Tente novamente.\n");
        }
    }

    return 0;
}

