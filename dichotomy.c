// Algoritmo de Dicotomia implementado
//
// Pedro Salviano Santos - 236586
// Luiz Gustavo Motta Viana - 236428
// Erick Ferreira Ribeiro - 237046

// Esse código implementa o algoritmo estatístico da "Dicotomia"
// Foram utilizados ponteiros e alocação dinâmica de memória
// Busca-se utilizar boas práticas de memória e clang-tidy

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Biblioteca para as funções matemáticas

#define INTERVAL_SIZE 2
#define EQUATION_TERMS 3

// Códigos de saída para a função exit()
// 0 > sucesso
// 1 > erro genérico
// 2 > argumento inválido
// 3 > falha lógica
#define EXIT_SUCESS 0
#define UNDEFINED_ERROR 1
#define INVALID_ARGUMENT_ERROR 2

// No código não existe cobertura para cenários clang-tidy, as funções scanf não garantem a conversão de tipos

void showIterationValues(int i, const float *ptrInterval, const float *ptrResults);
void alocFloats(float **p, int size);
void alocInts(int **p, int size);
void storeBaseNumbers(float *p, int functionDegree);
void storePowerNumbers(int *p, int functionDegree);
int receiveFunctioDegree();
void showFunction(const float *ptrBaseNumbers, const int *ptrPowerNumbers, int functionDegree);
void storeIntervalNumbers(float *ptrInterval);
void storeErrorMargin(float *errorMargin);
void validateFunction(
    const float *ptrInterval,
    const int *ptrPowerNumbers,
    const float *ptrBaseNumbers,
    int functionDegree,
    float *ptrResults
);
double calculateKValue(const float *ptrInterval, const float *errorMargin);
void calculateValues(
    const float *ptrInterval,
    const int *ptrPowerNumbers,
    const float *ptrBaseNumbers,
    int functionDegree,
    float *ptrResults
);
void dichotomyIteration(
    float *ptrInterval,
    const int *ptrPowerNumbers,
    const float *ptrBaseNumbers,
    int functionDegree,
    float *ptrResults,
    int iteration
);
void updateIntervalValues(const float *ptrResults, float *ptrInterval);

void main() {
    int functionDegree = 0;
    float *ptrBaseNumbers = NULL;
    int *ptrPowerNumbers = NULL;
    float *ptrInterval = NULL;
    float *errorMargin = NULL;
    float *ptrResults = NULL;

    // Recebe do usuário o grau da equação por uma função
    functionDegree = receiveFunctioDegree();

    // Aloca um ponteiro e armazena nele os valores das potências dos termos
    alocInts(&ptrPowerNumbers, functionDegree+1);
    storePowerNumbers(ptrPowerNumbers, functionDegree);

    // Aloca um ponteiro para o valor dos termos
    alocFloats(&ptrBaseNumbers, functionDegree+1);
    storeBaseNumbers(ptrBaseNumbers, functionDegree);

    // Mostra a função ao usuário
    showFunction(ptrBaseNumbers, ptrPowerNumbers, functionDegree);

    // Aloca e armazena os valores do intervalo digitados pelo usuário
    alocFloats(&ptrInterval, INTERVAL_SIZE+1);
    storeIntervalNumbers(ptrInterval);

    // Criar uma função que recebe o valor do erro
    // Aloca e armazena o valor do erro
    alocFloats(&errorMargin, 1);
    storeErrorMargin(errorMargin);

    // Aloca o ponteiro para os resultados
    alocFloats(&ptrResults, INTERVAL_SIZE+1);

    // Valida se a dicotomia é possível
    validateFunction(ptrInterval, ptrPowerNumbers, ptrBaseNumbers, functionDegree, ptrResults);

    // Calcula o valor de K
    const double kValue = calculateKValue(ptrInterval, errorMargin);

    for (int iteration=1; iteration<=kValue; iteration++) {
        // Implementação da dicotomia
        dichotomyIteration(ptrInterval, ptrPowerNumbers, ptrBaseNumbers, functionDegree, ptrResults, iteration);

        // São atualizados os valores de referência do intervalo com base nos resultados
        updateIntervalValues(ptrResults, ptrInterval);
    }

    int iteration = (int) kValue;

    // Caso após as iterações calculados com base em K os critérios de parada ainda não tenha sido atingidos
    // a dicotomia continua
    // Entende-se por critérios de parada:
    //      f(M) < margem de erro
    //      (b-a) < margem erro
    while (
        fabsf(*(ptrResults+2)) >= *errorMargin &&
        *(ptrInterval+1)-*ptrInterval >= *errorMargin)
    {
        // Implementação da dicotomia
        iteration++;
        dichotomyIteration(ptrInterval, ptrPowerNumbers, ptrBaseNumbers, functionDegree, ptrResults, iteration);

        // São atualizados os valores de referência do intervalo com base nos resultados
        updateIntervalValues(ptrResults, ptrInterval);
    }

    printf("\n\nValor de x para a raiz da funcao: %.4f\n", *(ptrInterval+2));
    exit(EXIT_SUCESS);
}

