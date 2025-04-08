// Algoritmo de Lagrange implementado
//
// Pedro Salviano Santos - 236586
// Luiz Gustavo Motta Viana - 236428
// Erick Ferreira Ribeiro - 237046

// Esse código implementa o algoritmo estatístico da "Lagrange"
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
} KnownPoint;

// Definição dos protótipos de funções utilizadas dentro da main
// A necessidade da declaração a aqui se dá para evitar que seja necessário definir as funções completas antes da main
void allocKnownPoints(KnownPoint **ptr, int size);
void allocInts(int **ptr, int size);
void allocDoubles(double **ptr, int size);
void storeDegree(int *ptr);
void storeXValue(double *ptr);
void storeKnownPoints(KnownPoint *ptr, int knownPointsAmount);
void chartKnownPoints(const KnownPoint *ptr, int knownPointsAmount);
void cleanConsoleOutput();
void calculateLagrangePolynomials(
    double xValue,
    const int *polynomialDegree,
    const KnownPoint *ptrKnowPoints,
    double *polynomialsResults
);
double calculateResult(
    const int *polynomialDegree,
    const KnownPoint *ptrKnowPoints,
    const double *polynomialsResults
);
void freeAll(int *degree, double *xValue, double *results, KnownPoint *points);

int main(void) {
    KnownPoint *ptrKnownPoints = NULL;
    int *polynomialDegree = NULL;
    int knowPointsAmount = 0;
    double *xValue = NULL;
    double *polynomialsResults = NULL;

    // O primeiro passo que é executado aqui é a determinação do grau do polinômio que deve ser calculado
    // A primeira função aloca o espaço de memória que será utilizado pelo ponteiro
    allocInts(&polynomialDegree, 1);
    // Armazena no ponteiro e na variável o valor do grau do polinômio e quantidade de pontos
    storeDegree(polynomialDegree);
    knowPointsAmount = *polynomialDegree + 1;

    // O segundo passo é armazenar em um ponteiro (xValue) o valor de X que se quer obter através do polinômio
    allocDoubles(&xValue, knowPointsAmount);
    // Armazena no ponteiro o último valor de X que o usuário digitou
    storeXValue(xValue);

    // Para o terceiro passo é necessário alocar e armazenar em um ponteiro os valores conhecidos/tabelados
    // Para isso utilizamos um struct e um ponteiro dessa struct
    allocKnownPoints(&ptrKnownPoints, knowPointsAmount);
    // Armazena no ponteiro os valores tabelados
    storeKnownPoints(ptrKnownPoints, knowPointsAmount);

    cleanConsoleOutput();

    chartKnownPoints(ptrKnownPoints, knowPointsAmount);

    printf("\nGrau do polinômio: %d", *polynomialDegree);
    printf("\nQuantidade de pontos conhecidos: %d", knowPointsAmount);
    printf("\nValor de X alvo: %.2f", *xValue);

    allocDoubles(&polynomialsResults, *polynomialDegree);

    int userInput = 0;
    do {
        calculateLagrangePolynomials(*xValue, polynomialDegree, ptrKnownPoints, polynomialsResults);
        for (int i = 0; i < knowPointsAmount; i++) {
            printf("\nValor de L_%d: %.2lf", i, polynomialsResults[i]);
        }
        const double result = calculateResult(polynomialDegree, ptrKnownPoints, polynomialsResults);
        printf("\n\nValor de f(x) para x = %.5lf: %.5lf", *xValue, result);
        printf("\n\nDeseja continuar?");
        printf("\n1. Sim");
        printf("\n2. Nao");
        printf("\n->");
        scanf("%d", &userInput);
        if (userInput == 1) {
            printf("\nDigite o novo valor de x para o qual quer calcular f(x): ");
            scanf("%lf", xValue);
        }
    }while (userInput != 2);

    freeAll(polynomialDegree, xValue, polynomialsResults, ptrKnownPoints);

    exit(EXIT_SUCCESS);
}

void cleanConsoleOutput() {
    // Limpa a tela
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
}

// Função para alocar espaços de memória para o tipo das struct knowPoints
void allocKnownPoints(KnownPoint **ptr, const int size) {
    if ((*ptr = (KnownPoint*) realloc(*ptr, size*sizeof(KnownPoint)))==NULL) {
        printf("\nErro de alocacao.");
        exit(ALLOCATION_ERROR);
    }
}

// Função genérica para alocar espaços de memória para o tipo int
void allocInts(int **ptr, const int size) {
    if ((*ptr = (int*) realloc(*ptr, size*sizeof(int)))==NULL) {
        printf("\nErro de alocacao.");
        exit(ALLOCATION_ERROR);
    }
}

// Função genérica para alocar espaços de memória para o tipo double
void allocDoubles(double **ptr, const int size) {
    if ((*ptr = (double*) realloc(*ptr, size*sizeof(double)))==NULL) {
        printf("\nErro de alocacao.");
        exit(ALLOCATION_ERROR);
    }
}

// Função utilizada para armazenar em um ponteiro um valor que indica o grau do polinômio que será calculado
void storeDegree(int *ptr) {
    printf("\nDigite o grau do polinomio: ");
    scanf("%d", ptr);
}

// Função utilizada para armazenar em um ponteiro o valor que indica para qual X deseja-se obter f(X) pelo polinomio
void storeXValue(double *ptr) {
    printf("\nDigite o valor de X desconhecido que se quer obter com o polinomio: ");
    scanf("%lf", ptr);
}

void storeKnownPoints(KnownPoint *ptr, const int knownPointsAmount) {
    for (int i = 0; i < knownPointsAmount; i++) {
        printf("\nDigite o valor de X_%d: ", i);
        scanf("%lf", &(ptr+i)->x);
        printf("\nDigite o valor de f(x_%d): ", i);
        scanf("%lf", &(ptr+i)->y);
    }
}

void chartKnownPoints(const KnownPoint *ptr, const int knownPointsAmount) {
    printf("\n");
    printf("| %-2s | %-8s | %-8s |\n", "i", "x", "f(x)");
    printf("+----+----------+----------+\n");
    for (int i = 0; i < knownPointsAmount; i++) {
        printf("| %-2d | %-8.5lf | %-8.5lf |", i, (ptr+i)->x, (ptr+i)->y);
        printf("\n");
    }
}

void calculateLagrangePolynomials(
    const double xValue,
    const int *polynomialDegree,
    const KnownPoint *ptrKnowPoints,
    double *polynomialsResults
) {
    for (int i=0 ; i<=*polynomialDegree; i++) {
        double Li = 1;
        for (int j=0; j<=*polynomialDegree; j++) {
            if (i!=j) {
                Li *= (xValue - (ptrKnowPoints+j)->x) / ((ptrKnowPoints+i)->x - (ptrKnowPoints+j)->x);
            }
        }
        *(polynomialsResults+i) = Li;
    }
}

double calculateResult(
    const int *polynomialDegree,
    const KnownPoint *ptrKnowPoints,
    const double *polynomialsResults
) {
    double lagrangeResultForX = 0;
    for (int i=0; i<=*polynomialDegree; i++) {
        const double Li = *(polynomialsResults+i);
        const double y = (ptrKnowPoints+i)->y;
        lagrangeResultForX += Li*y;
    }
    return lagrangeResultForX;
}

void freeAll(int *degree, double *xValue, double *results, KnownPoint *points) {
    free(degree);
    free(xValue);
    free(results);
    free(points);
}