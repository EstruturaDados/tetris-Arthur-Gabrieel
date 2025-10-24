#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5     // Capacidade da fila circular
#define TAM_PILHA 3    // Capacidade da pilha

// --- Estrutura da Peça ---
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// --- Estrutura da Fila Circular ---
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

// --- Estrutura da Pilha ---
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// --- Protótipos ---
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
void trocarPeçaAtual(Fila *fila, Pilha *pilha);
void trocarMultiplas(Fila *fila, Pilha *pilha);

// --- Variável global para IDs únicos ---
int contadorID = 0;

// --- Função principal ---
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicialmente
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    do {
        printf("\n==============================\n");
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("\n>> Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca()); // Mantém a fila cheia
                } else {
                    printf("\nFila vazia! Nenhuma peça para jogar.\n");
                }
                break;
            }

            case 2: { // Reservar peça
                if (filaVazia(&fila)) {
                    printf("\nFila vazia! Não há peça para reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\nPilha cheia! Não é possível reservar mais peças.\n");
                } else {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf("\n>> Peça [%c %d] movida para a pilha de reserva.\n",
                           reservada.nome, reservada.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }

            case 3: { // Usar peça da pilha
                if (!pilhaVazia(&pilha)) {
                    Peca usada = pop(&pilha);
                    printf("\n>> Peça reservada usada: [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("\nPilha de reserva está vazia!\n");
                }
                break;
            }

            case 4: // Trocar peça atual
                trocarPeçaAtual(&fila, &pilha);
                break;

            case 5: // Troca múltipla
                trocarMultiplas(&fila, &pilha);
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

// --- Inicializações ---
void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = -1;
    fila->qtd = 0;
}

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// --- Verificações ---
int filaCheia(Fila *fila) { return fila->qtd == TAM_FILA; }
int filaVazia(Fila *fila) { return fila->qtd == 0; }
int pilhaCheia(Pilha *pilha) { return pilha->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *pilha) { return pilha->topo == -1; }

// --- Operações da Fila ---
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

// --- Operações da Pilha ---
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

// --- Exibição do estado atual ---
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n=== Estado Atual ===\n");

    printf("Fila de peças:\t");
    if (filaVazia(fila)) {
        printf("[vazia]");
    } else {
        for (int i = 0; i < fila->qtd; i++) {
            int idx = (fila->inicio + i) % TAM_FILA;
            printf("[%c %d] ", fila->itens[idx].nome, fila->itens[idx].id);
        }
    }

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

// --- Troca de peça atual (frente da fila com topo da pilha) ---
void trocarPeçaAtual(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("\nNão é possível realizar a troca. Fila ou pilha vazia.\n");
        return;
    }

    int idxFrente = fila->inicio;
    Peca temp = fila->itens[idxFrente];
    fila->itens[idxFrente] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;

    printf("\n>> Troca realizada entre [%c %d] e [%c %d].\n",
           fila->itens[idxFrente].nome, fila->itens[idxFrente].id,
           pilha->itens[pilha->topo].nome, pilha->itens[pilha->topo].id);
}

// --- Troca múltipla (3 primeiras da fila com 3 da pilha) ---
void trocarMultiplas(Fila *fila, Pilha *pilha) {
    if (fila->qtd < 3 || pilha->topo < 2) {
        printf("\nNão é possível trocar múltiplas. Ambas precisam ter ao menos 3 peças.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (fila->inicio + i) % TAM_FILA;
        Peca temp = fila->itens[idxFila];
        fila->itens[idxFila] = pilha->itens[pilha->topo - i];
        pilha->itens[pilha->topo - i] = temp;
    }

    printf("\n>> Troca em bloco realizada entre as 3 primeiras peças da fila e as 3 da pilha.\n");
}
