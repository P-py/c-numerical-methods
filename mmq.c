// Algoritmo do "Método dos Mínimos Quadrados" implementado
//
// Pedro Salviano Santos - 236586
// Luiz Gustavo Motta Viana - 236428
// Erick Ferreira Ribeiro - 237046

// Esse código implementa o algoritmo estatístico da "Método dos Mínimos Quadrados"
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

// Estrutura criada para suportar o conjunto de pontos conhecidos da função
typedef struct {
    double x;
    double y;
} Point;

// Definição dos protótipos de funções utilizadas dentro da main
// A necessidade da declaração a aqui se dá para evitar que seja necessário definir as funções completas antes da main
int getPointsAmount();
void allocPoints(Point **ptr, int size);
void chartPoints(const Point *ptr, int knownPointsAmount);
void storePoints(Point *ptr, int knownPointsAmount);
void getUserInput(Point *ptr, int knownPointsAmount);
void cleanConsoleOutput();
void allocGaussMatrix(double ***matrix, int n);
void buildLinearSystem(const Point *points, double ***matrix, int numPoints, int degree);
void chartLinearSystem(double **matrix, int degree);
void applyGaussElimination(double **matrix, int n);
void solveGaussLinearSystem(double **matrix, double *resultsVector, int n);
void allocResultsVector(double **vector, int n);
void chartResultsVector(const double *resultsVector, int degree);
void chartPolynomial(const double *resultsVector, int degree);

int main(void) {
    int polynomialDegree;
    double **matrix = NULL;
    Point *ptrKnownPoints = NULL;
    double *resultsVector = NULL;
    const int pointsAmount = getPointsAmount();

    allocPoints(&ptrKnownPoints, pointsAmount);

    getUserInput(ptrKnownPoints, pointsAmount);
    cleanConsoleOutput();

    do {
        printf("\nQual o grau do polinômio para ajuste? (1 - Reta | 2 - Parabola): ");
        scanf("%d", &polynomialDegree);
    } while (polynomialDegree != 1 && polynomialDegree != 2);
    allocGaussMatrix(&matrix, polynomialDegree+1);
    buildLinearSystem(ptrKnownPoints, &matrix, pointsAmount, polynomialDegree);

    printf("\nMatriz que representa o sistema linear escalavel composto com somatorias: ");
    chartLinearSystem(matrix, polynomialDegree);

    applyGaussElimination(matrix, polynomialDegree+1);

    printf("\nMatriz depois da eliminacao de gauss: ");
    chartLinearSystem(matrix, polynomialDegree);

    allocResultsVector(&resultsVector, polynomialDegree+1);

    solveGaussLinearSystem(matrix, resultsVector, polynomialDegree);
    chartResultsVector(resultsVector, polynomialDegree+1);

    chartPolynomial(resultsVector, polynomialDegree);

    for (int i = 0; i <= polynomialDegree; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(ptrKnownPoints);
    free(resultsVector);

    return EXIT_SUCCESS;
}

int getPointsAmount() {
    int pointsAmount;

    do {
        printf("\nDigite a quantidade de pontos [x | f(x)]: ");
        scanf("%d", &pointsAmount);
    } while (pointsAmount < 0);

    return pointsAmount;
}

// Função para alocar espaços de memória para o tipo das struct knowPoints
void allocPoints(Point **ptr, const int size) {
    if ((*ptr = (Point*) realloc(*ptr, size*sizeof(Point)))==NULL) {
        printf("\nErro de alocacao.");
        exit(ALLOCATION_ERROR);
    }
}

// Função utilizada para formatar e mostrar na tela a tabela de pontos conhecidos
// Utiliza-se formatação avançada de ‘strings’ para alinhar os valores de forma simétrica
void chartPoints(const Point *ptr, const int knownPointsAmount) {
    printf("\n");

    // Primeira linha: valores de x
    printf("|   x   |");
    for (int i = 0; i < knownPointsAmount; i++) {
        // O formatador 6.2lf garante que a 'string' tenha no mínimo 6 espaços e seja exibida com 2 casas decimais
        printf(" %6.2lf |", (ptr + i)->x);
    }
    printf("\n");

    // Segunda linha: valores de f(x)
    printf("| f(x)  |");
    for (int i = 0; i < knownPointsAmount; i++) {
        printf(" %6.2lf |", (ptr + i)->y);
    }
    printf("\n");
}

// Função utilizada para armazenar em um ponteiro os valores dos pontos conhecidos
// Com base na struct KnownPoint são necessários dois valores para cada variável
// Onde x representa x_i e y representa f(x_i)
void storePoints(Point *ptr, const int knownPointsAmount)  {
    printf("\n### Digite os pontos conhecidos ###\n");
    chartPoints(ptr,  knownPointsAmount);

    for (int i = 0; i < knownPointsAmount; i++) {
        printf("\nx_%d: ", i+1);
        scanf("%lf", &(ptr+i)->x);
        chartPoints(ptr,  knownPointsAmount);
        printf("\nf_(x_%d): ", i+1);
        scanf("%lf", &(ptr+i)->y);
        chartPoints(ptr,  knownPointsAmount);
    }
}

// Função que utiliza diferentes comandos para limpar o registro do console com base no tipo de S.O
void cleanConsoleOutput() {
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
}

void getUserInput(Point *ptr, const int knownPointsAmount) {
    int userChoice;
    do {
        storePoints(ptr, knownPointsAmount);
        printf("\nPontos corretos?\n1 - Sim\n2 - Nao\n->");
        scanf("%d", &userChoice);
    } while (userChoice != 1);
}

void allocGaussMatrix(double ***matrix, const int n) {
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

void buildLinearSystem(const Point *points, double ***matrix, const int numPoints, const int degree) {
    const int matrixSize = degree + 1;

    // Inicializa somatórios
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize + 1; col++) {
            (*matrix)[row][col] = 0.0;
        }
    }

    // Preenche os valores da matriz conforme as equações
    for (int i = 0; i < numPoints; i++) {
        const double xi = points[i].x;
        const double yi = points[i].y;

        for (int row = 0; row <= degree; row++) {
            for (int col = 0; col <= degree; col++) {
                (*matrix)[row][col] += pow(xi, row + col);
            }
            (*matrix)[row][matrixSize] += yi * pow(xi, row); // Lado direito do sistema
        }
    }
}

