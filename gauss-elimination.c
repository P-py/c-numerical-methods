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

int main(void) {
    const int n = getNvalue();

    double **matrix = NULL;
    allocMatrix(&matrix, n);
    readUserInput(matrix, n);

    printf("\nMatriz aumentada:\n\n");
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
            printf("(%6.2lf * x%d)", matrix[i][j], j + 1);
            if (j < n - 1) {
                printf(" + ");
            }
        }
        printf(" = %6.2lf", matrix[i][n]); // termo independente
        printf(" }\n");
    }
}

void printMatrix(double **matrix, const int n) {
    for (int i = 0; i < n; i++) {
        printf("| ");
        for (int j = 0; j < n; j++) {
            printf("%6.2lf ", matrix[i][j]);
        }
        printf("| %6.2lf |\n", matrix[i][n]);
    }
    printf("\n");
}

void readUserInput(double **matrix, const int n) {
    printf("\n### Digite os coeficientes das equações do sistema linear ###\n");

    for (int i = 0; i < n; i++) {
        printf("\nEquação %d:\n", i + 1);

        for (int j = 0; j < n+1; j++) {
            printEquationSystem(matrix, n);
            // Exibe a descrição de maneira mais compacta
            printf("\n  %s%d: ", (j == n - 1) ? "b_" : "Coeficiente de x_", j + 1);
            scanf("%lf", &matrix[i][j]);
        }
    }
}

void gaussElimination(double **matrix, const int n) {
    for (int k = 1; k < n; k++) {
        printf("\nPasso %d da eliminação de gauss...", k);
        const int superiorLimit = n;
        const int inferiorLimit = k + 1;
        const double pivotElement = matrix[k-1][k-1];
        printf(
            "\ni >= %d | i <= %d | pivô (a_kk) = (a_%d%d) = %lf",
            inferiorLimit,
            superiorLimit,
            k,
            k,
            pivotElement
        );
        for (int i = inferiorLimit; i <= superiorLimit; i++) {
            const double lineMultiplier = matrix[i][k] / pivotElement;
            for (int j = k; j <= n; j++) {
                matrix[k][j] = matrix[k][j] - lineMultiplier * pivotElement;
            }
        }
    }
}