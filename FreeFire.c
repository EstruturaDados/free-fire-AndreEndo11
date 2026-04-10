#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Biblioteca necessária para a função de strung (strcpy, strcmp)

// Define uma constante para o número máximo de itens que nossa lista pode conter.
#define MAX_ITENS 10

// --- DEFINIÇÃO DA STRUCT ---
// Agregador de dados para representar um item do jogo
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// --- PROTÓTIPOS DAS FUNÇÕES ---
void inserirItem(Item mochila[], int *total);
void removerItem(Item mochila[], int *total);
void listarItens(Item mochila[], int total);
void buscarItem(Item mochila[], int total);
void limparBuffer();

int main() {
    Item mochila[MAX_ITENS];
    int totalItens = 0;
    int opcao;

    do {
        printf("\n================================================\n");
        printf("  MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
        printf("================================================\n");
        printf("Itens na Mochila: %d/%d\n\n", totalItens, MAX_ITENS);
        printf("1. Adicionar Item (Loot)\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens na Mochila\n");
        printf("4. Buscar Item\n");
        printf("0. Sair\n");
        printf("-------------------------------------------------\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                inserirItem(mochila, &totalItens);
                listarItens(mochila, totalItens);
                break;
            case 2:
                removerItem(mochila, &totalItens);
                listarItens(mochila, totalItens);
                break;
            case 3:
                listarItens(mochila, totalItens);
                break;
            case 4:
                buscarItem(mochila, totalItens);
                break;
            case 0:
                printf("Saindo do jogo... Boa sorte na ilha!\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

// --- CADASTRO DE ITENS ---
void inserirItem(Item mochila[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("\nMochila cheia! Voce precisa descartar algo primeiro.\n");
        return;
    }

    printf("\n--- Adicionar Novo Item ---\n");
    printf("Nome do item: ");
    fgets(mochila[*total].nome, 30, stdin);
    mochila[*total].nome[strcspn(mochila[*total].nome, "\n")] = 0;

    printf("Tipo do item (arma, municao, cura, etc.): ");
    fgets(mochila[*total].tipo, 20, stdin);
    mochila[*total].tipo[strcspn(mochila[*total].tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &mochila[*total].quantidade);
    limparBuffer();

    (*total)++;
    printf("Item adicionado com sucesso!\n");
}

// --- REMOÇÃO DE ITENS ---
void removerItem(Item mochila[], int *total) {
    if (*total == 0) {
        printf("\nMochila vazia!\n");
        return;
    }

    char busca[30];
    printf("\nNome do item para remover: ");
    fgets(busca, 30, stdin);
    busca[strcspn(busca, "\n")] = 0;

    int encontrado = -1;
    for (int i = 0; i < *total; i++) {
        if (strcmp(mochila[i].nome, busca) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        // Desloca os itens para preencher o buraco (Manter lista sequencial)
        for (int i = encontrado; i < (*total) - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        (*total)--;
        printf("Item '%s' removido!\n", busca);
    } else {
        printf("Item nao encontrado na mochila.\n");
    }
}

// --- LISTAGEM DOS ITENS ---
void listarItens(Item mochila[], int total) {
    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", total, MAX_ITENS);
    printf("--------------------------------------------------\n");
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QTD");
    printf("--------------------------------------------------\n");

    if (total == 0) {
        printf("Mochila vazia.\n");
    } else {
        for (int i = 0; i < total; i++) {
            printf("%-20s | %-15s | %d\n", 
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
        }
    }
    printf("--------------------------------------------------\n");
    printf("Pressione Enter para continuar...");
    getchar();
}

// --- BUSCA SEQUENCIAL ---
void buscarItem(Item mochila[], int total) {
    char busca[30];
    printf("\nDigite o nome do item que procura: ");
    fgets(busca, 30, stdin);
    busca[strcspn(busca, "\n")] = 0;

    for (int i = 0; i < total; i++) {
        if (strcmp(mochila[i].nome, busca) == 0) {
            printf("\nItem Encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n", 
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }
    printf("\nItem '%s' nao esta na mochila.\n", busca);
}

// Auxiliar para limpar o \n deixado pelo scanf
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
