

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


#define CAPACIDADE 50
#define TAM_NOME   30
#define TAM_TIPO   20

typedef struct {
    char nome[TAM_NOME];   
    char tipo[TAM_TIPO];   
    int  quantidade;       
} Item;

typedef struct No {
    Item        dados;
    struct No * proximo;
} No;


static void removeNovaLinha(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}
static void limpaEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* descarta */ }
}

/* ============================
   BLOCO � MOCHILA (VETOR)
   ============================ */

void listarItensVetor(const Item v[], int n) {
    printf("\n=== Mochila (Vetor) � %d/%d ===\n", n, CAPACIDADE);
    if (n == 0) { puts("(vazia)"); return; }
    for (int i = 0; i < n; i++) {
        printf("[%02d] Nome: %-28s | Tipo: %-12s | Qtd: %d\n",
               i, v[i].nome, v[i].tipo, v[i].quantidade);
    }
}

int buscarSequencialVetor(const Item v[], int n, const char *nome, long *comparacoes) {
    if (comparacoes) *comparacoes = 0;
    for (int i = 0; i < n; i++) {
        if (comparacoes) (*comparacoes)++;
        if (strcmp(v[i].nome, nome) == 0) return i;
    }
    return -1;
}


void ordenarVetor(Item v[], int n) {
    int i, j, min;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++) {
            if (strcmp(v[j].nome, v[min].nome) < 0) {
                min = j;
            }
        }
        if (min != i) {
            Item tmp = v[i];
            v[i] = v[min];
            v[min] = tmp;
        }
    }
}


int buscarBinariaVetor(const Item v[], int n, const char *nome, long *comparacoes) {
    if (comparacoes) *comparacoes = 0;
    int ini = 0, fim = n - 1;
    while (ini <= fim) {
        int meio = ini + (fim - ini) / 2;
        int cmp = strcmp(nome, v[meio].nome);
        if (comparacoes) (*comparacoes)++; 
        if (cmp == 0) return meio;
        else if (cmp < 0) fim = meio - 1;
        else ini = meio + 1;
    }
    return -1;
}

void inserirItemVetor(Item v[], int *n) {
    if (*n >= CAPACIDADE) { puts("\n[Vetor] Inventario cheio."); return; }
    Item novo;
    printf("\n[Vetor] Inserir item\n");
    printf("Nome: ");  if (fgets(novo.nome, sizeof(novo.nome), stdin) == NULL) return;  removeNovaLinha(novo.nome);
    printf("Tipo: ");  if (fgets(novo.tipo, sizeof(novo.tipo), stdin) == NULL) return;  removeNovaLinha(novo.tipo);
    printf("Quantidade (>=1): ");
    while (scanf("%d", &novo.quantidade) != 1 || novo.quantidade < 1) {
        limpaEntrada(); printf("Valor invalido. Digite inteiro >=1: ");
    }
    limpaEntrada();

    v[*n] = novo;
    (*n)++;
    puts("[Vetor] Item inserido.");
    listarItensVetor(v, *n);
}

void removerItemVetor(Item v[], int *n) {
    if (*n == 0) { puts("\n[Vetor] Inventario vazio."); return; }
    char alvo[TAM_NOME];
    printf("\n[Vetor] Remover por nome: ");
    if (fgets(alvo, sizeof(alvo), stdin) == NULL) return; removeNovaLinha(alvo);

    int idx = -1;
    for (int i = 0; i < *n; i++) {
        if (strcmp(v[i].nome, alvo) == 0) { idx = i; break; }
    }
    if (idx == -1) { printf("[Vetor] \"%s\" nao encontrado.\n", alvo); return; }
    for (int i = idx; i < *n - 1; i++) v[i] = v[i + 1];
    (*n)--;
    printf("[Vetor] \"%s\" removido.\n", alvo);
    listarItensVetor(v, *n);
}

/* ============================
   BLOCO � MOCHILA (LISTA)
   ============================ */

