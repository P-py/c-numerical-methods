// Algoritmo de "Regra dos Trapézios" implementado
//
// Pedro Salviano Santos - 236586
// Luiz Gustavo Motta Viana - 236428
// Erick Ferreira Ribeiro - 237046

// Esse código implementa o algoritmo estatístico da "Regra dos trapézios"
// Foram utilizados ponteiros e alocação dinâmica de memória
// Busca-se utilizar boas práticas de memória e clang-tidy

#include <stdio.h> // Entrada e saída padrão
#include <stdlib.h> // Para alocação de memória dinâmica, exit(), etc.
#include <math.h> // Biblioteca para as funções matemáticas como pow()

// Códigos de saída para a função exit()
// 0 > sucesso
// 1 > erro genérico
// 2 > argumento inválido
// 3 > erro de alocação de memória
#define EXIT_SUCCESS 0
#define UNDEFINED_ERROR 1
#define INVALID_ARGUMENT_ERROR 2
#define ALLOCATION_ERROR 3

// Constantes auxiliares para facilitar alocação e acesso aos valores
#define INTERVAL_SIZE 2 // Define de forma constante o tamanho do intervalo
#define INTERVAL_START 0 // Constante para acesso ao primeiro termo do intervalo
#define INTERVAL_END 1 // Constante para acesso ao segundo termo do intervalo

// Definição dos protótipos de funções utilizadas dentro da main
// A necessidade da declaração a aqui se dá para evitar que seja necessário definir as funções completas antes da main
int getPolynomialDegree();
void allocDouble(double **ptr, int size);
void allocAndStorePowers(int **p, int functionDegree);
void storeBaseNumbers(double *p, int functionDegree);
void showFunction(const double *ptrBaseNumbers, const int *ptrPowerNumbers, int functionDegree);
void allocInt(int **ptr, int size);
void storeIntervalNumbers(int *p, int intervalSize);
void applyTrapezoidalRule(
    const int *interval,
    const double *ptrCoefficients,
    const int *ptrPowerNumbers,
    int functionDegree,
    double **xPoints,
    int numberOfSubintervals
);
double calculatePolynomialWithX(
    double xValue,
    const double *ptrCoefficients,
    const int *ptrPowerNumbers,
    int functionDegree
);

// Escopo principal do programa
int main(void) {
    int numberOfSubintervals;       // Número de subdivisões do intervalo (determinado pelo usuário)
    int *interval = NULL;           // Ponteiro para alocar dinamicamente o intervalo de integração [a, b]
    double *xPoints = NULL;         // Pontos de divisão do intervalo, determinado pelo número de subdivisões
    double *ptrCoefficients = NULL; // Coeficientes do polinômio inseridos pelo usuário
    int *ptrPowerNumbers = NULL;    // Expoentes do polinômio, determinados com base no grau da função
    int userOption;                 // Controle de repetição

    // Loop que abre a possibilidade de repetição do cálculo para o usuário
    do {
        // Lê o grau do polinômio que será informado pelo usuário
        const int polynomialDegree = getPolynomialDegree();

        // Aloca um ponteiro e armazena nele os valores das potências dos termos
        allocAndStorePowers(&ptrPowerNumbers, polynomialDegree);

        // Aloca espaço para os coeficientes
        allocDouble(&ptrCoefficients, polynomialDegree+1);

        // Solicita ao usuário os coeficientes do polinômio
        storeBaseNumbers(ptrCoefficients, polynomialDegree);

        // Mostra a função para o usuário
        showFunction(ptrCoefficients, ptrPowerNumbers, polynomialDegree);

        // Aloca e lê os limites do intervalo [a, b]
        allocInt(&interval, INTERVAL_SIZE);
        storeIntervalNumbers(interval, INTERVAL_SIZE);

        // Solicita o número de subdivisões, garantindo valor positivo
        do {
            printf("\nDigite o número de subdivisões: ");
            scanf("%d", &numberOfSubintervals);
        } while (numberOfSubintervals <= 0);

        // Aplica a Regra dos Trapézios
        applyTrapezoidalRule(
            interval,
            ptrCoefficients,
            ptrPowerNumbers,
            polynomialDegree,
            &xPoints,
            numberOfSubintervals
        );

        // Pergunta se o usuário deseja repetir o processo
        do {
            printf("\nDeseja repetir o processo (1 - Sim | 2 - Nao)?\n->");
            scanf("%d", &userOption);
        } while (userOption != 1 && userOption != 2);

        // Liberação de memória alocada
        // Evita que lixo de memória afete os cálculos seguintes
        // Deve ser feita no final do programa como boa prática clang-tidy
        free(interval);
        free(xPoints);
        free(ptrCoefficients);
        free(ptrPowerNumbers);
    } while (userOption != 2); // Repete ou não, com base na escolha do usuário

    return EXIT_SUCCESS;
}

// Solicita e retorna o grau do polinômio (deve ser > 0)
int getPolynomialDegree() {
    int degree;

    do {
        printf("\nDigite o grau do polinomio que sera integrado: ");
        scanf("%d", &degree);
    } while (degree < 0);

    return degree;
}

// Aloca vetor de double com tamanho 'size'
void allocDouble(double **ptr, const int size) {
    if ((*ptr = (double*) calloc(size, sizeof(double))) == NULL) {
        fprintf(stderr, "\nErro de alocação.");
        exit(ALLOCATION_ERROR);
    }
}

