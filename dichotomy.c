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

// No código não existe cobertura para cenários clang-tidy, as funções scanf não garantem a conversão de tipos

// Função genérica para alocar espaços de memória para tipos float
void alocFloats(float **p, const int size){
    if ((*p = (float*) realloc(*p, size*sizeof(float)))==NULL){
        printf("\nErro de alocacao.");
        exit(0);
    }
}

// Função genérica para alocar espaços de memória para tipos int
void alocInts(int **p, const int size) {
    if ((*p = (int*) realloc(*p, size*sizeof(int)))==NULL) {
        printf("\nErro de alocacao.");
        exit(0);
    }
}

// Nessa função utiliza-se o ponteiro para armazenar valores digitados pelo usuário para os termos dependentes da função
void storeBaseNumbers(float *p, const int functionDegree) {
    for (int i = functionDegree; i>=0; i--) {
        printf("\nDigite o valor do termo x^%d (%c): ", i, 65+(functionDegree-i));
        scanf("%f", p+i);
    }
}

// Utiliza-se o ponteiro int para armazenar valores digitados das potências dos termos
void storePowerNumbers(int *p, const int functionDegree) {
    for (int i = 0; i<functionDegree+1; i++) {
        *(p+i) = i;
    }
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
        printf("\nDicotomia invalida para o intervalo.");
        exit(0);
    } else {
        printf("\nDicotomia validada para o intervalo passado.");
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

// Função que implementa iterações individuais da dicotomia
void dichotomyIteration(
    float *ptrInterval,
    const int *ptrPowerNumbers,
    const float *ptrBaseNumbers,
    const int functionDegree,
    float *ptrResults
) {
    // Calcula-se o ponto médio com base nos intervalos
    const float a = *ptrInterval;
    const float b = *(ptrInterval+1);
    const float mPointValue = (a+b)/2;

    // Atualiza-se o valor do ponto médio para a próxima iteração
    *(ptrInterval+2) = mPointValue;

    // São calculados e atualizados os valores de f(A), f(B), f(M)
    calculateValues(ptrInterval, ptrPowerNumbers, ptrBaseNumbers, functionDegree, ptrResults);
    const float aResult = *ptrResults;
    const float mPointResult = *(ptrResults+2);

    // São atualizados os valores de referência do intervalo com base nos resultados
    if (aResult*mPointResult < 0) {
        *(ptrInterval+1) = mPointValue;
    } else {
        *ptrInterval = mPointValue;
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

    // Mostra os resultados a iteraçao
    printf("\n\nITERATION %d\n", i);
    printf("A: %.4f | ", a);
    printf("B: %.4f | ", b);
    printf("M: %.4f | ", m);

    printf("f(A): %.4f | ", aResult);
    printf("f(B): %.4f | ", bResult);
    printf("f(M): %.4f | ", mPointResult);

    printf("f(A)*f(M): %.4f | ", aResult*mPointResult);
    printf("f(B)*f(M): %.4f | ", bResult*mPointResult);

    printf("B-A: %.4f", b - a);
}

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
    alocFloats(&ptrInterval, 3);
    storeIntervalNumbers(ptrInterval);

    // Criar uma função que recebe o valor do erro
    // Aloca e armazena o valor do erro
    alocFloats(&errorMargin, 1);
    storeErrorMargin(errorMargin);

    // Aloca o ponteiro para os resultados
    alocFloats(&ptrResults, 3);

    // Valida se a dicotomia é possível
    validateFunction(ptrInterval, ptrPowerNumbers, ptrBaseNumbers, functionDegree, ptrResults);

    // Calcula o valor de K
    const double kValue = calculateKValue(ptrInterval, errorMargin);

    for (int i=1; i<=kValue; i++) {
        // Implementação da dicotomia
        dichotomyIteration(ptrInterval, ptrPowerNumbers, ptrBaseNumbers, functionDegree, ptrResults);
        // Mostra os valores pós-iteração
        showIterationValues(i, ptrInterval, ptrResults);
    }

    printf("\n\nValor de x para a raiz da funcao: %.4f\n", *(ptrInterval+2));
    exit(0);
}