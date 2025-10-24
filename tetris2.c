#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5     // Tamanho fixo da fila
#define TAM_PILHA 3    // Tamanho máximo da pilha

// --- Estrutura da peça ---
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// --- Estrutura da fila circular ---
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

// --- Estrutura da pilha ---
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// --- Protótipos de função ---
void inicializarFila(Fila *fila);
void inicializarPilha(Pilha *pilha);
int filaCheia(Fila *fila);
int filaVazia(Fila *fila);
int pilhaCheia(Pilha *pilha);
int pilhaVazia(Pilha *pilha);
void enqueue(Fila *fila, Peca nova);
Peca dequeue(Fila *fila);
void push(Pilha *pilha, Peca nova);
Peca pop(Pilha *pilha);
void exibirEstado(Fila *fila, Pilha *pilha);
Peca gerarPeca();

// --- Variável global para IDs únicos ---
int contadorID = 0;

// --- Função principal ---
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    srand(time(NULL)); // Inicializa semente aleatória

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicialmente
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    do {
        printf("\n==============================\n");
        exibirEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça (mover da fila para pilha)\n");
        printf("3 - Usar peça reservada (pop da pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("\n>> Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    // Sempre mantém a fila cheia
                    enqueue(&fila, gerarPeca());
                } else {
                    printf("\nA fila está vazia!\n");
                }
                break;

            case 2: // Reservar peça
                if (filaVazia(&fila)) {
                    printf("\nA fila está vazia! Nada a reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\nA pilha de reserva está cheia!\n");
                } else {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf("\n>> Peça [%c %d] movida para a pilha de reserva.\n",
                           reservada.nome, reservada.id);
                    // Reposição automática na fila
                    enqueue(&fila, gerarPeca());
                }
                break;

            case 3: // Usar peça reservada
                if (!pilhaVazia(&pilha)) {
                    Peca usada = pop(&pilha);
                    printf("\n>> Peça reservada usada: [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("\nA pilha de reserva está vazia!\n");
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

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int filaCheia(Fila *fila) {
    return fila->qtd == TAM_FILA;
}

int filaVazia(Fila *fila) {
    return fila->qtd == 0;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

void enqueue(Fila *fila, Peca nova) {
    if (filaCheia(fila)) return;
    fila->fim = (fila->fim + 1) % TAM_FILA;
    fila->itens[fila->fim] = nova;
    fila->qtd++;
}

Peca dequeue(Fila *fila) {
    Peca removida = {'?', -1};
    if (filaVazia(fila)) return removida;
    removida = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAM_FILA;
    fila->qtd--;
    return removida;
}

void push(Pilha *pilha, Peca nova) {
    if (pilhaCheia(pilha)) return;
    pilha->topo++;
    pilha->itens[pilha->topo] = nova;
}

Peca pop(Pilha *pilha) {
    Peca removida = {'?', -1};
    if (pilhaVazia(pilha)) return removida;
    removida = pilha->itens[pilha->topo];
    pilha->topo--;
    return removida;
}

void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n=== Estado Atual ===\n");

    // Exibir fila
    printf("Fila de peças:\t");
    if (filaVazia(fila)) {
        printf("[vazia]");
    } else {
        for (int i = 0; i < fila->qtd; i++) {
            int index = (fila->inicio + i) % TAM_FILA;
            printf("[%c %d] ", fila->itens[index].nome, fila->itens[index].id);
        }
    }

    // Exibir pilha
    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("[vazia]");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
        }
    }
    printf("\n");
}

// --- Geração automática de peças ---
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorID++;
    return nova;
}