// Aloca memória para um vetor de inteiros e armazena os expoentes de 0 até functionDegree
// 'p' é o ponteiro que receberá o endereço do vetor alocado
// 'functionDegree' define o grau do polinômio, portanto o vetor terá (functionDegree + 1) posições
void allocAndStorePowers(int **p, const int functionDegree) {
    const int size = functionDegree + 1;

    // Aloca memória para o vetor de inteiros
    if ((*p = (int *) calloc(size, sizeof(int))) == NULL) {
        fprintf(stderr, "\nERRO: Erro ao alocar intervalo de memória.");
        exit(UNDEFINED_ERROR);
    }

    // Armazena os expoentes: 0, 1, 2, ..., functionDegree
    for (int i = 0; i < size; i++) {
        (*p)[i] = i;
    }
}

// Nessa função utiliza-se o ponteiro para armazenar valores digitados pelo usuário para os termos dependentes da função
void storeBaseNumbers(double *p, const int functionDegree) {
    for (int i = functionDegree; i>=0; i--) {
        printf("\nDigite o valor do termo x^%d (%c): ", i, 65+(functionDegree-i));
        scanf("%lf", p+i);
    }
}

// Função utilizada para mostrar a equação
void showFunction(const double *ptrBaseNumbers, const int *ptrPowerNumbers, const int functionDegree) {
    printf("\nA funcao recebida foi: ");
    // Aqui percorremos a função conforme o seu grau de forma 'inversa'
    // Começamos pelo termo de maior grau (sempre mostrado na frente matematicamente)
    printf("Int {");
    for (int i = functionDegree; i>=0; i--) {
        const double number = *(ptrBaseNumbers+i);
        const int power = *(ptrPowerNumbers+i);
        // Utilizamos o ponteiro que tem as potências e os termos dependentes
        if (number != 0) {
            printf("%.4lfx^%d", number, power);
            if (i != 0) {
                printf(" + ");
            }
        }
    }
    printf("\n");
}

// Aloca ou realoca dinamicamente um vetor de inteiros de tamanho 'size'.
// O ponteiro fornecido é atualizado com o novo endereço de memória.
// Encerra o programa com código de erro caso a alocação falhe.
void allocInt(int **ptr, const int size) {
    if ((*ptr = (int*) calloc(size, sizeof(int)))==NULL) {
        printf("\nErro de alocacao.");
        exit(ALLOCATION_ERROR);
    }
}

// Solicita os dois valores que definem o intervalo [a, b]
void storeIntervalNumbers(int *p, const int intervalSize) {
    for (int i = 0; i < intervalSize; i++) {
        printf("\nDigite o valor do termo (%c | %d) do intervalo: ", 65+i, i);
        scanf("%d", &p[i]);
    }
}

// Aplica a Regra dos Trapézios para calcular a integral aproximada
void applyTrapezoidalRule(
    const int *interval,
    const double *ptrCoefficients,
    const int *ptrPowerNumbers,
    const int functionDegree,
    double **xPoints,
    const int numberOfSubintervals
) {
    // Aqui são criadas variáveis para guardar o início da integral e o seu fim
    // Calculamos também o tamanho de H, que define o intervalo entre cada ponto X da
    // Regra dos Trapézios Repetida
    const double integralStart = interval[INTERVAL_START];
    const double integralEnd = interval[INTERVAL_END];
    const double hSize = (integralEnd - integralStart)/numberOfSubintervals;
    printf("\nValor de H = %.3lf", hSize);

    // Aloca memória para armazenar os pontos X com base na quantidade de 'subintervalos'
    allocDouble(xPoints, numberOfSubintervals+1);

    // Calcula o valor de cada ponto entre Xi e Xf com base no valor de H e na quantidade de intervalos
    for (int i = numberOfSubintervals; i >= 0; i--) {
        (*xPoints)[i] = integralStart + i * hSize;
        printf("\n\n(X_%d) = (%.3lf)", i, (*xPoints)[i]);
    }

    double numericalIntegrationResult = 0.0;

    // Calcula f(x0) e f(xn)
    numericalIntegrationResult += calculatePolynomialWithX(
        (*xPoints)[0],
        ptrCoefficients,
        ptrPowerNumbers,
        functionDegree
    );
    numericalIntegrationResult += calculatePolynomialWithX(
        (*xPoints)[numberOfSubintervals],
        ptrCoefficients,
        ptrPowerNumbers,
        functionDegree
    );

    // Soma 2*f(xi) para os pontos intermediários
    for (int i = 1; i < numberOfSubintervals; i++) {
        numericalIntegrationResult += 2 * calculatePolynomialWithX(
            (*xPoints)[i],
            ptrCoefficients,
            ptrPowerNumbers,
            functionDegree
        );
    }

    // Multiplica por h/2
    numericalIntegrationResult *= hSize / 2.0;

    printf("\n\nResultado da integração numérica: %.6lf\n", numericalIntegrationResult);
}

// Função que calcula o valor do polinômio em um determinado ponto x
double calculatePolynomialWithX(
    const double xValue,
    const double *ptrCoefficients,
    const int *ptrPowerNumbers,
    const int functionDegree
) {
    double result = 0.0;
    for (int i = 0; i <= functionDegree; i++) {
        result += ptrCoefficients[i] * pow(xValue, ptrPowerNumbers[i]);
    }
    return result;
}
