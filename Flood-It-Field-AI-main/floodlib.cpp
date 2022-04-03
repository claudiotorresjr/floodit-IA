/**
 * @file  floodlib.cpp
 * @brief Arquivo com as implementações das funções de flood.
 * @date 15/08/2021
 * @author Richard Fernando Heise Ferreira (GRR20191053)
 **/

#include "floodlib.h"
using namespace std;

//=============================================================//

void floodFill (matriz *m, char novaCor, coordenada inicio)  {
    // Flood Fill implementado usando um vetor como lista.
    vector<coordenada> lista;

    lista.push_back(inicio);
    char corAnt = (*m)[0][0];

    while (!lista.empty()) {
        coordenada aux = lista.back();
        lista.pop_back();

        // BFS
        if ( flood(&(*m), aux, corAnt, novaCor) ) {

            lista.push_back(make_pair(aux.first+1, aux.second));
            lista.push_back(make_pair(aux.first-1, aux.second));
            lista.push_back(make_pair(aux.first, aux.second+1));
            lista.push_back(make_pair(aux.first, aux.second-1));
            
        }
    }
}   

//=============================================================//

int flood (matriz *m, coordenada c, char corAnt, char novaCor) {

    // Checa se a coordenada pertence ao tabuleiro, se a nova cor não é a anterior
    if (c.first < 0 || c.first >= (*m).size() || c.second < 0 || c.second >= (*m)[0].size()
        || (*m)[c.first][c.second] != corAnt || (*m)[c.first][c.second] == novaCor)
        return 0;

    // Pinta com a nova cor.
    (*m)[c.first][c.second] = novaCor;

    return 1;
}

//=============================================================//

int resolveu (matriz m) {    

    // Se encontrar um valor diferente do que está no canto superior
    // esquerdo, retorne falso.
    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].size(); j++)
            if (m[i][j] != m[0][0]) 
                return 0;
    }

    return 1;
}

//=============================================================//