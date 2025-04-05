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

// Estrutura criada para suportar o conjunto de pontos conhecidos da função
typedef struct {
    float x;
    float y;
} knownPoints;

void alocKnownPoints(knownPoints **ptr, int size);
void alocInts(int **ptr, int size);
void alocFloats(float **ptr, int size);
void storeDegree(int *ptr);
void storeXValue(float *ptr);

void main() {
    knownPoints *ptrKnowPoints = NULL;
    int *polynomialDegree = NULL;
    int knowPointsAmount = 0;
    float *xValue = NULL;

    // O primeiro passo que é executado aqui é a determinação do grau do polinômio que deve ser calculado
    // A primeira função aloca o espaço de memória que será utilizado pelo ponteiro
    alocInts(&polynomialDegree, 1);
    // Armazena no ponteiro e na variável o valor do grau do polinômio e quantidade de pontos
    storeDegree(polynomialDegree);
    knowPointsAmount = *polynomialDegree + 1;

    // O segundo passo é armazenar em um ponteiro (xValue) o valor de X que se quer obter através do polinômio
    alocFloats(&xValue, knowPointsAmount);
    // Armazena no ponteiro o último valor de X que o usuário digitou
    storeXValue(xValue);

    // Para o terceiro passo é necessário alocar e armazenar em um ponteiro os valores conhecidos/tabelados
    // Para isso utilizamos um struct e um ponteiro dessa struct
    alocKnownPoints(&ptrKnowPoints, knowPointsAmount);
    // Armazena no ponteiro os valores tabelados
    storeKnownPoints(ptrKnowPoints, knowPointsAmount);

    printf("\nQuantidade de pontos conhecidos: %d", knowPointsAmount);
    printf("\nValor de X alvo: %.2f", *xValue);

    exit(0);
}

// Função para alocar espaços de memória para o tipo das struct knowPoints
void alocKnownPoints(knownPoints **ptr, const int size) {
    if ((*ptr = (knownPoints*) realloc(*ptr, size*sizeof(knownPoints)))==NULL) {
        printf("\nErro de alocacao.");
        exit(0);
    }
}

// Função genérica para alocar espaços de memória para o tipo int
void alocInts(int **ptr, const int size) {
    if ((*ptr = (int*) realloc(*ptr, size*sizeof(int)))==NULL) {
        printf("\nErro de alocacao.");
        exit(0);
    }
}

// Função genérica para alocar espaços de memória para o tipo float
void alocFloats(float **ptr, const int size) {
    if ((*ptr = (float*) realloc(*ptr, size*sizeof(float)))==NULL) {
        printf("\nErro de alocacao.");
        exit(0);
    }
}

// Função utilizada para armazenar em um ponteiro um valor que indica o grau do polinômio que será calculado
void storeDegree(int *ptr) {
    printf("\nDigite o grau do polinomio: ");
    scanf("%d", ptr);
}

// Função utilizada para armazenar em um ponteiro o valor que indica para qual X deseja-se obter f(X) pelo polinomio
void storeXValue(float *ptr) {
    printf("\nDigite o valor de X desconhecido que se quer obter com o polinomio: ");
    scanf("%f", ptr);
}

void storeKnownPoints() {
    
}