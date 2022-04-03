/**
 * @file  IAra.cpp
 * @brief Arquivo com as implementações das funções da inteligência artificial.
 * @date 15/08/2021
 * @author Richard Fernando Heise Ferreira (GRR20191053)
 **/

#include "IAra.h"

//=============================================================//

vector<char> possiveisCores (matriz m) {
    
    // As bordas de nosso cluster são as jogadas possiveis.
    vector<char> bordas;

    int controle = 0;

    // Floodamos de 0s nosso cluster para não o considerarmos.
    floodFill(&m, 0, make_pair(0,0));

    // Percorremos o tabuleiro
    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].size(); j++) {

            // Se não estamos no cluster, então estamos em uma borda.
            if (m[i][j] != m[0][0]) {

                //  Resetamos o controle
                if (j-1 >= 0 && m[i][j-1] == 0) {
                    
                    bordas.push_back(m[i][j]); 
                    controle = 0;

                } else if (i-1 >= 0 && m[i-1][j] == 0) {

                    bordas.push_back(m[i][j]);
                    controle = 0;

                }

                controle++;
            } 
        }   
        // Se controle não foi resetado, então percorremos uma linha inteira
        // sem bordas, portanto, não devemos continuar buscando bordas.
        if (controle == m[0].size() ) break;
    }

    // Ordenar para depois retirar as duplicatas.
    sort(bordas.begin(), bordas.end(), [](auto &left, auto &right) {
        return left < right;
    });

    //Retira duplicatas.
    vector<char>::iterator it;
    it = unique(bordas.begin(), bordas.begin() + bordas.size());

    bordas.resize( distance(bordas.begin(), it) );

    return bordas;
}

//=============================================================//

float h(matriz m, int cores) {


    int regioes = 0;
    
    // Matriz de distâncias.
    matriz distancias = m;

    // Floodamos nosso cluster com 0s para não o considerarmos.
    floodFill(&distancias, 0, make_pair(0,0));

    int maiorDist = 0;

    // Percorremos o tabuleiro.
    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].size(); j++) {

            //=========================================================================================================
            //======================================= HEURÍSTICA 1 (PRINCIPAL) ========================================
            //========================================================================================================>

            // Calculamos as distâncias de todos os pontos do tabuleiro referente ao nosso cluster.
            if (distancias[i][j] != 0) {

                // A distância é sempre o mínimo somado de 1 entre o valor que está à esquerda e acima de cada elemento. 
                if (i-1 >= 0 && i-1 < distancias.size() && j-1 >= 0 && j-1 < distancias[0].size()) {
                    distancias[i][j] = min(distancias[i-1][j], distancias[i][j-1]) + 1; 

                } else if ( i-1 >= 0 && i-1 < distancias.size() ) {
                    distancias[i][j] = distancias[i-1][j] + 1; 

                } else if ( j-1 >= 0 && j-1 < distancias[0].size() ) {
                    distancias[i][j] = distancias[i][j-1] + 1; 
                } 

                // Guardamos a maior distância.
                if (distancias[i][j] > maiorDist)
                    maiorDist = distancias[i][j];
            }
            //<=========================================================================================================

            //==========================================================================================================
            //======================================= HEURÍSTICA 2 (SECUNDÁRIA) ========================================
            //=========================================================================================================>

            if (m[i][j] != 0) {
                // Literalmente o FloodFill() na matriz de cópia dos parametros para evitar contar a mesma região.
                vector<coordenada> lista;

                lista.push_back(make_pair(i,j));
                char corAnt = m[i][j];

                while (!lista.empty()) {
                    coordenada aux = lista.back();
                    lista.pop_back();

                    if ( flood(&m, aux, corAnt, 0) ) {

                        lista.push_back(make_pair(aux.first+1, aux.second));
                        lista.push_back(make_pair(aux.first-1, aux.second));
                        lista.push_back(make_pair(aux.first, aux.second+1));
                        lista.push_back(make_pair(aux.first, aux.second-1));
                        
                    }
                }

                // Contamos quantas regiões temos ao total.
                regioes++;
            }
            //<========================================================================================================
        }
    }

    // Retornamos a soma das regiões divididos pelo número de cores (que nos dá uma aproximação das jogadas totais)
    // com a jogada que diminui a distância. Quanto menor esse score, melhor.

    return (((float) regioes / (float) cores) + (float)maiorDist);
}

//=============================================================//

