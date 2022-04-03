#include "IAra.h"

int main() {

    int col, lin, cores;

    if (!scanf("%d %d %d", &col, &lin, &cores)) {
        perror("Não foi possível ler as entradas, abortando.\n");
        exit(SAIDA_LEITURA);
    }

    matriz mapa = alocaMatriz(lin, col);
    leMatriz(&mapa, lin, col);

    resolve(mapa, cores);
    
    return 1;
}