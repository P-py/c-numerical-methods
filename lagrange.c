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
void storeDegree(int *p);

void main() {
    knownPoints *ptrKnowPoints = NULL;
    int *polynomialDegree = NULL;
    int knowPointsAmount = 0;

    alocInts(&polynomialDegree, 1);

    storeDegree(polynomialDegree);
    knowPointsAmount = *polynomialDegree + 1;

    printf("\nQuantidade de pontos conhecidos: %d", knowPointsAmount);
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

// Função utilizada para armazenar em um ponteiro um valor que indica o grau do polinômio que será calculado
void storeDegree(int *p) {
    printf("\nDigite o grau do polinomio: ");
    scanf("%d", p);
}