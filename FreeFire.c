#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMP 20

// 1. Definição da Struct
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// Protótipos das funções
void limparBuffer();
void cadastrarComponentes(Componente comps[], int *total);
void mostrarComponentes(Componente comps[], int total);
int bubbleSortNome(Componente comps[], int n);
int insertionSortTipo(Componente comps[], int n);
int selectionSortPrioridade(Componente comps[], int n);
int buscaBinariaNome(Componente comps[], int n, char alvo[], int *comparacoes);

int main() {
    Componente torre[MAX_COMP];
    int total = 0;
    int opcao, comps_algoritmo, pos;
    char busca[30];
    clock_t inicio, fim;
    double tempo_cpu;
    int ordenadoPorNome = 0;

    do {
        printf("\n================================================\n");
        printf("    #########   JOGO FREE FIRE   ######## \n");
        printf("================================================\n");
        printf("Itens na Mochila: %d/%d\n", total, MAX_COMP);
        printf("1. Adicionar Itens\n");
        printf("2. Ordenar por Nome (Bubble Sort)\n");
        printf("3. Ordenar por Tipo (Insertion Sort)\n");
        printf("4. Ordenar por Prioridade (Selection Sort)\n");
        printf("5. Buscar Componente-Chave (Busca Binaria)\n");
        printf("6. Listar Componentes\n");
        printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarComponentes(torre, &total);
                ordenadoPorNome = 0; // qualquer alteração de cadastro quebra ordenação por nome
                break;
            case 2:
                inicio = clock();
                comps_algoritmo = bubbleSortNome(torre, total);
                fim = clock();
                tempo_cpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
                printf("\nOrdenado por NOME via Bubble Sort.\nComparacoes: %d | Tempo: %f s\n", comps_algoritmo, tempo_cpu);
                ordenadoPorNome = 1;
                mostrarComponentes(torre, total);
                break;
            case 3:
                inicio = clock();
                comps_algoritmo = insertionSortTipo(torre, total);
                fim = clock();
                tempo_cpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
                printf("\nOrdenado por TIPO via Insertion Sort.\nComparacoes: %d | Tempo: %f s\n", comps_algoritmo, tempo_cpu);
                ordenadoPorNome = 0;
                mostrarComponentes(torre, total);
                break;
            case 4:
                inicio = clock();
                comps_algoritmo = selectionSortPrioridade(torre, total);
                fim = clock();
                tempo_cpu = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
                printf("\nOrdenado por PRIORIDADE via Selection Sort.\nComparacoes: %d | Tempo: %f s\n", comps_algoritmo, tempo_cpu);
                ordenadoPorNome = 0;
                mostrarComponentes(torre, total);
                break;
            case 5:
                if (!ordenadoPorNome) {
                    printf("\n[AVISO] A busca binaria requer ordenacao previa por NOME (Opcao 2).\n");
                } else {
                    printf("Digite o nome do componente-chave: ");
                    fgets(busca, sizeof(busca), stdin);
                    busca[strcspn(busca, "\n")] = 0;
                    
                    int comp_busca = 0; // inicializa contagem de comparações antes do uso
                    pos = buscaBinariaNome(torre, total, busca, &comp_busca);
                    
                    if (pos != -1) {
                        printf("\nITEM ENCONTRADO (Posicao %d)!\n", pos + 1);
                        printf("Nome: %s | Prioridade: %d\n", torre[pos].nome, torre[pos].prioridade);
                    } else {
                        printf("\nComponente nao localizado.\n");
                    }
                    printf("Comparacoes na busca: %d\n", comp_busca);
                }
                break;
            case 6:
                mostrarComponentes(torre, total);
                break;
            case 0:
                // sair, nada a fazer aqui
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

// Limpa o buffer de entrada até '\n' ou EOF (evitar lixo ao usar fgets após scanf)
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Cadastra componentes com validações básicas de entrada
void cadastrarComponentes(Componente comps[], int *total) {
    int qtd;
    int disponiveis = MAX_COMP - *total;
    printf("Quantos componentes deseja cadastrar (Max %d): ", disponiveis);

    // valida leitura de inteiro
    if (scanf("%d", &qtd) != 1) {
        limparBuffer();
        printf("Entrada invalida.\n");
        return;
    }
    limparBuffer();

    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }
    if (qtd > disponiveis) qtd = disponiveis; // limita ao espaço disponível

    for (int i = 0; i < qtd && *total < MAX_COMP; i++) {
        printf("\nComponente #%d\n", *total + 1);

        // Leitura de nome com fgets: garante não exceder buffer e remove '\n'
        printf("Nome: ");
        if (fgets(comps[*total].nome, sizeof(comps[*total].nome), stdin) == NULL) {
            comps[*total].nome[0] = '\0';
        } else {
            comps[*total].nome[strcspn(comps[*total].nome, "\n")] = 0;
        }

        // Leitura de tipo
        printf("Tipo: ");
        if (fgets(comps[*total].tipo, sizeof(comps[*total].tipo), stdin) == NULL) {
            comps[*total].tipo[0] = '\0';
        } else {
            comps[*total].tipo[strcspn(comps[*total].tipo, "\n")] = 0;
        }

        // Validação da prioridade numérica
        printf("Prioridade (1-10): ");
        if (scanf("%d", &comps[*total].prioridade) != 1) {
            limparBuffer();
            printf("Prioridade invalida. Usando 1.\n");
            comps[*total].prioridade = 1;
        } else {
            // Ajuste se fora do intervalo esperado
            if (comps[*total].prioridade < 1 || comps[*total].prioridade > 10) {
                printf("Prioridade fora do intervalo (1-10). Ajustando para 1.\n");
                comps[*total].prioridade = 1;
            }
            limparBuffer();
        }

        (*total)++;
    }
}

// Mostra todos os componentes cadastrados em formato tabular simples
void mostrarComponentes(Componente comps[], int total) {
    printf("\n%-20s | %-15s | %s\n", "NOME", "TIPO", "PRIORIDADE");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < total; i++) {
        printf("%-20s | %-15s | %d\n", comps[i].nome, comps[i].tipo, comps[i].prioridade);
    }
}

