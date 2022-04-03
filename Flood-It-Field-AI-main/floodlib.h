/**
 * @file  floodlib.h
 * @brief Arquivo com as cabeçalhos das funções de flood.
 * @date 15/08/2021
 * @author Richard Fernando Heise Ferreira (GRR20191053)
 **/

#ifndef __floodlib__
#define __floodlib__

#include "matriz.h"
using namespace std;

 /**
  * @typedef coordenada é um par de inteiros.
  */
typedef pair<int, int> coordenada;

/**
 * @param matriz m
 * @param char novaCor
 * @param coordenada inicio
 * @brief Função que flooda uma matriz com a nova cor na coordenada de inicio.
 */
void floodFill (matriz *m, char novaCor, coordenada inicio);

/**
 * @param matriz m
 * @param coordenada c
 * @param char corAnt
 * @param char novaCor
 * @brief Função que pinta uma coordenada c da matriz m com a nova cor.
 * @return 1 se deu certo, 0 caso contrário.
 */
int flood (matriz *m, coordenada c, char corAnt, char novaCor);

/**
 * @param matriz m
 * @brief Função que verifica se um tabuleiro foi resolvido.
 * @return 1 se deu certo, 0 caso contrário.
 */
int resolveu (matriz m);

#endif