void chartLinearSystem(double **matrix, const int degree) {
    const int matrixSize = degree + 1;

    printf("\n");

    for (int i = 0; i < matrixSize; i++) {
        printf("(");
        for (int j = 0; j < matrixSize; j++) {
            printf("%8.3lf", matrix[i][j]);
            if (j < matrixSize-1) printf(" ");
        }
        printf(") * (a%d) = (%8.3lf)\n", i, matrix[i][matrixSize]);
    }
}

void applyGaussElimination(double **matrix, const int n) {
    for (int k = 1; k < n; k++) {
        printf("\n\n### Passo %d da eliminação de gauss ###\n", k);

        const int superiorLimit = n;
        const int inferiorLimit = k + 1;
        // — 1 devido à indexação de vetores/matrizes começar em 0
        const double pivotElement = matrix[k-1][k-1];

        printf(
            "\nk = %d | i >= %d | i <= %d | pivô (a_kk) = (a_%d%d) = %lf\n",
            k, inferiorLimit, superiorLimit, k, k, pivotElement
        );

        // A variável 'i' nesse caso indica a linha que está sendo acessada
        for (int i = inferiorLimit; i <= superiorLimit; i++) {
            const double lineMultiplier = matrix[i-1][k-1] / pivotElement;
            printf("\nm_%d%d = %.3lf\n", i, k, lineMultiplier);

            // A variável 'j' nesse caso indica a coluna que está sendo acessada
            // O +1 na variável 'n' representa a indicação de que temos um termo independente na matriz aumentada
            for (int j = k; j <= n + 1; j++) {
                const double oldValue = matrix[i-1][j-1];
                const double newValue = oldValue - lineMultiplier * matrix[k-1][j-1];

                // Usa ternário com snprintf para gerar a string label
                char label[64];
                j == n + 1
                    ? snprintf(
                        label, sizeof(label),
                        "b_%d = b_%d - (m_%d%d * b_%d) = ", i, i, i, k, k
                    )
                    : snprintf(
                        label, sizeof(label),
                        "a_%d%d = a_%d%d - (m_%d%d * a_%d%d) = ", i, j, i, j, i, k, k, j
                    );

                printf("%s", label);

                printf(
                    "%.3lf - (%.3lf * %.3lf) = %.3lf\n",
                    oldValue, lineMultiplier, matrix[k-1][j-1], newValue
                );

                matrix[i-1][j-1] = newValue;
            }
        }
    }
}

void allocResultsVector(double **vector, const int n) {
    *vector = (double *) realloc(*vector, n * sizeof(double));
    if (*vector == NULL) {
        printf("Erro ao alocar o vetor de resultados X.\n");
        exit(ALLOCATION_ERROR);
    }
}

void solveGaussLinearSystem(double **matrix, double *resultsVector, const int n) {
    const int matrixSize = n + 1;

    for (int i = matrixSize - 1; i >= 0; i--) {
        // Inicia-se igualando o valor do vetor X para o termo independente
        resultsVector[i] = matrix[i][matrixSize];

        // Para cada valor das equações, subtrai aqueles cujos termos independentes
        // já foram resolvidos
        for (int j = i+1; j < matrixSize; j++) {
            resultsVector[i] -= matrix[i][j] * resultsVector[j];
        }

        // Divide pelo coeficiente da incógnita atual
        resultsVector[i] = resultsVector[i] / matrix[i][i];
    }
}

void chartResultsVector(const double *resultsVector, const int degree) {
    for (int i = 0; i < degree; i++) {
        printf("\n( a_%d ) = ( %6.3lf )", i, resultsVector[i]);
    }
}

void chartPolynomial(const double *resultsVector, const int degree) {
    printf("\n\nP(X) = ");
    for (int i = 0; i < degree + 1; i++) {
        char label[50];
        i == 0
            ? snprintf(label, sizeof(label), "%.3lf", resultsVector[i])
            : resultsVector[i] >= 0
                ? snprintf(label, sizeof(label), " + %.3lf * x^%d", resultsVector[i], i)
                : snprintf(label, sizeof(label), " - %.3lf * x^%d", -resultsVector[i], i);

        printf("%s", label);
    }
    printf("\n");
}