// Função usada para receber o grau da função
// Não se espera um grau maior do que 6
int receiveFunctioDegree() {
    int userInput = -1;
    // Programa não segue enquanto usuário não digitar um valor válido
    do {
        printf("\nInforme o grau da funcao (2 a 6): ");
        scanf("%d", &userInput);
    } while (userInput< 2 || userInput > 6);
    return userInput;
}

// Função genérica para alocar espaços de memória para tipos int
void alocInts(int **p, const int size) {
    if ((*p = (int*) malloc(size*sizeof(int)))==NULL) {
        fprintf(stderr, "\nERRO: Erro ao alocar intervalo de memória.");
        exit(UNDEFINED_ERROR);
    }
}

// Utiliza-se o ponteiro int para armazenar valores digitados das potências dos termos
void storePowerNumbers(int *p, const int functionDegree) {
    for (int i = 0; i<functionDegree+1; i++) {
        *(p+i) = i;
    }
}

// Função genérica para alocar espaços de memória para tipos float
void alocFloats(float **p, const int size){
    if ((*p = (float*) malloc(size*sizeof(float)))==NULL){
        fprintf(stderr, "\nERRO: Erro ao alocar intervalo de memória.");
        exit(UNDEFINED_ERROR);
    }
}

// Nessa função utiliza-se o ponteiro para armazenar valores digitados pelo usuário para os termos dependentes da função
void storeBaseNumbers(float *p, const int functionDegree) {
    for (int i = functionDegree; i>=0; i--) {
        printf("\nDigite o valor do termo x^%d (%c): ", i, 65+(functionDegree-i));
        scanf("%f", p+i);
    }
}

// Função utilizada para mostrar a equação
void showFunction(const float *ptrBaseNumbers, const int *ptrPowerNumbers, const int functionDegree) {
    printf("\nA funcao recebida foi: ");
    // Aqui percorremos a função conforme o seu grau de forma 'inversa'
    // Começamos pelo termo de maior grau (sempre mostrado na frente matematicamente)
    for (int i = functionDegree; i>=0; i--) {
        const float number = *(ptrBaseNumbers+i);
        const int power = *(ptrPowerNumbers+i);
        // Utilizamos o ponteiro que tem as potências e os termos dependentes
        if (number != 0) {
            printf("%.4fx^%d", number, power);
            if (i != 0) {
                printf(" + ");
            }
        }
    }
}

// Função simples para armazenar os valores do intervalo em um ponteiro de forma dinâmica
// Usamos uma constante no ‘loop’, pois o intervalo sempre terá 2 valores
void storeIntervalNumbers(float *ptrInterval) {
    for (int i = 0; i<INTERVAL_SIZE; i++) {
        printf("\nDigite o valor do valor %d do intervalo: ", i+1);
        scanf("%f", ptrInterval+i);
    }
}

// Armazenamos o valor da margem de erro
void storeErrorMargin(float *errorMargin) {
    printf("\nDigite o valor da margem erro: ");
    scanf("%f", errorMargin);
}

// Função utilizada para validar se os valores passados são validos para aplicação da dicotomia
// Nessa validação se espera que f(A)*f(B) < 0
void validateFunction(
    const float *ptrInterval,
    const int *ptrPowerNumbers,
    const float *ptrBaseNumbers,
    const int functionDegree,
    float *ptrResults
){
    // Percorremos os termos da função com base no seu grau somando ao ponteiro de resultados o valor de cada termo
    for (int i = 0; i<INTERVAL_SIZE; i++) {
        for (int j=functionDegree; j>=0; j--) {
            const float baseNumber = *(ptrBaseNumbers+j);
            const float power = (float) *(ptrPowerNumbers+j);

            *(ptrResults+i) += powf(*(ptrInterval+i), power)*baseNumber;
        }
    }

    const float resultForA = *ptrResults;
    printf("\nResult for A_0 in interval: %.4f", resultForA);
    const float resultForB = *(ptrResults+1);
    printf("\nResult for B_0 in interval: %.4f", resultForB);

    // Caso os valores sejam inválidos para a função, o código termina aqui
    if (resultForA*resultForB > 0) {
        fprintf(stderr, "\nERRO: Dicotomia invalida para o intervalo.\n");
        exit(INVALID_ARGUMENT_ERROR);
    } else {
        printf("\nDicotomia validada para o intervalo passado.\n");
    }
}