// BUBBLE SORT: Ordenar por Nome
// Retorna número de comparações realizadas.
// Se n < 2, retorna 0 (nenhuma comparação necessária).
int bubbleSortNome(Componente comps[], int n) {
    if (n < 2) return 0;
    int i, j, comparacoes = 0;
    Componente temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            comparacoes++;
            if (strcmp(comps[j].nome, comps[j + 1].nome) > 0) {
                temp = comps[j];
                comps[j] = comps[j + 1];
                comps[j + 1] = temp;
            }
        }
    }
    return comparacoes;
}

// INSERTION SORT: Ordenar por Tipo
// Retorna número de comparações realizadas.
// Usa incremento de comparacoes dentro do while.
int insertionSortTipo(Componente comps[], int n) {
    if (n < 2) return 0;
    int i, j, comparacoes = 0;
    Componente chave;
    for (i = 1; i < n; i++) {
        chave = comps[i];
        j = i - 1;
        // incrementa comparacoes a cada comparação de strings
        while (j >= 0 && (comparacoes++, strcmp(comps[j].tipo, chave.tipo) > 0)) {
            comps[j + 1] = comps[j];
            j--;
        }
        comps[j + 1] = chave;
    }
    return comparacoes;
}

// SELECTION SORT: Ordenar por Prioridade
// Retorna número de comparações realizadas.
int selectionSortPrioridade(Componente comps[], int n) {
    if (n < 2) return 0;
    int i, j, min_idx, comparacoes = 0;
    Componente temp;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            comparacoes++;
            if (comps[j].prioridade < comps[min_idx].prioridade)
                min_idx = j;
        }
        // troca elemento mínimo com posição i
        temp = comps[min_idx];
        comps[min_idx] = comps[i];
        comps[i] = temp;
    }
    return comparacoes;
}

// BUSCA BINÁRIA: Por Nome
// Recebe ponteiro comparacoes; inicializa *comparacoes = 0 para segurança.
// Retorna índice do elemento ou -1 se não encontrado.
// Pré-condição: array deve estar ordenado por nome.
int buscaBinariaNome(Componente comps[], int n, char alvo[], int *comparacoes) {
    *comparacoes = 0; // inicializa para evitar contagens lixo
    int esq = 0, dir = n - 1;
    while (esq <= dir) {
        (*comparacoes)++;
        int meio = esq + (dir - esq) / 2;
        int res = strcmp(comps[meio].nome, alvo);

        if (res == 0) return meio;
        if (res < 0) esq = meio + 1;
        else dir = meio - 1;
    }
    return -1;
}
