* Para compilar

    ```gcc main.c stack.c -o main```

# Nao testar com o geramapa ou teu Pc explode! Testa com o mapa 3_5_3.txt :D

* Como testar:

    1. Gerar o mapa com:

        ```./geramapa <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>] > mapa.txt```

    2. Executar o main

        ```./main < mapa.txt > result.txt```

    3. Verificar resultado

        ```cat mapa.txt result.txt | ./verifica```

    4. Pegar o resultado no bash:

        ```
        if [ "$?" -eq 0 ]
        then
            echo ok
        else
            echo fail
        fi
        ```