// Função que calcula o valor da constante de iterações K
double calculateKValue(const float *ptrInterval, const float *errorMargin) {
    const float a = *ptrInterval;
    const float b = *(ptrInterval+1);

    printf("\nCalculando valor de K");
    printf("\nA: %.4f", a);
    printf("\nB: %.4f", b);
    printf("\nError: %.4f", *errorMargin);

    // Para melhor validação durante a execução foram separados os termos da equação
    const float logAB = log10f(b-a);
    const float logError = log10f(*errorMargin);
    const float kValue = ceilf((logAB-logError)/log10f(2));

    printf("\nLog B-A: %.4f", logAB);
    printf("\nLog Error: %.4f", logError);
    printf("\nValor de K: %.4f", kValue);

    return kValue;
}

// Função que implementa iterações individuais da dicotomia
void dichotomyIteration(
    float *ptrInterval,
    const int *ptrPowerNumbers,
    const float *ptrBaseNumbers,
    const int functionDegree,
    float *ptrResults,
    const int iteration
) {
    // Calcula-se o ponto médio com base nos intervalos
    const float a = *ptrInterval;
    const float b = *(ptrInterval+1);
    const float mPointValue = (a+b)/2;

    // Atualiza-se o valor do ponto médio para a próxima iteração
    *(ptrInterval+2) = mPointValue;

    // São calculados e atualizados os valores de f(A), f(B), f(M)
    calculateValues(ptrInterval, ptrPowerNumbers, ptrBaseNumbers, functionDegree, ptrResults);

    // Mostra os valores pós-iteração
    showIterationValues(iteration, ptrInterval, ptrResults);
}

// Função genérica que calcula o valor de um ponto na função com base em um valor de X
void calculateValues(
    const float *ptrInterval,
    const int *ptrPowerNumbers,
    const float *ptrBaseNumbers,
    const int functionDegree,
    float *ptrResults
) {
    // Para cada iteração precisamos de 3 valores (A, B, M - ponto médio)
    // Zeramos o valor do ponteiro de resultados no começo das iterações já que é utilizada soma
    // Percorremos de forma decrescente os termos da função somando os seus valores ao resultado de cada ponto
    for (int i = 0; i<EQUATION_TERMS; i++) {
        *(ptrResults+i) = 0.0f;
        for (int j=functionDegree; j>=0; j--) {
            const float baseNumber = *(ptrBaseNumbers+j);
            const float power = (float) *(ptrPowerNumbers+j);

            *(ptrResults+i) += powf(*(ptrInterval+i), power)*baseNumber;
        }
    }
}

// Função utilizada para mostrar os valores a cada iteração
void showIterationValues(const int i, const float *ptrInterval, const float *ptrResults) {
    const float a = *ptrInterval;
    const float b = *(ptrInterval+1);
    const float m = *(ptrInterval+2);
    const float aResult = *ptrResults;
    const float bResult = *(ptrResults+1);
    const float mPointResult = *(ptrResults+2);

    // Mostra os resultados da iteração
    printf("\n\nITERATION %d\n", i);
    printf("A: %.4f | ", a);
    printf("B: %.4f | ", b);
    printf("M: %.4f | ", m);

    printf("f(A): %.4f | ", aResult);
    printf("f(B): %.4f | ", bResult);
    printf("f(M): %.4f | ", mPointResult);

    printf("f(A)*f(M): %.4f | ", aResult*mPointResult);
    printf("f(B)*f(M): %.4f | ", bResult*mPointResult);

    printf("B-A: %.4f | ", b - a);
    printf("mod f(M): %.4f", fabsf(mPointResult));
}

// Função utilizada para atualizar os valores de intervalo com base nos resultados da dicotomia
// Se f(A)*f(M) < 0 então B = M
// Se f(B)*f(M) < 0 então A = M
void updateIntervalValues(
    const float *ptrResults,
    float *ptrInterval
) {
    const float aResult = *ptrResults;
    const float mPointResult = *(ptrResults+2);
    const float a = *ptrInterval;
    const float b = *(ptrInterval+1);
    const float mPointValue = (a+b)/2;

    // São atualizados os valores de referência do intervalo com base nos resultados
    if (aResult*mPointResult < 0) {
        *(ptrInterval+1) = mPointValue;
    } else {
        *ptrInterval = mPointValue;
    }
}
