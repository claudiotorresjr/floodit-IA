/**
 * @file  matriz.cpp
 * @brief Arquivo com as implementações das funções de matrizes.
 * @date 15/08/2021
 * @author Richard Fernando Heise Ferreira (GRR20191053)
 **/

#include "matriz.h"
using namespace std;

//=============================================================//

void leMatriz (matriz *m, int l, int c) {
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            int aux;
             if (!scanf(" %d", &aux)) {
                perror("Não foi possível ler a matriz, abortando.\n");
                exit(SAIDA_LEITURA);
            }
            (*m)[i][j] = aux;
        }
    }
}

//=============================================================//

void EscreveMatriz (matriz m) {
    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].size(); j++)
            printf("%d ", m[i][j]);
        printf("\n");
    }
}

//=============================================================//

matriz alocaMatriz (int l, int c) {
    matriz m(l);
    for (int i = 0; i < l; i++) {
        m[i].resize(c);
    }

    return m;
}

//=============================================================//

matriz geraMatriz (int l, int c, int cors) { 

    matriz m = alocaMatriz(l, c);

    // A semente é o tempo do computador
    srand(time(NULL));

    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++)
            m[i][j] = (rand() % cors + 1);
    }

    return m;
} 
//=============================================================//