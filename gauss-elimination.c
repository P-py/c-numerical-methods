// Algoritmo de Eliminação de Gauss implementado
//
// Pedro Salviano Santos - 236586
// Luiz Gustavo Motta Viana - 236428
// Erick Ferreira Ribeiro - 237046

// Esse código implementa o algoritmo estatístico da "Eliminação de Gauss"
// Foram utilizados ponteiros e alocação dinâmica de memória
// Busca-se utilizar boas práticas de memória e clang-tidy

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Biblioteca para as funções matemáticas

// Códigos de saída para a função exit()
// 0 > sucesso
// 1 > erro genérico
// 2 > argumento inválido
// 3 > erro de alocação de memória
#define EXIT_SUCCESS 0
#define UNDEFINED_ERROR 1
#define INVALID_ARGUMENT_ERROR 2
#define ALLOCATION_ERROR 3

int getNvalue();
void allocMatrix(double ***matrix, int n);
void printEquationSystem(double **matrix, int n);
void printMatrix(double **matrix, int n);
void readUserInput(double **matrix, int n);
void gaussElimination(double **matrix, int n);

int main(void) {
    const int n = getNvalue();

    double **matrix = NULL;
    allocMatrix(&matrix, n);
    readUserInput(matrix, n);

    printf("\nMatriz aumentada:\n\n");
    printMatrix(matrix, n);

    gaussElimination(matrix, n);
    printf("\nMatriz depois da eliminacao de gauss:\n");
    printMatrix(matrix, n);

    return EXIT_SUCCESS;
}

int getNvalue() {
    int n;
    do {
        printf("Digite o valor de N (ordem do sistema linear NxN): ");
        scanf("%d", &n);
    } while (n <= 0);
    return n;
}

void allocMatrix(double ***matrix, const int n) {
    *matrix = (double **) realloc(*matrix, n * sizeof(double *));
    if (*matrix == NULL) {
        printf("Erro ao alocar linhas da matriz.\n");
        exit(ALLOCATION_ERROR);
    }

    for (int i = 0; i < n; i++) {
        (*matrix)[i] = (double *) calloc(n+1, sizeof(double));
        if ((*matrix)[i] == NULL) {
            printf("Erro ao alocar colunas da matriz.\n");
            exit(ALLOCATION_ERROR);
        }
    }
}

void printEquationSystem(double **matrix, const int n) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("{ ");
        for (int j = 0; j < n; j++) {
            printf("(%6.3lf * x%d)", matrix[i][j], j + 1);
            if (j < n - 1) {
                printf(" + ");
            }
        }
        printf(" = %6.3lf", matrix[i][n]); // termo independente
        printf(" }\n");
    }
}

void printMatrix(double **matrix, const int n) {
    for (int i = 0; i < n; i++) {
        printf("| ");
        for (int j = 0; j < n; j++) {
            printf("%6.3lf ", matrix[i][j]);
        }
        printf("| %6.3lf |\n", matrix[i][n]);
    }
    printf("\n");
}

void readUserInput(double **matrix, const int n) {
    printf("\n### Digite os coeficientes das equações do sistema linear ###\n");

    for (int i = 0; i < n; i++) {
        printf("\nEquação %d:\n", i + 1);

        for (int j = 0; j < n + 1; j++) {
            char label[50];
            printEquationSystem(matrix, n);

            // Usa ternário com snprintf para gerar a string label
            j == n ? snprintf(label, sizeof(label), "b_%d", i + 1)
                     : snprintf(label, sizeof(label), "Coeficiente de x_%d", j + 1);

            printf("\n  %s: ", label);
            scanf("%lf", &matrix[i][j]);
        }
    }
}

void gaussElimination(double **matrix, const int n) {
    for (int k = 1; k < n; k++) {
        printf("\n\n### Passo %d da eliminação de gauss ###\n", k);
        const int superiorLimit = n;
        const int inferiorLimit = k + 1;
        // — 1 devido à indexação de vetores/matrizes começar em 0
        const double pivotElement = matrix[k-1][k-1];
        printf(
            "\nk = %d | i >= %d | i <= %d | pivô (a_kk) = (a_%d%d) = %lf\n",
            k,
            inferiorLimit,
            superiorLimit,
            k,
            k,
            pivotElement
        );
        // A variável 'i' nesse caso indica a linha que está sendo acessada
        for (int i = inferiorLimit; i <= superiorLimit; i++) {
            // — 1 devido à indexação de vetores/matrizes começar em 0
            const double lineMultiplier = matrix[i-1][k-1] / pivotElement;
            printf("\nm_%d%d = %.3lf\n", i, k, lineMultiplier);
            // A variável 'j' nesse caso indica a coluna que está sendo acessada
            // O +1 na variável 'j' representa a indicação de que temos um termo independente na matriz aumentada
            for (int j = k; j <= n + 1; j++) {
                const double oldValue = matrix[i-1][j-1];
                const double newValue = oldValue - lineMultiplier * matrix[k-1][j-1];

                // Usa ternário com snprintf para gerar a string label
                char label[64];
                j == n + 1 ? snprintf(label, sizeof(label), "b_%d = b_%d - (m_%d%d * b_%d) = ", i, i, i, k, k)
                         : snprintf(label, sizeof(label), "a_%d%d = a_%d%d - (m_%d%d * a_%d%d) = ", i, j, i, j, i, k, k, j);
                printf("%s", label);

                printf("%.3lf - (%.3lf * %.3lf) = %.3lf\n", oldValue, lineMultiplier, matrix[k-1][j-1], newValue);

                matrix[i-1][j-1] = newValue;
            }
        }
    }
}