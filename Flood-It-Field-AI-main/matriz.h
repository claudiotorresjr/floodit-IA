/**
 * @file  matriz.h
 * @brief Arquivo com os cabaçalhos das funções de matrizes.
 * @date 15/08/2021
 * @author Richard Fernando Heise Ferreira (GRR20191053)
 **/

#ifndef __matriz__
#define __matriz__

#include <iostream> // Fluxo de dados
#include <time.h>   // srand 
#include <vector>   // Estrutura de dados de vetor
using namespace std;

#define SAIDA_LEITURA -1

/**
 * @typedef Matriz é um vetor de vetores de chars.
 */
typedef vector<vector<char>> matriz;

/**
 * @param matriz m
 * @param int l
 * @param int c
 * @brief Função que lê uma matriz de chars.
 */
void leMatriz (matriz *m, int l, int c);

/**
 * @param matriz m
 * @brief Função que escreve uma matriz.
 */
void EscreveMatriz (matriz m);

/**
 * @param int l
 * @param int c
 * @brief Função que aloca uma matriz de l linhas e c colunas.
 * @return Retorna uma matriz alocada.
 */
matriz alocaMatriz (int l, int c);

/**
 * @param int l
 * @param int c
 * @param int cores
 * @brief Função que gera uma matriz com cores aleatórias.
 * @return Retorna um tabuleiro.
 */
matriz geraMatriz (int l, int c, int cores);

#endif