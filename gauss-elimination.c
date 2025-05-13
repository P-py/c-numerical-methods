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

int main(void) {
    int n = getNvalue();

    for (int i = 0; i < n; i++) {
        printf("|\t");
        for (int j = 0; j < n; j++) {
            printf("*\t");
        }
        printf("|\t\n");
    }

    return 0;
}

int getNvalue() {
    int n;

    printf("Digite o valor de N para compor a matriz (NxN): ");
    scanf("%d", &n);
    return n;
}