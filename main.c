#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void alocFloats(float **p, const int size){
    if ((*p = (float*) realloc(*p, size*sizeof(float)))==NULL){
        printf("\nErro de alocacao.");
        exit(0);
    }
}

void alocInts(int **p, const int size) {
    if ((*p = (int*) realloc(*p, size*sizeof(int)))==NULL) {
        printf("\nErro de alocacao.");
        exit(0);
    }
}

void storeBaseNumbers(float *p, const int functionDegree) {
    for (int i = functionDegree; i>=0; i--) {
        printf("\nDigite o valor do termo x^%d (%c): ", i, 65+(functionDegree-i));
        scanf("%f", p+i);
    }
}

void storePowerNumbers(int *p, const int functionDegree) {
    for (int i = 0; i<functionDegree+1; i++) {
        *(p+i) = i;
    }
}

void showEquation(const int numbersAmount, const float independentNumber, float *ptrBaseNumbers, int *ptrPowerNumbers) {
    printf("\n");
    for (int i = 0; i<numbersAmount; i++) {
        printf("%fx^%d", *(ptrBaseNumbers+i), *(ptrPowerNumbers+i));
    }
    printf("%.4f", independentNumber);
}

int receiveFunctioDegree() {
    int userInput = -1;
    do {
        printf("\nInforme o grau da funcao (2 a 6): ");
        scanf("%d", &userInput);
    } while (userInput< 2 || userInput > 6);
    return userInput;
}

void showFunction(const float *ptrBaseNumbers, const int *ptrPowerNumbers, const int functionDegree) {
    printf("\nA funcao recebida foi: ");
    for (int i = functionDegree; i>=0; i--) {
        const float number = *(ptrBaseNumbers+i);
        const int power = *(ptrPowerNumbers+i);
        if (number != 0) {
            printf("%.4fx^%d", number, power);
            if (i != 0) {
                printf(" + ");
            }
        }
    }
}

void storeIntervalNumbers(float *ptrInterval) {
    for (int i = 0; i<2; i++) {
        printf("\nDigite o valor do valor %d do intervalo: ", i+1);
        scanf("%f", ptrInterval+i);
    }
}

void storeErrorMargin(float *errorMargin) {
    printf("\nDigite o valor da margem erro: ");
    scanf("%f", errorMargin);
}

void validateFunction(
    const float *ptrInterval,
    const int *ptrPowerNumbers,
    const float *ptrBaseNumbers,
    const int functionDegree,
    float *ptrResults
){
    for (int i = 0; i<2; i++) {
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
    if (resultForA*resultForB > 0) {
        printf("\nDicotomia invalida para o intervalo.");
        exit(0);
    } else {
        printf("\nDicotomia validada para o intervalo passado.");
    }
}

double calculateKValue(const float *ptrInterval, const float *errorMargin) {
    const float a = *ptrInterval;
    const float b = *(ptrInterval+1);
    printf("\nCalculando valor de K");
    printf("\nA: %.4f", a);
    printf("\nB: %.4f", b);
    printf("\nError: %.4f", *errorMargin);
    const float logAB = log10f(b-a);
    printf("\nLog B-A: %.4f", logAB);
    const float logError = log10f(*errorMargin);
    printf("\nLog Error: %.4f", logError);
    const float kValue = ceilf((logAB-logError)/log10f(2));
    printf("\nValor de K: %.4f", kValue);
    return kValue;
}

void calculateValues(
    const float *ptrInterval,
    const int *ptrPowerNumbers,
    const float *ptrBaseNumbers,
    const int functionDegree,
    float *ptrResults
) {
    for (int i = 0; i<3; i++) {
        *(ptrResults+i) = 0.0f;
        for (int j=functionDegree; j>=0; j--) {
            const float baseNumber = *(ptrBaseNumbers+j);
            const float power = (float) *(ptrPowerNumbers+j);

            *(ptrResults+i) += powf(*(ptrInterval+i), power)*baseNumber;
        }
    }
}

void main() {
    int functionDegree = 0;
    float *ptrBaseNumbers = NULL;
    int *ptrPowerNumbers = NULL;
    float *ptrInterval = NULL;
    float *errorMargin = NULL;
    float *ptrResults = NULL;

    // Recebe do usuário o grau da equação através de uma função
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
        const float a = *ptrInterval;
        const float b = *(ptrInterval+1);
        const float mPointValue = (a+b)/2;

        *(ptrInterval+2) = mPointValue;

        printf("\n\nITERATION %d\n", i);
        printf("A: %.4f | ", *ptrInterval);
        printf("B: %.4f | ", *(ptrInterval+1));
        printf("M: %.4f | ", *(ptrInterval+2));

        calculateValues(ptrInterval, ptrPowerNumbers, ptrBaseNumbers, functionDegree, ptrResults);
        const float aResult = *ptrResults;
        const float bResult = *(ptrResults+1);
        const float mPointResult = *(ptrResults+2);

        printf("f(A): %.4f | ", aResult);
        printf("f(B): %.4f | ", bResult);
        printf("f(M): %.4f | ", mPointResult);

        printf("f(A)*f(M): %.4f | ", aResult*mPointResult);
        printf("f(B)*f(M): %.4f | ", bResult*mPointResult);

        printf("B-A: %.4f", b - a);

        if (aResult*mPointResult < 0) {
           *(ptrInterval+1) = mPointValue;
        } else {
            *ptrInterval = mPointValue;
        }
    }

    printf("\n\nValor de x para a raiz da funcao: %.4f\n", *(ptrInterval+2));
    exit(0);
}