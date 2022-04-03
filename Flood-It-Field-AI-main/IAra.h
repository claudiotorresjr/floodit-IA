/**
 * @file  IAra.h
 * @brief Arquivo com os cabeçalhos das funções da inteligência artificial.
 * @date 15/08/2021
 * @author Richard Fernando Heise Ferreira (GRR20191053)
 **/

#ifndef __IAra__
#define __IAra__
#include "floodlib.h"  //funções do flood
#include <algorithm>   // sort, unique
#include <cmath>       // log (natural), floor

using namespace std;

/**
 * @param matriz m
 * @param int cores
 * @brief Função que fornece a heurística de um tabuleiro.
 * @return Retorna um valor decimal usado como score de um tabuleiro.
 */
float h(matriz m, int cores);

/**
 * @param matriz m
 * @param int cores
 * @brief Função que retorna a melhor jogada em um tabuleiro.
 * @return Retorna um char contendo a melhor possível cor a ser jogada.
 */
char buscaMelhorJogada (matriz m, int cores);

/**
 * @param matriz m
 * @param int cores
 * @brief Função que escolhe as possiveis jogadas com base nas cores adjacentes ao cluster principal.
 * @return Retorna um vetor de jogadas possiveis.
 */
vector<char> possiveisCores (matriz m);

/**
 * @param matriz m
 * @param int cores
 * @param int maxEscopos
 * @brief Função que prevê jogadas no futuro a partir de um tabuleiro.
 * @return Retorna um valor float com a heurística da melhor jogada prevista.
 */
float preveJogada (matriz m, int cores, int maxEscopos);

/**
 * @param matriz m
 * @param int cores
 * @brief Função que resolve o flood-it dado um tabuleiro.
 * @return Não há, ela mesma printa todas as jogadas que resolveram o tabuleiro.
 */
void resolve (matriz m, int cores);

#endif