void listarItensLista(const No *head) {
    printf("\n=== Mochila (Lista) ===\n");
    if (!head) { puts("(vazia)"); return; }
    int i = 0;
    for (const No *p = head; p; p = p->proximo, i++) {
        printf("[%02d] Nome: %-28s | Tipo: %-12s | Qtd: %d\n",
               i, p->dados.nome, p->dados.tipo, p->dados.quantidade);
    }
}

No* buscarSequencialLista(No *head, const char *nome, long *comparacoes) {
    if (comparacoes) *comparacoes = 0;
    for (No *p = head; p; p = p->proximo) {
        if (comparacoes) (*comparacoes)++;
        if (strcmp(p->dados.nome, nome) == 0) return p;
    }
    return NULL;
}

void inserirItemLista(No **head) {
    Item novo;
    printf("\n[Lista] Inserir item\n");
    printf("Nome: ");  if (fgets(novo.nome, sizeof(novo.nome), stdin) == NULL) return;  removeNovaLinha(novo.nome);
    printf("Tipo: ");  if (fgets(novo.tipo, sizeof(novo.tipo), stdin) == NULL) return;  removeNovaLinha(novo.tipo);
    printf("Quantidade (>=1): ");
    while (scanf("%d", &novo.quantidade) != 1 || novo.quantidade < 1) {
        limpaEntrada(); printf("Valor invalido. Digite inteiro >=1: ");
    }
    limpaEntrada();

    No *n = (No*)malloc(sizeof(No));
    if (!n) { perror("[Lista] malloc"); return; }
    n->dados = novo;
    n->proximo = NULL;

    if (!*head) *head = n;
    else {
        No *p = *head;
        while (p->proximo) p = p->proximo;
        p->proximo = n;
    }
    puts("[Lista] Item inserido.");
    listarItensLista(*head);
}

void removerItemLista(No **head) {
    if (!*head) { puts("\n[Lista] Inventario vazio."); return; }
    char alvo[TAM_NOME];
    printf("\n[Lista] Remover por nome: ");
    if (fgets(alvo, sizeof(alvo), stdin) == NULL) return; removeNovaLinha(alvo);

    No *ant = NULL, *p = *head;
    while (p && strcmp(p->dados.nome, alvo) != 0) { ant = p; p = p->proximo; }
    if (!p) { printf("[Lista] \"%s\" nao encontrado.\n", alvo); return; }

    if (!ant) *head = p->proximo; 
    else ant->proximo = p->proximo;

    free(p);
    printf("[Lista] \"%s\" removido.\n", alvo);
    listarItensLista(*head);
}

void liberarLista(No **head) {
    No *p = *head;
    while (p) { No *tmp = p->proximo; free(p); p = tmp; }
    *head = NULL;
}



