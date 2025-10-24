#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_MAX 5  // Tamanho fixo da fila

// --- Estrutura da peça ---
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// --- Estrutura da fila circular ---
typedef struct {
    Peca itens[TAM_MAX];
    int inicio;
    int fim;
    int qtd;
} Fila;

// --- Protótipos de função ---
void inicializarFila(Fila *fila);
int filaCheia(Fila *fila);
int filaVazia(Fila *fila);
void enqueue(Fila *fila, Peca nova);
Peca dequeue(Fila *fila);
void exibirFila(Fila *fila);
Peca gerarPeca();

// --- Variável global para gerar IDs únicos ---
int contadorID = 0;

// --- Função principal ---
int main() {
    Fila fila;
    int opcao;
    
    srand(time(NULL)); // Semente para números aleatórios
    inicializarFila(&fila);

    // Preenche a fila inicialmente com 5 peças
    for (int i = 0; i < TAM_MAX; i++) {
        enqueue(&fila, gerarPeca());
    }

    do {
        printf("\n===== Fila de peças =====\n");
        exibirFila(&fila);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Inserir nova peça\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("\n>> Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                } else {
                    printf("\nA fila está vazia! Nenhuma peça para jogar.\n");
                }
                break;

            case 2:
                if (!filaCheia(&fila)) {
                    Peca nova = gerarPeca();
                    enqueue(&fila, nova);
                    printf("\n>> Nova peça inserida: [%c %d]\n", nova.nome, nova.id);
                } else {
                    printf("\nA fila está cheia! Não é possível inserir nova peça.\n");
                }
                break;

            case 0:
                printf("\nEncerrando o programa...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// --- Implementações das funções ---

void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = -1;
    fila->qtd = 0;
}

int filaCheia(Fila *fila) {
    return fila->qtd == TAM_MAX;
}

int filaVazia(Fila *fila) {
    return fila->qtd == 0;
}

void enqueue(Fila *fila, Peca nova) {
    if (filaCheia(fila)) return;
    fila->fim = (fila->fim + 1) % TAM_MAX;
    fila->itens[fila->fim] = nova;
    fila->qtd++;
}

Peca dequeue(Fila *fila) {
    Peca removida = {'?', -1};
    if (filaVazia(fila)) return removida;
    removida = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAM_MAX;
    fila->qtd--;
    return removida;
}

void exibirFila(Fila *fila) {
    if (filaVazia(fila)) {
        printf("[Fila vazia]\n");
        return;
    }

    printf("Fila de peças:\n");
    int i, index;
    for (i = 0; i < fila->qtd; i++) {
        index = (fila->inicio + i) % TAM_MAX;
        printf("[%c %d] ", fila->itens[index].nome, fila->itens[index].id);
    }
    printf("\n");
}

// --- Geração automática de peças ---
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4]; // Escolhe um tipo aleatório
    nova.id = contadorID++;        // Gera ID único
    return nova;
}