float preveJogada (matriz m, int cores, int maxEscopos) {

    // Vetor que relaciona cada cor a uma heurística.
    vector<pair<char, float>> controle;

    // Matriz temporária.
    matriz temp_m = m;

    // Se a matriz vier resolvida, então a cor que a resolveu é a melhor jogada.
    // Logo, retornamos o menor valor heurístico possível.
    if ( resolveu(temp_m) ) return 0;

    // Repetição do laço para cada tabuleiro.
    for (int escopos = 0; escopos < maxEscopos; escopos++) {

        // Se resolvemos, saia do laço, pois achamos a melhor cor.
        if ( resolveu(temp_m) ) break;

        // Limpamos controle a cada nova previsão.
        controle.clear();

        // Possíveis cores adjacentes.
        vector<char> coresBorda = possiveisCores(temp_m); 

        for (char cor : coresBorda) {
            if (cor != temp_m[0][0]) {

                matriz aux = temp_m;
                
                floodFill(&aux, cor, make_pair(0,0));

                // Mais de uma previsão pode estar correta, porém, queremos a que está correta
                // antes das demais, portanto, somamos escopos e heurística para determinar 
                // em qual profundidade encontramos a solução.
                controle.push_back(make_pair(cor, (float)escopos + h(aux, cores)));
            }
        }

        // Ordenamos o vetor pela melhor jogada.
        sort(controle.begin(), controle.end(), [](auto &left, auto &right) {
            return left.second < right.second;
        });

        // Jogamos a melhor jogada decidida anteriormente.
        floodFill(&temp_m, controle.front().first, make_pair(0,0));
    }
    
    // Retornamos a heuristica da melhor jogada;
    return controle.front().second;
}   

//=============================================================//

char buscaMelhorJogada (matriz m, int cores) {

    // Vetor de pares que relaciona uma cor a uma heurística.
    vector<pair<char, float>> controle;

    // Cores adjacentes ao cluster.
    vector<char> coresBorda = possiveisCores(m);

    /*                      IMPORTANTE
    /* Essa equação foi obtida com base em testes:
    /* defini que, com 20 cores em um tabuleiro 100x100
    /* seria impossível prever jogadas no futuro dentro de 2 minutos.
    /* Já com 19, poderia prever uma jogada no futuro. Com 14, duas.
    /* Portanto, tendo três pontos {(20,0); (19,1); (14,2)} gerei um gráfico
    /* que resultou nessa função. Ela só vale 0 para 20, que é quando não prevemos,
    /* e sim usamos a heurística diretamente. 
    /* **Portanto, prevemos o número de jogadas que devemos prever para cada jogada possível.**
    */

    int previsoes = (-4.796l*log(coresBorda.size()) + 14.715);

    // Se devemos prever, prevemos. Senão, não prevemos.
    if (previsoes) {
        // Para cada cor na borda.
        for (char cor : coresBorda) {
            if (cor != m[0][0]) {
                
                float heuristica;

                matriz temp_m = m;
                
                // Floodamos uma matriz temporária com a cor.
                floodFill(&temp_m, cor, make_pair(0,0));

                // Chamamos nossa heurística com previsão para dada matriz pintada com cor.
                heuristica = preveJogada(temp_m, cores, previsoes);
                
                // Guardamos o valor que nossa cor atingiu.
                controle.push_back(make_pair(cor, heuristica));
            }
        }        
    } else {
        // Mesmo do de cima.
        for (char cor : coresBorda) {
            if (cor != m[0][0]) {
                
                float heuristica;

                matriz temp_m = m;
                floodFill(&temp_m, cor, make_pair(0,0));

                // Aqui, contudo, chamamos h diretamente, pois não queremos prever devido ao custo.
                heuristica = h(temp_m, cores);
                
                controle.push_back(make_pair(cor, heuristica));
            }
        }
    }

    // Ordenamos o vetor, porque é mais eficiente do que guardar o menos para cada iteração acima.
    sort(controle.begin(), controle.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });

    // Peguemos a cor com menor heurística, guardada na frente do vetor.
    return controle.front().first;        
}

//=============================================================//

void resolve (matriz m, int cores) {

    vector<char> jogadas;

    // Enquanto não resolvemos, buscamos a melhor jogada, a jogamos e a guardamos.
    while ( !resolveu(m) ) {
        
        char melhorJogada = buscaMelhorJogada(m, cores);
        floodFill(&m, melhorJogada, make_pair(0,0));
        jogadas.push_back(melhorJogada);
       
    }

    // Coloca na tela o tamanho e a seuquência de boas jogadas.
    printf("%lu\n", jogadas.size());
    for (int i = 0; i < jogadas.size(); i++)
        printf("%d ", jogadas[i]);
    printf("\n");

}

//=============================================================//