static void menuVetor(Item v[], int *n, bool *vetorOrdenado) {
    while (1) {
        int op;
        printf("\n--- Mochila (VETOR) ---\n");
        printf("1 Inserir\t2 Remover\t3 Listar\n");
        printf("4 Ordenar (Selection)\t5 Buscar sequencial\n");
        printf("6 Buscar binaria (requer ordenado)\n");
        printf("0 Voltar\nEscolha: ");
        if (scanf("%d", &op) != 1) { limpaEntrada(); puts("Entrada invalida."); continue; }
        limpaEntrada();

        if (op == 0) return;
        else if (op == 1) { inserirItemVetor(v, n); *vetorOrdenado = false; }
        else if (op == 2) { removerItemVetor(v, n); /* ordenar invalida */ }
        else if (op == 3) { listarItensVetor(v, *n); }
        else if (op == 4) { ordenarVetor(v, *n); *vetorOrdenado = true; puts("[Vetor] Ordenado por nome."); }
        else if (op == 5) {
            char alvo[TAM_NOME];
            printf("Nome a buscar (sequencial): ");
            if (fgets(alvo, sizeof(alvo), stdin) == NULL) continue; removeNovaLinha(alvo);

            long comps = 0;
            clock_t t0 = clock();
            int idx = buscarSequencialVetor(v, *n, alvo, &comps);
            clock_t t1 = clock();

            if (idx >= 0) printf("Encontrado em %d.\n", idx);
            else printf("Nao encontrado.\n");
            printf("Comparacoes: %ld | Tempo: %.6f s\n",
                   comps, (double)(t1 - t0) / CLOCKS_PER_SEC);
        }
        else if (op == 6) {
            if (!*vetorOrdenado) {
                puts("[Vetor] Atencao: vetor nao esta ordenado. Ordene antes (opcao 4).");
                continue;
            }
            char alvo[TAM_NOME];
            printf("Nome a buscar (binaria): ");
            if (fgets(alvo, sizeof(alvo), stdin) == NULL) continue; removeNovaLinha(alvo);

            long comps = 0;
            clock_t t0 = clock();
            int idx = buscarBinariaVetor(v, *n, alvo, &comps);
            clock_t t1 = clock();

            if (idx >= 0) printf("Encontrado em %d.\n", idx);
            else printf("Nao encontrado.\n");
            printf("Comparacoes: %ld | Tempo: %.6f s\n",
                   comps, (double)(t1 - t0) / CLOCKS_PER_SEC);
        }
        else puts("Opcao invalida.");
    }
}

static void menuLista(No **head) {
    while (1) {
        int op;
        printf("\n--- Mochila (LISTA) ---\n");
        printf("1 Inserir\t2 Remover\t3 Listar\t4 Buscar sequencial\n");
        printf("0 Voltar\nEscolha: ");
        if (scanf("%d", &op) != 1) { limpaEntrada(); puts("Entrada invalida."); continue; }
        limpaEntrada();

        if (op == 0) return;
        else if (op == 1) inserirItemLista(head);
        else if (op == 2) removerItemLista(head);
        else if (op == 3) listarItensLista(*head);
        else if (op == 4) {
            if (!*head) { puts("[Lista] Vazia."); continue; }
            char alvo[TAM_NOME];
            printf("Nome a buscar (sequencial): ");
            if (fgets(alvo, sizeof(alvo), stdin) == NULL) continue; removeNovaLinha(alvo);

            long comps = 0;
            clock_t t0 = clock();
            No *n = buscarSequencialLista(*head, alvo, &comps);
            clock_t t1 = clock();

            if (n) printf("Encontrado: Nome=%s | Tipo=%s | Qtd=%d\n",
                          n->dados.nome, n->dados.tipo, n->dados.quantidade);
            else printf("Nao encontrado.\n");
            printf("Comparacoes: %ld | Tempo: %.6f s\n",
                   comps, (double)(t1 - t0) / CLOCKS_PER_SEC);
        }
        else puts("Opcao invalida.");
    }
}

int main(void) {
    Item vetor[CAPACIDADE];
    int  nVetor = 0;
    bool vetorOrdenado = false;

    No *lista = NULL;

    puts("=== Comparacao de Mochilas: Vetor vs Lista Encadeada ===");

    while (1) {
        int op;
        printf("\nMENU PRINCIPAL\n");
        printf("1 - Usar mochila (Vetor)\n");
        printf("2 - Usar mochila (Lista Encadeada)\n");
        printf("9 - Exibir ambas (listagens atuais)\n");
        printf("0 - Sair\nEscolha: ");

        if (scanf("%d", &op) != 1) { limpaEntrada(); puts("Entrada invalida."); continue; }
        limpaEntrada();

        if (op == 0) break;
        else if (op == 1) menuVetor(vetor, &nVetor, &vetorOrdenado);
        else if (op == 2) menuLista(&lista);
        else if (op == 9) { listarItensVetor(vetor, nVetor); listarItensLista(lista); }
        else puts("Opcao invalida.");
    }

    liberarLista(&lista);
    puts("Encerrado. Memoria da lista liberada.");
    return 0;
}

