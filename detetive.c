#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct Sala {
    char *nome;
    char *pista;            
    struct Sala *esq;       
    struct Sala *dir;       
} Sala;


typedef struct PistaNode {
    char *pista;
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;


Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if (!s) { fprintf(stderr, "Erro de memoria ao criar sala.\n"); exit(1); }
    s->nome = (char*) malloc(strlen(nome) + 1);
    strcpy(s->nome, nome);
    if (pista) {
        s->pista = (char*) malloc(strlen(pista) + 1);
        strcpy(s->pista, pista);
    } else {
        s->pista = NULL;
    }
    s->esq = s->dir = NULL;
    return s;
}


PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (pista == NULL) return raiz; 
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        novo->pista = (char*) malloc(strlen(pista) + 1);
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);
    return raiz;
}


void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esq);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}


void liberarArvoreSalas(Sala *r) {
    if (!r) return;
    liberarArvoreSalas(r->esq);
    liberarArvoreSalas(r->dir);
    free(r->nome);
    if (r->pista) free(r->pista);
    free(r);
}
void liberarArvorePistas(PistaNode *r) {
    if (!r) return;
    liberarArvorePistas(r->esq);
    liberarArvorePistas(r->dir);
    free(r->pista);
    free(r);
}


char lerOpcao(void) {
    char linha[32];
    if (!fgets(linha, sizeof(linha), stdin)) return 's';
    for (size_t i = 0; linha[i]; ++i) {
        if (!isspace((unsigned char)linha[i])) return (char)tolower((unsigned char)linha[i]);
    }
    return '\0';
}


void explorarSalasComPistas(Sala *atual, PistaNode **pistas) {
    printf("=== Detective Quest: Exploracao da Mansao ===\n");
    printf("Use [e] esquerda, [d] direita, [s] sair.\n\n");

    while (atual) {
        printf("Voce esta em: %s\n", atual->nome);

        if (atual->pista) {
            printf("Voce encontrou uma pista: \"%s\"\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        } else {
            printf("Nenhuma pista neste comodo.\n");
        }

        printf("\nEscolha o caminho - [e]%s  [d]%s  [s]air: ",
               atual->esq ? "" : " indisponivel",
               atual->dir ? "" : " indisponivel");

        char op = lerOpcao();

        if (op == 's') {
            printf("Exploracao encerrada.\n");
            break;
        } else if (op == 'e' && atual->esq) {
            atual = atual->esq;
        } else if (op == 'd' && atual->dir) {
            atual = atual->dir;
        } else {
            printf("Caminho invalido.\n");
        }

        printf("\n");
    }
}

int main(void) {
  
    Sala *hall       = criarSala("Hall de Entrada", "Mapa antigo com anotacoes");
    Sala *estar      = criarSala("Sala de Estar", "Foto de uma pessoa desconhecida");
    Sala *corredor   = criarSala("Corredor", NULL);
    Sala *cozinha    = criarSala("Cozinha", "Copo quebrado com liquido estranho");
    Sala *biblioteca = criarSala("Biblioteca", "Pagina rasgada de um diario");
    Sala *quarto     = criarSala("Quarto", "Chave enferrujada");
    Sala *jardim     = criarSala("Jardim", NULL);

    
    hall->esq = estar;       hall->dir = corredor;
    estar->esq = cozinha;    estar->dir = biblioteca;
    corredor->esq = quarto;  corredor->dir = jardim;

    
    PistaNode *pistas = NULL;

    
    explorarSalasComPistas(hall, &pistas);

    
    printf("\n=== Pistas Coletadas ===\n");
    if (pistas)
        exibirPistas(pistas);
    else
        printf("Nenhuma pista foi encontrada.\n");


    liberarArvoreSalas(hall);
    liberarArvorePistas(pistas);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
