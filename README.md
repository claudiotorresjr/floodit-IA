## v1 sumiu

## O v2_guloso ta funcionando normal. Nao chega a ser guloso, mas ele pinta com a cor que mais pintou quadrinho

## A versão final é a v3_grafo. A v4_certo eh so um backupzinho :D

* Como testar:

    1. Gerar o mapa com:

        ```./geramapa <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>] > mapa.txt```

    2. No diretório v3_grafo, basta executar ```make```

    3. Executar o main

        ```./floodit < mapa.txt > result.txt```

    4. Verificar resultado

        ```cat mapa.txt result.txt | ./verifica```

    5. Pegar o resultado no bash:

        ```
        if [ "$?" -eq 0 ]
        then
            echo ok
        else
            echo fail
        fi
        ```