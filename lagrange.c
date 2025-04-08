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

    // Limpamos o histórico de saídas do console para uma melhor visualização do resultado
    cleanConsoleOutput();

    // Função genérica utilizada para mostrar os pontos conhecidos com base no ponteiro da struct de pontos
    chartKnownPoints(ptrKnownPoints, knowPointsAmount);

    printf("\nGrau do polinômio: %d", *polynomialDegree);
    printf("\nQuantidade de pontos conhecidos: %d", knowPointsAmount);
    printf("\nValor de X alvo: %.2f", *xValue);

    // São alocados espaços de memória do tipo double para armazenar os resultados calculados
    allocDoubles(&polynomialsResults, *polynomialDegree);

    // Estrutura de repetição para executar o algoritmo de lagrange até que o usuário escolha sair
    int userInput = 0;
    do {
        // Função para calcular os polinômios de lagrange Li(k)
        calculateLagrangePolynomials(*xValue, polynomialDegree, ptrKnownPoints, polynomialsResults);
        for (int i = 0; i < knowPointsAmount; i++) {
            printf("\nValor de L_%d: %.2lf", i, polynomialsResults[i]);
        }
        // Calcula o resultado com base nos polinômios de lagrange calculados anteriormente
        // Utiliza-se: p(x) = [Li(x0) * f(x0)] + [Li(x1) * f(x1)]
        // Onde ptrKnownPoints serve para pegar os valores de f(x) e polynonialsResults para Li(x)
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

    // Função para liberar todos os espaços de memória utilizados no programa via ponteiros
    // Boa prática para evitar memory-leaks
    freeAll(polynomialDegree, xValue, polynomialsResults, ptrKnownPoints);

    exit(EXIT_SUCCESS);
}

// Função que utiliza diferentes comandos para limpar o registro do console com base no tipo de S.O
void cleanConsoleOutput() {
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

// Função utilizada para armazenar em um ponteiro os valores dos pontos conhecidos
// Com base na struct KnownPoint são necessários dois valores para cada variável
// Onde x representa x_i e y representa f(x_i)
void storeKnownPoints(KnownPoint *ptr, const int knownPointsAmount) {
    for (int i = 0; i < knownPointsAmount; i++) {
        // Percorremos o loop fazendo uma leitura de entrada para cada termo dos pontos
        printf("\nDigite o valor de X_%d: ", i);
        scanf("%lf", &(ptr+i)->x);
        printf("\nDigite o valor de f(x_%d): ", i);
        scanf("%lf", &(ptr+i)->y);
    }
}

// Função utilizada para formatar e mostrar na tela a tabela de pontos conhecidos
// Utiliza-se formatação avançada de strings para alinhar os valores de forma simétrica
void chartKnownPoints(const KnownPoint *ptr, const int knownPointsAmount) {
    printf("\n");
    // %-2 indica que devem ser utilizados 2 espaços alinhando na esquerda o valor
    // %-8 indica que devem ser utilizados 8 espaços alinhando na esquerda o valor
    printf("| %-2s | %-8s | %-8s |\n", "i", "x", "f(x)");
    printf("+----+----------+----------+\n");
    for (int i = 0; i < knownPointsAmount; i++) {
        printf("| %-2d | %-8.5lf | %-8.5lf |", i, (ptr+i)->x, (ptr+i)->y);
        printf("\n");
    }
}

// Função utilizada para calcular os polinômios de lagrange com base no valor de X
// Além de utilizar os pontos conhecidos da tabela
void calculateLagrangePolynomials(
    const double xValue,
    const int *polynomialDegree,
    const KnownPoint *ptrKnowPoints,
    double *polynomialsResults
) {
    // Para cada valor do grau num loop entre 0 e o grau precisamos calular um Li
    for (int i=0 ; i<=*polynomialDegree; i++) {
        double Li = 1;
        // Para cada valor num loop entre 0 e grau precisamos somar o resultado do polinômio equivalente
        // Isso significa que para cada valor no loop cria-se um termo da multiplicação e um da divisão do polinômio Li
        // Para cada iteração desse loop são multiplicados com o valor final as estruturas:
        // (x - x_j) / (x_i - x_j)
        for (int j=0; j<=*polynomialDegree; j++) {
            if (i!=j) {
                Li *= (xValue - (ptrKnowPoints+j)->x) / ((ptrKnowPoints+i)->x - (ptrKnowPoints+j)->x);
            }
        }
        *(polynomialsResults+i) = Li;
    }
}

// Agora, com os valores calculados de Li(x) e já sabendo o valor de cada f(x) para x podemos calcular o resultado
// Para isso aplicamos a estrutura p(x) = [Li(x0) * f(x0)] + [Li(x1) * f(x1)] até o valor do grau do polinômio
double calculateResult(
    const int *polynomialDegree,
    const KnownPoint *ptrKnowPoints,
    const double *polynomialsResults
) {
    double lagrangeResultForX = 0;
    // Para cada valor do loop entre 0 e o grau do polinômio interpolador soma-se o valor da multiplicação Li*f(x)
    for (int i=0; i<=*polynomialDegree; i++) {
        const double Li = *(polynomialsResults+i);
        const double y = (ptrKnowPoints+i)->y;
        lagrangeResultForX += Li*y;
    }
    return lagrangeResultForX;
}

// Função para liberar os espaços de memória utilizados
void freeAll(int *degree, double *xValue, double *results, KnownPoint *points) {
    free(degree);
    free(xValue);
    free(results);
    free(points);
}